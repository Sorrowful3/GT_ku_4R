/**
 * @file    axi_uartlite.h
 * @brief   实现AXI UARTLite IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月8日
 */

#ifndef __AXI_UARTLITE_H_
#define __AXI_UARTLITE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_UARTLITE

#include "xparameters.h"
#include "xuartlite_i.h"
#include "xuartlite.h"

typedef struct{
    const char  *device_name;       /* 设备名称 */

    uint16_t    device_id;          /* 设备ID */
    uint16_t    intr_id;            /* 设备中断号 */

    size_t      rx_fifo_size;       /* 接收FIFO大小 */
    size_t      tx_fifo_size;       /* 发送FIFO大小 */
}axi_uartlite_config_t;

typedef struct{
    device_t                device;     /* 继承device_t */

    XUartLite               uartlite;   /* 串口句柄 */
    axi_uartlite_config_t   *config;    /* 串口配置参数 */

    fifo_t                  *rx_fifo;   /* 接收FIFO */

    SemaphoreHandle_t       tx_lock;    /* 发送锁 */
    SemaphoreHandle_t       tx_sync;    /* 发送同步信号量 */
    uint32_t                tx_idle;    /* 发送空闲标志 */
    fifo_t                  *tx_fifo;   /* 发送FIFO */

    int32_t                 stop_flag;  /* 停止标志 */
}axi_uartlite_t;

int32_t axi_uartlite_init(void);

#endif /* USING_AXI_UARTLITE */

#ifdef __cplusplus
}
#endif

#endif /* __AXI_UARTLITE_H_ */
