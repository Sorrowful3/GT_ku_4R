/*
 * @file    fifo.c
 * @brief   实现FIFO相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#include "fifo.h"

#define DEBUG_TAG "fifo,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "debug.h"

/**
 * @function nane: __fifo_lenght
 * @author: liaofei
 * @date: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 * @output param:
 *              return 返回FIFO中有效数据的字节数
 * @description:
 * 获取FIFO中的有效数据的长度
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static size_t __fifo_lenght(fifo_t *fifo)
{
    if(fifo->r_index == fifo->w_index) {
        return fifo->is_empty ? 0 : fifo->size;
    }
    else if(fifo->w_index > fifo->r_index) {
        return fifo->w_index - fifo->r_index;
    }
    else {
        return fifo->size - fifo->r_index + fifo->w_index;
    }
}

/**
 * @function nane: __update_r_index
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              size 已读取的字节数
 * @output param:
 *              无
 * @description:
 * 更新FIFO的读索引
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static void __update_r_index(fifo_t *fifo, size_t size)
{
    if(size >= __fifo_lenght(fifo)) {
        fifo->is_empty = 1;
        fifo->r_index = fifo->w_index;
    }
    else {
        fifo->r_index += size;
        if(fifo->r_index >= fifo->size) {
            fifo->r_index %= fifo->size;
        }
    }
}

/**
 * @function nane: __update_w_index
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              size 已写入的字节数
 * @output param:
 *              无
 * @description:
 * 更新FIFO的写索引
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static void __update_w_index(fifo_t *fifo, size_t size)
{
    if((fifo->r_index == fifo->w_index) && (!fifo->is_empty)) {
        /* FIFO已满 */
        fifo->w_index += size;
        if(fifo->w_index >= fifo->size) {
            fifo->w_index %= fifo->size;
        }
        fifo->r_index = fifo->w_index;
    }
    else if(fifo->r_index <= fifo->w_index) {
        fifo->w_index += size;
        if(fifo->w_index >= fifo->size) {
            fifo->w_index %= fifo->size;
            if(fifo->w_index >= fifo->r_index) {
                fifo->r_index = fifo->w_index;
                fifo->is_empty = 0;
            }
        }
    }
    else {
        fifo->w_index += size;
        if(fifo->w_index >= fifo->r_index) {
            if(fifo->w_index >= fifo->size) {
                fifo->w_index %= fifo->size;
            }
            fifo->r_index = fifo->w_index;
            fifo->is_empty = 0;
        }
    }
}

/**
 * @function nane: __fifo_in_force
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 欲写入的数据
 *              size 欲写入的字节数
 * @output param:
 *              return 返回成功写入的字节数
 * @description:
 * 将数据强制写入FIFO
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static ssize_t __fifo_in_force(fifo_t *fifo, const void *data, size_t size)
{
    size_t temp_bytes;
    const uint8_t *pdata = (const uint8_t*)data;
    uint8_t *ppool = (uint8_t*)fifo->pool;

    if(size > fifo->size) {
        size =  fifo->size;
    }

    temp_bytes = fifo->size - fifo->w_index;
    if(size > temp_bytes) {
        memcpy(&ppool[fifo->w_index], &pdata[0], temp_bytes);
        memcpy(&ppool[0], &pdata[temp_bytes], size-temp_bytes);
    }
    else {
        memcpy(&ppool[fifo->w_index], &pdata[0], size);
    }

    __update_w_index(fifo, size);
    return (ssize_t)size;
}

/**
 * @function nane:
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              buffer 读取缓存
 *              size 欲读取的字节数
 * @output param:
 *              return 返回成功读取的字节数
 * @description:
 * 从FIFO读取数据
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
static ssize_t __fifo_out(fifo_t *fifo, void *buffer, size_t size)
{
    size_t temp_bytes;
    uint8_t *pbuffer = (uint8_t*)buffer;
    uint8_t *ppool = (uint8_t*)fifo->pool;

    temp_bytes = __fifo_lenght(fifo);
    if(size > temp_bytes) {
        size = temp_bytes;
    }
    if(size == 0) {
        return size;
    }

    temp_bytes = fifo->size - fifo->r_index;
    if(size > temp_bytes) {
        memcpy(&pbuffer[0], &ppool[fifo->r_index], temp_bytes);
        memcpy(&pbuffer[temp_bytes], &ppool[0], size-temp_bytes);
    }
    else {
        memcpy(&pbuffer[0], &ppool[fifo->r_index], size);
    }

    __update_r_index(fifo, size);
    return (ssize_t)size;
}

/**
 * @function nane: fifo_create
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              size FIFO内存池大小
 * @output param:
 *              return 运行成功返回FIFO，失败返回NULL
 * @description:
 * 创建FIFO
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
fifo_t *fifo_create(size_t size)
{
    fifo_t *fifo;
    void *pool;

    if((size == 0) || (size > 0x7FFFFFFF)) {
        LOG_E("parameter error");
        return NULL;
    }

    /* 分配FIFO的内存池 */
    pool = pvPortMalloc(size);
    if(!pool) {
        LOG_E("malloc failed");
        return NULL;
    }

    /* 分配FIFO句柄 */
    fifo = (fifo_t*)pvPortMalloc(sizeof(fifo_t));
    if(!fifo) {
        vPortFree(pool);
        LOG_E("malloc failed");
        return NULL;
    }

    /* 创建挂起线程的信号量 */
    fifo->suspend = xSemaphoreCreateBinary();
    if(!fifo->suspend) {
        vPortFree(pool);
        vPortFree(fifo);
        LOG_E("create semaphore failed");
        return NULL;
    }

    /* 初始化FIFO池 */
    fifo->pool = pool;
    fifo->size = size;
    /* 设置FIFO为空 */
    fifo->is_empty = 1;
    fifo->r_index = 0;
    fifo->w_index = 0;

    return fifo;
}

/**
 * @function nane: fifo_length
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo fifo FIFO句柄
 * @output param:
 *              return 返回FIFO中的数据的字节数
 * @description:
 * 获取FIFO中有效数据的长度
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_length(fifo_t *fifo)
{
    ssize_t size;

    if(!fifo) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 获取FIFO长度 */
    taskENTER_CRITICAL();
    size = (ssize_t)__fifo_lenght(fifo);
    taskEXIT_CRITICAL();

    return size;
}

/**
 * @function nane: fifo_length_isr
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo fifo FIFO句柄
 * @output param:
 *              return 返回FIFO中的数据的字节数
 * @description:
 * 获取FIFO中有效数据的长度，此函数只能在中断中使用
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_length_isr(fifo_t *fifo)
{
    ssize_t size;
    UBaseType_t interrupt_status;

    if(!fifo) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 获取FIFO长度 */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();
    interrupt_status = portSET_INTERRUPT_MASK_FROM_ISR();
    size = (ssize_t)__fifo_lenght(fifo);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);

    return size;
}

/**
 * @function nane: fifo_in_force
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 待写入的数据
 *              size 期望写入的字节数
 * @output param:
 *              return 将数据强制写入FIFO
 * @description:
 * 将数据强制写入FIFO，此函数可以覆盖最先写入的数据
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in_force(fifo_t *fifo, const void *data, size_t size)
{
    ssize_t result;

    if((!fifo) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 将数据写入FIFO */
    taskENTER_CRITICAL();
    result = __fifo_in_force(fifo, data, size);
    taskEXIT_CRITICAL();

    /* 写入成功则唤醒第一个线程 */
    if(result > 0) {
        xSemaphoreGive(fifo->suspend);
    }

    return result;
}

/**
 * @function nane: fifo_in_force_isr
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 待写入的数据
 *              size 期望写入的字节数
 *              task_woken 是否有高优先级任务进入就绪状态，==pdFALSE表示无更高优先级线程就绪
 * @output param:
 *              return 将数据强制写入FIFO
 * @description:
 * 将数据强制写入FIFO，此函数可以覆盖最先写入的数据，且只能在中断中调用
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in_force_isr(fifo_t *fifo, const void *data, size_t size, BaseType_t *task_woken)
{
    ssize_t result;
    UBaseType_t interrupt_status;

    if((!data) || (!fifo) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 将数据写入FIFO */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();
    interrupt_status = portSET_INTERRUPT_MASK_FROM_ISR();
    result = __fifo_in_force(fifo, data, size);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);

    /* 写入成功则唤醒第一个线程 */
    if(result > 0) {
        if(task_woken) {
            *task_woken = pdFALSE;
        }
        xSemaphoreGiveFromISR(fifo->suspend, task_woken);
    }

    return result;
}

/**
 * @function nane: fifo_in
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 待写入的数据
 *              size 期望写入的字节数
 * @output param:
 *              return 将数据强制写入FIFO
 * @description:
 * 将数据强制写入FIFO
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in(fifo_t *fifo, const void *data, size_t size)
{
    size_t empty_lenght;
    ssize_t result;

    if((!fifo) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();

    /* 获取FIFO空闲长度 */
    empty_lenght = fifo->size - __fifo_lenght(fifo);
    if(empty_lenght == 0)  {
        taskEXIT_CRITICAL();
        LOG_W("FIFO full");
        return  ERR_FULL;
    }

    if(size > empty_lenght) {
        size = empty_lenght;
    }
    /* 将数据写入FIFO */
    result = __fifo_in_force(fifo, data, size);

    taskEXIT_CRITICAL();

    /* 写入成功则唤醒第一个线程 */
    if(result > 0) {
        xSemaphoreGive(fifo->suspend);
    }

    return result;
}

/**
 * @function nane: fifo_in_isr
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 待写入的数据
 *              size 期望写入的字节数
 *              task_woken 是否有高优先级任务进入就绪状态，==pdFALSE表示无更高优先级线程就绪
 * @output param:
 *              return 将数据强制写入FIFO
 * @description:
 * 将数据强制写入FIFO，此函数只能在中断中调用
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in_isr(fifo_t *fifo, const void *data, size_t size, BaseType_t *task_woken)
{
    UBaseType_t interrupt_status;
    size_t empty_lenght;
    ssize_t result;

    if((!fifo) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();
    interrupt_status = portSET_INTERRUPT_MASK_FROM_ISR();

    /* 获取FIFO空闲长度 */
    empty_lenght = fifo->size - __fifo_lenght(fifo);
    if(empty_lenght == 0)  {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);
        LOG_W("FIFO full");
        return  ERR_FULL;
    }

    if(size > empty_lenght) {
        size = empty_lenght;
    }
    /* 将数据写入FIFO */
    result = __fifo_in_force(fifo, data, size);

    portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);

    /* 写入成功则唤醒第一个线程 */
    if(result > 0) {
        if(task_woken) {
            *task_woken = pdFALSE;
        }
        xSemaphoreGiveFromISR(fifo->suspend, task_woken);
    }
    return result;
}

/**
 * @function nane: fifo_out
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              buffer 存放数据的缓存
 *              size 期望读取的字节数
 *              timeout 最长等待时间
 * @output param:
 *              return 返回成功读取的字节数，失败返回错误码
 * @description:
 * 从FIFO读取数据
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_out(fifo_t *fifo, void *buffer, size_t size, TickType_t timeout)
{
    portBASE_TYPE xresult;
    ssize_t result;
    size_t fifo_lenght;

    if((!fifo) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 进入临界区 */
    taskENTER_CRITICAL();

    /* 获取FIFO数据长度 */
    fifo_lenght = __fifo_lenght(fifo);
    if(fifo_lenght == 0) {
        if(timeout == 0) {
            taskEXIT_CRITICAL();
            LOG_W("FIFO empty");
            return ERR_EMPTY;
        }
        /* 清空信号量，确保后面能挂起线程 */
        xSemaphoreTake(fifo->suspend, 0);
        taskEXIT_CRITICAL();

        /* 挂起当前线程，并进行调度 */
        xresult = xSemaphoreTake(fifo->suspend, timeout);
        if(xresult != pdTRUE) {
            LOG_W("wait timeout");
            return ERR_TIMEOUT;
        }

        taskENTER_CRITICAL();
        /* 再次获取FIFO数据长度，因为线程可能会被信号唤醒 */
        fifo_lenght = __fifo_lenght(fifo);
        if(fifo_lenght == 0) {
            taskEXIT_CRITICAL();
            LOG_E("FIFO error");
            return ERR_EMPTY;
        }
    }

    if(size > fifo_lenght) {
        size = fifo_lenght;
    }
    /* 从FIFO中读取数据 */
    result = __fifo_out(fifo, buffer, size);

    taskEXIT_CRITICAL();

    return result;
}

/**
 * @function nane: fifo_out_isr
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              buffer 存放数据的缓存
 *              size 期望读取的字节数
 * @output param:
 *              return 返回成功读取的字节数，失败返回错误码
 * @description:
 * 从FIFO读取数据，此函数只能在中断中调用
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_out_isr(fifo_t *fifo, void *buffer, size_t size)
{
    UBaseType_t interrupt_status;
    size_t fifo_lenght;
    ssize_t result;

    if((!fifo) || (!buffer) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 进入临界区 */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();
    interrupt_status = portSET_INTERRUPT_MASK_FROM_ISR();

    /* 获取FIFO数据长度 */
    fifo_lenght = __fifo_lenght(fifo);
    if(fifo_lenght == 0) {
        /* 退出临界区 */
        portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);
        LOG_W("FIFO empty");
        return ERR_EMPTY;
    }

    if(size > fifo_lenght) {
        size = fifo_lenght;
    }
    /* 从FIFO中读取数据 */
    result = __fifo_out(fifo, buffer, size);

    /* 退出临界区 */
    portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);

    return result;
}

/**
 * @function nane: fifo_in_no_schedule
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 欲写入的数据
 *              size 欲写入的字节数
 * @output param:
 *              return 返回成功写入的字节数，失败返回错误码
 * @description:
 * 将数据写入FIFO，不会唤醒正在等待读取FIFO的线程
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in_no_schedule(fifo_t *fifo, const void *data, size_t size)
{
    size_t empty_lenght;
    ssize_t result;

    if((!data) || (!fifo) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();

    /* 获取FIFO空闲长度 */
    empty_lenght = fifo->size - __fifo_lenght(fifo);
    if(empty_lenght == 0)  {
        taskEXIT_CRITICAL();
        LOG_W("FIFO empty");
        return ERR_FULL;
    }

    if(size > empty_lenght) {
        size = empty_lenght;
    }
    /* 将数据写入FIFO */
    result = __fifo_in_force(fifo, data, size);

    taskEXIT_CRITICAL();

    return result;
}

/**
 * @function nane: fifo_in_no_schedule_isr
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              fifo FIFO句柄
 *              data 欲写入的数据
 *              size 欲写入的字节数
 * @output param:
 *              return 返回成功写入的字节数，失败返回错误码
 * @description:
 * 将数据写入FIFO，不会唤醒正在等待读取FIFO的线程，此函数只能在中断使用
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
ssize_t fifo_in_no_schedule_isr(fifo_t *fifo, const void *data, size_t size)
{
    UBaseType_t interrupt_status;
    size_t empty_lenght;
    ssize_t result;

    if((!fifo) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 进入临界区 */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();
    interrupt_status = portSET_INTERRUPT_MASK_FROM_ISR();

    /* 获取FIFO空闲长度 */
    empty_lenght = fifo->size - __fifo_lenght(fifo);
    if(empty_lenght == 0)  {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);
        LOG_W("FIFO empty");
        return ERR_FULL;
    }

    if(size > empty_lenght) {
        size = empty_lenght;
    }
    /* 将数据写入FIFO */
    result = __fifo_in_force(fifo, data, size);

    portCLEAR_INTERRUPT_MASK_FROM_ISR(interrupt_status);

    return result;
}
