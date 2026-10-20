/**
 * @file    ps_gpio.c
 * @brief   用于驱动PS端的GPIO外设
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月5日
 */

#include "ps_gpio.h"

#define DEBUG_TAG "ps_gpio, %d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_PS_GPIO

/* PS GPIO共有117个IO（包括EMIO），分为4个bank，这里定义的是每个bank的起始引脚 */
#define BANK0_BASE  0
#define BANK1_BASE  32
#define BANK2_BASE  54
#define BANK3_BASE  86

/* PS GPIO外设中断号和设备号 */
#define PS_GPIO_DEVICE_ID       XPAR_XGPIOPS_0_DEVICE_ID
#define PS_GPIO_INTERRUPT_ID    XPAR_XGPIOPS_0_INTR

/* GPIO数据结构 */
typedef struct{
    const uint32_t gpio_number;     /* 引脚编号，程序中无实际意义，仅用于方便阅读 */
    ps_gpio_handler irq_handler;    /* 引脚中断回调函数 */
    void *arg;                      /* 回调函数参数 */
}ps_gpio_t;

/* PS GPIO信息列表 */
static ps_gpio_t ps_gpio[] = {
    /* BANK0 */
    {0,   NULL, NULL},
    {1,   NULL, NULL},
    {2,   NULL, NULL},
    {3,   NULL, NULL},
    {4,   NULL, NULL},
    {5,   NULL, NULL},
    {6,   NULL, NULL},
    {7,   NULL, NULL},
    {8,   NULL, NULL},
    {9,   NULL, NULL},
    {10,  NULL, NULL},
    {11,  NULL, NULL},
    {12,  NULL, NULL},
    {13,  NULL, NULL},
    {14,  NULL, NULL},
    {15,  NULL, NULL},
    {16,  NULL, NULL},
    {17,  NULL, NULL},
    {18,  NULL, NULL},
    {19,  NULL, NULL},
    {20,  NULL, NULL},
    {21,  NULL, NULL},
    {22,  NULL, NULL},
    {23,  NULL, NULL},
    {24,  NULL, NULL},
    {25,  NULL, NULL},
    {26,  NULL, NULL},
    {27,  NULL, NULL},
    {28,  NULL, NULL},
    {29,  NULL, NULL},
    {30,  NULL, NULL},
    {31,  NULL, NULL},
    /* BANK1 */
    {32,  NULL, NULL},
    {33,  NULL, NULL},
    {34,  NULL, NULL},
    {35,  NULL, NULL},
    {36,  NULL, NULL},
    {37,  NULL, NULL},
    {38,  NULL, NULL},
    {39,  NULL, NULL},
    {40,  NULL, NULL},
    {41,  NULL, NULL},
    {42,  NULL, NULL},
    {43,  NULL, NULL},
    {44,  NULL, NULL},
    {45,  NULL, NULL},
    {46,  NULL, NULL},
    {47,  NULL, NULL},
    {48,  NULL, NULL},
    {49,  NULL, NULL},
    {50,  NULL, NULL},
    {51,  NULL, NULL},
    {52,  NULL, NULL},
    {53,  NULL, NULL},
    /* BANK2 */
    {54,  NULL, NULL},
    {55,  NULL, NULL},
    {56,  NULL, NULL},
    {57,  NULL, NULL},
    {58,  NULL, NULL},
    {59,  NULL, NULL},
    {60,  NULL, NULL},
    {61,  NULL, NULL},
    {62,  NULL, NULL},
    {63,  NULL, NULL},
    {64,  NULL, NULL},
    {65,  NULL, NULL},
    {66,  NULL, NULL},
    {67,  NULL, NULL},
    {68,  NULL, NULL},
    {69,  NULL, NULL},
    {70,  NULL, NULL},
    {71,  NULL, NULL},
    {72,  NULL, NULL},
    {73,  NULL, NULL},
    {74,  NULL, NULL},
    {75,  NULL, NULL},
    {76,  NULL, NULL},
    {77,  NULL, NULL},
    {78,  NULL, NULL},
    {79,  NULL, NULL},
    {80,  NULL, NULL},
    {81,  NULL, NULL},
    {82,  NULL, NULL},
    {83,  NULL, NULL},
    {84,  NULL, NULL},
    {85,  NULL, NULL},
    /* BANK3 */
    {86,  NULL, NULL},
    {87,  NULL, NULL},
    {88,  NULL, NULL},
    {89,  NULL, NULL},
    {90,  NULL, NULL},
    {91,  NULL, NULL},
    {92,  NULL, NULL},
    {93,  NULL, NULL},
    {94,  NULL, NULL},
    {95,  NULL, NULL},
    {96,  NULL, NULL},
    {97,  NULL, NULL},
    {98,  NULL, NULL},
    {99,  NULL, NULL},
    {100, NULL, NULL},
    {101, NULL, NULL},
    {102, NULL, NULL},
    {103, NULL, NULL},
    {104, NULL, NULL},
    {105, NULL, NULL},
    {106, NULL, NULL},
    {107, NULL, NULL},
    {108, NULL, NULL},
    {109, NULL, NULL},
    {110, NULL, NULL},
    {111, NULL, NULL},
    {112, NULL, NULL},
    {113, NULL, NULL},
    {114, NULL, NULL},
    {115, NULL, NULL},
    {116, NULL, NULL},
    {117, NULL, NULL},
};

/* PS GPIO句柄 */
static XGpioPs gpio_handle;

/**
 * @function nane: __irq_handler
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              arg 回调函数参数，这里未使用
 *              bank 发生中断的IO所在bank
 *              state 引脚掩码，置位表示对应IO有效
 * @output param:
 *              无
 * @description:
 * PS GPIO中断回调函数
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
static void __irq_handler(void *arg, uint32_t bank, uint32_t state)
{
    int32_t i;
    int32_t ffs;

    /* 判断是否有引脚产生了中断 */
    if(state == 0) {
        return;
    }

    for(i=0; i<32; i++) {
        /* 计算引脚偏移 */
        ffs = device_ffs(state);
        if(ffs < 0) {
            break;
        }
        /* 忽略已经处理的引脚 */
        state &= ~(0x01 << ffs);

        /* 查找用户回调函数 */
        if(bank == XGPIOPS_BANK0) {
            ffs += BANK0_BASE;
        }
        if(bank == XGPIOPS_BANK1) {
            ffs += BANK1_BASE;
        }
        if(bank == XGPIOPS_BANK2) {
            ffs += BANK2_BASE;
        }
        if(bank == XGPIOPS_BANK3) {
            ffs += BANK3_BASE;
        }

        /* 执行用户回调 */
        if(ps_gpio[ffs].irq_handler) {
            ps_gpio[ffs].irq_handler(ps_gpio[ffs].arg);
        }
    }
}

/**
 * @function nane: ps_gpio_config
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              gpio_number PS GPIO编号
 *              mode IO工作模式，PS_GPIO_MODE_OUTPUT 输出模式，PS_GPIO_MODE_INPUT 输入模式
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 配置PS GPIO工作模式
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_config(uint32_t gpio_number, uint32_t mode)
{

    if(gpio_number > 117) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //配置IO工作模式
    XGpioPs_SetDirectionPin(&gpio_handle, gpio_number, mode);
    if(mode != PS_GPIO_MODE_INPUT) {
        /* 使能IO输出 */
        XGpioPs_SetOutputEnablePin(&gpio_handle, gpio_number, 1);
    }
    else {
        /* 禁止IO输出 */
        XGpioPs_SetOutputEnablePin(&gpio_handle, gpio_number, 0);
    }

    return ERR_OK;
}

/**
 * @function nane: ps_gpio_write
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              gpio_number PS GPIO编号
 *              value 输出电平，PS_GPIO_LOW 表示输出低电平，PS_GPIO_HIGH 表示输出高电平
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 设置PS GPIO输出电平
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_write(uint32_t gpio_number, uint32_t value)
{
    if(gpio_number > 117) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 设置IO输出电平 */
    XGpioPs_WritePin(&gpio_handle, gpio_number, value);

    return ERR_OK;
}

/**
 * @function nane: ps_gpio_read
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              gpio_number PS GPIO编号
 * @output param:
 *              成功返回IO输入电平，PS_GPIO_LOW 表示低电平，PS_GPIO_HIGH 表示高电平，失败返回错误码
 * @description:
 * 读取PS GPIO输入电平
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_read(uint32_t gpio_number)
{
    if(gpio_number > 117) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 读取IO输入电平 */
    return XGpioPs_ReadPin(&gpio_handle, gpio_number);
}

/**
 * @function nane: ps_gpio_attach_irq
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              gpio_number PS GPIO编号
 *              type PS GPIO中断触发方式，PS_GPIO_IRQ_RISING 上升沿触发
 *                                       PS_GPIO_IRQ_FALLING 下降沿触发
 *                                       PS_GPIO_IRQ_RISING_FALLING 上升下降沿均触发
 *                                       PS_GPIO_IRQ_HIGH 高电平触发
 *                                       PS_GPIO_IRQ_LOW 低电平触发
 *              irq_handler PS GPIO中断回调函数
 *              arg 中断回调函数参数
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 挂接PS GPIO中断函数，并使能PS GPIO中断
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_attach_irq(uint32_t gpio_number, uint8_t type, ps_gpio_handler irq_handler, void *arg)
{
    if(gpio_number > 117) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 绑定回调函数 */
    ps_gpio[gpio_number].irq_handler = irq_handler;
    ps_gpio[gpio_number].arg = arg;

    /* 配置为输入模式 */
    XGpioPs_SetDirectionPin(&gpio_handle, gpio_number, 0);
    XGpioPs_SetOutputEnablePin(&gpio_handle, gpio_number, 0);

    /* 配置触发方式 */
    XGpioPs_SetIntrTypePin(&gpio_handle, gpio_number, type);
    /* 使能中断 */
    XGpioPs_IntrEnablePin(&gpio_handle, gpio_number);

    return ERR_OK;
}

/**
 * @function nane: ps_gpio_detach_irq
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              gpio_number PS GPIO编号
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 脱离PS GPIO中断函数，并关闭PS GPIO中断
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_detach_irq(uint32_t gpio_number)
{
    if(gpio_number > 117) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 注销中断回调函数 */
    ps_gpio[gpio_number].irq_handler = NULL;

    /* 禁止中断 */
    XGpioPs_IntrDisablePin(&gpio_handle, gpio_number);

    return ERR_OK;
}

/**
 * @function nane: ps_gpio_init
 * @author: liaofei
 * @data: 2021年3月5日
 * @input param:
 *              无
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 初始化PS GPIO模块
 * @modeify log:
 * 2021年3月5日 liaofei 初次创建
 */
int32_t ps_gpio_init(void)
{
    int Status;
    XGpioPs_Config *ConfigPtr;

    /* 使能GPIO控制器的总线时钟 */
    *((uint32_t*)0xF800012C) |= 0x01<<22;

    /* GPIO初始化 */
    ConfigPtr = XGpioPs_LookupConfig(PS_GPIO_DEVICE_ID);
    if (!ConfigPtr) {
        LOG_E("initialize failed");
        return ERR_IO;
    }
    Status = XGpioPs_CfgInitialize(&gpio_handle, ConfigPtr, ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
        LOG_E("initialize failed");
        return ERR_IO;
    }
    Status = XGpioPs_SelfTest(&gpio_handle);
    if (Status != XST_SUCCESS) {
        LOG_E("initialize failed");
        return ERR_IO;
    }

    /* 注册PS GPIO通用中断处理函数 */
    xPortInstallInterruptHandler(PS_GPIO_INTERRUPT_ID, (Xil_ExceptionHandler)XGpioPs_IntrHandler, (void *)&gpio_handle);
    /* 注册PS GPIO自定义中断回调函数 */
    XGpioPs_SetCallbackHandler(&gpio_handle, NULL, __irq_handler);
    /* 将中断映射到CPU1 */
    intr_map_to_cpu1(PS_GPIO_INTERRUPT_ID);
    /* 使能PS GPIO中断 */
    vPortEnableInterrupt(PS_GPIO_INTERRUPT_ID);

    return ERR_OK;
}

#endif /* USING_PS_GPIO */
