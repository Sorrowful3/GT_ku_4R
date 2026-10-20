/**
 * @file    axi_uartlite.c
 * @brief   实现AXI UARTLite IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月8日
 */

#include "axi_uartlite.h"

#define DEBUG_TAG "axi_uartlite,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_UARTLITE

/* UARTLite 硬件FIFO大小 */
#ifndef UARTLITE_HW_RX_FIFO
#define UARTLITE_HW_RX_FIFO 16
#endif
#ifndef UARTLITE_HW_TX_FIFO
#define UARTLITE_HW_TX_FIFO 16
#endif

#if UARTLITE_HW_RX_FIFO > UARTLITE_HW_TX_FIFO
#define UARTLITE_HW_FIFO UARTLITE_HW_RX_FIFO
#else
#define UARTLITE_HW_FIFO UARTLITE_HW_TX_FIFO
#endif

/* uartlite配置参数列表 */
static axi_uartlite_config_t  uartlite_config[] = {
#ifdef AXI_UARTLITE0_CONFIG
    AXI_UARTLITE0_CONFIG,
#endif
#ifdef AXI_UARTLITE1_CONFIG
    AXI_UARTLITE1_CONFIG,
#endif
#ifdef AXI_UARTLITE2_CONFIG
    AXI_UARTLITE2_CONFIG,
#endif
#ifdef AXI_UARTLITE3_CONFIG
    AXI_UARTLITE3_CONFIG,
#endif
#ifdef AXI_UARTLITE4_CONFIG
    AXI_UARTLITE4_CONFIG,
#endif
#ifdef AXI_UARTLITE5_CONFIG
    AXI_UARTLITE5_CONFIG,
#endif
};

/* uartlite设备句柄 */
static axi_uartlite_t axi_uartlite[sizeof(uartlite_config)/sizeof(uartlite_config[0])];

/**
 * @function nane: __irq_handler
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              arg 中断函数参数，这里应该是一个uartlite设备句柄
 * @output param:
 *              无
 * @description:
 * uartlite中断回调函数，由uartlite中断调用
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static void __irq_handler(void *arg)
{
    uint32_t status_register;
    uint32_t isr_status;
    ssize_t i;
    ssize_t result;
    BaseType_t task_woken;
    uint8_t buffer[UARTLITE_HW_FIFO];
    axi_uartlite_t *uartlite = (axi_uartlite_t*)arg;

    if((!uartlite) || (device_check(&uartlite->device, AXI_UARTLITE_MAGIC) != ERR_OK)) {
        return;
    }

    /* 读取中断状态寄存器，获取中断状态 */
    isr_status = XUartLite_ReadReg(uartlite->uartlite.RegBaseAddress, XUL_STATUS_REG_OFFSET);

    /* 处理接收中断 */
    if ((isr_status & (XUL_SR_RX_FIFO_FULL | XUL_SR_RX_FIFO_VALID_DATA)) != 0) {
        /* 从物理FIFO读取串口收到数据 */
        for(i=0; i<UARTLITE_HW_RX_FIFO; ) {
            /* 从物理FIFO读取一个byte */
            buffer[i++] = XUartLite_ReadReg(uartlite->uartlite.RegBaseAddress, XUL_RX_FIFO_OFFSET);
            /* 读取状态寄存器 */
            status_register = XUartLite_ReadReg(uartlite->uartlite.RegBaseAddress, XUL_STATUS_REG_OFFSET);
            /* 检测物理FIFO是否位0 */
            if (!(status_register & XUL_SR_RX_FIFO_VALID_DATA)) {
                break;
            }
        }

        /* 将数据写入软件FIFO */
        fifo_in_force_isr(uartlite->rx_fifo, buffer, i, &task_woken);
        portYIELD_FROM_ISR(task_woken);
    }

    /* 处理发送中断 */
    if ((isr_status & XUL_SR_TX_FIFO_EMPTY) && !uartlite->tx_idle) {
        /* 从软件FIFO中读数据 */
        result = fifo_out_isr(uartlite->tx_fifo, (void*)buffer, UARTLITE_HW_TX_FIFO);

        if(result > 0) {
            /* 读取到数据则将数据写入硬件FIFO */
            for(i=0; i<result; i++) {
                XUartLite_WriteReg(uartlite->uartlite.RegBaseAddress, XUL_TX_FIFO_OFFSET, (uint32_t)buffer[i]);
            }

            /* 设置发送同步信号量，表示FIFO可以写入 */
            task_woken = pdFALSE;
            xSemaphoreGiveFromISR(uartlite->tx_sync, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }
        else {
            /*软件FIFO中没有数据则设置发送为空闲 */
            uartlite->tx_idle = 1;
        }
    }
}

/**
 * @function nane: __uartlite_init
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个uartlite的设备句柄
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 初始化AXI UART设备，在第一次执行open时调用
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */

static int32_t __uartlite_init(device_t *device)
{
    ssize_t result;
    uint8_t buffer[UARTLITE_HW_FIFO];
    axi_uartlite_t *uartlite = container_of(device, axi_uartlite_t, device);

    /* 清空接收软件FIFO */
    for(; ; ) {
        result = fifo_out(uartlite->rx_fifo, (void*)buffer, UARTLITE_HW_FIFO, 0);
        if(result < UARTLITE_HW_FIFO) {
            break;
        }
    }

    /* 清空发送软件FIFO */
    for(; ; ) {
        result = fifo_out(uartlite->tx_fifo, (void*)buffer, UARTLITE_HW_FIFO, 0);
        if(result < UARTLITE_HW_FIFO) {
            break;
        }
    }
    /* 复位发送同步信号量，表示串口还未发送过数据帧 */
    xSemaphoreTake(uartlite->tx_sync, 0);
    /* 设置发送状态为空闲 */
    uartlite->tx_idle = 1;
    /* 复位停止标志 */
    uartlite->stop_flag = 0;

    /* 复位UART的硬件FIFO */
    XUartLite_ResetFifos(&uartlite->uartlite);
    /* 使能串口中断 */
    XUartLite_EnableInterrupt(&uartlite->uartlite);

    return ERR_OK;
}

/**
 * @function nane: __uartlite_release
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个uartlite设备句柄
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 反初始化DMA设备，在最后一次调用device_release时被调用
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static int32_t __uartlite_release(device_t *device)
{
    uint32_t i;
    axi_uartlite_t *uartlite = container_of(device, axi_uartlite_t, device);

    /* 设置停止标志 */
    uartlite->stop_flag = 1;

    /* 等待发送空闲 */
    for(i=0; i<30; i++) {
        if(uartlite->tx_idle) {
            break;
        }
        vTaskDelay(2);
    }
    if(i >= 30) {
        uartlite->stop_flag = 0;
        LOG_E("wait idle timeout");
        return ERR_IO;
    }

    /* 禁止串口中断 */
    XUartLite_DisableInterrupt(&uartlite->uartlite);

    return ERR_OK;
}

/**
 * @function nane: __uartlite_read
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个uartlite设备句柄
 *              buffer 用户缓存，用于存储读取到的字节数
 *              size 要读取的字节数
 *              pos 偏移，此处未实现
 * @output param:
 *              return 读取成功返回实际读取到的字节数，失败返回相应的错误码
 * @description:
 * 读取uartlite接收到的数据
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static ssize_t __uartlite_read(device_t *device, void *user_buf, size_t size, uint32_t pos)
{
    axi_uartlite_t *uartlite = container_of(device, axi_uartlite_t, device);

    /* 从接收FIFO读取数据 */
    return fifo_out(uartlite->rx_fifo, user_buf, size, device->receive_timeout);
}

/**
 * @function nane: __uartlite_write
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个uartlite设备句柄
 *              buffer 用户缓存，用于存储要发送的字节数
 *              size 要发送的字节数
 *              pos 偏移，此处未实现
 * @output param:
 *              return 读取成功返回实际发送的字节数，失败返回相应的错误码
 * @description:
 * 通过uartlite发送数据
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static ssize_t __uartlite_write(device_t *device, const void *user_buf, size_t size, uint32_t pos)
{
    ssize_t i;
    ssize_t tx_size;
    ssize_t result;
    ssize_t temp;
    uint8_t buffer[UARTLITE_HW_FIFO];
    const uint8_t *p_data = (const uint8_t*)user_buf;
    axi_uartlite_t *uartlite = container_of(device, axi_uartlite_t, device);

    /* 停止标志置位不在发送新数据包 */
    if(uartlite->stop_flag) {
        return ERR_IO;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(uartlite->tx_lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    for(tx_size=0; ; ) {
        /* 进入临界区 */
        taskENTER_CRITICAL();

        /* 如果发送空闲则启动发送 */
        if(uartlite->tx_idle) {
            /* 将数据拷贝到发送缓冲区 */
            temp = ((size-tx_size) < UARTLITE_HW_TX_FIFO) ? (size-tx_size) : UARTLITE_HW_TX_FIFO;
            memcpy(buffer, (void*)&p_data[tx_size], temp);
            
            /* 将数据写入硬件FIFO，以此启动发送 */
            for(i=0; i<temp; i++) {
                XUartLite_WriteReg(uartlite->uartlite.RegBaseAddress, XUL_TX_FIFO_OFFSET, (uint32_t)buffer[i]);
            }

            /* 设置发送为忙状态 */
            uartlite->tx_idle = 0;
            /* 累计已经发送的字节数 */
            tx_size += temp;
        }

        /* 将剩余数据继续写入发送FIFO */
        if(tx_size < (ssize_t)size) {
            result = fifo_in_no_schedule(uartlite->tx_fifo, (void*)&p_data[tx_size], size-tx_size);
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
        if(xSemaphoreTake(uartlite->tx_sync, portMAX_DELAY) != pdPASS) {
            LOG_E("take tx_sync failed");
            break;
        }
    }

    /* 解锁 */
    if(xSemaphoreGiveRecursive(uartlite->tx_lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    return tx_size;
}


static struct device_ops ops =
{
    .init = __uartlite_init,
    .open = NULL,
    .release = __uartlite_release,
    .pread = __uartlite_read,
    .pwrite = __uartlite_write,
    .control = NULL,
};

/**
 * @function nane: axi_uartlite_init
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              无
 * @output param:
 *              成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 根据配置宏初始化相应的uartlite，并将其注册到设备管理框架
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
int32_t axi_uartlite_init(void)
{
    uint32_t i;
    int32_t result;

    for(i=0; i<sizeof(axi_uartlite)/sizeof(axi_uartlite[0]); i++) {
        /* 绑定配置参数，复位停止标志 */
        axi_uartlite[i].config = &uartlite_config[i];
        axi_uartlite[i].stop_flag = 1;

        /* 创建接收fifo */
        axi_uartlite[i].rx_fifo = fifo_create(axi_uartlite[i].config->rx_fifo_size);
        if(!axi_uartlite[i].rx_fifo) {
            LOG_E("create rx_fifo failed");
            return ERR_NOMEM;
        }

        /* 创建互斥锁 */
        axi_uartlite[i].tx_lock = xSemaphoreCreateRecursiveMutex();
        if(!axi_uartlite[i].tx_lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }
        /* 创建发送同步信号量 */
        axi_uartlite[i].tx_sync = xSemaphoreCreateBinary();
        if(!axi_uartlite[i].tx_sync) {
            LOG_E("create tx_sync semaphore failed");
            return ERR_NOMEM;
        }
        /* 创建发送fifo */
        axi_uartlite[i].tx_fifo = fifo_create(axi_uartlite[i].config->tx_fifo_size);
        if(!axi_uartlite[i].tx_fifo) {
            LOG_E("create tx_fifo failed");
            return ERR_NOMEM;
        }

        /* 初始化串口 */
        result = XUartLite_Initialize(&axi_uartlite[i].uartlite, axi_uartlite[i].config->device_id);
        if (result != XST_SUCCESS) {
            LOG_E("initial failed");
            return ERR_IO;
        }
        result = XUartLite_SelfTest(&axi_uartlite[i].uartlite);
        if (result != XST_SUCCESS) {
            LOG_E("initial failed");
            return ERR_IO;
        }

        /* 设置AXI UART中断优先级 */
        set_intr_priority_trigger(axi_uartlite[i].config->intr_id, 0xA0, 0x3);
        /* 注册AXI UART中断处理函数 */
        xPortInstallInterruptHandler(axi_uartlite[i].config->intr_id, __irq_handler, (void *)&axi_uartlite[i]);
        /* 将中断映射到CPU1 */
        intr_map_to_cpu1(axi_uartlite[i].config->intr_id);
        /* 使能AXI UART中断 */
        vPortEnableInterrupt(axi_uartlite[i].config->intr_id);

        /* 注册串口设备 */
        axi_uartlite[i].device.magic = AXI_UARTLITE_MAGIC;
        axi_uartlite[i].device.ops = &ops;
        result = device_register(&axi_uartlite[i].device, axi_uartlite[i].config->device_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }

    return ERR_OK;
}

#endif /* USING_AXI_UARTLITE */
