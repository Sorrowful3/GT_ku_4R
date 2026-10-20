/*
 * transducer.c
 *
 *  Created on: 2021年3月24日
 *      Author: Administrator
 */

#include "transducer.h"

#define DEBUG_TAG "transducer,%d", __LINE__
//#define DEBUG_LEVEL WARNING_OUTPUT
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../core/debug.h"

#ifdef USING_TRANSDUCER

static int temper_cnt;

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

typedef enum{
    OK = 0,         //解析完成
    INCOMPLETE = 1, //正在解析
    BAD = 2,        //解析出错
}parse_state_t;

typedef struct{
    uint8_t buffer[12];
    uint32_t receive_counter;
}transducer_msg_t;


static transducer_config_t transducer_config[1] = {
#ifdef TRANSDUCER_0_CONFIG
    TRANSDUCER_0_CONFIG,
#endif
};

static transducer_t TR_transducer[sizeof(transducer_config)/sizeof(transducer_config[0])];

static inline ssize_t transducer_write(transducer_t *transducer, uint8_t *buffer, size_t lenght)
{
    uint16_t crc;

//    //添加CRC校验
//    crc = crc16(buffer, lenght-2, 0xffff);
//    buffer[lenght-2] = (uint8_t)(crc>>8);
//    buffer[lenght-1] = (uint8_t)crc;

#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("->\n");
    for(int i=0; i<lenght; i++) {
        xil_printf("%02x ", buffer[i]);
    }
    xil_printf("\r\n");
#endif

    //通过串口发送
    return device_write(transducer->uart, buffer, lenght);
}


parse_state_t transducer_parse_byte(transducer_t *transducer, transducer_msg_t *msg, uint8_t data)
{
    uint16_t crc;
    int32_t release_semaphore;

    msg->buffer[msg->receive_counter] = data;
    msg->receive_counter++;

    //校验帧头
    if(((msg->receive_counter == 1) && (msg->buffer[0] != 0x1b)) \
            || ((msg->receive_counter == 2) && (msg->buffer[1] != 0x83)) ){
        return BAD;
    }
    //是否继续接收
    if(msg->receive_counter < 8) {
        return INCOMPLETE;
    }

    if((msg->buffer[8-1] != (uint8_t)(0xfa)) ) {
        return BAD;
    }

    release_semaphore = 0;
    switch(msg->buffer[2]) {
    case 0x01:
    	taskENTER_CRITICAL();
    	transducer->data.down_Lo = (msg->buffer[3] << 24) | (msg->buffer[4] << 16)
    											| (msg->buffer[5] << 8) | msg->buffer[6];
    	release_semaphore = 1;
    	taskEXIT_CRITICAL();
        break;
    case 0x02:
    	taskENTER_CRITICAL();
    	transducer->data.gain = (msg->buffer[3] << 24) | (msg->buffer[4] << 16)
    	    					| (msg->buffer[5] << 8) | msg->buffer[6];
    	release_semaphore = 1;
    	taskEXIT_CRITICAL();
        break;
    case 0x03:
    	taskENTER_CRITICAL();
        transducer->data.clok_mode = (msg->buffer[3] << 24) | (msg->buffer[4] << 16)
        	    					| (msg->buffer[5] << 8) | msg->buffer[6];
        release_semaphore = 1;
        taskEXIT_CRITICAL();
        break;
    case 0x06:
    	taskENTER_CRITICAL();
            transducer->data.lock_state = (msg->buffer[3] << 24) | (msg->buffer[4] << 16)
            	    					| (msg->buffer[5] << 8) | msg->buffer[6];
            release_semaphore = 1;
            taskEXIT_CRITICAL();
            break;

    default:
        return BAD;
    }

    //调用接收完成回调
    if(transducer->callbk) {
        transducer->callbk(&transducer->device, msg->buffer, msg->receive_counter);
    }

    //释放信号量
    if(release_semaphore) {
        if(xSemaphoreGive(transducer->sync) != pdPASS) {
            LOG_E("release semaphore failed");
        }
    }

    return OK;
}

static void transducer_thread(void *arg)
{
    int32_t err_out;
    ssize_t i;
    ssize_t lenght;
    uint8_t buffer[12];
    parse_state_t state;
    transducer_msg_t msg;
    transducer_t *transducer = (transducer_t *)arg;

    if((!transducer) || (device_check(&transducer->device, TRANSDUCER_MAGIC) != ERR_OK)) {
        LOG_E("parameter error");
        vTaskDelete(NULL);
    }

    err_out = 0;
    msg.receive_counter = 0;
    while(1)
    {
        //从串口读取数据
        lenght = device_read(transducer->uart, buffer, sizeof(buffer));
        if(lenght <= 0) {
            continue;
        }

        for(i=0; i<lenght; i++) {
            //进行解包
            state = transducer_parse_byte(transducer, &msg, buffer[i]);

            //正在解析
            if(state == INCOMPLETE) {
                continue;
            }
            //连续5次收到正确数据包后开启解析出错输出
            if((state == OK) && (err_out < 5)) {
                err_out++;
            }
            //数据格式错误
            if((state == BAD) && (err_out >= 5)) {
                LOG_E("frame error");
            }
#if DEBUG_LEVEL >= DEBUG_OUTPUT
            xil_printf("<-\n");
            for(int i=0; i<msg.receive_counter; i++) {
                xil_printf("%02x ", (char)msg.buffer[i]);
            }
            xil_printf("\r\n");
#endif
            //清除接收缓存
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

int32_t transducer_init(void)
{
    uint32_t i;
    int32_t result;
    portBASE_TYPE xresult;
    TaskHandle_t task_handle;

    for(i=0; i<sizeof(TR_transducer)/sizeof(TR_transducer[0]); i++) {
        TR_transducer[i].config = &transducer_config[i];
        memset(&TR_transducer[i].data, 0, sizeof(TR_transducer[i].data));
#ifdef FREQUENCY_POINT
        TR_transducer[i].data.down_Lo = 1;
#else
        TR_transducer[i].data.down_Lo = (uint32_t)(9.75f * 1000.0f + 0.5f);
#endif

        //创建互斥锁
        TR_transducer[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!TR_transducer[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }
        //创建同步信号量
        TR_transducer[i].sync = xSemaphoreCreateBinary();
        if(!TR_transducer[i].sync) {
            LOG_E("create sync semaphore failed");
            return ERR_NOMEM;
        }
        //默认没有回调函数
        TR_transducer[i].callbk = NULL;

        //打开串口设备
        TR_transducer[i].uart = device_open(TR_transducer[i].config->uart_name, DEVICE_RDWR);
        if(!TR_transducer[i].uart) {
            LOG_E("open uart failed");
            return ERR_IO;
        }

        //注册变频器设备
        TR_transducer[i].device.magic = TRANSDUCER_MAGIC;
        TR_transducer[i].device.ops = &ops;
        result = device_register(&TR_transducer[i].device, TR_transducer[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed\r\n");
            return result;
        }

        //创建变频器回传数据解析线程
        xresult = xTaskCreate(transducer_thread,
                             "transducer_thread",
                             2048,
                             &TR_transducer[i],
                             configMAX_PRIORITIES - 2,
                             &task_handle );
        if(xresult != pdPASS) {
            LOG_E("create task transducer_thread failed\r\n");
            return ERR_NOMEM;
        }

        //开启电源
        power_on(TR_transducer[i].config->power5v5);
        power_on(TR_transducer[i].config->power12v);
    }

    return ERR_OK;
}

int32_t transducer_read_version(device_t *device, uint32_t *version)
{
    uint8_t cmd[12] = {0xAA, 0x55, 0x0C, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);
    //将命令发送到变频器
    if(transducer_write(transducer, cmd, 12) != 12) {
        UNLOCK(transducer);
        LOG_E("write data to transducer failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT) != pdTRUE) {
        UNLOCK(transducer);
        LOG_E("read version timeout");
        return ERR_TIMEOUT;
    }

    //读取版本
    if(version) {
        *version = transducer->data.version;
    }

    UNLOCK(transducer);
    return ERR_OK;
}

int32_t transducer_read_state(device_t *device, float *temperature, float *electricity, float *vlotage)
{
    uint8_t cmd[12] = {0xAA, 0x55, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);
    //将命令发送到变频器
    if(transducer_write(transducer, cmd, 12) != 12) {
        UNLOCK(transducer);
        LOG_E("write data to transducer failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT) != pdTRUE) {
        UNLOCK(transducer);
        LOG_E("read version timeout");
        return ERR_TIMEOUT;
    }

    //读取状态信息
    if(temperature) {
        *temperature = transducer->data.temperature;
    }
    if(electricity) {
        *electricity = transducer->data.electricity;
    }
    if(vlotage) {
        *vlotage = transducer->data.vlotage;
    }

    UNLOCK(transducer);
    return ERR_OK;
}



int32_t transducer_set_gain(device_t *device, float gain)
{

    uint32_t local_gain;

    uint8_t cmd[] = {0x1b, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0xFA};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

	cmd[6] = uint8_t(gain*2);

	local_gain=cmd[6] ;

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);

        //将命令发送到变频器
        if(transducer_write(transducer, cmd, 8) != 8) {
        	LOG_E("write gain data to transducer failed");
        	return ERR_IO;
        }

        cmd[1] = 0x82;
        if(transducer_write(transducer, cmd, 8) != 8) {
        	LOG_E("write gain data to transducer failed");
        	return ERR_IO;
        }
        //等待命令反馈标志
        if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT) != pdTRUE) {
        	LOG_E("read gain data to transducer failed");
        	return ERR_IO;
        }
    if(transducer->data.gain != local_gain) {
        UNLOCK(transducer);
        LOG_E("set gain failed");
        return ERR_IO;
    }

    UNLOCK(transducer);
    return ERR_OK;
}


int32_t transducer_set_lo(device_t *device, float lo)
{
#ifdef FREQUENCY_POINT
    const float support_lo[2] = {9.75f, 10.60f};
    uint8_t local_lo;
#else
    uint32_t local_lo;
#endif
    uint8_t cmd[] = {0x1b, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFA};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

#ifdef FREQUENCY_POINT
    //转换为频点
    for(local_lo=0; local_lo<sizeof(support_lo)/sizeof(support_lo[0]); local_lo++)  {
        if(fabsf32(support_lo[local_lo] - lo) < 0.01)
            break;
    }
    if(local_lo >= sizeof(support_lo)/sizeof(support_lo[0]))  {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    cmd[5] = local_lo+1;

    //lijiwen_add
    local_lo = (uint32_t)(lo * 1000.0f + 0.5f);
    cmd[6] = (uint8_t)(local_lo >> 24);
    cmd[7] = (uint8_t)(local_lo >> 16);
    cmd[8] = (uint8_t)(local_lo >> 8);
    cmd[9] = (uint8_t)local_lo;
#else

    if(lo==9.75)
    cmd[6] = 0;
   else
	cmd[6] = 1;
#endif

    local_lo=cmd[6] ;

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);

    //将命令发送到变频器
    if(transducer_write(transducer, cmd, 8) != 8) {
    	LOG_E("write data to transducer failed");
    	return ERR_IO;
    }

    cmd[1] = 0x82;
    if(transducer_write(transducer, cmd, 8) != 8) {
    	LOG_E("write data to transducer failed");
    	return ERR_IO;
    }
    //等待命令反馈标志
    if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT) != pdTRUE) {
    	LOG_E("set read data to transducer failed");
    	return ERR_IO;
    }

        //检查命令执行结果
#ifdef FREQUENCY_POINT
        if(transducer->data.down_Lo != (local_lo+1)) {
#else
       // if(transducer->data.down_Lo == local_lo) {
#endif
//        	return ERR_IO;
//        }


    //检查命令执行结果
#ifdef FREQUENCY_POINT
    if(transducer->data.down_Lo != (local_lo+1)) {
#else
    if(transducer->data.down_Lo != local_lo) {
#endif
        UNLOCK(transducer);
        LOG_E("set lo failed");
        return ERR_IO;
    }

    UNLOCK(transducer);
    return ERR_OK;
}

int32_t transducer_get_lo(device_t *device, float *lo)
{
#ifdef FREQUENCY_POINT
    const float support_lo[2] = {9.75f, 10.60f};
    uint8_t down_Lo;
#else
#endif
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!lo) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

#ifdef FREQUENCY_POINT
    //获取变频器本振频率
    taskENTER_CRITICAL();
    down_Lo = transducer->data.down_Lo;
    taskEXIT_CRITICAL();

    //将频点转换为频率
    if((down_Lo > 2) || (down_Lo < 1)) {
        return ERR_IO;
    }
    *lo = support_lo[down_Lo - 1];
#else
    taskENTER_CRITICAL();
        if( transducer->data.down_Lo==0)
        {
        	*lo=9.75;
        }
        else
        {
        	*lo=10.6;
        }
  //  *lo = transducer->data.down_Lo*1000;
    taskEXIT_CRITICAL();
#endif
    return ERR_OK;
}

int32_t transducer_set_mode(device_t *device, uint8_t mode)
{
    uint8_t cmd[12] = {0xAA, 0x55, 0x0C, 0x00, 0x12, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);
    //将命令发送到变频器
    if(transducer_write(transducer, cmd, 12) != 12) {
        UNLOCK(transducer);
        LOG_E("write data to transducer failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT) != pdTRUE) {
        UNLOCK(transducer);
        LOG_E("set mode timeout");
        return ERR_TIMEOUT;
    }

    //检查命令执行结果
    if(transducer->data.mode != mode) {
        UNLOCK(transducer);
        LOG_E("set mode failed");
        return ERR_IO;
    }

    UNLOCK(transducer);
    return ERR_OK;
}

int32_t transducer_get_mode(device_t *device, uint8_t *mode)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!mode) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *mode = transducer->data.mode;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t transducer_get_fault_code(device_t *device, uint8_t *fault_code)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!fault_code) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *fault_code = transducer->data.fault_code;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t transducer_get_if_value(device_t *device, float *if_Value)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!if_Value) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *if_Value = transducer->data.if_Value;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t transducer_get_rf_value(device_t *device, float *rf_Value)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!rf_Value) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *rf_Value = transducer->data.rf_Value;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t transducer_get_lock_state(device_t *device, uint8_t *lock_state)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!lock_state) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *lock_state = transducer->data.lock_state;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t transducer_check(device_t *device, uint8_t *fault_code)
{
    int32_t result;
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(!fault_code) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //通信状态检查
    result = transducer_read_version(device, NULL);
    if(result != ERR_OK) {
        *fault_code = 0x80;
        return ERR_IO;
    }

    //故障码获取
    taskENTER_CRITICAL();
    *fault_code = transducer->data.fault_code;
    taskEXIT_CRITICAL();

    if(*fault_code == 0) {
        return ERR_OK;
    }

    return ERR_IO;
}

int32_t transducer_restart(device_t *device)
{
    uint8_t cmd[12] = {0xAA, 0x55, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    xSemaphoreTake(transducer->sync, 0);
    //将命令发送到变频器
    if(transducer_write(transducer, cmd, 12) != 12) {
        UNLOCK(transducer);
        LOG_E("write data to transducer failed");
        return ERR_IO;
    }

    //等待命令反馈标志
    if(xSemaphoreTake(transducer->sync, TRANSDUCER_TIMEOUT*10) != pdTRUE) {
        UNLOCK(transducer);
        LOG_E("restart timeout");
        return ERR_TIMEOUT;
    }

    //检查命令执行结果
    if(transducer->data.reset_state != 0xFF) {
        UNLOCK(transducer);
        LOG_E("restart failed");
        return ERR_IO;
    }

    UNLOCK(transducer);
    return ERR_OK;
}

int32_t transducer_write_command(device_t *device, const uint8_t *command, uint32_t lenght)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    //将命令发送到变频器
    if(device_write(transducer->uart, command, (size_t)lenght) != lenght) {
        UNLOCK(transducer);
        LOG_E("write data to transducer failed");
        return ERR_IO;
    }
#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("->\n");
    for(int i=0; i<lenght; i++) {
        xil_printf("%02x ", command[i]);
    }
    xil_printf("\r\n");
#endif

    UNLOCK(transducer);

    return ERR_OK;
}

int32_t transducer_set_receive_callbk(device_t *device, transducer_receive_callbk callbk)
{
    transducer_t *transducer = container_of(device, transducer_t, device);

    if(device_check(device, TRANSDUCER_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    LOCK(transducer);

    //注册接收回调函数
    transducer->callbk = callbk;

    UNLOCK(transducer);

    return ERR_OK;
}

#endif /* USING_TRANSDUCER */
