/*
 * axi_uart550.h
 *
 *  Created on: 2021年3月10日
 *      Author: Administrator
 */

#ifndef __AXI_UART550_H_
#define __AXI_UART550_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_UART550

#include "xparameters.h"
#include "xuartns550.h"

#define AXI_UART550_DATA_BITS_6 XUN_FORMAT_6_BITS
#define AXI_UART550_DATA_BITS_7 XUN_FORMAT_7_BITS
#define AXI_UART550_DATA_BITS_8 XUN_FORMAT_8_BITS

#define AXI_UART550_PARITY_NONE XUN_FORMAT_NO_PARITY
#define AXI_UART550_PARITY_ODD  XUN_FORMAT_ODD_PARITY
#define AXI_UART550_PARITY_EVEN XUN_FORMAT_EVEN_PARITY

#define AXI_UART550_STOP_BITS_1 XUN_FORMAT_1_STOP_BIT
#define AXI_UART550_STOP_BITS_2 XUN_FORMAT_2_STOP_BIT

typedef struct{
    const char  *device_name;   //设备名称

    uint16_t    device_id;      //设备ID
    uint16_t    intr_id;        //中断号

    uint32_t    baud_rate;      //波特率
    uint32_t    data_bits;      //数据位宽度
    uint32_t    parity;         //奇偶校验
    uint8_t     stop_bits;      //停止位

    size_t      rx_fifo_size;   //接收FIFO大小
    size_t      tx_fifo_size;   //发送FIFO大小
}axi_uart550_config_t;

typedef struct{
    device_t                device;     //继承device_t

    XUartNs550              uart550;    //串口句柄
    axi_uart550_config_t    *config;    //串口配置参数

    SemaphoreHandle_t       tx_lock;    //发送互斥锁
    SemaphoreHandle_t       tx_sync;    //发送同步信号量
    uint32_t                tx_idle;    //发送空闲标志
    fifo_t                  *tx_fifo;   //发送FIFO
    fifo_t                  *rx_fifo;   //接收FIFO
    int32_t                 stop_flag;  /* 停止标志 */
}axi_uart550_t;

int32_t axi_uart550_init(void);

#endif /* USING_AXI_UART550 */

#ifdef __cplusplus
}
#endif

#endif /* __AXI_UART550_H_ */
