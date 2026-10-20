/**
 * @file    axi_dma.h
 * @brief   实现PL端的DMA IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月8日
 */

#ifndef __AXI_DMA_H_
#define __AXI_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_DMA

#include "xaxidma.h"
#include "xparameters.h"

typedef struct{
    const char  *device_name;   /* 设备名称 */

    uint16_t    device_id;      /* 设备ID */
    uint16_t    rx_intr_id;     /* 接收中断 */
    uint16_t    tx_intr_id;     /* 发送中断 */

    size_t      rx_fifo_size;   /* 接收FIFO大小 */
}axi_dma_config_t;

typedef struct{
    device_t            device;     /* 继承device_t */

    XAxiDma             axi_dma;    /* AXI DMA句柄 */
    axi_dma_config_t    *config;    /* 配置数据 */

    fifo_t              *rx_fifo;   /* 接收FIFO */
    uint8_t             *rx_buffer; /* 接收缓存 */

    SemaphoreHandle_t   tx_lock;    /* 发送互斥锁 */
    SemaphoreHandle_t   tx_sync;    /* 发送同步信号量 */
    uint8_t             *tx_buffer; /* 发送缓存 */

    int32_t             stop_flag;  /* 发送停止标志 */
}axi_dma_t;

int32_t axi_dma_init(void);

#endif /* USING_AXI_DMA */

#ifdef __cplusplus
}
#endif

#endif /* _AXI_DMA_H_ */
