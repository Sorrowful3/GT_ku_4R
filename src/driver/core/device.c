/**
 * @file    device.c
 * @brief   实现设备管理和接口抽象功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#include "device.h"

#define DEBUG_TAG "device,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "debug.h"

/* 设备链表 */
static list_t device_list = {&device_list, &device_list};

/**
 * @function nane: __device_take
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              oflag 打开方式
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 获取设备，获取成功设备的引用计数会加1
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static int32_t __device_take(device_t *device, uint32_t oflag)
{
    int32_t result;

    /* 检查打开方式是否支持 */
    if((device->flags & oflag) != oflag) {
        LOG_E("non support");
        return ERR_NOSUP;
    }

    /* 第一次打开时执行驱动层的init函数 */
    if((device->ops->init) && (device->reference_count == 0)) {
        result = device->ops->init(device);
        if(result != ERR_OK) {
            return result;
        }
    }

    /* 执行驱动层的open函数 */
    if(device->ops->open) {
        result = device->ops->open(device, oflag);
        if(result != ERR_OK) {
            return result;
        }
    }

    /* 记录打开方式 */
    device->oflags |= oflag;
    /* 引用计数+1 */
    device->reference_count++;

    return ERR_OK;
}

/**
 * @function nane: __device_release
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 * @output param:
 *              return 成功返回EOK，失败返回错误码
 * @description: 
 * 释放设备，设备引用计数减1
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static int32_t __device_release(device_t *device)
{
    int32_t result;

    /* 引用计数-1 */
    device->reference_count--;

    /* 最后一次释放时执行驱动层的release函数 */
    if((device->ops->release) && (device->reference_count == 0)) {
        result = device->ops->release(device);
        if(result != ERR_OK) {
            /* 释放失败 */
            device->reference_count++;
            return result;
        }
    }

    /* 最后一次释放时复位oflags */
    if(device->reference_count == 0) {
        device->oflags = 0;
    }

    return ERR_OK;
}

/**
 * @function nane: device_register
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              name 设备名称
 *              flags 设备支持的打开方式
 * @output param:
 *              return 成功返回EOK，失败返回错误码
 * @description: 
 * 向设备管理框架注册设备
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
int32_t device_register(device_t *device, const char *name, uint32_t flags)
{
    if((!device) || (!device->ops) || (!name)){
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 检查设备是否已经注册 */
    if (device->init_flag == 0x5A5A5A5A) {
        LOG_E("device is register");
        return ERR_INVAL;
    }
    device->init_flag = 0x5A5A5A5A;

    /* 检查设备是否重名 */
    if(device_find(name)) {
        device->init_flag = 0x00;
        LOG_E("device duplication of name");
        return ERR_INVAL;
    }

    /* 创建互斥锁 */
    device->lock = xSemaphoreCreateRecursiveMutex();
    if(!device->lock) {
        device->init_flag = 0x00;
        LOG_E("create lock failed");
        return ERR_NOMEM;
    }

    /* 初始化设备参数 */
    device->flags = flags;
    device->oflags = 0;
    device->reference_count = 0;
    strncpy(device->name, name, MAX_NAME_LENGHT);
    device->receive_timeout = portMAX_DELAY;

    /* 将设备插入链表 */
    taskENTER_CRITICAL();
    list_insert_after(&device_list, &device->node);
    taskEXIT_CRITICAL();

    return  ERR_OK;
}

/**
 * @function nane: device_find
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              name 设备名称
 * @output param:
 *              return 查找成功返回设备句柄，查找失败返回NULL
 * @description: 
 * 从设备管理框架查找一个已经注册的设备
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
device_t *device_find(const char *name)
{
    device_t *device;
    list_t *tmp;

    if(!name){
        LOG_E("parameter error");
        return NULL;
    }

    /* 遍历链表，查找设备 */
    taskENTER_CRITICAL();
    list_for_each(tmp, &device_list) {
        device = container_of(tmp, device_t, node);
        /* 匹配设备名称 */
        if(strncmp(device->name, name, MAX_NAME_LENGHT) == 0) {
            taskEXIT_CRITICAL();
            return device;
        }
    }
    taskEXIT_CRITICAL();

    return NULL;
}

/**
 * @function nane: device_check
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              magic 设备幻术
 * @output param:
 *              成功返回EOK，失败返回错误码
 * @description: 
 * 检查设备句柄及其幻数是否正确
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
int32_t device_check(device_t *device, uint32_t magic)
{
    if(!device) {
        return ERR_INVAL;
    }
    return device->magic==magic ? ERR_OK : ERR_INVAL;;
}

/**
 * @function nane: device_open
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              name 设备名称
 *              oflag 打开方式
 * @output param:
 *              return 成功返回设备句柄，失败返回NULL
 * @description: 
 * 根据设备名称和打开方式打开设备
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
device_t *device_open(const char *name, uint32_t oflag)
{
    int32_t result;
    device_t *device;

    if((!name) || (oflag == 0)){
        LOG_E("parameter error");
        return NULL;
    }

    /* 查找设备 */
    device = device_find(name);
    if(!device) {
        LOG_E("no device");
        return NULL;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(device->lock, portMAX_DELAY) != pdPASS) {
        LOG_E("lock failed");
        return NULL;
    }

    /* 获取设备 */
    result = __device_take(device, oflag);

    /* 解锁 */
    xSemaphoreGiveRecursive(device->lock);

    if(result != ERR_OK) {
        return NULL;
    }
    return device;
}

/**
 * @function nane: device_close
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 * @output param:
 *              return 成功返回ERR_OK，设备返回错误码
 * @description: 
 * 关闭一个已经打开的设备
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
int32_t device_close(device_t *device)
{
    int32_t result;

    if(!device) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(device->lock, portMAX_DELAY) != pdPASS) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    /* 释放设备 */
    result = __device_release(device);

    /* 解锁 */
    xSemaphoreGiveRecursive(device->lock);

    return result;
}

/**
 * @function nane: device_read
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              buffer 用户缓存
 *              size 要读取的字节数
 * @output param:
 *              return 成功返回实际读取到的字节数，失败返回错误码
 * @description: 
 * 从一个打开的设备中读取数据
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */

ssize_t device_read(device_t *device, void *buffer, size_t size)
{
    if((!device) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 检查打开方式 */
    if(!(device->oflags & DEVICE_RDONLY)) {
        LOG_E("non support");
        return ERR_NOSUP;
    }

    /* 调用驱动的pread函数 */
    if(device->ops->pread) {
        return device->ops->pread(device, buffer, size, 0);
    }

    LOG_E("non support");
    return ERR_NOSUP;
}

/**
 * @function nane: device_write
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              buffer 欲写入的数据
 *              size 欲写入的字节数
 * @output param:
 *              return 成功返回实际写入到的字节数，失败返回错误码
 * @description: 
 * 向一个打开的设备写入数据
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t device_write(device_t *device, const void *buffer, size_t size)
{
    if((!device) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 检查打开方式 */
    if(!(device->oflags & DEVICE_WRONLY)) {
        LOG_E("non support");
        return ERR_NOSUP;
    }

    /* 调用驱动层的pwrite函数 */
    if(device->ops->pwrite) {
        return device->ops->pwrite(device, buffer, size, 0);
    }

    LOG_E("non support");
    return ERR_NOSUP;
}

/**
 * @function nane: device_pread
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              buffer 用户缓存
 *              size 要读取的字节数
 *              pos 偏移
 * @output param:
 *              return 成功返回实际读取的字节数，失败返回错误码
 * @description: 
 * 从一个已经打开的设备的pos偏移开始读取数据
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t device_pread(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    if((!device) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 检查打开方式 */
    if(!(device->oflags & DEVICE_RDONLY)) {
        LOG_E("non support");
        return ERR_NOSUP;
    }

    /* 调用驱动的pread函数 */
    if(device->ops->pread) {
        return device->ops->pread(device, buffer, size, pos);
    }

    LOG_E("non support");
    return ERR_NOSUP;
}

/**
 * @function nane: device_pwrite
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              buffer 欲写入的数据
 *              size 欲写入的字节数
 *              pos 偏移
 * @output param:
 *              return 成功返回实际写入的字节数，失败返回错误码
 * @description: 
 * 向一个已经打开的设备的pos偏移开始写入数据
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t device_pwrite(device_t *device, const void *buffer, size_t size, uint32_t pos)
{
    if((!device) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 检查打开方式 */
    if(!(device->oflags & DEVICE_WRONLY)) {
        LOG_E("non support");
        return ERR_NOSUP;
    }

    /* 调用驱动层的pwrite函数 */
    if(device->ops->pwrite) {
        return device->ops->pwrite(device, buffer, size, pos);
    }

    LOG_E("non support");
    return ERR_NOSUP;
}

/**
 * @function nane: device_ctrl
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              device 设备句柄
 *              command 控制命令，驱动框架已经占用的有DEVICE_GET_OFLAG、DEVICE_SET_OFLAG、DEVICE_GET_TIMEOUT、DEVICE_SET_TIMEOUT
 *              args 控制参数
 * @output param:
 *              成功返回EOK，失败返回错误码
 * @description: 
 * 控制设备执行非读写类的操作
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
int32_t device_ctrl(device_t *device, uint32_t command, void *args)
{
    uint32_t *p_oflags = (uint32_t*)args;
    TickType_t *p_timeout = (TickType_t*)args;

    if(!device) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    switch(command) {
    /* 获取打开方式 */
    case DEVICE_GET_OFLAG:
        if(!p_oflags) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
        *p_oflags = device->oflags;
        return ERR_OK;
    /* 设置打开方式 */
    case DEVICE_SET_OFLAG:
        if(!p_oflags) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
        if ((device->flags & *p_oflags) != *p_oflags) {
            LOG_E("non support");
            return ERR_NOSUP;
        }
        device->oflags = *p_oflags;
        return ERR_OK;
    /* 获取接收超时时间 */
    case DEVICE_GET_TIMEOUT:
        if(!p_timeout) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
        *p_timeout = device->receive_timeout;
         return ERR_OK;
    /* 设置接收超时时间 */
    case DEVICE_SET_TIMEOUT:
        if(!p_timeout) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
        device->receive_timeout = *p_timeout;
        return ERR_OK;
    /* 执行驱动层的control函数 */
    default:
        if(device->ops->control) {
            return device->ops->control(device, command, args);
        }
        else {
            LOG_E("non support");
            return ERR_NOSUP;
        }
    }
}
