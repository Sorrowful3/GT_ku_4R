/*
 * antenna_v1_ctrl.c
 *
 *  Created on: 2021年9月11日
 *      Author: Administrator
 */

#include "antenna_v1_ctrl.h"
#include "xtime_l.h"

static volatile XTime beam_send_time;
static volatile XTime beam_end_time;

static volatile uint8_t beam_timing_enable = 0;

#define DEBUG_TAG "antenna_v1_ctrl,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../../core/debug.h"


#ifdef USING_ANTENNA_V1

#ifndef USING_GLOBAL_LOCK

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive((x).lock, portMAX_DELAY) != pdPASS) {        \
        LOG_E("lock failed");                                               \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive((x).lock) != pdPASS) {                       \
        LOG_E("unlock failed");                                             \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
}while(0)

#else

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive(global_lock, portMAX_DELAY) != pdPASS) {     \
        LOG_E("lock failed");                                               \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive(global_lock) != pdPASS) {                    \
        LOG_E("unlock failed");                                             \
        return ERR_ROUTINE;                                                 \
    }                                                                       \
}while(0)

#endif

typedef struct {
    antenna_v1_init_check_tx_t tx_command;
    antenna_v1_init_check_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    SemaphoreHandle_t sync;
}init_check_command_t;

typedef struct {
    antenna_v1_comm_error_rx_t rx_command;
}comm_error_command_t;

typedef struct {
    antenna_v1_reset_tx_t tx_command;
    antenna_v1_reset_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    EventGroupHandle_t sync;
}reset_command_t;

typedef struct {
    antenna_v1_versions_tx_t tx_command;
    antenna_v1_versions_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    SemaphoreHandle_t sync;
}versions_command_t;

typedef struct {
    antenna_v1_power_tx_t tx_command;
    antenna_v1_power_rx_t rx_command[ANTENNA_V1_MAX_POWER_FRAME];
    antenna_v1_power_rx_t rx_temp;
    uint8_t temp_count;
    uint8_t frame_count;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    SemaphoreHandle_t sync;
}power_command_t;

typedef struct {
    antenna_v1_beam_tx_t tx_command;
    antenna_v1_beam_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    EventGroupHandle_t sync;
}beam_command_t;

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
typedef struct {
    antenna_v1_offset_tx_t tx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
}offset_command_t;
#endif

typedef struct {
    antenna_v1_config_tx_t tx_command;
    antenna_v1_config_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    EventGroupHandle_t sync;
}config_command_t;

typedef struct {
    antenna_v1_config_abnormal_rx_t rx_command;
}config_abnormal_command_t;

typedef struct {
    antenna_v1_rf_vcc_tx_t tx_command;
    antenna_v1_rf_vcc_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    EventGroupHandle_t sync;
}rf_vcc_command_t;

typedef struct {
    antenna_v1_temperature_tx_t tx_command;
    antenna_v1_temperature_rx_t rx_command;
#ifndef USING_GLOBAL_LOCK
    SemaphoreHandle_t lock;
#endif
    SemaphoreHandle_t sync;
}temperature_command_t;

static uint32_t power_mask = 0x00;
static uint32_t power_number[ANTENNA_V1_RX_TOTAL+ANTENNA_V1_TX_TOTAL] =
{
#if (ANTENNA_V1_RX_TOTAL >= 1)
    ANTENNA_V1_R0_VCC,
#endif
#if (ANTENNA_V1_RX_TOTAL >= 2)
    ANTENNA_V1_R1_VCC,
#endif
#if (ANTENNA_V1_RX_TOTAL >= 3)
    ANTENNA_V1_R2_VCC,
#endif
#if (ANTENNA_V1_RX_TOTAL >= 4)
    ANTENNA_V1_R3_VCC,
#endif

#if (ANTENNA_V1_TX_TOTAL >= 1)
    ANTENNA_V1_T0_VCC,
#endif
#if (ANTENNA_V1_TX_TOTAL >= 2)
    ANTENNA_V1_T1_VCC,
#endif
#if (ANTENNA_V1_TX_TOTAL >= 3)
    ANTENNA_V1_T2_VCC,
#endif
#if (ANTENNA_V1_TX_TOTAL >= 4)
    ANTENNA_V1_T3_VCC,
#endif
};

static init_check_command_t init_check_command;
static comm_error_command_t comm_error_command;
static reset_command_t reset_command;
static versions_command_t versions_command;
static power_command_t power_command;
static beam_command_t beam_command;
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
static offset_command_t offset_command;
#endif
static config_command_t config_command;
static config_abnormal_command_t config_abnormal_command;
static rf_vcc_command_t rf_vcc_command;
static temperature_command_t temperature_command;

#ifdef USING_GLOBAL_LOCK
//全局锁
static SemaphoreHandle_t global_lock;
#endif

static antenna_v1_receive_callbk receive_callbk = NULL;

//计算阵面掩码，忽略未开直流电源的阵面
//用ANTENNA_V1_RX_TOTAL+ANTENNA_V1_TX_TOTAL位表示天线阵面
//其中0~ANTENNA_V1_RX_TOTAL-1位表示接收
//ANTENNA_V1_RX_TOTAL~ANTENNA_V1_TX_TOTAL-1表示发射
//0表示无此阵面，1表示有此阵面
static inline uint32_t antenna_v1_make_mask(uint8_t TorR, uint8_t subset)
{
    int32_t i;
    uint32_t mask;

    mask = 0;
#if ANTENNA_V1_RX_TOTAL > 0
    //计算发射阵面对应的掩码
    if(TorR == ANTENNA_V1_RX) {
        if(subset == ANTENNA_V1_SUBSET_ALL) {
            for(i=0; i<ANTENNA_V1_RX_TOTAL; i++) {
                mask |= 0x01 << i;
            }
        }
        else if(subset <= ANTENNA_V1_RX_TOTAL) {
            mask |= 0x01 << subset;
        }
    }
#endif

#if ANTENNA_V1_TX_TOTAL > 0
    //计算发射阵面对应的掩码
    if(TorR == ANTENNA_V1_TX) {
        if(subset == ANTENNA_V1_SUBSET_ALL) {
            for(i=0; i<ANTENNA_V1_TX_TOTAL; i++) {
                mask |= 0x01 << (i + ANTENNA_V1_RX_TOTAL);
            }
        }
        else if(subset <= ANTENNA_V1_TX_TOTAL) {
            mask |= 0x01 << (subset + ANTENNA_V1_RX_TOTAL);
        }
    }
#endif
    //屏蔽未开启直流电源的阵面
    return mask & power_mask;
}

//计算天线编号，忽略未开直流电源的阵面
//接收天线阵面用0~RX_ANTENNA_TOTAL-1表示
//发射天线阵面用RX_ANTENNA_TOTAL~TX_ANTENNA_TOTAL-1表示
static inline int32_t antenna_v1_make_number(uint8_t TorR, uint8_t subset)
{
    int32_t number;

    if((TorR == ANTENNA_V1_RX) && (subset < ANTENNA_V1_RX_TOTAL)) {
        number = subset;
    }
    else if((TorR == ANTENNA_V1_TX) && (subset < ANTENNA_V1_TX_TOTAL)) {
        number = subset + ANTENNA_V1_RX_TOTAL;
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
static inline int32_t antenna_v1_make_power_number(uint8_t TorR, uint8_t subset)
{
    if((TorR == ANTENNA_V1_RX) && (subset < ANTENNA_V1_RX_TOTAL)) {
        return subset;
    }
    else if((TorR == ANTENNA_V1_TX) && (subset < ANTENNA_V1_TX_TOTAL)) {
        return subset + ANTENNA_V1_RX_TOTAL;
    }
    return ERR_INVAL;
}


static void parse_thread(void *arg)
{
    ssize_t lenght;
    int32_t i;
    uint32_t mask;
    uint8_t buffer[22];
    antenna_v1_msg_t msg;
    antenna_v1_parse_state_t state;
    XTime receive_time;
    uint64_t elapsed_us;

    msg.current_lenght = 0;
    msg.frame_lenght = 0;

    while(1)
    {
        lenght = antenna_v1_port_read(buffer, sizeof(buffer), portMAX_DELAY);
        if(lenght <= 0) {
            continue;
        }
        for(i=0; i<lenght; i++) {
            //解析一个字节
            state = antenna_v1_parse_byte(&msg, buffer[i]);
            //继续解析
            if(state == ANTENNA_V1_PARSE_INCOMPLETE) {
                continue;
            }
            for(int j = 0; j < msg.frame_lenght; j++) {
            	 LOG_E("%02X ", msg.load[j]);
              }
            LOG_E("\r\n");
            //成功解析完一帧
            if(state == ANTENNA_V1_PARSE_OK) {
                switch(msg.msg_id) {
                case 0x09:
                    //初始化检测
                    antenna_v1_init_check_decode(&msg, &init_check_command.rx_command);
                    xSemaphoreGive(init_check_command.sync);
                    break;
                case 0x0B:
                    //通信错误处理
                    antenna_v1_comm_error_decode(&msg, &comm_error_command.rx_command);
                    if(comm_error_command.rx_command.fault_code != 0x00) {
                        LOG_E("communication abnormal, TorR:%02x, subset:%02x, fault code:%02x",
                                comm_error_command.rx_command.TorR, comm_error_command.rx_command.subset, comm_error_command.rx_command.fault_code);
                    }
                    break;
                case 0x11:
                    //波控复位
                    antenna_v1_reset_decode(&msg, &reset_command.rx_command);
                    mask = antenna_v1_make_mask(reset_command.rx_command.TorR, reset_command.rx_command.subset);
                    if(mask != 0) {
                        xEventGroupSetBits(reset_command.sync, mask);
                    }
                    break;
                case 0x12:
                    //读版本
                    antenna_v1_versions_decode(&msg, &versions_command.rx_command);
                    xSemaphoreGive(versions_command.sync);
                    break;
                case 0x1A:
                    //读总功率
                    antenna_v1_power_decode(&msg, &power_command.rx_temp);
                    if((power_command.rx_temp.current > 0) && (power_command.rx_temp.current <= ANTENNA_V1_MAX_POWER_FRAME)) {
                        power_command.rx_command[power_command.rx_temp.current-1] = power_command.rx_temp;
                        power_command.temp_count++;
                        //接收完所有数据帧
                        if(power_command.temp_count >= power_command.rx_temp.total) {
                            xSemaphoreGive(power_command.sync);
                            power_command.frame_count = power_command.temp_count;
                            power_command.temp_count = 0;
                        }
                    }
                    break;
                case 0x22:
                    //设置波束指向
                    antenna_v1_beam_decode(&msg, &beam_command.rx_command);
                    if(beam_command.rx_command.result == 0x00) {
                        XTime_GetTime(&receive_time);

                          elapsed_us =
                              ((uint64_t)(receive_time - beam_send_time) *
                               1000000ULL) /
                              COUNTS_PER_SECOND;

                        mask = antenna_v1_make_mask(beam_command.rx_command.TorR, beam_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(beam_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("set beam failed, TorR:%02x, subset:%02x",
                                beam_command.rx_command.TorR, beam_command.rx_command.subset);
                    }
                    break;
                case 0x26:
                    //配置天线极化
                    antenna_v1_config_decode(&msg, &config_command.rx_command);
                    //配置完成，或重复配置
                    if(config_command.rx_command.result == 0x00) {
                        mask = antenna_v1_make_mask(config_command.rx_command.TorR, config_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(config_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("config antenna failed, TorR:%02x, subset:%02x",
                                config_command.rx_command.TorR, config_command.rx_command.subset);
                    }
                    break;
                case 0x27:
                    //配置异常回复
                    antenna_v1_config_abnormal_decode(&msg, &config_abnormal_command.rx_command);
                    //不需要配置天线则认为配置成功
                    if((config_abnormal_command.rx_command.hint == 0x03) || (config_abnormal_command.rx_command.hint == 0x04)) {
                        mask = antenna_v1_make_mask(config_abnormal_command.rx_command.TorR, config_abnormal_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(config_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("config abnormal failed, TorR:%02x, subset:%02x, state:%02x, error:%02x, hint:%02x",
                                config_abnormal_command.rx_command.TorR, config_abnormal_command.rx_command.subset,
                                config_abnormal_command.rx_command.state, config_abnormal_command.rx_command.error,
                                config_abnormal_command.rx_command.hint);
                    }
                    break;
                case 0x36:
                    //配置天线射频电源
                    antenna_v1_rf_vcc_decode(&msg, &rf_vcc_command.rx_command);
                    if(rf_vcc_command.rx_command.sw_state == rf_vcc_command.tx_command.sw_state) {
                        mask = antenna_v1_make_mask(rf_vcc_command.rx_command.TorR, rf_vcc_command.rx_command.subset);
                        if(mask != 0) {
                            xEventGroupSetBits(rf_vcc_command.sync, mask);
                        }
                    }
                    else {
                        LOG_E("switch rf vcc failed, TorR:%02x, subset:%02x",
                                rf_vcc_command.rx_command.TorR, rf_vcc_command.rx_command.subset);
                    }
                    break;
                case 0x3C:
                    //读取天线温度
                    antenna_v1_temperature_decode(&msg, &temperature_command.rx_command);
                    xSemaphoreGive(temperature_command.sync);
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
            if(state == ANTENNA_V1_PARSE_BAD) {
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
int32_t antenna_v1_power_on(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v1_make_power_number(TorR, subset);
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

int32_t antenna_v1_power_on_nodelay(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v1_make_power_number(TorR, subset);
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
int32_t antenna_v1_power_off(uint8_t TorR, uint8_t subset)
{
    int32_t number;
    int32_t result;

    //获取阵面编号
    number = antenna_v1_make_power_number(TorR, subset);
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
int32_t antenna_v1_power_state(uint8_t TorR, uint8_t subset)
{
//    int32_t number;
//
//    //获取阵面编号
//    number = antenna_v1_make_power_number(TorR, subset);
//    if(number < 0) {
//        LOG_E("parameter error");
//        return ERR_INVAL;
//    }
//
//    return (power_mask & (0x01 << number)) ? 1 : 0;
    uint32_t mask;

    mask = antenna_v1_make_mask(TorR, subset);

    return (mask) ? 1 : 0;
}

int32_t antenna_v1_init_check(uint8_t TorR, uint8_t subset)
{
    int32_t result;

    if(antenna_v1_make_number(TorR, subset) < 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(init_check_command);
    xSemaphoreTake(init_check_command.sync, 0);

    //发送初始化检测命令
    init_check_command.tx_command.TorR = TorR;
    init_check_command.tx_command.subset = subset;
    result = antenna_v1_init_check_send(&init_check_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(init_check_command);
        LOG_E("send init check command failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(init_check_command.sync, ANTENNA_V1_TIMEOUT) != pdTRUE) {
        UNLOCK(init_check_command);
        LOG_E("recv init check command timeout");
        return ERR_TIMEOUT;
    }

    //检测初始化结果
    if(init_check_command.rx_command.state != 0x00) {
        UNLOCK(init_check_command);
        LOG_W("Under initialization,init falg:%02x", init_check_command.rx_command.init_flag);
        return ERR_BUSY;
    }

    UNLOCK(init_check_command);

    return ERR_OK;
}

int32_t antenna_v1_reset(void)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v1_make_mask(ANTENNA_V1_RX, ANTENNA_V1_SUBSET_ALL) | antenna_v1_make_mask(ANTENNA_V1_TX, ANTENNA_V1_SUBSET_ALL);
    if(mask == 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(reset_command);
    xEventGroupClearBits(reset_command.sync, mask);

    //发送命令
    reset_command.tx_command.reserved = 0x00;
    result = antenna_v1_reset_send(&reset_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(reset_command);
        LOG_E("send reset command failed");
        return result;
    }

    //等待天线回复
    receive_event = xEventGroupWaitBits(reset_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V1_TIMEOUT);
    if((receive_event & mask) != mask) {
        UNLOCK(reset_command);
        LOG_E("reset antenna timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(reset_command);

    return ERR_OK;
}

int32_t antenna_v1_read_versions(uint8_t TorR, uint8_t subset, antenna_v1_version_t *verversions)
{
    int32_t result;

    if(TorR != ANTENNA_V1_ACU) {
        //读天线阵面版本
        if((antenna_v1_make_number(TorR, subset) < 0) || (!verversions)) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
    }
    else {
        //读ACU逻辑版本
        if(!verversions) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
    }

    LOCK(versions_command);
    xSemaphoreTake(versions_command.sync, 0);

    //发送版本读取命令
    versions_command.tx_command.TorR = TorR;
    versions_command.tx_command.subset = subset;
    result = antenna_v1_versions_send(&versions_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(versions_command);
        LOG_E("send versions command failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(versions_command.sync, ANTENNA_V1_TIMEOUT) != pdTRUE) {
        UNLOCK(versions_command);
        LOG_E("recv versions command timeout");
        return ERR_TIMEOUT;
    }

    //提取版本信息
    verversions->major = versions_command.rx_command.major;
    verversions->minor = versions_command.rx_command.minor;
    verversions->amend = versions_command.rx_command.amend;
    verversions->data = versions_command.rx_command.data;

    UNLOCK(versions_command);

    return ERR_OK;
}

int32_t antenna_v1_read_power(uint8_t TorR, uint8_t subset, antenna_v1_power_t *power)
{
    int32_t result;

    if((antenna_v1_make_number(TorR, subset) < 0) || (!power)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(power_command);
    xSemaphoreTake(power_command.sync, 0);

    //发送功率读取命令
    power_command.tx_command.TorR = TorR;
    power_command.tx_command.subset = subset;
    result = antenna_v1_power_send(&power_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(power_command);
        LOG_E("send voltage command failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(power_command.sync, ANTENNA_V1_TIMEOUT) != pdTRUE) {
        UNLOCK(power_command);
        LOG_E("recv voltage command timeout");
        return ERR_TIMEOUT;
    }

    //提取总电压、电压
    power->total_voltage = power_command.rx_command[0].total_voltage/1000.0f;
    power->total_electricity = power_command.rx_command[0].total_electricity/1000.0f;
    //计算功率
    power->total_power = power->total_electricity * power->total_voltage;

    UNLOCK(power_command);

    return ERR_OK;
}

int32_t antenna_v1_set_beam(uint8_t TorR, const antenna_v1_beam_t *beam, uint8_t block_flag)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v1_make_mask(TorR, ANTENNA_V1_SUBSET_ALL);
    if((mask == 0) || (!beam)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beam_command);

#ifndef USING_GLOBAL_LOCK
    if(block_flag) {
        xEventGroupClearBits(beam_command.sync, mask);
    }
#else
    xEventGroupClearBits(beam_command.sync, mask);
#endif


    //发送波束控制命令
    beam_command.tx_command.TorR = TorR;
    beam_command.tx_command.polarity = beam->polarity;
    beam_command.tx_command.theta = beam->theta;
    beam_command.tx_command.phi = beam->phi;
    beam_command.tx_command.weight = beam->weight;
    beam_command.tx_command.freq = beam->freq;
#ifdef ANTENNA_V1_TX_ROTATE
    if(TorR == ANTENNA_V1_TX) {
        //旋转发射指向
        beam_command.tx_command.phi += ANTENNA_V1_TX_ROTATE;
        beam_command.tx_command.phi = fmod(beam_command.tx_command.phi, 360.0f);
        if(beam_command.tx_command.phi < 0.0f) {
            beam_command.tx_command.phi += 360.0f;
        }
        beam_command.tx_command.polarity += ANTENNA_V1_TX_ROTATE;
        beam_command.tx_command.polarity = fmod(beam_command.tx_command.polarity, 180.0f);
        if(beam_command.tx_command.polarity < 0.0f) {
            beam_command.tx_command.polarity += 180.0f;
        }
    }
#endif
    result = antenna_v1_beam_send(&beam_command.tx_command);
    XTime now;

    XTime_GetTime(&now);
    beam_send_time = now;
    beam_timing_enable = 1;

    if(result != ERR_OK) {
        UNLOCK(beam_command);
        LOG_E("set beam send failed");
        return result;
    }

#ifndef USING_GLOBAL_LOCK
    //等待天线回复
    if(block_flag) {
        receive_event = xEventGroupWaitBits(beam_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V1_TIMEOUT);
        if((receive_event & mask) != mask) {
            UNLOCK(beam_command);
            LOG_E("set beam timeout");
            return ERR_TIMEOUT;
        }
    }
#else
    receive_event = xEventGroupWaitBits(beam_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V1_TIMEOUT);
    LOG_E("receive_event = %2x, mask = %2x\n\r" ,receive_event ,mask);
    if((receive_event & mask) != mask) {
        UNLOCK(beam_command);
        LOG_E("set beam timeout");
        return ERR_TIMEOUT;
    }
#endif

    UNLOCK(beam_command);

    return ERR_OK;
}

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
int32_t antenna_v1_offset(uint8_t TorR, const antenna_v1_beam_t *beam)
{
    int32_t result;
    uint32_t mask;
    antenna_v1_point_t point;
    antenna_v1_offset_t offset_data;

    mask = antenna_v1_make_mask(TorR, ANTENNA_V1_SUBSET_ALL);
    if((mask == 0) || (!beam)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //接收不需要补偿
    if(TorR == ANTENNA_V1_RX) {
        return ERR_OK;
    }

    LOCK(offset_command);

    //查找校准数据
    point.freq = beam->freq;
    point.theta = beam->theta;
    point.phi = beam->phi;
    point.polarity = beam->polarity;
    point.polarity_type = beam->polarity_mode;
#ifdef ANTENNA_V1_TX_ROTATE
    if(TorR == ANTENNA_V1_TX) {
        //旋转发射指向
        point.phi += ANTENNA_V1_TX_ROTATE;
        if(point.phi > 360.0f) {
            point.phi -= 360.0f;
        }
        if(ANTENNA_V1_TX_ROTATE > 180.0f) {
            point.polarity += ANTENNA_V1_TX_ROTATE - 180.0f;
        }else {
            point.polarity += ANTENNA_V1_TX_ROTATE;
        }
        if(point.polarity > 180.0f) {
            point.polarity -= 180.0f;
        }
    }
#endif
    result = antenna_v1_offset_find(TorR, &point, &offset_data);
    if(result != ERR_OK) {
        UNLOCK(offset_command);
        LOG_E("find offset data failed");
        return result;
    }

    //发送补偿数据
    offset_command.tx_command.TorR = TorR;
    offset_command.tx_command.phase = offset_data.phase;
    offset_command.tx_command.range = offset_data.range;
    result = antenna_v1_offset_send(&offset_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(offset_command);
        LOG_E("set beam send failed");
        return result;
    }

    UNLOCK(offset_command);

    return ERR_OK;
}
#endif

int32_t antenna_v1_config(uint8_t TorR, uint8_t polarity_mode)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v1_make_mask(TorR, ANTENNA_V1_SUBSET_ALL);
    if(mask == 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(config_command);
    xEventGroupClearBits(config_command.sync, mask);

    //发送配置极化命令
    config_command.tx_command.TorR = TorR;
    config_command.tx_command.polarity_type = polarity_mode;
    result = antenna_v1_config_send(&config_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(config_command);
        LOG_E("set config antenna failed");
        return result;
    }

    //等待天线回复
    receive_event = xEventGroupWaitBits(config_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V1_TIMEOUT);
    if((receive_event & mask) != mask) {
        UNLOCK(config_command);
        LOG_E("config antenna timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(config_command);

    return ERR_OK;
}

int32_t antenna_v1_ctrl_rf_vcc(uint8_t TorR, uint8_t subset, uint8_t sw_state)
{
    int32_t result;
    uint32_t mask;
    uint32_t receive_event;

    mask = antenna_v1_make_mask(TorR, subset);
    if(mask == 0) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(rf_vcc_command);
    xEventGroupClearBits(rf_vcc_command.sync, mask);

    //发送电源控制命令
    rf_vcc_command.tx_command.TorR = TorR;
    rf_vcc_command.tx_command.subset = subset;
    rf_vcc_command.tx_command.sw_state = sw_state;
    result = antenna_v1_rf_vcc_send(&rf_vcc_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(rf_vcc_command);
        LOG_E("set config rf_vcc failed");
        return result;
    }

    //等待天线回复
    receive_event = xEventGroupWaitBits(rf_vcc_command.sync, mask, pdTRUE, pdTRUE, ANTENNA_V1_TIMEOUT);
    if((receive_event & mask) != mask) {
        UNLOCK(rf_vcc_command);
        LOG_E("config rf_vcc timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(rf_vcc_command);

    return ERR_OK;
}

int32_t antenna_v1_read_temperature(uint8_t TorR, uint8_t subset, uint16_t chip_number, antenna_v1_temperature_t *temperature)
{
    int32_t result;

    if((antenna_v1_make_number(TorR, subset) < 0) || (!temperature)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(temperature_command);
    xSemaphoreTake(temperature_command.sync, 0);

    //发送初始化检测命令
    temperature_command.tx_command.TorR = TorR;
    temperature_command.tx_command.subset = subset;
    temperature_command.tx_command.chip_number = chip_number;
    result = antenna_v1_temperature_send(&temperature_command.tx_command);
    if(result != ERR_OK) {
        UNLOCK(temperature_command);
        LOG_E("send temperature command failed");
        return result;
    }

    //等待天线回复
    if(xSemaphoreTake(temperature_command.sync, ANTENNA_V1_TIMEOUT) != pdTRUE) {
        UNLOCK(temperature_command);
        LOG_E("recv temperature command timeout");
        return ERR_TIMEOUT;
    }

    //提取温度数据
    memcpy(&temperature->board_temperature,
            temperature_command.rx_command.board_temperature,
                   sizeof(temperature_command.rx_command.board_temperature));
    temperature->chip_temperature = temperature_command.rx_command.chip_temperature;

    UNLOCK(temperature_command);

    return ERR_OK;
}

int32_t antenna_v1_set_callbk(antenna_v1_receive_callbk callbk)
{
    receive_callbk = callbk;
    return ERR_OK;
}

int32_t antenna_v1_ctrl_init(void)
{
    TaskHandle_t task_handle;
    portBASE_TYPE xresult;

    //初始化软件资源
#ifndef USING_GLOBAL_LOCK
    init_check_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!init_check_command.lock) {
        LOG_E("init_check command initialize failed");
        return ERR_NOMEM;
    }
#endif
    init_check_command.sync = xSemaphoreCreateBinary();
    if(!init_check_command.sync) {
        LOG_E("init_check command initialize failed");
        return ERR_NOMEM;
    }

#ifndef USING_GLOBAL_LOCK
    reset_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!reset_command.lock) {
        LOG_E("reset command initialize failed");
        return ERR_NOMEM;
    }
#endif
    reset_command.sync = xEventGroupCreate();
    if(!reset_command.sync) {
        LOG_E("reset command initialize failed");
        return ERR_NOMEM;
    }

#ifndef USING_GLOBAL_LOCK
    versions_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!versions_command.lock) {
        LOG_E("versions command initialize failed");
        return ERR_NOMEM;
    }
#endif
    versions_command.sync = xSemaphoreCreateBinary();
    if(!versions_command.sync) {
        LOG_E("versions command initialize failed");
        return ERR_NOMEM;
    }

#ifndef USING_GLOBAL_LOCK
    power_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!power_command.lock) {
        LOG_E("voltage command initialize failed");
        return ERR_NOMEM;
    }
#endif
    power_command.sync = xSemaphoreCreateBinary();
    if(!power_command.sync) {
        LOG_E("voltage command initialize failed");
        return ERR_NOMEM;
    }
    power_command.frame_count = 0;
    power_command.temp_count = 0;

#ifndef USING_GLOBAL_LOCK
    beam_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!beam_command.lock) {
        LOG_E("beam command initialize failed");
        return ERR_NOMEM;
    }
#endif
    beam_command.sync = xEventGroupCreate();
    if(!beam_command.sync) {
        LOG_E("beam command initialize failed");
        return ERR_NOMEM;
    }

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
#ifndef USING_GLOBAL_LOCK
    offset_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!offset_command.lock) {
        LOG_E("reboot command initialize failed");
        return ERR_NOMEM;
    }
#endif
#endif

#ifndef USING_GLOBAL_LOCK
    config_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!config_command.lock) {
        LOG_E("config command initialize failed");
        return ERR_NOMEM;
    }
#endif
    config_command.sync = xEventGroupCreate();
    if(!config_command.sync) {
        LOG_E("config command initialize failed");
        return ERR_NOMEM;
    }

#ifndef USING_GLOBAL_LOCK
    rf_vcc_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!rf_vcc_command.lock) {
        LOG_E("rf_vcc command initialize failed");
        return ERR_NOMEM;
    }
#endif
    rf_vcc_command.sync = xEventGroupCreate();
    if(!rf_vcc_command.sync) {
        LOG_E("rf_vcc command initialize failed");
        return ERR_NOMEM;
    }

#ifndef USING_GLOBAL_LOCK
    temperature_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!temperature_command.lock) {
        LOG_E("temperature command initialize failed");
        return ERR_NOMEM;
    }
#endif
    temperature_command.sync = xSemaphoreCreateBinary();
    if(!temperature_command.sync) {
        LOG_E("temperature command initialize failed");
        return ERR_NOMEM;
    }

#ifdef USING_GLOBAL_LOCK
    global_lock = xSemaphoreCreateRecursiveMutex();
    if(!global_lock) {
        LOG_E("global_lock initialize failed");
        return ERR_NOMEM;
    }
#endif

    //创建解包线程
    xresult = xTaskCreate(parse_thread,
                          "antenna_v1_parse",
                          1024*6,
                          NULL,
                          configMAX_PRIORITIES - 2,
                          &task_handle);
    if(xresult != pdPASS) {
        LOG_E("create task antenna_v1_parse failed");
        return ERR_NOMEM;
    }




    return ERR_OK;
}

#endif /* USING_ANTENNA_V1 */
