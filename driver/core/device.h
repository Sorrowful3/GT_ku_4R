/**
 * @file    device.h
 * @brief   实现设备管理和接口抽象功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#ifndef __DEVICE_H_
#define __DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "define.h"
#include "list.h"

#include "../driver_config.h"

/**
 * 打开方式
 */
#define DEVICE_RDONLY   0X01                            /* 只读 */
#define DEVICE_WRONLY   0X02                            /* 只写 */
#define DEVICE_RDWR     (DEVICE_RDONLY|DEVICE_WRONLY)   /* 读写 */
#define DEVICE_NOBLOCK  0x04                            /* 阻塞式 */

/**
 * device_ctrl函数的内部命令
 */
#define DEVICE_GET_OFLAG    0x01    /* 获取打开方式 */
#define DEVICE_SET_OFLAG    0x02    /* 设置打开方式 */
#define DEVICE_GET_TIMEOUT  0x03    /* 获取接收超时 */
#define DEVICE_SET_TIMEOUT  0x04    /* 设置接收超时 */
#define DEVICE_USER_CTRL    0x05

/**
 * 设备操作函数集
 */
struct device;
struct device_ops{
    /* 初始化设备 */
    int32_t (*init)(struct device *device);
    /* 打开设备 */
    int32_t (*open)(struct device *device, uint32_t oflag);
    /* 释放设备 */
    int32_t (*release)(struct device *device);
    /* 读设备 */
    ssize_t (*pread)(struct device *device, void *buffer, size_t size, uint32_t pos);
    /* 写设备 */
    ssize_t (*pwrite)(struct device *device, const void *buffer, size_t size, uint32_t pos);
    /* 控制设备 */
    int32_t (*control)(struct device *device, uint32_t command, void *user_arg);
};

/**
 * 表示一个设备
 */
typedef struct device{
    list_t              node;                   /* 设备链表节点 */
    char                name[MAX_NAME_LENGHT];  /* 设备名称 */
    SemaphoreHandle_t   lock;                   /* 互斥锁 */
    uint32_t            flags;                  /* 设备支持的打开方式 */
    uint32_t            oflags;                 /* 设备当前的打开方式 */
    uint32_t            reference_count;        /* 引用计数 */
    TickType_t          receive_timeout;        /* 接收超时 */
    uint32_t            init_flag;              /* 初始化标志，为0x5A5A5A5A表示已经初始化 */

    uint32_t            magic;                  /* 幻数，用于快速区分不同的设备 */
    struct device_ops   *ops;                   /* 驱动接口函数 */
    void                *private_data;          /* 预留到驱动层的一个指针 */
}device_t;

int32_t device_register(device_t *device, const char *name, uint32_t flags);
device_t *device_find(const char *name);
int32_t device_check(device_t *device, uint32_t magic);

device_t *device_open(const char *name, uint32_t oflag);
int32_t device_close(device_t *device);
ssize_t device_read(device_t *device, void *buffer, size_t size);
ssize_t device_write(device_t *device, const void *buffer, size_t size);
ssize_t device_pread(device_t *device, void *buffer, size_t size, uint32_t pos);
ssize_t device_pwrite(device_t *device, const void *buffer, size_t size, uint32_t pos);
int32_t device_ctrl(device_t *device, uint32_t command, void *args);

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_H_ */
