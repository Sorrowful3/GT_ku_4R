/*
 * axi_gpio.c
 *
 *  Created on: 2021年3月5日
 *      Author: Administrator
 */

#include "axi_gpio.h"

#define DEBUG_TAG "axi_gpio,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_GPIO

typedef struct{
    const uint32_t number;          //逻辑编号
    const uint32_t channel;         //物理通道
    const uint32_t physics;         //物理引脚编号

    axi_gpio_handler irq_handler;   //中断回调函数
    void *arg;                      //回调函数参数
}axi_gpio_t;

static axi_gpio_t axi_gpio[] = {
    //通道1
    { 0, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<0 ), NULL, NULL},
    { 1, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<1 ), NULL, NULL},
    { 2, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<2 ), NULL, NULL},
    { 3, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<3 ), NULL, NULL},
    { 4, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<4 ), NULL, NULL},
    { 5, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<5 ), NULL, NULL},
    { 6, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<6 ), NULL, NULL},
    { 7, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<7 ), NULL, NULL},
    { 8, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<8 ), NULL, NULL},
    { 9, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<9 ), NULL, NULL},
    {10, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<10), NULL, NULL},
    {11, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<11), NULL, NULL},
    {12, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<12), NULL, NULL},
    {13, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<13), NULL, NULL},
    {14, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<14), NULL, NULL},
    {15, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<15), NULL, NULL},
    {16, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<16), NULL, NULL},
    {17, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<17), NULL, NULL},
    {18, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<18), NULL, NULL},
    {19, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<19), NULL, NULL},
    {20, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<20), NULL, NULL},
    {21, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<21), NULL, NULL},
    {22, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<22), NULL, NULL},
    {23, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<23), NULL, NULL},
    {24, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<24), NULL, NULL},
    {25, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<25), NULL, NULL},
    {26, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<26), NULL, NULL},
    {27, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<27), NULL, NULL},
    {28, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<28), NULL, NULL},
    {29, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<29), NULL, NULL},
    {30, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<30), NULL, NULL},
    {31, AXI_GPIO_CHANNEL1, (uint32_t)(0x01<<31), NULL, NULL},
    //通道2
    {32, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<0 ), NULL, NULL},
    {33, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<1 ), NULL, NULL},
    {34, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<2 ), NULL, NULL},
    {35, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<3 ), NULL, NULL},
    {36, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<4 ), NULL, NULL},
    {37, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<5 ), NULL, NULL},
    {38, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<6 ), NULL, NULL},
    {39, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<7 ), NULL, NULL},
    {40, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<8 ), NULL, NULL},
    {41, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<9 ), NULL, NULL},
    {42, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<10), NULL, NULL},
    {43, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<11), NULL, NULL},
    {44, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<12), NULL, NULL},
    {45, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<13), NULL, NULL},
    {46, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<14), NULL, NULL},
    {47, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<15), NULL, NULL},
    {48, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<16), NULL, NULL},
    {49, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<17), NULL, NULL},
    {50, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<18), NULL, NULL},
    {51, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<19), NULL, NULL},
    {52, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<20), NULL, NULL},
    {53, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<21), NULL, NULL},
    {54, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<22), NULL, NULL},
    {55, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<23), NULL, NULL},
    {56, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<24), NULL, NULL},
    {57, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<25), NULL, NULL},
    {58, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<26), NULL, NULL},
    {59, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<27), NULL, NULL},
    {60, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<28), NULL, NULL},
    {61, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<29), NULL, NULL},
    {62, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<30), NULL, NULL},
    {63, AXI_GPIO_CHANNEL2, (uint32_t)(0x01<<31), NULL, NULL},
};

//输出缓存
static uint32_t channel1_output = CHANNEL1_DEFAULF_OUTPUT;
static uint32_t channel2_output = CHANNEL2_DEFAULF_OUTPUT;
//AXI GPIO句柄
static XGpio gpio_handle;

#ifdef USING_AXI_GPIO_INTERRUPT
//中断函数挂接标志
static uint32_t channel1_attach = 0;
static uint32_t channel2_attach = 0;
//中断触发电平
static uint32_t channel1_level = 0;
static uint32_t channel2_level = 0;

/*
 * AXI GPIO中断回调函数
 *
 * @param arg 回调函数参数
 *
 * @return 无
 */
static void __irq_handler(void *arg)
{
    uint32_t interrupt_status;
    uint32_t input_data;
    uint32_t tmp_attach;
    uint32_t tmp_mask;
    int32_t ffs;
    uint32_t i;

    //获取中断状态
    interrupt_status = XGpio_InterruptGetStatus(&gpio_handle);

    if(interrupt_status & AXI_GPIO_CHANNEL1) {
        //读取GPIO电平值
        input_data = XGpio_DiscreteRead(&gpio_handle, AXI_GPIO_CHANNEL1);
        tmp_attach = channel1_attach;

        //根据中断函数挂接情况和触发电平执行中断回调
        for(i=0; i<32; i++) {
            ffs = device_ffs(tmp_attach);
            if(ffs < 0) {
                break;
            }
            tmp_mask = 0x01 << ffs;
            tmp_attach &= ~tmp_mask;

            if((input_data & tmp_mask) == (channel1_level & tmp_mask)) {
                axi_gpio[ffs].irq_handler(axi_gpio[ffs].arg);
            }
        }

        //清除中断
        XGpio_InterruptClear(&gpio_handle, AXI_GPIO_CHANNEL1);
    }

    if(interrupt_status & AXI_GPIO_CHANNEL2) {
        //读取GPIO电平值
        input_data = XGpio_DiscreteRead(&gpio_handle, AXI_GPIO_CHANNEL2);
        tmp_attach = channel2_attach;

        //根据中断函数挂接情况和触发电平执行中断回调
        for(i=32; i<64; i++) {
            ffs = device_ffs(tmp_attach);
            if(ffs < 0) {
                break;
            }
            tmp_mask = 0x01 << ffs;
            tmp_attach &= ~tmp_mask;

            if((input_data & tmp_mask) == (channel2_level & tmp_mask)) {
                axi_gpio[ffs+32].irq_handler(axi_gpio[ffs+32].arg);
            }
        }

        //清除中断
        XGpio_InterruptClear(&gpio_handle, AXI_GPIO_CHANNEL2);
    }
}
#endif/* USING_AXI_GPIO_INTERRUPT */

/*
 * 配置AXI GPIO工作模式
 *
 * @param number AXI GPIO编号
 * @param mode AXI GPIO工作模式
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_config(uint32_t number, uint32_t mode)
{
    uint32_t direction_mask;

    if(number > 63) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    direction_mask = XGpio_GetDataDirection(&gpio_handle, axi_gpio[number].channel);
    if(mode == AXI_GPIO_MODE_OUTPUT) {
        direction_mask &= ~axi_gpio[number].physics;
    }
    else {
        direction_mask |= axi_gpio[number].physics;
    }
    XGpio_SetDataDirection(&gpio_handle, axi_gpio[number].channel, direction_mask);
    return ERR_OK;
}

/*
 * 设置AXI GPIO输出电平
 *
 * @param number AXI GPIO编号
 * @param value AXI GPIO电平状态
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_write(uint32_t number, uint32_t value)
{
    uint32_t *p_output;

    if(number > 63) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //获取上一次输出状态
    if(axi_gpio[number].channel == AXI_GPIO_CHANNEL1) {
        p_output = &channel1_output;
    }
    else {
        p_output = &channel2_output;
    }

    //改变输出状态
    if(value == AXI_GPIO_HIGH) {
        *p_output |= axi_gpio[number].physics;
    }
    else {
        *p_output &= ~axi_gpio[number].physics;
    }

    //设置IO输出状态
    XGpio_DiscreteWrite(&gpio_handle, axi_gpio[number].channel, *p_output);

    return ERR_OK;
}

/*
 * 读取AXI GPIO输入电平
 *
 * @param number AXI GPIO编号
 *
 * @return 成功返回电平状态
 */
int32_t axi_gpio_read(uint32_t number)
{
    uint32_t io_value;
    uint32_t direction_mask;

    if(number > 63) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //获取引脚输入输出配置
    direction_mask = XGpio_GetDataDirection(&gpio_handle, axi_gpio[number].channel);

    if(direction_mask & axi_gpio[number].physics) {
        //输入从寄存器读取IO状态
        io_value = XGpio_DiscreteRead(&gpio_handle, axi_gpio[number].channel);
    }
    else {
        //输出模式则从输出缓存读取IO状态
        if(axi_gpio[number].channel == AXI_GPIO_CHANNEL1) {
            io_value = channel1_output;
        }
        else {
            io_value = channel2_output;
        }
    }

    //返回引脚状态
    if(io_value & axi_gpio[number].physics) {
        return AXI_GPIO_HIGH;
    }
    return AXI_GPIO_LOW;
}

#ifdef USING_AXI_GPIO_INTERRUPT
/*
 * 挂接AXI GPIO中断函数
 *
 * @param number AXI GPIO编号
 * @param level AXI GPIO中断触发方式
 * @param irq_handler AXI GPIO中断回调函数
 * @param arg 中断回调函数参数
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_attach_irq(uint32_t number, uint32_t level, axi_gpio_handler irq_handler, void *arg)
{
    if(number > 63) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //挂接回调函数
    axi_gpio[number].arg = arg;
    axi_gpio[number].irq_handler = irq_handler;

    //设置中断触发类型和挂接标志
    if(axi_gpio[number].channel == AXI_GPIO_CHANNEL1) {
        if(level == AXI_GPIO_IRQ_HIGH) {
            channel1_level |= 0x01 << (number - 0);
        }
        else {
            channel1_level &= ~(0x01 << (number - 0));
        }
        channel1_attach |= 0x01 << (number - 0);
    }
    else {
        if(level == AXI_GPIO_IRQ_HIGH) {
            channel2_level |= 0x01 << (number - 32);
        }
        else {
            channel2_level &= ~(0x01 << (number - 32));
        }
        channel2_attach |= 0x01 << (number - 32);
    }

    return ERR_OK;
}

/*
 * 脱离AXI GPIO中断函数
 *
 * @param number AXI GPIO编号
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_detach_irq(uint32_t number)
{
    if(number > 63) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //注销回调函数
    axi_gpio[number].arg = NULL;
    axi_gpio[number].irq_handler = NULL;

    //清除挂接标志和中断触发类型
    if(axi_gpio[number].channel == AXI_GPIO_CHANNEL1) {
        channel1_attach &= ~(0x01 << (number - 0));
        channel1_level &= ~(0x01 << (number - 0));
    }
    else {
        channel2_attach &= ~(0x01 << (number - 32));
        channel2_level &= ~(0x01 << (number - 32));
    }

    return ERR_OK;
}

/*
 * 使能AXI GPIO中断
 *
 * @param channel AXI GPIO通道号
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_irq_enable(uint32_t channel)
{
    if(!(channel & AXI_GPIO_CHANNEL1) && !(channel & AXI_GPIO_CHANNEL2)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //使能通道的中断
    XGpio_InterruptEnable(&gpio_handle, channel);

    return ERR_OK;
}

/*
 * 禁止AXI GPIO中断
 *
 * @param channel AXI GPIO通道号
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_irq_disable(uint32_t channel)
{
    if(!(channel & AXI_GPIO_CHANNEL1) && !(channel & AXI_GPIO_CHANNEL2)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //禁止通道的中断
    XGpio_InterruptDisable(&gpio_handle, channel);

    return ERR_OK;
}
#endif /* USING_AXI_GPIO_INTERRUPT */

/*
 * 初始化AXI GPIO
 *
 * @return 成功返回EOK
 */
int32_t axi_gpio_init(void)
{
    int status;

    //初始化AXI GPIO
    status = XGpio_Initialize(&gpio_handle, AXI_GPIO_DEVICE_ID);
    if (status != XST_SUCCESS) {
        return ERR_ROUTINE;
    }

#ifdef USING_AXI_GPIO_INTERRUPT
    //设置优先级和触发方式
    set_intr_priority_trigger(AXI_GPIO_INTERRUPT_ID, 0xA0, 0x3);
    //注册AXI GPIO中断处理函数
    xPortInstallInterruptHandler(AXI_GPIO_INTERRUPT_ID, __irq_handler, (void *)&gpio_handle);

    //将中断映射到CPU1
    intr_map_to_cpu1(AXI_GPIO_INTERRUPT_ID);

    //使能AXI GPIO中断
    vPortEnableInterrupt(AXI_GPIO_INTERRUPT_ID);
    //使能AXI GPIO的全局中断
    XGpio_InterruptGlobalEnable(&gpio_handle);
#endif /* USING_AXI_GPIO_INTERRUPT */

    return ERR_OK;
}

#endif /* USING_AXI_GPIO */
