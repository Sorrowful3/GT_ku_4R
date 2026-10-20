/*
 * antenna_v0_ctrl.c
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#include "antenna_v0_ctrl.h"

#define DEBUG_TAG "antenna_v0_ctrl,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V0

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive((x).lock, portMAX_DELAY) != pdPASS) {        \
        LOG_E("lock failed");                                               \
        return -1;                                                          \
    }                                                                       \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive((x).lock) != pdPASS) {                       \
        LOG_E("unlock failed");                                             \
        return -1;                                                          \
    }                                                                       \
}while(0)


typedef struct {
    antenna_v0_beam_tx_t tx_command;
    antenna_v0_beam_rx_t rx_command;
    SemaphoreHandle_t lock;
    EventGroupHandle_t sync;
}beam_command_t;

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
typedef struct {
    antenna_v0_offset_tx_t tx_command;
    SemaphoreHandle_t lock;
}offset_command_t;
#endif

typedef struct {
    antenna_v0_versions_tx_t tx_command;
    antenna_v0_versions_rx_t rx_command;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;
}versions_command_t;

typedef struct {
    antenna_v0_rf_vcc_tx_t tx_command;
    antenna_v0_rf_vcc_rx_t rx_command;
    SemaphoreHandle_t lock;
    EventGroupHandle_t sync;
}rf_vcc_command_t;

typedef struct {
    antenna_v0_reboot_tx_t tx_command;
    SemaphoreHandle_t lock;
}reboot_command_t;

typedef struct {
    antenna_v0_selfcheck_tx_t tx_command;
    antenna_v0_selfcheck_rx_t rx_command[ANTENNA_V0_MAX_SELFCHECK_FRAME];
    antenna_v0_selfcheck_rx_t rx_temp;
    uint8_t frame_count;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;
}selfcheck_command_t;

typedef struct {
    antenna_v0_temperature_tx_t tx_command;
    antenna_v0_temperature_rx_t rx_command;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;
}temperature_command_t;

typedef struct {
    antenna_v0_power_tx_t tx_command;
    antenna_v0_power_rx_t rx_command;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;
}power_command_t;

typedef struct {
    antenna_v0_voltage_tx_t tx_command;
    antenna_v0_voltage_rx_t rx_command;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;
}voltage_command_t;

static uint32_t power_mask = 0x00;
static uint32_t power_number[ANTENNA_V0_RX_TOTAL+ANTENNA_V0_TX_TOTAL] =
{
#if (ANTENNA_V0_RX_TOTAL >= 1)
        ANTENNA_V0_R0_VCC,
#endif
#if (ANTENNA_V0_RX_TOTAL >= 2)
        ANTENNA_V0_R1_VCC,
#endif
#if (ANTENNA_V0_RX_TOTAL >= 3)
        ANTENNA_V0_R2_VCC,
#endif
#if (ANTENNA_V0_RX_TOTAL >= 4)
        ANTENNA_V0_R3_VCC,
#endif

#if (ANTENNA_V0_TX_TOTAL >= 1)
        ANTENNA_V0_T0_VCC,
#endif
#if (ANTENNA_V0_TX_TOTAL >= 2)
        ANTENNA_V0_T1_VCC,
#endif
#if (ANTENNA_V0_TX_TOTAL >= 3)
        ANTENNA_V0_T2_VCC,
#endif
#if (ANTENNA_V0_TX_TOTAL >= 4)
        ANTENNA_V0_T3_VCC,
#endif
};

static beam_command_t beam_command;
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
static offset_command_t offset_command;
#endif
static versions_command_t versions_command;
static rf_vcc_command_t rf_vcc_command;
static reboot_command_t reboot_command;
static selfcheck_command_t selfcheck_command;
static temperature_command_t temperature_command;
static power_command_t power_command;
static voltage_command_t voltage_command;

static antenna_v0_receive_callbk receive_callbk = NULL;

//计算阵面掩码，忽略未开直流电源的阵面
//用RX_ANTENNA_TOTAL+TX_ANTENNA_TOTAL位表示天线阵面
//其中0~RX_ANTENNA_TOTAL-1位表示接收
//RX_ANTENNA_TOTAL~TX_ANTENNA_TOTAL-1表示发射
//0表示无此阵面，1表示有此阵面
static inline uint32_t antenna_v0_make_mask(uint8_t TorR, uint8_t subset)
{
    int32_t i;
    uint32_t mask;

    mask = 0;
    i = 0;
#if ANTENNA_V0_RX_TOTAL > 0
    //计算发射阵面对应的掩码
    if(TorR == ANTENNA_V0_RX) {
        if(subset == ANTENNA_V0_SUBSET_ALL) {
            for(; i<ANTENNA_V0_RX_TOTAL; i++) {
                mask |= 0x01 << i;
            }
        }
        else if(subset <= ANTENNA_V0_RX_TOTAL) {
            mask |= 0x01 << subset;
        }
    }
#endif

#if ANTENNA_V0_TX_TOTAL > 0
    //计算发射阵面对应的掩码
    if(TorR == ANTENNA_V0_TX) {
        if(subset == ANTENNA_V0_SUBSET_ALL) {
            for(i=0; i<ANTENNA_V0_TX_TOTAL; i++) {
                mask |= 0x01 << (i + ANTENNA_V0_RX_TOTAL);
            }
        }
        else if(subset <= ANTENNA_V0_TX_TOTAL) {
            mask |= 0x01 << (subset + ANTENNA_V0_RX_TOTAL);
        }
    }
#endif
    //屏蔽未开启直流电源的阵面
    return mask & power_mask;
}

//计算天线编号，忽略未开直流电源的阵面
//接收天线阵面用0~RX_ANTENNA_TOTAL-1表示
//发射天线阵面用RX_ANTENNA_TOTAL~TX_ANTENNA_TOTAL-1表示
static inline int32_t antenna_v0_make_number(uint8_t TorR, uint8_t subset)
{
    int32_t number;

    if((TorR == ANTENNA_V0_RX) && (subset < ANTENNA_V0_RX_TOTAL)) {
        number = subset;
    }
    else if((TorR == ANTENNA_V0_TX) && (subset < ANTENNA_V0_TX_TOTAL)) {
        number = subset + ANTENNA_V0_RX_TOTAL;
    }
    else {
        return ERR_INVAL;
    }
    //忽略未开直流电源的阵面
    if(power_mask & (0x01 << number)) {
        return number;
    }
    return ERR_INVAL;
}

//计算天线直流电源编号
static inline int32_t antenna_v0_make_power_number(uint8_t TorR, uint8_t subset)
{
    if((TorR == ANTENNA_V0_RX) && (subset < ANTENNA_V0_RX_TOTAL)) {
        return subset;
    }
    else if((TorR == ANTENNA_V0_TX) && (subset < ANTENNA_V0_TX_TOTAL)) {
        return subset + ANTENNA_V0_RX_TOTAL;
    }
    return ERR_INVAL;
}

static void parse_thread(void *arg)
{
    ssize_t lenght;
    int32_t i;
    uint32_t mask;
    uint8_t buffer[22];
    antenna_v0_msg_t msg;
    antenna_v0_parse_state_t state;

    msg.current_lenght = 0;
    msg.frame_lenght = 0;
    while(1)
    {
        lenght = antenna_v0_port_read(buffer, sizeof(buffer), portMAX_DELAY);
        if(lenght <= 0) {
            continue;
        }
        for(i=0; i<lenght; i++) {
            //解析一个字节
            state = antenna_v0_parse_byte(&msg, buffer[i]);

            //继续解析
            if(state == ANTENNA_PARSE_INCOMPLETE) {
                continue;
            }
            //成功解析完一帧
            if(state == ANTENNA_PARSE_OK) {
                switch(msg.msg_id) {
                case 0x22:
                    //解析波束控制回复帧
                    antenna_v0_beam_decode(&msg, &beam_command.rx_command);
                    //波束设置成功则设置相应阵面的标志位
                    if(beam_command.rx_command.error == 0x00) {
                        mask = antenna_v0_make_mask(beam_command.rx_command.TorR, beam_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(beam_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("set beam failed");
                    }
                    break;
                case 0x12:
                    //解析版本读取回复帧
                    antenna_v0_versions_decode(&msg, &versions_command.rx_command);
                    //回复帧阵面与下发帧阵面对应则设置信号量
                    if((versions_command.rx_command.TorR == versions_command.tx_command.TorR) &&
                            (versions_command.rx_command.subset == versions_command.tx_command.subset)) {
                        xSemaphoreGive(versions_command.sync);
                    }
                    break;
                case 0x36:
                    //解析射频电源控制回复帧
                    antenna_v0_rf_vcc_decode(&msg, &rf_vcc_command.rx_command);
                    //如果设置成功则设置事件标志组
                    if(rf_vcc_command.rx_command.state == rf_vcc_command.tx_command.state) {
                        mask = antenna_v0_make_mask(rf_vcc_command.rx_command.TorR, rf_vcc_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(rf_vcc_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("control RF VCC failed");
                    }
                    break;
                case 0x21:
                    //解析自检命令回复帧
                    antenna_v0_selfcheck_decode(&msg, &selfcheck_command.rx_temp);
                    //校验回复帧阵面与下发帧阵面
                    if((selfcheck_command.rx_temp.TorR == selfcheck_command.tx_command.TorR) &&
                            (selfcheck_command.rx_temp.subset == selfcheck_command.tx_command.subset) &&
                            (selfcheck_command.rx_temp.current_frame <= ANTENNA_V0_MAX_SELFCHECK_FRAME)) {
                        //拷贝到缓存
                        selfcheck_command.rx_command[selfcheck_command.rx_temp.current_frame-1] = selfcheck_command.rx_temp;
                        selfcheck_command.frame_count++;
                        //接收完自检帧则释放信号量
                        if(selfcheck_command.frame_count >= selfcheck_command.rx_temp.total_frame) {
                            xSemaphoreGive(selfcheck_command.sync);
                        }
                    }
                    break;
                case 0x13:
                    //解析温度读取命令回复帧
                    antenna_v0_temperature_decode(&msg, &temperature_command.rx_command);
                    //回复帧阵面与下发帧阵面对应则设置信号量
                    if((temperature_command.rx_command.TorR == temperature_command.tx_command.TorR) &&
                            (temperature_command.rx_command.subset == temperature_command.tx_command.subset)) {
                        //释放信号量
                        xSemaphoreGive(temperature_command.sync);
                    }
                    break;
                case 0x18:
                    //解析功率读取命令回复帧
                    antenna_v0_power_decode(&msg, &power_command.rx_command);
                    //回复帧阵面与下发帧阵面对应则设置信号量
                    if((power_command.rx_command.TorR == power_command.tx_command.TorR) &&
                            (power_command.rx_command.subset == power_command.tx_command.subset)) {
                        //释放信号量
                        xSemaphoreGive(power_command.sync);
                    }
                    break;
                case 0x1A:
                    //解析电压读取命令回复帧
                    antenna_v0_voltage_decode(&msg, &voltage_command.rx_command);
                    //回复帧阵面与下发帧阵面对应则设置信号量
                    if((voltage_command.rx_command.TorR == voltage_command.tx_command.TorR) &&
                            (voltage_command.rx_command.subset == voltage_command.tx_command.subset)) {
                        //释放信号量
                        xSemaphoreGive(voltage_command.sync);
                    }
                    break;
                default:
                    break;
                }
                //执行接收完成回调函数
                if(receive_callbk) {
                    receive_callbk(msg.load, msg.frame_lenght);
                }
            }
            //数据帧异常
            if(state == ANTENNA_PARSE_BAD) {
                LOG_E("format error");
            }

            //复位MSG
            msg.frame_lenght = 0;
            msg.current_lenght = 0;
        }
    }
}

//开启直流供电
//Rx与Tx是一对，必须再20ms内同时开启
int32_t antenna_v0_power_on(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v0_make_power_number(TorR, subset);
    if(number < 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //开直流电源
    result = power_on(power_number[number]);
    if(result == ERR_OK) {
        //延时50ms，等待上电完成
        vTaskDelay(50);
        power_mask |= 0x01 << number;
    }
    return result;
}

int32_t antenna_v0_power_on_nodelay(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v0_make_power_number(TorR, subset);
    if(number < 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //开直流电源
    result = power_on(power_number[number]);
    if(result == ERR_OK) {
        power_mask |= 0x01 << number;
    }
    return result;
}

//关闭直流供电
int32_t antenna_v0_power_off(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v0_make_power_number(TorR, subset);
    if(number < 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //关直流电源
    result = power_off(power_number[number]);
    if(result == ERR_OK) {
        power_mask &= ~(0x01 << number);
    }
    return result;
}

//获取直流供电状态
int32_t antenna_v0_power_state(uint8_t TorR, uint8_t subset)
{
//    int32_t number;
//
//    //获取阵面编号
//    number = antenna_v0_make_power_number(TorR, subset);
//    if(number < 0) {
//        LOG_E("parameter error");
//        return ERR_INVAL;
//    }
//
//    return (power_mask & (0x01 << number)) ? 1 : 0;
    uint32_t mask;

    mask = antenna_v0_make_mask(TorR, subset);

    return (mask) ? 1 : 0;
}

int32_t antenna_v0_set_beam(uint8_t TorR, const antenna_v0_beam_t *beam, uint8_t block_flag)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v0_make_mask(TorR, ANTENNA_V0_SUBSET_ALL);
    if((mask == 0) || (!beam)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beam_command);

    if(block_flag) {
        xEventGroupClearBits(beam_command.sync, mask);
    }

    //发送波束控制命令
    beam_command.tx_command.TorR = TorR;
    beam_command.tx_command.polarity = beam->polarity;
    beam_command.tx_command.theta = beam->theta;
    beam_command.tx_command.phi = beam->phi;
    beam_command.tx_command.weight = beam->weight;
    beam_command.tx_command.polarity_mode = beam->polarity_mode;
    beam_command.tx_command.freq = beam->freq;
#ifdef ANTENNA_V0_TX_ROTATE
    if(TorR == ANTENNA_V0_TX) {
        //旋转发射指向
        beam_command.tx_command.phi += ANTENNA_V0_TX_ROTATE;
        if(beam_command.tx_command.phi > 360.0f) {
            beam_command.tx_command.phi -= 360.0f;
        }
        if(ANTENNA_V0_TX_ROTATE > 180.0f) {
            beam_command.tx_command.polarity += ANTENNA_V0_TX_ROTATE - 180.0f;
        }else {
            beam_command.tx_command.polarity += ANTENNA_V0_TX_ROTATE;
        }
        if(beam_command.tx_command.polarity > 180.0f) {
            beam_command.tx_command.polarity -= 180.0f;
        }
    }
#endif
    result = antenna_v0_beam_send(&beam_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(beam_command);
        LOG_E("set beam send failed");
        return result;
    }

    //等待天线回复
    if(block_flag) {
        receive_event = xEventGroupWaitBits(beam_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V0_TIMEOUT);
        if((receive_event & mask) != mask) {
            UNLOCK(beam_command);
            LOG_E("set beam timeout");
            return ERR_TIMEOUT;
        }
    }

    UNLOCK(beam_command);
    return ERR_OK;
}

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
int32_t antenna_v0_offset(uint8_t TorR, const antenna_v0_beam_t *beam)
{
    int32_t result;
    uint32_t mask;
    antenna_v0_point_t point;
    antenna_v0_offset_t offset_data;

    mask = antenna_v0_make_mask(TorR, ANTENNA_V0_SUBSET_ALL);
    if((mask == 0) || (!beam)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //接收不需要补偿
    if(TorR == ANTENNA_V0_RX) {
        return ERR_OK;
    }

    LOCK(offset_command);

    //查找校准数据
    point.freq = beam->freq;
    point.theta = beam->theta;
    point.phi = beam->phi;
    point.polarity = beam->polarity;
    point.polarity_type = beam->polarity_mode;
#ifdef ANTENNA_V0_TX_ROTATE
    if(TorR == ANTENNA_V0_TX) {
        //旋转发射指向
        point.phi += ANTENNA_V0_TX_ROTATE;
        if(point.phi > 360.0f) {
            point.phi -= 360.0f;
        }
        if(ANTENNA_V0_TX_ROTATE > 180.0f) {
            point.polarity += ANTENNA_V0_TX_ROTATE - 180.0f;
        }else {
            point.polarity += ANTENNA_V0_TX_ROTATE;
        }
        if(point.polarity > 180.0f) {
            point.polarity -= 180.0f;
        }
    }
#endif
    result = antenna_v0_offset_find(TorR, &point, &offset_data);
    if(result != ERR_OK) {
        UNLOCK(offset_command);
        LOG_E("find offset data failed");
        return result;
    }
    //发送补偿数据
    offset_command.tx_command.TorR = TorR;
    offset_command.tx_command.phase = offset_data.phase;
    offset_command.tx_command.range = offset_data.range;
    result = antenna_v0_offset_send(&offset_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(offset_command);
        LOG_E("set beam send failed");
        return result;
    }

    UNLOCK(offset_command);
    return ERR_OK;
}
#endif

int32_t antenna_v0_read_versions(uint8_t TorR, uint8_t subset, antenna_v0_version_t *version)
{
    int32_t result;
    int32_t number;

    if(!version) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(TorR != ANTENNA_V0_ACU) {
        number = antenna_v0_make_number(TorR, subset);
        if(number < 0) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
    }

    LOCK(versions_command);

    xSemaphoreTake(versions_command.sync, 0);
    //发送版本回读命令
    versions_command.tx_command.TorR = TorR;
    versions_command.tx_command.subset = subset;
    result = antenna_v0_versions_send(&versions_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(versions_command);
        LOG_E("read versions send failed");
        return result;
    }

    //等待天线响应
    if(xSemaphoreTake(versions_command.sync, ANTENNA_V0_TIMEOUT) != pdTRUE) {
        UNLOCK(versions_command);
        LOG_E("read versions timeout");
        return ERR_TIMEOUT;
    }

    //获取功版本信息
    version->version = versions_command.rx_command.version;
    version->baud =  versions_command.rx_command.baud;

    UNLOCK(versions_command);
    return ERR_OK;
}

int32_t antenna_v0_ctrl_rf_vcc(uint8_t TorR, const antenna_v0_rf_vcc_t *rf_vcc)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v0_make_mask(TorR, ANTENNA_V0_SUBSET_ALL);
    if((mask == 0) || (!rf_vcc)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(rf_vcc_command);

    xEventGroupClearBits(rf_vcc_command.sync, mask);
    //发送通道供电开关命令
    rf_vcc_command.tx_command.TorR = TorR;
    rf_vcc_command.tx_command.polarity = rf_vcc->polarity;
    rf_vcc_command.tx_command.state = rf_vcc->state;
    result = antenna_v0_rf_vcc_send(&rf_vcc_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(rf_vcc_command);
        LOG_E("control RF VCC send failed");
        return result;
    }

    //等待天线回复
    receive_event = xEventGroupWaitBits(rf_vcc_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V0_TIMEOUT);
    if((receive_event & mask) != mask) {
        UNLOCK(rf_vcc_command);
        LOG_E("control RF VCC timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(rf_vcc_command);
    return ERR_OK;
}

int32_t antenna_v0_reboot(void)
{
    int32_t result;

    LOCK(reboot_command);

    //发送波控复位命令
    result = antenna_v0_reboot_send(&reboot_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(reboot_command);
        LOG_E("reboot_command send failed");
        return result;
    }

    UNLOCK(reboot_command);
    return ERR_OK;
}

int32_t antenna_v0_check_channel(uint8_t TorR, uint8_t subset, antenna_v0_selfcheck_t *selfcheck)
{
    uint32_t i;
    int32_t result;
    int32_t number;

    number = antenna_v0_make_number(TorR, subset);
    if((number < 0) || (!selfcheck)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(selfcheck_command);

    xSemaphoreTake(selfcheck_command.sync, 0);
    selfcheck_command.frame_count = 0;
    //发送自检命令
    selfcheck_command.tx_command.TorR  = TorR;
    selfcheck_command.tx_command.subset = subset;
    result = antenna_v0_selfcheck_send(&selfcheck_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(selfcheck_command);
        LOG_E("self check send failed");
        return result;
    }

    //等待天线响应
    if(xSemaphoreTake(selfcheck_command.sync, ANTENNA_V0_TIMEOUT*3) != pdTRUE) {
        UNLOCK(selfcheck_command);
        LOG_E("self check timeout");
        return ERR_TIMEOUT;
    }

    //获取自检数据
    for(i=0; (i<selfcheck_command.frame_count)&&(i*4 < ANTENNA_V0_MAX_SELFCHECK_FRAME*4); i++) {
        memcpy(&selfcheck->data[i*4], selfcheck_command.rx_command[i].check_data, 4*2);
    }
    selfcheck->lenght = i*4;

    UNLOCK(selfcheck_command);
    return ERR_OK;
}

int32_t antenna_v0_read_temperature(uint8_t TorR, uint8_t subset, const uint8_t chip_number[9], antenna_v0_temperature_t *temperature)
{
    int32_t result;
    int32_t number;

    number = antenna_v0_make_number(TorR, subset);
    if((number < 0) || (!temperature)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(temperature_command);

    xSemaphoreTake(temperature_command.sync, 0);
    //发送温度读取命令
    temperature_command.tx_command.TorR = TorR;
    temperature_command.tx_command.subset = subset;
    if(chip_number) {
        memcpy(temperature_command.tx_command.chip_number, chip_number, 9);
    }
    else {
        memset(temperature_command.tx_command.chip_number, 0, 9);
    }
    result = antenna_v0_temperature_send(&temperature_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(temperature_command);
        LOG_E("read temperature send failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(temperature_command.sync, ANTENNA_V0_TIMEOUT*3) != pdTRUE) {
        UNLOCK(temperature_command);
        LOG_E("read temperature timeout");
        return ERR_TIMEOUT;
    }

    //提取温度数据
    memcpy(temperature->board_temperature, temperature_command.rx_command.board_temperature, 3);
    memcpy(temperature->chip_temperature, temperature_command.rx_command.chip_temperature, 9);

    UNLOCK(temperature_command);
    return ERR_OK;
}

int32_t antenna_v0_read_power(uint8_t TorR, uint8_t subset, antenna_v0_power_t *power)
{
    int32_t result;
    int32_t number;

    number = antenna_v0_make_number(TorR, subset);
    if((number < 0) || (!power)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(power_command);

    xSemaphoreTake(power_command.sync, 0);
    //发送功率读取命令
    power_command.tx_command.TorR = TorR;
    power_command.tx_command.subset = subset;
    result = antenna_v0_power_send(&power_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(power_command);
        LOG_E("read power send failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(power_command.sync, ANTENNA_V0_TIMEOUT*3) != pdTRUE) {
        UNLOCK(power_command);
        LOG_E("read power timeout");
        return ERR_TIMEOUT;
    }

    //提取温度数据
    power->total_electricity = power_command.rx_command.total_electricity;
    power->total_voltage = power_command.rx_command.total_voltage;
    power->total_power = power_command.rx_command.total_power;

    UNLOCK(power_command);
    return ERR_OK;
}

int32_t antenna_v0_read_voltage(uint8_t TorR, uint8_t subset, antenna_v0_voltage_t *voltage)
{
    int32_t result;
    int32_t number;

    number = antenna_v0_make_number(TorR, subset);
    if((number < 0) || (!voltage)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(voltage_command);

    xSemaphoreTake(voltage_command.sync, 0);
    //发送电压读取命令
    voltage_command.tx_command.TorR = TorR;
    voltage_command.tx_command.subset = subset;
    result = antenna_v0_voltage_send(&voltage_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(voltage_command);
        LOG_E("read voltage send failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(voltage_command.sync, ANTENNA_V0_TIMEOUT*3) != pdTRUE) {
        UNLOCK(voltage_command);
        LOG_E("read voltage timeout");
        return ERR_TIMEOUT;
    }

    //提取电压数据
    voltage->maam_vdd = voltage_command.rx_command.maam_vdd;
    voltage->maam_vg = voltage_command.rx_command.maam_vg;
    voltage->vdd1_00 = voltage_command.rx_command.vdd1_00;
    voltage->vdd1_01 = voltage_command.rx_command.vdd1_01;

    UNLOCK(voltage_command);
    return ERR_OK;
}

int32_t antenna_v0_set_callbk(antenna_v0_receive_callbk callbk)
{
    receive_callbk = callbk;
    return ERR_OK;
}

int32_t antenna_v0_ctrl_init(void)
{
    TaskHandle_t task_handle;
    portBASE_TYPE xresult;

    //初始化软件资源
    beam_command.lock = xSemaphoreCreateRecursiveMutex();
    beam_command.sync = xEventGroupCreate();
    if((!beam_command.lock) || (!beam_command.sync)) {
        LOG_E("beam command initialize failed");
        return ERR_NOMEM;
    }
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
    offset_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!offset_command.lock) {
        LOG_E("reboot command initialize failed");
        return ERR_NOMEM;
    }
#endif
    versions_command.lock = xSemaphoreCreateRecursiveMutex();
    versions_command.sync = xSemaphoreCreateBinary();
    if((!versions_command.lock) || (!versions_command.sync)) {
        LOG_E("versions command initialize failed");
        return ERR_NOMEM;
    }
    rf_vcc_command.lock = xSemaphoreCreateRecursiveMutex();
    rf_vcc_command.sync = xEventGroupCreate();
    if((!rf_vcc_command.lock) || (!rf_vcc_command.sync)) {
        LOG_E("rf_vcc command initialize failed");
        return ERR_NOMEM;
    }
    reboot_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!reboot_command.lock) {
        LOG_E("reboot command initialize failed");
        return ERR_NOMEM;
    }
    selfcheck_command.lock = xSemaphoreCreateRecursiveMutex();
    selfcheck_command.sync = xSemaphoreCreateBinary();
    if((!selfcheck_command.lock) || (!selfcheck_command.sync)) {
        LOG_E("self check command initialize failed");
        return ERR_NOMEM;
    }
    temperature_command.lock = xSemaphoreCreateRecursiveMutex();
    temperature_command.sync = xSemaphoreCreateBinary();
    if((!temperature_command.lock) || (!temperature_command.sync)) {
        LOG_E("temperature command initialize failed");
        return ERR_NOMEM;
    }
    power_command.lock = xSemaphoreCreateRecursiveMutex();
    power_command.sync = xSemaphoreCreateBinary();
    if((!power_command.lock) || (!power_command.sync)) {
        LOG_E("power command initialize failed");
        return ERR_NOMEM;
    }
    voltage_command.lock = xSemaphoreCreateRecursiveMutex();
    voltage_command.sync = xSemaphoreCreateBinary();
    if((!voltage_command.lock) || (!voltage_command.sync)) {
        LOG_E("voltage command initialize failed");
        return ERR_NOMEM;
    }

    //创建解包线程
    xresult = xTaskCreate(parse_thread,
                          "antenna_v0_parse",
                          1024*6,
                          NULL,
                          configMAX_PRIORITIES - 2,
                          &task_handle);
    if(xresult != pdPASS) {
        LOG_E("create task parse_thread failed");
        return ERR_NOMEM;
    }

    return ERR_OK;
}

#endif /* USING_ANTENNA_V0 */
