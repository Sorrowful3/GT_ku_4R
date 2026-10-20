/**
 * @file    axi_dma.c
 * @brief   实现PL端的DMA IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月8日
 */

#include "axi_dma.h"

#define DEBUG_TAG "axi_dma,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_DMA

/* AXI DMA配置参数列表 */
static axi_dma_config_t dma_config[] = {
#ifdef AXI_DMA0_CONFIG
    AXI_DMA0_CONFIG,
#endif
#ifdef AXI_DMA1_CONFIG
    AXI_DMA1_CONFIG,
#endif
#ifdef AXI_DMA2_CONFIG
    AXI_DMA2_CONFIG,
#endif
#ifdef AXI_DMA3_CONFIG
    AXI_DMA3_CONFIG,
#endif
#ifdef AXI_DMA4_CONFIG
    AXI_DMA4_CONFIG,
#endif
};

/* AXI DMA句柄 */
static axi_dma_t axi_dma[sizeof(dma_config)/sizeof(dma_config[0])];

/* AXI DMA收发缓存，因为DMA传输时需要保持cache一致，因此内存地址必须对其到cache行长度。 */
static uint8_t rx_buffer[sizeof(dma_config)/sizeof(dma_config[0])][RX_BUFFER_SIZE] __attribute__((aligned(DMA_ALIGN)));
static uint8_t tx_buffer[sizeof(dma_config)/sizeof(dma_config[0])][TX_BUFFER_SIZE] __attribute__((aligned(DMA_ALIGN)));

/**
 * @function nane: __tx_handler
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              callback 注册中断回调函数时绑定的回调函数传输，这里是一个DMA设备句柄
 * @output param:
 *              无
 * @description:
 * DMA发送完成中断回调函数
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static void __tx_handler(void *callback)
{
    uint32_t irq_status;
    BaseType_t task_woken;
    axi_dma_t *dma = (axi_dma_t *)callback;

    /* 验证形参，判断是否是一个DMA句柄 */
    if((!dma) || (device_check(&dma->device, AXI_DMA_MAGIC) != ERR_OK)) {
        return;
    }

    /* 获取中断标志 */
    irq_status = XAxiDma_IntrGetIrq(&dma->axi_dma, XAXIDMA_DMA_TO_DEVICE);
    /* 清除中断标志 */
    XAxiDma_IntrAckIrq(&dma->axi_dma, irq_status, XAXIDMA_DMA_TO_DEVICE);
    /* 如果没有中断标志位置位则退出 */
    if (!(irq_status & XAXIDMA_IRQ_ALL_MASK)) {
        return;
    }
    /* 检查是否发送了传输完成中断 */
    if(irq_status & XAXIDMA_IRQ_IOC_MASK) {
        /* 产生发送同步信号 */
        task_woken = pdFALSE;
        xSemaphoreGiveFromISR(dma->tx_sync, &task_woken);
        /* 进行一次中断级任务调度 */
        portYIELD_FROM_ISR(task_woken);
    }
}

/**
 * @function nane: __rx_handler
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              callback 注册中断回调函数时绑定的回调函数传输，这里是一个DMA设备句柄
 * @output param:
 *              无
 * @description:
 * DMA接收完成中断回调函数
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static void __rx_handler(void *callback)
{
    uint32_t status;
    uint32_t result;
    uint32_t recv_lenght;
    BaseType_t task_woken;
    axi_dma_t *dma = (axi_dma_t *)callback;

    /* 验证形参，判断是否是一个DMA句柄 */
    if((!dma) || (device_check(&dma->device, AXI_DMA_MAGIC) != ERR_OK)) {
        return;
    }

    /* 获取中断标志 */
    status = XAxiDma_IntrGetIrq(&dma->axi_dma, XAXIDMA_DEVICE_TO_DMA);
    /* 清除中断标志 */
    XAxiDma_IntrAckIrq(&dma->axi_dma, status, XAXIDMA_DEVICE_TO_DMA);
    /* 如果没有中断标志位置位则退出 */
    if (!(status & XAXIDMA_IRQ_ALL_MASK)) {
        return;
    }
    /* 检查是否发送了传输完成中断 */
    if(status & XAXIDMA_IRQ_IOC_MASK) {
        /* 获取DMA接收了的字节数 */
        recv_lenght = XAxiDma_ReadReg(dma->axi_dma.RxBdRing[0].ChanBase, XAXIDMA_BUFFLEN_OFFSET);
        /* cache失效 */
        Xil_DCacheInvalidateRange((UINTPTR)dma->rx_buffer, RX_BUFFER_SIZE);

        /* 将接收的数据写入FIFO */
        fifo_in_force_isr(dma->rx_fifo, dma->rx_buffer, recv_lenght, &task_woken);
        portYIELD_FROM_ISR(task_woken);

        /* 如果停止标志置位则不启动新的接收任务 */
        if(dma->stop_flag) {
            return;
        }

        /* 重启接收 */
        result = XAxiDma_SimpleTransfer(&dma->axi_dma, (UINTPTR) dma->rx_buffer, RX_BUFFER_SIZE, XAXIDMA_DEVICE_TO_DMA);
        if(result != XST_SUCCESS) {
            LOG_E("restart receive failed");
        }
    }
}

/**
 * @function nane: __dma_init
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个DMA设备句柄
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 初始化AXI DMA设备，在第一次执行open_device时调用
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static int32_t __dma_init(device_t *device)
{
    uint32_t status;
    ssize_t result;
    axi_dma_t *dma = container_of(device, axi_dma_t, device);

    /* 设置发送为空闲 */
    xSemaphoreGive(dma->tx_sync);
    /* 清除发送中断标志 */
    status = XAxiDma_IntrGetIrq(&dma->axi_dma, XAXIDMA_DMA_TO_DEVICE);
    XAxiDma_IntrAckIrq(&dma->axi_dma, status, XAXIDMA_DMA_TO_DEVICE);
    /* 使能所有发送DMA事件 */
    XAxiDma_IntrDisable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
    XAxiDma_IntrEnable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

    /* 清空接收FIFO */
    for(; ; ) {
        result = fifo_out(dma->rx_fifo, dma->rx_buffer, RX_BUFFER_SIZE, 0);
        if(result < RX_BUFFER_SIZE) {
            break;
        }
    }
    /* 清除接收中断标志 */
    status = XAxiDma_IntrGetIrq(&dma->axi_dma, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrAckIrq(&dma->axi_dma, status, XAXIDMA_DEVICE_TO_DMA);
    /* 使能所有接收DMA事件 */
    XAxiDma_IntrDisable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrEnable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    /* 允许启动发送和接收 */
    dma->stop_flag = 0;

    /* 开启接收 */
    status = XAxiDma_SimpleTransfer(&dma->axi_dma, (UINTPTR)dma->rx_buffer, RX_BUFFER_SIZE, XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
        dma->stop_flag = 1;
        LOG_E("start receive failed");
        return ERR_IO;
    }

    return ERR_OK;
}

/**
 * @function nane: __dma_release
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个DMA设备句柄
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 反初始化DMA设备，在最后一次调用device_release时被调用
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static int32_t __dma_release(device_t *device)
{
    uint32_t i;
    axi_dma_t *dma = container_of(device, axi_dma_t, device);

    /* 停止启动新的发送、接收任务 */
    dma->stop_flag = 1;

    /* 等待发送空闲 */
    for(i=0; XAxiDma_Busy(&dma->axi_dma, XAXIDMA_DMA_TO_DEVICE)&&i<20; i++) {
        vTaskDelay(2);
    }
    if(i >= 20) {
        dma->stop_flag = 0;
        LOG_E("wait idle timeout");
        return ERR_IO;
    }
    /* 禁止所有DMA发送事件 */
    XAxiDma_IntrDisable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

    /* 等待接收空闲 */
    for(i=0; XAxiDma_Busy(&dma->axi_dma, XAXIDMA_DEVICE_TO_DMA)&&i<20; i++) {
        vTaskDelay(2);
    }
    if(i >= 20) {
        dma->stop_flag = 0;
        LOG_E("wait idle timeout");
        return ERR_IO;
    }
    /* 禁止所有DMA接收事件 */
    XAxiDma_IntrDisable(&dma->axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    return ERR_OK;
}

/**
 * @function nane: __dma_read
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个DMA设备句柄
 *              buffer 用户缓存，用于存储读取到的字节数
 *              size 要读取的字节数
 *              pos 偏移，此处未实现
 * @output param:
 *              return 读取成功返回实际读取到的字节数，失败返回相应的错误码
 * @description:
 * 读取DMA接收到的数据
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static ssize_t __dma_read(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    axi_dma_t *dma = container_of(device, axi_dma_t, device);

    /* 从接收FIFO读取数据 */
    return fifo_out(dma->rx_fifo, buffer, size, device->receive_timeout);
}

/**
 * @function nane: __dma_write
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              device 设备句柄，这里应该是一个DMA设备句柄
 *              buffer 用户缓存，用于存储要发送的字节数
 *              size 要发送的字节数
 *              pos 偏移，此处未实现
 * @output param:
 *              return 读取成功返回实际发送的字节数，失败返回相应的错误码
 * @description:
 * 通过DMA发送数据包
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
static ssize_t __dma_write(device_t *device, const void *buffer, size_t size, uint32_t pos)
{
    uint32_t result;
    size_t tx_size;
    size_t tmp_size;
    const uint8_t *p_data = (const uint8_t*)buffer;
    axi_dma_t *dma = container_of(device, axi_dma_t, device);

    /* 停止标志置位后不能继续发送 */
    if(dma->stop_flag) {
        return ERR_IO;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(dma->tx_lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    for(tx_size=0; tx_size<size; tx_size+=tmp_size) {
        /* 等待发送DMA空闲 */
        if(xSemaphoreTake(dma->tx_sync, 15) != pdPASS) {
            xSemaphoreGiveRecursive(dma->tx_lock);
            LOG_E("wait timeout!");
            return ERR_ROUTINE;
        }

        /* 获取本次可以发送的字节数 */
        if((size - tx_size) > TX_BUFFER_SIZE) {
            tmp_size = TX_BUFFER_SIZE;
        }
        else {
            tmp_size = size - tx_size;
        }

        /* 拷贝数据到发送缓存 */
        memcpy(dma->tx_buffer, &p_data[tx_size], tmp_size);
        /* cache写回 */
        Xil_DCacheFlushRange((INTPTR)dma->tx_buffer, TX_BUFFER_SIZE);

        /* 启动DMA发送 */
        result = XAxiDma_SimpleTransfer(&dma->axi_dma, (UINTPTR)dma->tx_buffer, tmp_size, XAXIDMA_DMA_TO_DEVICE);
        if(result != XST_SUCCESS) {
            xSemaphoreGiveRecursive(dma->tx_lock);
            LOG_E("wait timeout!");
            return ERR_IO;
        }
    }

    /* 解锁 */
    if(xSemaphoreGiveRecursive(dma->tx_lock) != pdPASS) {
        LOG_E("tx unlock failed");
        return ERR_ROUTINE;
    }

    return tx_size;
}

/* 操作DMA设备的函数集合 */
static struct device_ops ops =
{
    .init = __dma_init,
    .open = NULL,
    .release = __dma_release,
    .pread = __dma_read,
    .pwrite = __dma_write,
    .control = NULL,
};

/**
 * @function nane: axi_dma_init
 * @author: liaofei
 * @data: 2021年3月8日
 * @input param:
 *              无
 * @output param:
 *              成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 根据配置宏初始化相应的DMA，并将其注册到设备管理框架
 * @modeify log:
 * 2021年3月8日 liaofei 初次创建
 */
int32_t axi_dma_init(void)
{
    int32_t result;
    uint32_t i;
    XAxiDma_Config *config;

    for(i=0; i<sizeof(axi_dma)/sizeof(axi_dma[0]); i++) {
        /* 绑定配置参数、收发缓存，复位停止标志 */
        axi_dma[i].config = &dma_config[i];
        axi_dma[i].tx_buffer = tx_buffer[i];
        axi_dma[i].rx_buffer = rx_buffer[i];
        axi_dma[i].stop_flag = 1;

        /* 创建发送同步信号量 */
        axi_dma[i].tx_sync = xSemaphoreCreateBinary();
        if(!axi_dma[i].tx_sync) {
            LOG_E("create tx sync failed");
            return ERR_NOMEM;
        }
        /* 创建发送互斥量 */
        axi_dma[i].tx_lock = xSemaphoreCreateRecursiveMutex();
        if(!axi_dma[i].tx_lock) {
            LOG_E("create tx lock failed");
            return ERR_NOMEM;
        }

        /* 创建接收fifo */
        axi_dma[i].rx_fifo = fifo_create(axi_dma[i].config->rx_fifo_size);
        if(!axi_dma[i].rx_fifo) {
            LOG_E("create rx fifo failed");
            return ERR_NOMEM;
        }

        /* 初始化DMA外设 */
        config = XAxiDma_LookupConfig(axi_dma[i].config->device_id);
        if (!config) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        result = XAxiDma_CfgInitialize(&axi_dma[i].axi_dma, config);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        if(XAxiDma_HasSg(&axi_dma[i].axi_dma)) {
            LOG_E("initialize failed");
            return ERR_IO;
        }

        /* 配置发送DMA中断 */
        /* 设置优先级和触发方式 */
        set_intr_priority_trigger(axi_dma[i].config->tx_intr_id, 0xA0, 0x3);
        /* 注册GIC中断回调函数 */
        xPortInstallInterruptHandler(axi_dma[i].config->tx_intr_id, (Xil_ExceptionHandler)__tx_handler, (void *)&axi_dma[i]);
        /* 将GIC中断映射到CPU1 */
        intr_map_to_cpu1(axi_dma[i].config->tx_intr_id);
        /* 暂时禁止DMA事件 */
        XAxiDma_IntrDisable(&axi_dma[i].axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
        /* 使能GIC的中断号 */
        vPortEnableInterrupt(axi_dma[i].config->tx_intr_id);

        /* 配置接收DMA中断 */
        /* 设置优先级和触发方式 */
        set_intr_priority_trigger(axi_dma[i].config->rx_intr_id, 0xA0, 0x3);
        /* 注册GIC中断回调函数 */
        xPortInstallInterruptHandler(axi_dma[i].config->rx_intr_id, (Xil_ExceptionHandler)__rx_handler, (void *)&axi_dma[i]);
        /* 将GIC中断映射到CPU1 */
        intr_map_to_cpu1(axi_dma[i].config->rx_intr_id);
        /* 暂时禁止DMA事件 */
        XAxiDma_IntrDisable(&axi_dma[i].axi_dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
        /* 使能GIC的中断号 */
        vPortEnableInterrupt(axi_dma[i].config->rx_intr_id);

        /* 注册DMA设备 */
        axi_dma[i].device.magic = AXI_DMA_MAGIC;
        axi_dma[i].device.ops = &ops;
        result = device_register(&axi_dma[i].device, axi_dma[i].config->device_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }

    return ERR_OK;
}

#endif /* USING_AXI_DMA */
