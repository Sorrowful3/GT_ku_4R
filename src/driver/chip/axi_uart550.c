/*
 * axi_uart550.c
 *
 *  Created on: 2021年3月10日
 *      Author: Administrator
 */

#include "axi_uart550.h"

#define DEBUG_TAG "axi_uart550,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_UART550

static axi_uart550_config_t uart550_config[] =
{
#ifdef AXI_UART5500_CONFIG
    AXI_UART5500_CONFIG,
#endif
#ifdef AXI_UART5501_CONFIG
    AXI_UART5501_CONFIG,
#endif
#ifdef AXI_UART5502_CONFIG
    AXI_UART5502_CONFIG,
#endif
#ifdef AXI_UART5503_CONFIG
    AXI_UART5503_CONFIG,
#endif
#ifdef AXI_UART5504_CONFIG
    AXI_UART5504_CONFIG,
#endif
#ifdef AXI_UART5505_CONFIG
    AXI_UART5504_CONFIG,
#endif
};

static axi_uart550_t axi_uart550[sizeof(uart550_config)/sizeof(uart550_config[0])];

/*
 * AXI UART16550用户中断回调函数
 *
 * @param arg 回调函数参数，这里传递的是AXI UART句柄
 * @param event 所产生中断事件
 * @param event_data 中断事件所对应的参数
 *
 * @return 无
 */
static void __irq_handler(void *arg)
{
    uint8_t isr_state;
    uint32_t lsr_reg;
    uint32_t iir_reg;
    size_t hw_fifo_size;
    ssize_t i;
    ssize_t result;
    BaseType_t task_woken;
    uint8_t buffer[XUN_FIFO_SIZE];
    axi_uart550_t *uart550 = (axi_uart550_t *)arg;

    if((!uart550) || (device_check(&uart550->device, AXI_UART550_MAGIC) != ERR_OK)) {
        return;
    }

    //读取ISR寄存器，用于判断中断类型
    isr_state = XUartNs550_ReadReg(uart550->uart550.BaseAddress, XUN_IIR_OFFSET) & XUN_INT_ID_MASK;
    //模式中断
    if(isr_state == 0) {
    }
    //中断标志无效，此时可能是发生了错误
    if(isr_state == 1) {
    }
    //读取LSR寄存器，用于判断发送状态
    lsr_reg = XUartNs550_GetLineStatusReg(uart550->uart550.BaseAddress);
    //发送完成中断
    if((isr_state == 2) || (lsr_reg & XUN_LSR_TX_BUFFER_EMPTY)) {
        //判断发送是否空闲
        if(lsr_reg & XUN_LSR_TX_BUFFER_EMPTY) {
            //读取IIR寄存器，判断FIFO使能状态
            iir_reg = XUartNs550_ReadReg(uart550->uart550.BaseAddress, XUN_IIR_OFFSET);
            if (iir_reg & XUN_INT_ID_FIFOS_ENABLED) {
                //FIFO使能
                if (lsr_reg & XUN_LSR_TX_EMPTY) {
                    hw_fifo_size = XUN_FIFO_SIZE;
                } else {
                    hw_fifo_size = XUN_FIFO_SIZE - 1;
                }
            }
            else {
                hw_fifo_size = 1;
            }
            //从FIFO中读数据
            result = fifo_out_isr(uart550->tx_fifo, (void*)buffer, hw_fifo_size);
            if(result > 0) {
                //将数据写入硬件FIFO
                for (i=0; i<result; i++) {
                    XUartNs550_WriteReg(uart550->uart550.BaseAddress, XUN_THR_OFFSET, buffer[i]);
                }

                //设置发送同步信号量，表示FIFO可以写入
                task_woken = pdFALSE;
                xSemaphoreGiveFromISR(uart550->tx_sync, &task_woken);
                portYIELD_FROM_ISR(task_woken);
            }
            else {
                //设置发送为空闲
                uart550->tx_idle = 1;
            }
        }
    }
    //读取LSR寄存器，用于判断发送状态
    lsr_reg = XUartNs550_GetLineStatusReg(uart550->uart550.BaseAddress);
    //接收完成中断,接收状态改变中断,接收超时中断
    if((isr_state == 4) || (isr_state == 6) || (isr_state == 12) || (lsr_reg & (XUN_LSR_BREAK_INT|XUN_LSR_DATA_READY))) {
        for(i=0; i<sizeof(buffer); ) {
            //读取LSR寄存器用于判断接收状态
            lsr_reg = XUartNs550_GetLineStatusReg(uart550->uart550.BaseAddress);
            //从硬件FIFO中读取数据
            if(lsr_reg & XUN_LSR_BREAK_INT) {
                (void)XUartNs550_ReadReg(uart550->uart550.BaseAddress,  XUN_RBR_OFFSET);
            }
            else if (lsr_reg & XUN_LSR_DATA_READY) {
                buffer[i] = XUartNs550_ReadReg(uart550->uart550.BaseAddress, XUN_RBR_OFFSET);
                i++;
            }
            else {
                //读取完成
                break;
            }
        }

        //将数据写入软件FIFO
        fifo_in_force_isr(uart550->rx_fifo, (void*)buffer, i, &task_woken);
        portYIELD_FROM_ISR(task_woken);
    }
}

/*
 * 初始化AXI UART16550设备，在第一次执行open时调用
 *
 * @param device 设备句柄
 *
 * @return 成功返回EOK
 */
static int32_t __uart550_init(device_t *device)
{
    uint16_t options;
    uint32_t ier_register;
    ssize_t result;
    uint8_t buffer[XUN_FIFO_SIZE];
    axi_uart550_t *uart550 = container_of(device, axi_uart550_t, device);

    //清空接收FIFO
    for(; ; ) {
        result = fifo_out(uart550->rx_fifo, (void*)buffer, XUN_FIFO_SIZE, 0);
        if(result < (ssize_t)XUN_FIFO_SIZE) {
            break;
        }
    }
    //清空发送FIFO
    for(; ; ) {
        result = fifo_out(uart550->tx_fifo, (void*)buffer, XUN_FIFO_SIZE, 0);
        if(result < (ssize_t)XUN_FIFO_SIZE) {
            break;
        }
    }
    /* 复位发送同步信号量，表示串口还未发送过数据帧 */
    xSemaphoreTake(uart550->tx_sync, 0);
    //设置发送状态为空闲
    uart550->tx_idle = 1;
    /* 复位停止标志 */
    uart550->stop_flag = 0;

    //开启接收中断，使能并清空接收和发送FIFO
    options = XUN_OPTION_DATA_INTR | XUN_OPTION_FIFOS_ENABLE |
            XUN_OPTION_RESET_TX_FIFO | XUN_OPTION_RESET_RX_FIFO |
            XUN_OPTION_RXLINE_INTR;
    XUartNs550_SetOptions(&uart550->uart550, options);

    //使能所有中断
    ier_register = XUartNs550_ReadReg(uart550->uart550.BaseAddress, XUN_IER_OFFSET);
    ier_register |= XUN_IER_MODEM_STATUS;
    ier_register |= XUN_IER_RX_LINE;
    ier_register |= XUN_IER_TX_EMPTY;
    ier_register |= XUN_IER_TX_EMPTY;
    XUartNs550_WriteReg(uart550->uart550.BaseAddress, XUN_IER_OFFSET, ier_register);

    return ERR_OK;
}

/*
 * 释放AXI UART16550设备，在最后一次调用close时执行
 *
 * @param device 设备句柄
 *
 * @return 成功返回EOK
 */
static int32_t __uart550_release(device_t *device)
{
    uint32_t i;
    axi_uart550_t *uart550 = container_of(device, axi_uart550_t, device);

    /* 设置停止标志 */
    uart550->stop_flag = 1;

    /* 等待发送空闲 */
    for(i=0; i<30; i++) {
        if(uart550->tx_idle) {
            break;
        }
        vTaskDelay(2);
    }
    if(i >= 30) {
        uart550->stop_flag = 0;
        LOG_E("wait idle timeout");
        return ERR_IO;
    }

    //禁止串口的中断事件
    XUartNs550_SetOptions(&uart550->uart550, 0x00);

    return ERR_OK;
}

/*
 * 读AXI UART16550接收到的数据
 *
 * @param device 设备句柄
 * @param buffer 用户缓存
 * @param size 要读取的字节数
 * @param pos 偏移，此处未实现
 *
 * @return 成功返回读取的字节数
 */
static ssize_t __uart550_read(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    axi_uart550_t *uart550 = container_of(device, axi_uart550_t, device);

    return fifo_out(uart550->rx_fifo, buffer, size, device->receive_timeout);
}

/*
 * 通过AXI UART16550发送数据
 *
 * @param device 设备句柄
 * @param buffer 用户缓存
 * @param size 要发送的字节数
 * @param pos 偏移，此处未实现
 *
 * @return 成功返回发送的字节数
 */
static ssize_t __uart550_write(device_t *device, const void *user_buf, size_t size, uint32_t pos)
{
    uint32_t lsr_reg;
    uint32_t iir_reg;
    ssize_t i;
    ssize_t result;
    ssize_t tx_size;
    ssize_t temp;
    size_t hw_fifo_size;
    uint8_t buffer[XUN_FIFO_SIZE];
    const uint8_t *p_data = (const uint8_t*)user_buf;
    axi_uart550_t *uart550 = container_of(device, axi_uart550_t, device);

    /* 停止标志置位不在发送新数据包 */
    if(uart550->stop_flag) {
        return ERR_IO;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(uart550->tx_lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    for(tx_size=0; ; ) {
        /* 进入临界区 */
        taskENTER_CRITICAL();

        /* 如果发送空闲则启动发送 */
        if(uart550->tx_idle) {
            //读取LSR寄存器，用于判断发生FIFO状态
            lsr_reg = XUartNs550_GetLineStatusReg(uart550->uart550.BaseAddress);
            if(lsr_reg & XUN_LSR_TX_BUFFER_EMPTY) {
                //读取IIR寄存器，用于判断FIFO使能状态
                iir_reg = XUartNs550_ReadReg(uart550->uart550.BaseAddress, XUN_IIR_OFFSET);
                if (iir_reg & XUN_INT_ID_FIFOS_ENABLED) {
                    //FIFO使能
                    if (lsr_reg & XUN_LSR_TX_EMPTY) {
                        hw_fifo_size = XUN_FIFO_SIZE;
                    } else {
                        hw_fifo_size = XUN_FIFO_SIZE - 1;
                    }
                }
                else {
                    hw_fifo_size = 1;
                }
                /* 将数据拷贝到发送缓冲区 */
                temp = ((size-tx_size) < hw_fifo_size) ? (size-tx_size) : hw_fifo_size;
                memcpy(buffer, (void*)&p_data[tx_size], temp);
                //将数据写入硬件FIFO
                for(i=0; i<temp; i++) {
                    XUartNs550_WriteReg(uart550->uart550.BaseAddress, XUN_THR_OFFSET, buffer[i]);
                }

                /* 设置发送为忙状态 */
                uart550->tx_idle = 0;
                /* 累计已经发送的字节数 */
                tx_size += temp;
            }
            else{
                //发送FIFO非空
                taskEXIT_CRITICAL();
                xSemaphoreTake(uart550->tx_sync, portMAX_DELAY);
                return ERR_IO;
            }
        }

        /* 将剩余数据继续写入发送FIFO */
        if(tx_size < (ssize_t)size) {
            result = fifo_in_no_schedule(uart550->tx_fifo, (void*)&p_data[tx_size], size-tx_size);
            if(result > 0) {
                tx_size += result;
            }
        }

        /* 退出临界区 */
        taskEXIT_CRITICAL();

        /* 发送完成则退出循环 */
        if(tx_size >= (ssize_t)size) {
            break;
        }

        /* 等待串口完成发送完一帧数据 */
        if(xSemaphoreTake(uart550->tx_sync, portMAX_DELAY) != pdPASS) {
            LOG_E("take tx_sync failed");
            break;
        }
    }

    /* 解锁 */
    if(xSemaphoreGiveRecursive(uart550->tx_lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    return tx_size;
}

static struct device_ops ops =
{
    .init = __uart550_init,
    .open = NULL,
    .release = __uart550_release,
    .pread = __uart550_read,
    .pwrite = __uart550_write,
    .control = NULL
};

/*
 * 初始化AXI UART16550设备
 *
 * @return 成功返回EOK
 */
int32_t axi_uart550_init(void)
{
    int32_t result;
    uint32_t i;
    XUartNs550Format format;

    for(i=0; i<sizeof(axi_uart550)/sizeof(axi_uart550[0]); i++) {
        axi_uart550[i].config = &uart550_config[i];
        axi_uart550[i].stop_flag = 1;

        //创建发送互斥锁
        axi_uart550[i].tx_lock = xSemaphoreCreateRecursiveMutex();
        if(!axi_uart550[i].tx_lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }
        /* 创建发送同步信号量 */
        axi_uart550[i].tx_sync = xSemaphoreCreateBinary();
        if(!axi_uart550[i].tx_sync) {
            LOG_E("create tx_sync failed");
            return ERR_NOMEM;
        }
        //创建发送FIFO
        axi_uart550[i].tx_fifo = fifo_create(axi_uart550[i].config->tx_fifo_size);
        if(!axi_uart550[i].tx_fifo) {
            LOG_E("create rx_fifo failed");
            return ERR_NOMEM;
        }

        //创建接收FIFO
        axi_uart550[i].rx_fifo = fifo_create(axi_uart550[i].config->rx_fifo_size);
        if(!axi_uart550[i].rx_fifo) {
            LOG_E("create rx_fifo failed");
            return ERR_NOMEM;
        }

        //初始化串口
        result = XUartNs550_Initialize(&axi_uart550[i].uart550, axi_uart550[i].config->device_id);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        result = XUartNs550_SelfTest(&axi_uart550[i].uart550);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        //设置串口数据格式率
        format.BaudRate = axi_uart550[i].config->baud_rate;
        format.DataBits =axi_uart550[i].config->data_bits;
        format.Parity = axi_uart550[i].config->parity;
        format.StopBits = axi_uart550[i].config->stop_bits;
        result = XUartNs550_SetDataFormat(&axi_uart550[i].uart550, &format);
        if (result != XST_SUCCESS) {
            LOG_E("set data format failed");
            return ERR_IO;
        }
        //设置FIFO阈值
        result = XUartNs550_SetFifoThreshold(&axi_uart550[i].uart550, XUN_FIFO_TRIGGER_14);
        if (result != XST_SUCCESS) {
            LOG_E("set FIFO trigger level failed");
            return ERR_IO;
        }

        //设置中断优先级
        set_intr_priority_trigger(axi_uart550[i].config->intr_id, 0xA0, 0x3);
        //注册串口中断处理函数
        xPortInstallInterruptHandler(axi_uart550[i].config->intr_id, (Xil_ExceptionHandler)__irq_handler, (void *)&axi_uart550[i]);
        //将中断映射到CPU1
        intr_map_to_cpu1(axi_uart550[i].config->intr_id);
        //使能串口中断
        vPortEnableInterrupt(axi_uart550[i].config->intr_id);

        //注册串口设备
        axi_uart550[i].device.magic = AXI_UART550_MAGIC;
        axi_uart550[i].device.ops = &ops;
        result = device_register(&axi_uart550[i].device, axi_uart550[i].config->device_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }

    return ERR_OK;
}

#endif /* USING_AXI_UART550 */
