/*
 * beacon.c
 *
 *  Created on: 2021年4月6日
 *      Author: Administrator
 */

#include "beacon.h"

#define DEBUG_TAG "beacon,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_BEACON

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive(x->lock, portMAX_DELAY) != pdTRUE ) {        \
        LOG_E("lock failed");                                               \
            return -1;                                                      \
    }                                                                       \
    xSemaphoreTake(x->sync, 0);                                             \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive(x->lock) != pdPASS) {                        \
        LOG_E("unlock failed");                                             \
        return -1;                                                          \
    }                                                                       \
}while(0)

#define FRAME_CHECK(data, current_lenght, total_lenght) \
do{                                                     \
    if((current_lenght) < (total_lenght)) {             \
        return INCOMPLETE;                              \
    }                                                   \
    if((current_lenght) > (total_lenght)) {             \
        return BAD;                                     \
    }                                                   \
    if(data[(current_lenght)-1] != 0x7D) {              \
        return BAD;                                     \
    }                                                   \
}while(0)

typedef enum{
    OK = 0,         //解析完成
    INCOMPLETE = 1, //正在解析
    BAD = 2,        //解析出错
}parse_state_t;

typedef struct{
    uint8_t buffer[22];
    uint32_t receive_counter;
}beacon_msg_t;

static beacon_config_t beacon_config[] =
{
#ifdef BEACON0_CONFIG
    BEACON0_CONFIG,
#endif
};

static beacon_t LX_beacon[sizeof(beacon_config)/sizeof(beacon_config[0])];

static int cnr_cnt;
//返回 OK成功处理完此帧，BAD数据格式错误，INCOMPLETE不完整的数据帧
static parse_state_t beacon_parse_byte(beacon_t *beacon, beacon_msg_t *msg, uint8_t data)
{
    int32_t tmp_i32;
    int32_t release_semaphore;

    msg->buffer[msg->receive_counter] = data;
    msg->receive_counter++;

    //命令以0X7B开头，最大长度22B
    if((msg->buffer[0] != 0x7B) || (msg->receive_counter > MAX_FRAME_LENGHT)) {
        return BAD;
    }
    //命令不可能小于5B
    if(msg->receive_counter < 5) {
        return INCOMPLETE;
    }

    release_semaphore = 0;
    switch(msg->buffer[1]) {
        //设置或查询工作模式
        case 0x09:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '=')) {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //设置或查询DVB参数
        case 0x0B:
            if(msg->buffer[2] == '<') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            else if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 12);
                memcpy(beacon->receive_buffer, msg->buffer, 12);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //设置或查询 DVB 搜索参数（高级）
        case 0x03:
            if(msg->buffer[2] == '<') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            else if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 22);
                memcpy(beacon->receive_buffer, msg->buffer, 22);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //上报载噪比
        case 0x02:
            if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 8);
                //进入临界区
                taskENTER_CRITICAL();
                //提取原始数据
                BIG32_2_LITTLE32(&tmp_i32, &msg->buffer[3]);
                //进行解算
                beacon->data.cnr_power = tmp_i32 / 100.0f;
//                if (cnr_cnt > 1000) {
//                	LOG_I("---cnr=%.3f", beacon->data.cnr_power);
//                	cnr_cnt = 0;
//                }
//                else {
//                	cnr_cnt++;
//                }
                beacon->data.filter_cnr = biquad_filter_apply(&beacon->filter, beacon->data.cnr_power);
                beacon->data.freq = beacon->dvb_freq;
                beacon->data.sym = beacon->dvb_sym;
                beacon->data.rang = beacon->beacon_rang;
                beacon->data.mode = beacon->mode;
                //退出临界区
                taskEXIT_CRITICAL();
                break;
            }
            return BAD;
        //设置或查询信标频率
        case 0x08:
            if(msg->buffer[2] == '<') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            else if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 8);
                memcpy(beacon->receive_buffer, msg->buffer, 8);
//                LOG_I("---%x，%x，%x，%x", beacon->receive_buffer[0], beacon->receive_buffer[1],beacon->receive_buffer[2], beacon->receive_buffer[3]);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //设置或查询信标捕获范围
        case 0x0c:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '=')) {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //设置或查询信标刷新周期
        case 0x0A:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '=')) {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //上报接收功率电平（信标模式）
        case 0x07:
            if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 8);
                //进入临界区
                taskENTER_CRITICAL();
                //提取原始数据
                BIG32_2_LITTLE32(&tmp_i32, &msg->buffer[3]);
                //进行解算
                beacon->data.cnr_power = tmp_i32 / 1000.0f;
                beacon->data.filter_cnr = biquad_filter_apply(&beacon->filter, beacon->data.cnr_power);
                beacon->data.freq = beacon->beacon_freq;
                beacon->data.sym = beacon->dvb_sym;
                beacon->data.rang = beacon->beacon_rang;
                beacon->data.mode = beacon->mode;
                //退出临界区
                taskEXIT_CRITICAL();
                break;
            }
            return BAD;
        //设置或查询检波参数
        case 0x0E:
            if(msg->buffer[2] == '<')  {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            else if(msg->buffer[2] == '=')  {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 15);
                memcpy(beacon->receive_buffer, msg->buffer, 15);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //上报接收功率电平（检波模式）
        case 0x0D:
            if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 8);
                //进入临界区
                taskENTER_CRITICAL();
                //提取原始数据
                BIG32_2_LITTLE32(&tmp_i32, &msg->buffer[3]);
                //进行解算
                beacon->data.cnr_power = tmp_i32 / 1000.0f;
                beacon->data.filter_cnr = biquad_filter_apply(&beacon->filter, beacon->data.cnr_power);
                beacon->data.freq = beacon->detection_freq;
                beacon->data.sym = beacon->detection_sym;
                beacon->data.rang = beacon->beacon_rang;
                beacon->data.mode = beacon->mode;
                //退出临界区
                taskEXIT_CRITICAL();
                break;
            }
            return BAD;
        //设置或查询电压映射
        case 0x10:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '='))
            {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //上报映射电压
        case 0x0F:
            if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 6);
                break;
            }
            return BAD;
        //设置或查询馈电
        case 0x05:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '=')) {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;
        //设置或查询 22KHz 单音
        case 0x06:
            if((msg->buffer[2] == '<') || (msg->buffer[2] == '=')) {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 5);
                memcpy(beacon->receive_buffer, msg->buffer, 5);
                release_semaphore = 1;
                break;
            }
            return BAD;

        //查询设备版本信息
        case 'V':
            if(msg->buffer[2] == '=') {
                FRAME_CHECK(msg->buffer, msg->receive_counter, 8);
                memcpy(beacon->receive_buffer, msg->buffer, 8);
                release_semaphore = 1;
                break;
            }
            return BAD;
        default:
            return BAD;
    }

    //调用接收完成回调
    if(beacon->callbk) {
        beacon->callbk(&beacon->device, msg->buffer, msg->receive_counter);
    }

    //释放信号量
    if(release_semaphore) {
        if(xSemaphoreGive(beacon->sync) != pdPASS) {
            LOG_E("release semaphore failed");
        }
    }

    return OK;
}

static void beacon_thread(void *arg)
{
    int32_t err_out;
    ssize_t i;
    ssize_t lenght;
    uint8_t buffer[22];
    parse_state_t state;
    beacon_msg_t msg;
    beacon_t *beacon = (beacon_t*)arg;

    if((!beacon) || (device_check(&beacon->device, BEACON_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        vTaskDelete(NULL);
    }

    err_out = 0;
    //重置解析计数器
    msg.receive_counter = 0;
    while(1)
    {
        //从串口读取数据
        lenght = device_read(beacon->uart, buffer, sizeof(buffer));
        if(lenght <= 0) {
            continue;
        }

        for(i=0; i<lenght; i++) {
            //进行解包
            state = beacon_parse_byte(beacon, &msg, buffer[i]);

            //接收未完成
            if(state == INCOMPLETE) {
                continue;
            }
            //连续3次收到正确数据包后开启解析出错输出
            if((state == OK) && (err_out < 5)) {
                err_out++;
            }
            //数据格式错误
            if((state == BAD) && (err_out >= 5)) {
                LOG_E("frame error");
            }

            //重置解析计数器
            msg.receive_counter = 0;
        }
    }
}

static struct device_ops ops ={
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = NULL,
    .pwrite = NULL,
    .control = NULL,
};

int32_t beacon_init(void)
{
    uint32_t i;
    int32_t result;
    portBASE_TYPE xresult;
    TaskHandle_t task_handle;

    for(i=0; i<sizeof(LX_beacon)/sizeof(LX_beacon[0]); i++) {
        LX_beacon[i].config = &beacon_config[i];
        LX_beacon[i].data.mode = 0xff;
        LX_beacon[i].data.rang = 0xff;
        LX_beacon[i].data.sym = -1.0f;
        LX_beacon[i].data.freq = -1.0f;
        LX_beacon[i].data.cnr_power = -10.0f;
        //默认工作参数
        LX_beacon[i].mode = 0xff;
        LX_beacon[i].dvb_sym = -1.0f;
        LX_beacon[i].dvb_freq = -1.0f;
        LX_beacon[i].detection_sym = -1.0f;
        LX_beacon[i].detection_freq = -1.0f;
        LX_beacon[i].beacon_freq = -1.0f;
        LX_beacon[i].beacon_rang = 0xff;

        //创建互斥锁
        LX_beacon[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!LX_beacon[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }
        //创建同步信号量
        LX_beacon[i].sync = xSemaphoreCreateBinary();
        if(!LX_beacon[i].sync) {
            LOG_E("create sync semaphore failed");
            return ERR_NOMEM;
        }
        //默认没有回调函数
        LX_beacon[i].callbk = NULL;

        //初始化低通滤波器
        result = biquad_filter_init_LPF(&LX_beacon[i].filter, 250.0f, 25.0f);
        if(result != ERR_OK) {
            LOG_E("filter init failed");
            return result;
        }

        //打开串口设备
        LX_beacon[i].uart = device_open(LX_beacon[i].config->uart_name, DEVICE_RDWR);
        if(!LX_beacon[i].uart) {
            LOG_E("open uart failed");
            return ERR_IO;
        }

        //注册信标机设备
        LX_beacon[i].device.magic = BEACON_MAGIC;
        LX_beacon[i].device.ops = &ops;
        result = device_register(&LX_beacon[i].device, LX_beacon[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }

        //创建信标机回传数据解析线程
        xresult = xTaskCreate(beacon_thread,
                             "beacon_thread",
                             2048,
                             &LX_beacon[i],
                             configMAX_PRIORITIES - 2,
                             &task_handle );
        if(xresult != pdPASS) {
            LOG_E("create task beacon_thread failed");
            return ERR_NOMEM;
        }

        //开启电源
        power_on(LX_beacon[i].config->power);
    }
    return ERR_OK;
}

int32_t beacon_set_mode(device_t *device, uint8_t mode)
{
    uint8_t cmd[5] = {0X7B, 0X09, '>', mode, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //如果新的模式与之前的一致则不用配置
    if(beacon->mode != mode) {
        //复位信号量
        xSemaphoreTake(beacon->sync, 0);
        //发送设置模式命令
        if(device_write(beacon->uart, cmd, 5) != 5) {
            UNLOCK(beacon);
            LOG_E("write data to beacon failed");
            return ERR_IO;
        }

        //等待命令反馈标志
        if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
            UNLOCK(beacon);
            LOG_E("wait timeout");
            return ERR_TIMEOUT;
        }

        //检查命令执行结果
        if(beacon->receive_buffer[3] != 1) {
            UNLOCK(beacon);
            LOG_E("set mode failed");
            return ERR_IO;
        }

        //缓存模式设置
        beacon->mode = mode;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_get_mode(device_t *device, uint8_t *mode)
{
    uint8_t cmd[4] = {0X7B, 0X09, '?',  0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(!mode) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //从缓存获取工作模式
    if(beacon->mode != 0xff) {
        *mode = beacon->mode;
        UNLOCK(beacon);
        return ERR_OK;
    }

    xSemaphoreTake(beacon->sync, 0);
    //发送查询模式命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //获取工作模式
    *mode = beacon->receive_buffer[3];
    //缓存工作模式
    beacon->mode = *mode;

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_DVB(device_t *device, float freq, float sym)
{
    uint32_t freq_hz;
    uint32_t sym_sps;
    uint8_t cmd[12] = {0X7B, 0X0B, '>', 0X00, 0x00, 0x00, 0x00, 0X00, 0x00, 0x00, 0x00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    if((fabs(beacon->dvb_freq - freq) > 0.001f) || (fabs(beacon->dvb_sym - sym) > 0.001f)) {
        //进行单位转换
        freq_hz = (uint32_t)(freq * 1000.0f + 0.5f);
        sym_sps = (uint32_t)(sym * 1000.0f + 0.5f);

        //f复位信号量
        xSemaphoreTake(beacon->sync, 0);
        //发送设置DVB命令
        LITTLE32_2_BIG32(&cmd[3], &freq_hz);
        LITTLE32_2_BIG32(&cmd[7], &sym_sps);
        if(device_write(beacon->uart, cmd, 12) != 12) {
            UNLOCK(beacon);
            LOG_E("write data to beacon failed");
            return ERR_IO;
        }

        //等待命令反馈标志
        if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
            UNLOCK(beacon);
            LOG_E("wait timeout");
            return ERR_TIMEOUT;
        }

        //检查反馈结果
        if(beacon->receive_buffer[3] != 1) {
            UNLOCK(beacon);
            LOG_E("set DVB failed");
            return ERR_IO;
        }

        //缓存DVB设置
        beacon->dvb_freq = freq;
        beacon->dvb_sym = sym;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_get_DVB(device_t *device, float *freq, float *sym)
{
    uint32_t freq_hz;
    uint32_t sym_sps;
    uint8_t cmd[4] = {0X7B, 0X0B, '?',  0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if((!freq) || (!sym)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //从缓存读取数据
    if((beacon->dvb_freq > 0.0f) && (beacon->dvb_sym > 0.0f)) {
        *freq = beacon->dvb_freq;
        *sym = beacon->dvb_sym;
        UNLOCK(beacon);
        return ERR_OK;
    }

    xSemaphoreTake(beacon->sync, 0);
    //发送查询DVB命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //获取载波频率和符号速率
    BIG32_2_LITTLE32(&freq_hz, &beacon->receive_buffer[3]);
    BIG32_2_LITTLE32(&sym_sps, &beacon->receive_buffer[7]);
    *freq  = freq_hz / 1000.0f;
    *sym = sym_sps / 1000.0f;
    //缓存载波频率和符号速率
    beacon->dvb_freq = *freq;
    beacon->dvb_sym = *sym;

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_freq(device_t *device, float freq)
{
    uint32_t freq_hz;
    uint8_t cmd[8] = {0X7B, 0X08, '>', 0X00, 0x00, 0x00, 0x00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //与缓存不一致才重新配置
    if(fabs(beacon->beacon_freq - freq) > 0.001f) {
        //进行单位转换
        freq_hz = (uint32_t)(freq * 1000.0f + 0.5f);

        //复位信号量
        xSemaphoreTake(beacon->sync, 0);
        //发送设置信标频率的命令
        LITTLE32_2_BIG32(&cmd[3], &freq_hz);
        if(device_write(beacon->uart, cmd, 8) != 8) {
            UNLOCK(beacon);
            LOG_E("write data to beacon failed");
            return ERR_IO;
        }

        //等待命令反馈标志
        if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
            UNLOCK(beacon);
            LOG_E("wait timeout");
            return ERR_TIMEOUT;
        }

        //检查反馈结果
        if(beacon->receive_buffer[3] != 1) {
            UNLOCK(beacon);
            LOG_E("set freq failed");
            return ERR_IO;
        }

        //缓存信标频率
        beacon->beacon_freq = freq;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_get_freq(device_t *device, float *freq)
{
    uint32_t freq_hz;
    uint8_t cmd[4] = {0X7B, 0X08, '?', 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(!freq) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //从缓存读取数据
    if(beacon->beacon_freq > 0.0) {
        *freq = beacon->beacon_freq;
        UNLOCK(beacon);
        return ERR_OK;
    }

    xSemaphoreTake(beacon->sync, 0);
    //发送查询信标频率的命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //获取信标频率
    BIG32_2_LITTLE32(&freq_hz, &beacon->receive_buffer[3]);
    *freq  = freq_hz /1000.0f;
    //缓存信标频率
    beacon->beacon_freq = *freq;

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_range(device_t *device, uint8_t rang)
{
    uint8_t cmd[5] = {0X7B, 0X0C, '>', 0X00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //与缓存不一致才配置
    if(beacon->beacon_rang != rang) {
        //复位信号量
        xSemaphoreTake(beacon->sync, 0);
        //发送设置信标捕获范围的命令
        cmd[3] = rang;
        if(device_write(beacon->uart, cmd, 5) != 5) {
            UNLOCK(beacon);
            LOG_E("write data to beacon failed");
            return ERR_IO;
        }

        //等待命令反馈标志
        if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
            UNLOCK(beacon);
            LOG_E("wait timeout");
            return ERR_TIMEOUT;
        }

        //检查反馈结果
        if(beacon->receive_buffer[3] != 1) {
            UNLOCK(beacon);
            LOG_E("set range failed");
            return ERR_IO;
        }

        //缓存信标捕获范围
        beacon->beacon_rang = rang;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_get_range(device_t *device, uint8_t *rang)
{
    uint8_t cmd[4] = {0X7B, 0X0C, '?', 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(!rang) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //从缓存读取数据
    if(beacon->beacon_rang != 0xFF) {
        *rang = beacon->beacon_rang;
        UNLOCK(beacon);
        return ERR_OK;
    }

    xSemaphoreTake(beacon->sync, 0);
    //发送查询信标捕获范围的命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //获取信标捕获范围
    *rang  = beacon->receive_buffer[3];
    //缓存信标捕获范围
    beacon->beacon_rang = *rang;

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_refresh(device_t *device, uint8_t period)
{
    uint8_t cmd[5] = {0X7B, 0X0A, '>', 0x00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送设置信标刷新周期命令
    cmd[3] = period;
    if(device_write(beacon->uart, cmd, 5) != 5) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //检查反馈结果
    if(beacon->receive_buffer[3] != 1) {
        UNLOCK(beacon);
        LOG_E("set refresh failed");
        return ERR_IO;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_detection(device_t *device, float freq, float sym)
{
    uint32_t freq_hz;
    uint32_t sym_sps;
    uint16_t range;
    uint8_t cmd[15] = {0X7B, 0X0E, '>', 0X00, 0x00, 0x00, 0x00, 0X00, 0x00, 0x00, 0x00, 0X00, 0x00, 35, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //与之前的不一样才进行配置
    if((fabs(beacon->detection_freq - freq) > 0.001f) || (fabs(beacon->detection_sym -sym) > 0.001f)) {
        //进行单位转换
        freq_hz = (uint32_t)(freq * 1000.0f + 0.5f);
        sym_sps = (uint32_t)(sym * 1000.0f + 0.5f);
        range = 200;
        LITTLE32_2_BIG32(&cmd[3], &freq_hz);
        LITTLE32_2_BIG32(&cmd[7], &sym_sps);
        LITTLE16_2_BIG16(&cmd[11], &range);

        //复位信号量
        xSemaphoreTake(beacon->sync, 0);
        //发送设置检波参数的命令
        cmd[13] = 35;
        if(device_write(beacon->uart, cmd, 15) != 15) {
            UNLOCK(beacon);
            LOG_E("write data to beacon failed");
            return ERR_IO;
        }

        //等待命令反馈标志
        if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
            UNLOCK(beacon);
            LOG_E("wait timeout");
            return ERR_TIMEOUT;
        }

        //检查反馈结果
        if(beacon->receive_buffer[3] != 1) {
            UNLOCK(beacon);
            LOG_E("set detection failed");
            return ERR_IO;
        }

        //缓存检波参数设置
        beacon->detection_freq = freq;
        beacon->detection_sym = sym;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_get_detection(device_t *device, float *freq, float *sym)
{
    uint32_t freq_hz;
    uint32_t sym_sps;
    uint8_t cmd[4] = {0X7B, 0X0E, '?', 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if((!freq) || (!sym)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //从缓存读取数据
    if((beacon->detection_freq > 0.0f) && (beacon->detection_sym > 0.0f)) {
        *freq = beacon->detection_freq;
        *sym = beacon->detection_sym;
        UNLOCK(beacon);
        return ERR_OK;
    }

    xSemaphoreTake(beacon->sync, 0);
    //发送查询DVB命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //获取检波参数
    BIG32_2_LITTLE32(&freq_hz, &beacon->receive_buffer[3]);
    BIG32_2_LITTLE32(&sym_sps, &beacon->receive_buffer[7]);
    *freq  = freq_hz /1000.0f;
    *sym = sym_sps/1000.0f;
    //缓存检波参数
    beacon->detection_freq = *freq;
    beacon->detection_sym = *sym;

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_voltage_map(device_t *device, uint8_t on_off)
{
    uint8_t cmd[5] = {0X7B, 0X10, '>', 0X00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送设置电压映射命令
    cmd[3] = on_off;
    if(device_write(beacon->uart, cmd, 5) != 5) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //检查反馈结果
    if(beacon->receive_buffer[3] != 1) {
        UNLOCK(beacon);
        LOG_E("set voltage map failed");
        return ERR_IO;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_feed(device_t *device, uint8_t level)
{
    uint8_t cmd[5] = {0X7B, 0X05, '>', 0X00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送设置馈电的命令
    cmd[3] = level;
    if(device_write(beacon->uart, cmd, 5) != 5) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //检查反馈结果
    if(beacon->receive_buffer[3] != 1) {
        UNLOCK(beacon);
        LOG_E("set set feed failed");
        return ERR_IO;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_set_single_tone(device_t *device, uint8_t on_off)
{
    uint8_t cmd[5] = {0X7B, 0X06, '>', 0X00, 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送设置单音的命令
    cmd[3] = on_off;
    if(device_write(beacon->uart, cmd, 5) != 5) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //检查反馈结果
    if(beacon->receive_buffer[3] != 1) {
        UNLOCK(beacon);
        LOG_E("set single tone failed");
        return ERR_IO;
    }

    UNLOCK(beacon);
    return ERR_OK;
}

int32_t beacon_read_version(device_t *device, uint32_t *version)
{
    uint8_t cmd[4] = {0X7B, 'V', '?', 0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送版本查询的命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_TIMEOUT;
    }

    //读取版本信息
    memcpy(version, &beacon->receive_buffer[3], 4);

    UNLOCK(beacon);

    return ERR_OK;
}

int32_t beacon_read_data(device_t *device, beacon_data_t *data)
{
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(!data) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *data = beacon->data;
    taskEXIT_CRITICAL();

    return ERR_OK;
}


int32_t beacon_check(device_t *device)
{
    uint8_t cmd[4] = {0X7B, 0X09, '?',  0X7D};
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    xSemaphoreTake(beacon->sync, 0);
    //发送查询模式命令
    if(device_write(beacon->uart, cmd, 4) != 4) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(beacon->sync, BEACON_TIMEOUT) != pdTRUE) {
        UNLOCK(beacon);
        LOG_E("wait timeout");
        return ERR_IO;
    }

    UNLOCK(beacon);

    return ERR_OK;
}

int32_t beacon_reset(device_t *device, int32_t level)
{
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //复位工作参数
    beacon->mode = 0xff;
    beacon->dvb_sym = -1.0f;
    beacon->dvb_freq = -1.0f;
    beacon->detection_sym = -1.0f;
    beacon->detection_freq = -1.0f;
    beacon->beacon_freq = -1.0f;
    beacon->beacon_rang = 0xff;

    //复位电源
    if(level > 0) {
        power_off(beacon->config->power);
        vTaskDelay(80);
        power_on(beacon->config->power);
        vTaskDelay(80);
    }

    UNLOCK(beacon);

    return ERR_OK;
}

int32_t beacon_write_command(device_t *device, const uint8_t *command, uint32_t lenght)
{
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //发送查询模式命令
    if(device_write(beacon->uart, command, lenght) != lenght) {
        UNLOCK(beacon);
        LOG_E("write data to beacon failed");
        return ERR_IO;
    }

    UNLOCK(beacon);

    return ERR_OK;
}

int32_t beacon_set_receive_callbk(device_t *device, beacon_receive_callbk callbk)
{
    beacon_t *beacon = container_of(device, beacon_t, device);

    if(device_check(device, BEACON_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(beacon);

    //注册回调函数
    beacon->callbk = callbk;

    UNLOCK(beacon);

    return ERR_OK;
}

#endif /* USING_BEACON */
