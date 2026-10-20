/**
 * @file    axi_capture.c
 * @brief   实现输入捕获IP核驱动
 * @author  liaofei
 * @version v2.1.0
 * @data
 */

#include "axi_capture.h"

#define DEBUG_TAG "axi_capture,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_CAPTURE

/**
 * @function nane: axi_capture_init
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              无
 * @output param:
 *              return 返回ERR_OK，表示初始化成功。
 * @description:
 * 初始化输入捕获IP核，禁止所有输入捕获特点，根据宏定义配置最大捕获周期
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_capture_init(void)
{
    /* 禁止所有通道 */
    INPUT_CAPTURE_mWriteReg(CAPTURE_BASEADDR, CAPTURE_ENABLE_REG, 0x00);
    /* 设置最大捕获周期 */
    INPUT_CAPTURE_mWriteReg(CAPTURE_BASEADDR, CAPTURE_MAX_PERIOD_REG, DEFAULT_MAX_CAPTURE);
    return ERR_OK;
}
/**
 * @function nane: axi_capture_set_max_period
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              period 最大捕获周期，以IP核时钟频率为单位
 * @output param:
 *              return 返回ERR_OK，表示配置成功。
 * @description:
 * 配置输入捕获IP核的最大捕获周期，因为所有捕获通道共用一个计数器，所以此配置针对于所以通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_capture_set_max_period(uint32_t period)
{
    /* 设置输入捕获周期寄存器 */
    INPUT_CAPTURE_mWriteReg(CAPTURE_BASEADDR, CAPTURE_MAX_PERIOD_REG, period);

    return ERR_OK;
}

/**
 * @function nane: axi_capture_enable
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 指定要配置的通道号，0~15分别对应通道0~15，置位表示选中相应通道
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 使能相应的输入捕获通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_capture_enable(uint32_t channel)
{
    uint32_t data;

    /* 检查通道掩码是否合法 */
    if((channel > INPUT_CHANNEL_MASK) || (channel == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 读取使能寄存器的值 */
    data = INPUT_CAPTURE_mReadReg(CAPTURE_BASEADDR, CAPTURE_ENABLE_REG);
    /* 设置对应的使能控制位 */
    data |= channel;
    /* 将更改后的值写入寄存器 */
    INPUT_CAPTURE_mWriteReg(CAPTURE_BASEADDR, CAPTURE_ENABLE_REG, data);

    return ERR_OK;
}

/**
 * @function nane: axi_capture_disable
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 指定要配置的通道号，0~15分别对应通道0~15，置位表示选中相应通道
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 禁止相应的输入捕获通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_capture_disable(uint32_t channel)
{
    uint32_t data;

    /* 检查通道掩码是否合法 */
    if((channel > INPUT_CHANNEL_MASK) || (channel == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 读取使能寄存器的值 */
    data = INPUT_CAPTURE_mReadReg(CAPTURE_BASEADDR, CAPTURE_ENABLE_REG);
    /* 清除对应的使能控制位 */
    data &= ~channel;
    /* 将更改后的值写入寄存器 */
    INPUT_CAPTURE_mWriteReg(CAPTURE_BASEADDR, CAPTURE_ENABLE_REG, data);

    return ERR_OK;
}

/**
 * @function nane: axi_capture_get_period
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 指定要配置的通道号，0~15分别对应通道0~15，置位表示选中相应通道
 *              period[] 一个数组，用于存储对应通道捕获到的脉冲宽度，根据通道号从小到大存储
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 获取指定通道捕获到的的脉冲宽度
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_capture_get_period(uint32_t channel, uint32_t period[])
{
    int32_t i;
    int32_t temp_channel;
    uint32_t reg_addr;

    /* 检查通道掩码、数组地址是否合法 */
    if((channel > INPUT_CHANNEL_MASK) || (channel == 0) || (!period)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    for(i=0; i<16; i++)
    {
        /* 从通道掩码中获取一个序号最低的通道 */
        temp_channel = device_ffs(channel);
        if(temp_channel < 0) {
            break;
        }
        /* 计数对应通道的捕获脉冲宽度寄存器地址 */
        reg_addr = CAPTURE_PERIOD0_REG + (temp_channel * 4);

        /* 从捕获脉冲宽度寄存器读取脉冲宽度 */
        period[i] = INPUT_CAPTURE_mReadReg(CAPTURE_BASEADDR, reg_addr);

        /* 忽略已经读取的通道 */
        channel &= ~(0x01 << temp_channel);
    }
    return ERR_OK;
}

#endif /* USING_AXI_CAPTURE */
