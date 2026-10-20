/*
 * @file    fifo.h
 * @brief   实现FIFO相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */


#ifndef __FIFO_H_
#define __FIFO_H_

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

/**
 * FIFO结构体
 */
typedef struct{
    SemaphoreHandle_t   suspend;    /* 用于挂起线程的信号量 */

    void                *pool;      /* FIFO内存地址 */
    size_t              size;       /* FIFO大小 */

    size_t              r_index;    /* FIFO读索引 */
    size_t              w_index;    /* FIFO写索引 */
    int32_t             is_empty;   /* FIFO空标识(当r_index和w_index相等时，为1表示FIFO空，为0表示FIFO满) */
}fifo_t;


fifo_t *fifo_create(size_t size);
ssize_t fifo_length(fifo_t *fifo);
ssize_t fifo_length_isr(fifo_t *fifo);

ssize_t fifo_in_force(fifo_t *fifo, const void *data, size_t size);
ssize_t fifo_in_force_isr(fifo_t *fifo, const void *data, size_t size, BaseType_t *task_woken);
ssize_t fifo_in(fifo_t *fifo, const void *data, size_t size);
ssize_t fifo_in_isr(fifo_t *fifo, const void *data, size_t size, BaseType_t *task_woken);
ssize_t fifo_out(fifo_t *fifo, void *buffer, size_t size, TickType_t timeout);
ssize_t fifo_out_isr(fifo_t *fifo, void *buffer, size_t size);

//以下函数不会引起线程切换
ssize_t fifo_in_no_schedule(fifo_t *fifo, const void *data, size_t size);
ssize_t fifo_in_no_schedule_isr(fifo_t *fifo, const void *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __FIFO_H_ */
