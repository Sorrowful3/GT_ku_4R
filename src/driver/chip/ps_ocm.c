/*
 * ps_ocm.c
 *
 *  Created on: 2021年3月8日
 *      Author: Administrator
 */

#include "ps_ocm.h"

#define DEBUG_TAG "ps_ocm,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_PS_OCM

static ps_ocm_t ps_ocm;

/*
 * OCM软中断中断回调函数
 *
 * @param arg 回调函数参数，这里传递的是MSG地址
 *
 * @return 无
 */
static void __irq_handler(void *arg)
{
    int32_t lenght;
    BaseType_t task_woken;
    ocm_msg_t *volatile ocm_msg = (ocm_msg_t *volatile)arg;

#ifdef USING_CORE0
    if(ocm_msg == CORE0_TO_CORE1_MSG) {
        //从发送FIFO中读数据
        lenght = fifo_out_isr(ps_ocm.tx_fifo, ocm_msg->msg, PS_OCM_MSG_SIZE);

        if(lenght > 0) {
            ocm_msg->lenght = lenght;

            //向CORE1发送中断
            generate_software_intr(SOFT_IRQ_CORE0_TO_CORE1, CORE1_ID);
            //设置发送同步信号量，表示FIFO可以写入
            task_woken = pdFALSE;
            xSemaphoreGiveFromISR(ps_ocm.tx_sync, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }
        else {
            //设置为发送空闲
            ps_ocm.tx_idle = 1;
        }
    }

    if(ocm_msg == CORE1_TO_CORE0_MSG) {
        //判断接收数据是否有效，有效则将数据从msg拷贝到接收fifo
        if(ocm_msg->lenght > 0) {
            fifo_in_force_isr(ps_ocm.rx_fifo, ocm_msg->msg, ocm_msg->lenght, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }

        //将数据设置为无效
        ocm_msg->lenght = 0;
        //向CORE1回复中断
        generate_software_intr(SOFT_IRQ_CORE1_TO_CORE0, CORE1_ID);
    }
#else
    if(ocm_msg == CORE1_TO_CORE0_MSG) {
        //从发送FIFO中读数据
        lenght = fifo_out_isr(ps_ocm.tx_fifo, ocm_msg->msg, PS_OCM_MSG_SIZE);

        if(lenght > 0) {
            ocm_msg->lenght = lenght;

            //向CORE0发送中断
            generate_software_intr(SOFT_IRQ_CORE1_TO_CORE0, CORE0_ID);
            //设置发送同步信号量，表示FIFO可以写入
            task_woken = pdFALSE;
            xSemaphoreGiveFromISR(ps_ocm.tx_sync, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }
        else {
            //设置为发送空闲
            ps_ocm.tx_idle = 1;
        }
    }

    if(ocm_msg == CORE0_TO_CORE1_MSG) {
        //判断接收数据是否有效则将数据从msg拷贝到fifo
        if(ocm_msg->lenght > 0) {
            fifo_in_force_isr(ps_ocm.rx_fifo, ocm_msg->msg, ocm_msg->lenght, &task_woken);
            portYIELD_FROM_ISR(task_woken);
        }

        //将数据设置为无效
        ocm_msg->lenght = 0;
        //向CORE0回复中断
        generate_software_intr(SOFT_IRQ_CORE0_TO_CORE1, CORE0_ID);
    }
#endif
}

/*
 * 读OCM接收到的数据
 *
 * @param device 设备句柄
 * @param buffer 用户缓存
 * @param size 要读取的字节数
 * @param pos 偏移，此处未实现
 *
 * @return 成功返回读取的字节数
 */
static ssize_t __ocm_read(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    //从接收FIFO中读数据
    return fifo_out(ps_ocm.rx_fifo, buffer, size, ps_ocm.device.receive_timeout);
}

/*
 * 通过OCM发送数据
 *
 * @param device 设备句柄
 * @param buffer 用户缓存
 * @param size 要发送的字节数
 * @param pos 偏移，此处未实现
 *
 * @return 成功返回发送的字节数
 */
static ssize_t __ocm_write(device_t *device, const void *buffer, size_t size, uint32_t pos)
{
    ssize_t result;
    ssize_t tx_size;
    ssize_t temp;
    const uint8_t *p_data = (uint8_t*)buffer;

    //加锁
    if(xSemaphoreTakeRecursive(ps_ocm.tx_lock, portMAX_DELAY) != pdPASS) {
        LOG_E(PS_OCM_DEVICE_NAME" lock failed");
        return ERR_ROUTINE;
    }

    for(tx_size=0; ; ) {
        //进入临界区
        taskENTER_CRITICAL();

        //如果发送空闲则启动发送
        if(ps_ocm.tx_idle) {
            //将数据拷贝到OCM
#ifdef USING_CORE0
            temp = ((size-tx_size) < PS_OCM_MSG_SIZE) ? (size-tx_size) : PS_OCM_MSG_SIZE;
            memcpy(CORE0_TO_CORE1_MSG->msg, &p_data[tx_size], temp);
            CORE0_TO_CORE1_MSG->lenght = temp;

            //向CORE0发送中断
            generate_software_intr(SOFT_IRQ_CORE0_TO_CORE1, CORE1_ID);
#else
            temp = ((size-tx_size) < PS_OCM_MSG_SIZE) ? (size-tx_size) : PS_OCM_MSG_SIZE;
            memcpy(CORE1_TO_CORE0_MSG->msg, &p_data[tx_size], temp);
            CORE1_TO_CORE0_MSG->lenght = temp;

            //向CORE0发送中断
            generate_software_intr(SOFT_IRQ_CORE1_TO_CORE0, CORE0_ID);
#endif
            //设置发送为忙状态
            ps_ocm.tx_idle = 0;

            //累计已经发送的字节数
            tx_size += temp;
        }

        //将剩余数据继续写入发送FIFO
        if(tx_size < (ssize_t)size) {
            result = fifo_in_no_schedule(ps_ocm.tx_fifo, &p_data[tx_size], size-tx_size);
            if(result > 0) {
                //累计已经发送的字节数
                tx_size += result;
            }
        }

        //退出临界区
        taskEXIT_CRITICAL();

        //发送完成则退出循环
        if(tx_size >= (ssize_t)size) {
            break;
        }

        //等待发送FIFO可以写入
        if(xSemaphoreTake(ps_ocm.tx_sync, portMAX_DELAY) != pdPASS) {
            LOG_E("take tx_sync failed");
            break;
        }
    }

    //解锁
    if(xSemaphoreGiveRecursive(ps_ocm.tx_lock) != pdPASS) {
        LOG_E(PS_OCM_DEVICE_NAME" unlock failed");
        return ERR_ROUTINE;
    }

    return tx_size;
}

/*
 * 控制OCM设备
 *
 * @param device 设备句柄
 * @param command OCM控制命令
 * @param arg 命令参数
 *
 * @return 成功返回EOK
 */
static int32_t __ocm_control(device_t *device, uint32_t cmd, void *arg)
{
    int32_t *p_msg_size = (int32_t*)arg;

    switch(cmd) {
    case PS_OCM_GET_MSG_SIZE:
        if(!p_msg_size) {
            LOG_E("parameter error");
            return ERR_INVAL;
        }
        *p_msg_size = PS_OCM_MSG_SIZE;
        break;
    default:
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    return ERR_OK;
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = __ocm_read,
    .pwrite = __ocm_write,
    .control = __ocm_control,
};

/*
 * 初始化OCM设备
 *
 * @return 成功返回EOK
 */
int32_t ps_ocm_init(void)
{
    int32_t result;

#ifdef USING_CORE0
    ps_ocm.tx_idle = 0;
#else
    ps_ocm.tx_idle = 1;
#endif
    //创建发送互斥锁
    ps_ocm.tx_lock = xSemaphoreCreateRecursiveMutex();
    if(!ps_ocm.tx_lock) {
        LOG_E("%s create mutex lock failed", PS_OCM_DEVICE_NAME);
        return ERR_NOMEM;
    }
    //创建发送FIFO可写标志信号量
    ps_ocm.tx_sync = xSemaphoreCreateBinary();
    if(!ps_ocm.tx_sync) {
        LOG_E("%s create tx_sync failed", PS_OCM_DEVICE_NAME);
        return ERR_NOMEM;
    }
    //创建发送FIFO
    ps_ocm.tx_fifo = fifo_create(PS_OCM_TX_FIFO_SIZE);
    if(!ps_ocm.tx_fifo) {
        LOG_E(PS_OCM_DEVICE_NAME" create rx_fifo failed");
        return ERR_NOMEM;
    }

    //创建接收FIFO
    ps_ocm.rx_fifo = fifo_create(PS_OCM_RX_FIFO_SIZE);
    if(!ps_ocm.rx_fifo) {
        LOG_E(PS_OCM_DEVICE_NAME" create rx_fifo failed");
        return ERR_NOMEM;
    }

    //禁用Cache属性
    Xil_SetTlbAttributes(PS_OCM_BASE_ADDRESS, 0x14de2);

    //配置中断
    set_intr_priority_trigger(SOFT_IRQ_CORE0_TO_CORE1, 0xA0, 0x3);
    xPortInstallInterruptHandler(SOFT_IRQ_CORE0_TO_CORE1, __irq_handler, CORE0_TO_CORE1_MSG);
    set_intr_priority_trigger(SOFT_IRQ_CORE1_TO_CORE0, 0xA0, 0x3);
    xPortInstallInterruptHandler(SOFT_IRQ_CORE1_TO_CORE0, __irq_handler, CORE1_TO_CORE0_MSG);
    //使能中断
    vPortEnableInterrupt(SOFT_IRQ_CORE0_TO_CORE1);
    vPortEnableInterrupt(SOFT_IRQ_CORE1_TO_CORE0);

    //注册OCM设备
    ps_ocm.device.magic = PS_OCM_MAGIC;
    ps_ocm.device.ops = &ops;
    result = device_register(&ps_ocm.device, PS_OCM_DEVICE_NAME, DEVICE_RDWR);
    if(result != ERR_OK) {
        LOG_E(PS_OCM_DEVICE_NAME" register failed");
        return result;
    }

#ifdef USING_CORE1
    //回复中断到CORE0，表示CORE1已经就绪
    generate_software_intr(SOFT_IRQ_CORE0_TO_CORE1, CORE0_ID);
#endif

    return ERR_OK;
}

#endif /* USING_PS_OCM */


