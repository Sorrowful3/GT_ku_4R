/*
 * rav260.c
 *
 *  Created on: 2021年3月19日
 *      Author: Administrator
 */

#include "rav260.h"

#define DEBUG_TAG "rav260,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../core/debug.h"

#ifdef USING_RAV260

//GPS时间起点;1980-1-6 0:0:0
#define GPS_TIME_START  315936000LL

//修正完成标志
#define CORRECT_MASK    0x00000002
//对准完成标志
#define AIM_MASK        0x00000004
//数据更新标志
#define UPDATE_MASK     0x00000008
//自检标志
#define CHECK_MASK      0x00000010

#define LOCK(x)                                                             \
do{                                                                         \
    if(xSemaphoreTakeRecursive((x)->lock, portMAX_DELAY) != pdPASS) {       \
        LOG_E("lock failed");                                               \
        return ERR_ROUTINE;                                                       \
    }                                                                       \
}while(0)

#define UNLOCK(x)                                                           \
do{                                                                         \
    if(xSemaphoreGiveRecursive((x)->lock) != pdPASS) {                      \
        LOG_E("unlock failed");                                             \
        return ERR_ROUTINE;                                                       \
    }                                                                       \
}while(0)

typedef enum{
    OK = 0,         //解析完成
    INCOMPLETE = 1, //正在解析
    BAD = 2,        //解析出错
}parse_state_t;

typedef struct{
    uint8_t *buffer;
    uint32_t pack_length;
    uint32_t receive_counter;
}rav260_msg_t;

static rav260_config_t rav260_config[1] = {
#ifdef RAV260_0_CONFIG
    RAV260_0_CONFIG,
#endif
#ifdef RAV260_1_CONFIG
    RAV260_1_CONFIG,
#endif
};

static rav260_t rav260[sizeof(rav260_config)/sizeof(rav260_config[0])];

//闰秒
static uint64_t leap_second = 17LL;

parse_state_t rav260_parse_byte(rav260_t *imu, rav260_msg_t *msg, uint8_t data)
{
    uint8_t checksum;
    uint32_t i;

    //缓存一个字节
    msg->buffer[msg->receive_counter] = data;
    msg->receive_counter++;

    //校验帧头
    if(((msg->receive_counter == 1) && (msg->buffer[0] != 0xAA)) || \
            ((msg->receive_counter == 2) && (msg->buffer[1] != 0x55))){
        return BAD;
    }
    //继续接收
    if(msg->receive_counter < msg->pack_length) {
        return INCOMPLETE;
    }
    //和校验
    for(i=0, checksum=0; i<msg->pack_length; i++) {
        checksum += msg->buffer[i];
    }
    if(checksum != 0) {
        return BAD;
    }

    taskENTER_CRITICAL();
    //状态
    imu->orig_data.current_state = msg->buffer[2];
    //角速度
    memcpy(&imu->orig_data.x_speed, &msg->buffer[3], 2);
    memcpy(&imu->orig_data.y_speed, &msg->buffer[5], 2);
    memcpy(&imu->orig_data.z_speed, &msg->buffer[7], 2);
    //姿态角
    memcpy(&imu->orig_data.pitch, &msg->buffer[9], 2);
    memcpy(&imu->orig_data.roll, &msg->buffer[11], 2);
    memcpy(&imu->orig_data.yaw, &msg->buffer[13], 2);
    //经纬高
    memcpy(&imu->orig_data.longitude, &msg->buffer[15], 2);
    memcpy(&imu->orig_data.latitude, &msg->buffer[17], 2);
    memcpy(&imu->orig_data.height, &msg->buffer[19], 2);
    //地速
    imu->orig_data.speed = msg->buffer[21];
    //温度
    imu->orig_data.temperature = msg->buffer[22];
    //时间
    if(msg->pack_length > 24) {
        //GPS周秒
        memcpy(&imu->orig_data.second, &msg->buffer[23], 4);
        //GPS周
        memcpy(&imu->orig_data.week, &msg->buffer[27], 2);
    }
    taskEXIT_CRITICAL();

    //设置数据更新标志
    xEventGroupSetBits(imu->sync, UPDATE_MASK|CHECK_MASK);
    //修正数据有效
    if((imu->orig_data.current_state & 0x01) == 0x01) {
        xEventGroupSetBits(imu->sync, CORRECT_MASK);
    }
    //校准数据有效
    if((imu->orig_data.current_state & 0xF0) == 0X10) {
        xEventGroupSetBits(imu->sync, AIM_MASK);
    }

    return OK;
}

static void rav260_thread(void *arg)
{
    int32_t err_out;
    ssize_t i;
    ssize_t lenght;
    uint8_t *buffer;
    parse_state_t state;
    rav260_msg_t msg;
    rav260_t *imu = (rav260_t *)arg;

    if((!imu) || (device_check(&imu->device, RAV260_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        vTaskDelete(NULL);
    }

    if(imu->config->versions == 0) {
        msg.pack_length = 24;
    }
    else {
        msg.pack_length = 30;
    }
    msg.receive_counter = 0;
    msg.buffer = (uint8_t*)pvPortMalloc(msg.pack_length);
    if(!msg.buffer) {
        LOG_E("malloc failed");
        vTaskDelete(NULL);
    }

    buffer = (uint8_t*)pvPortMalloc(msg.pack_length);
    if(!buffer) {
        LOG_E("malloc failed");
        vTaskDelete(NULL);
    }

    err_out = 0;
    while(1)
    {
        //接收数据
        lenght = device_read(imu->uart, buffer, msg.pack_length);
        if(lenght <= 0) {
            continue;
        }

        for(i=0; i<lenght; i++) {
            state = rav260_parse_byte(imu, &msg, buffer[i]);

            //接收未完成
            if(state == INCOMPLETE) {
                continue;
            }
            //连续3次收到正确数据包后开启解析出错输出
            if((state == OK) && (err_out < 1500)) {
                err_out++;
            }
            //数据格式错误
            if((state == BAD) && (err_out >= 1500)) {
                LOG_E("frame error");
            }

            //重置解析计数器
            msg.receive_counter = 0;
        }
    }
}

static ssize_t __rav260_pread(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    uint8_t block_flag;
    int32_t result;

    if(size < sizeof(rav260_orig_data_t)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(device->oflags & DEVICE_NOBLOCK) {
        block_flag = 0;
    }
    else {
        block_flag = 1;
    }

    result = rav260_orig_data(device, (rav260_orig_data_t*)buffer, block_flag);
    if(result == ERR_OK) {
        return sizeof(rav260_orig_data_t);
    }
    return result;
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = __rav260_pread,
    .pwrite = NULL,
    .control = NULL,
};

int32_t rav260_init(void)
{
    uint32_t i;
    int32_t result;
    portBASE_TYPE xresult;
    TaskHandle_t task_handle;

    for(i=0; i<sizeof(rav260)/sizeof(rav260[0]); i++) {
        rav260[i].config = &rav260_config[i];
        memset(&rav260[i].orig_data, 0, sizeof(rav260_orig_data_t));
        rav260[i].manual_gps = 0;

        //创建互斥锁
        rav260[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!rav260[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }
        //创建同步事件标志
        rav260[i].sync = xEventGroupCreate();
        if(!rav260[i].sync) {
            LOG_E("create sync failed");
            return ERR_NOMEM;
        }

        //打开IMU所使用的串口
        rav260[i].uart = device_open(rav260[i].config->uart_name, DEVICE_RDWR);
        if(!rav260[i].uart) {
            LOG_E("open uart failed");
            return ERR_IO;
        }

        //注册IMU设备
        rav260[i].device.magic = RAV260_MAGIC;
        rav260[i].device.ops = &ops;
        result = device_register(&rav260[i].device, rav260[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }

        //创建解析线程
        xresult = xTaskCreate(rav260_thread,
                             "rav260_thread",
                             3072,
                             &rav260[i],
                             configMAX_PRIORITIES - 2,
                             &task_handle );
        if(xresult != pdPASS) {
            LOG_E("create task rav260_thread failed\r\n");
            return ERR_NOMEM;
        }

        //开启电源
        power_on(rav260[i].config->power);
    }

    return ERR_OK;
}

int32_t rav260_leap_second(uint64_t second)
{
    leap_second = second;
    return ERR_OK;
}

int32_t rav260_correct(device_t *device, uint8_t valid, float correct)
{
    int8_t i;
    uint8_t checksum;
    int16_t temp;
    uint32_t receive_event;
    uint8_t buffer[24] = {0xAA, 0x55, 0x91, valid};
    rav260_t *imu = container_of(device, rav260_t, device);

    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //修正数据
    correct = fmodf(correct, 360.0f);
    if(correct < 0.0f) {
        correct += 360.0f;
    }
    temp = (int16_t)(correct * 100.0f);
    memcpy(&buffer[4], &temp, 2);
    //和校验
    for(i=0, checksum=0; i<23; i++) {
        checksum += buffer[i];
    }
    buffer[23] = 0 - checksum;

    LOCK(imu);

    xEventGroupClearBits(imu->sync, CORRECT_MASK);
    //发送修正命令到IMU
    if(device_write(imu->uart, buffer, 24) != 24) {
        UNLOCK(imu);
        LOG_E("write data to IMU failed");
        return ERR_IO;
    }

    //等待命令执行成功
    receive_event = xEventGroupWaitBits(imu->sync, CORRECT_MASK, pdTRUE, pdTRUE, RAV260_TIMEOUT*10);
    if((receive_event & CORRECT_MASK) != CORRECT_MASK) {
        UNLOCK(imu);
        LOG_E("correct IMU timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(imu);
    return ERR_OK;
}

int32_t rav260_aim(device_t *device)
{
    uint32_t receive_event;
    uint8_t buffer[24] = {0xaa, 0x55, 0xa1, 0xab};
    rav260_t *imu = container_of(device, rav260_t, device);

    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //和校验
    buffer[23] = 0xb5;

    LOCK(imu);

    xEventGroupClearBits(imu->sync, AIM_MASK);
    //发送校准命令到IMU
    if(device_write(imu->uart, buffer, 24) != 24) {
        UNLOCK(imu);
        LOG_E("write data to IMU failed");
        return ERR_IO;
    }

    //等待命令执行成功
    receive_event = xEventGroupWaitBits(imu->sync, AIM_MASK, pdTRUE, pdTRUE, RAV260_TIMEOUT*10);
    if((receive_event & AIM_MASK) != AIM_MASK) {
        UNLOCK(imu);
        LOG_E("correct IMU timeout");
        return ERR_TIMEOUT;
    }

    UNLOCK(imu);
    return ERR_OK;
}

int32_t rav260_manual_gps(device_t *device, int32_t flag, float longitude, float latitude, int16_t height)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    imu->manual_gps = flag;
    imu->manual_longitude = longitude;
    imu->manual_latitude = latitude;
    imu->manual_height = height;
    return ERR_OK;
}

int32_t rav260_orig_data(device_t *device, rav260_orig_data_t *orig_data, uint8_t block_flag)
{
    uint32_t receive_event;
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!orig_data) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(block_flag) {
        //等待数据更新标志位置位
        receive_event = xEventGroupWaitBits(imu->sync, UPDATE_MASK, pdTRUE, pdTRUE, RAV260_TIMEOUT);
        if((receive_event & UPDATE_MASK) != UPDATE_MASK) {
            LOG_E("read IMU timeout");
            return ERR_TIMEOUT;
        }
    }

    //读取IMU数据
    taskENTER_CRITICAL();
    *orig_data = imu->orig_data;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t rav260_time(device_t *device, rav260_time_t *time)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!time) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //此版本惯导无时间信息
    if(imu->config->versions == 0) {
        time->valid_flag = 0;
        return ERR_IO;
    }

    //读取IMU数据
    taskENTER_CRITICAL();
    if((imu->orig_data.current_state & 0xF0) == 0x90) {
        time->valid_flag = 1;
    }
    else {
        time->valid_flag = 0;
    }
    time->second = imu->orig_data.second;
    time->week = imu->orig_data.week;
    taskEXIT_CRITICAL();

    //转换为ms
    time->time_ms = ((GPS_TIME_START - leap_second) * 1000LL);
    time->time_ms += (imu->orig_data.week * 604800LL) *1000LL;
    time->time_ms += imu->orig_data.second;
    //转换为s
    time->time_s = (uint32_t)(time->time_ms / 1000.0 + 0.5);

    return ERR_OK;
}

int32_t rav260_gps(device_t *device, rav260_gps_t *gps)
{
    uint8_t state;
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!gps) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(!imu->manual_gps) {
        //读取IMU数据
        taskENTER_CRITICAL();
        if((imu->orig_data.current_state & 0xF0) == 0x90) {
            gps->valid_flag = 1;
        }
        else {
            gps->valid_flag = 0;
        }
        //经纬高
        gps->longitude = imu->orig_data.longitude / 180.0f;
        gps->latitude = imu->orig_data.latitude / 360.0f;
        gps->height = imu->orig_data.height;
        //地速
        gps->speed = imu->orig_data.speed * 0.2f;
        taskEXIT_CRITICAL();
    }
    else {    //读取IMU数据
        taskENTER_CRITICAL();
        state = imu->orig_data.current_state & 0xF0;
        if((state == 0x70) || (state == 0x90)) {
            gps->valid_flag = 1;
        }
        else {
            gps->valid_flag = 0;
        }
        //经纬高
        gps->longitude = imu->manual_longitude;
        gps->latitude = imu->manual_latitude;
        gps->height = imu->manual_height;
        //地速
        gps->speed = 0;
        taskEXIT_CRITICAL();
    }

    return ERR_OK;
}

int32_t rav260_ins(device_t *device, rav260_ins_t *ins)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!ins) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //读取IMU数据
    taskENTER_CRITICAL();
    if(((imu->orig_data.current_state & 0xF0) == 0x90) || ((imu->orig_data.current_state & 0xF0) == 0x70)) {
        ins->valid_flag = 1;
    }
    else {
        ins->valid_flag = 0;
    }
    //角速度
    ins->x_speed = imu->orig_data.x_speed * 0.0033f;
    ins->y_speed = imu->orig_data.y_speed * 0.0033f;
    ins->z_speed = imu->orig_data.z_speed * 0.0033f;
    //姿态角
    ins->pitch = imu->orig_data.pitch * 0.01f;
    ins->roll = imu->orig_data.roll * 0.01f;
    ins->yaw = imu->orig_data.yaw * 0.01f;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t rav260_state(device_t *device, rav260_state_t *state)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!state) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //读取IMU数据
    taskENTER_CRITICAL();
    state->orig_state = imu->orig_data.current_state;
    taskEXIT_CRITICAL();

    if((state->orig_state & 0xF0) == 0x90) {
        state->state = RAV260_INTEGRATED_NAVIGATION_STATE;
    }
    else if((state->orig_state & 0xF0) == 0x70) {
        state->state = RAV260_INERTIAL_NAVIGATION_STATE;
    }
    else {
        state->state = RAV260_OTHER_STATE;
    }

    return ERR_OK;
}

int32_t rav260_temperature(device_t *device, float *temperature)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    if(!temperature) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //读取IMU数据
    taskENTER_CRITICAL();
    *temperature = imu->orig_data.temperature - 80.0f;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t rav260_check(device_t *device)
{
    uint32_t receive_event;
    rav260_t *imu = container_of(device, rav260_t, device);

    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    xEventGroupClearBits(imu->sync, CHECK_MASK);
    //等待自检标志位置位
    receive_event = xEventGroupWaitBits(imu->sync, CHECK_MASK, pdTRUE, pdTRUE, 20);
    if((receive_event & CHECK_MASK) != CHECK_MASK) {
        LOG_E("wait IMU timeout");
        return ERR_IO;
    }

    return ERR_OK;
}

int32_t rav260_restart(device_t *device)
{
    rav260_t *imu = container_of(device, rav260_t, device);

    if(device_check(device, RAV260_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //关电源
    power_off(imu->config->power);
    //延时，确保彻底断电
    vTaskDelay(300);
    //开启电源
    power_on(imu->config->power);

    return ERR_OK;
}

#endif /* USING_RAV260 */

