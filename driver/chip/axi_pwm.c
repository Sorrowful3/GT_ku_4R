/**
 * @file    axi_pwm.c
 * @brief   实现PL端的PWM IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年5月6日
 */


#include "axi_pwm.h"

#define DEBUG_TAG "axi_pwm,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_AXI_PWM

/**
 * @function nane: axi_pwm_init
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              无
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 初始化PWM IP核
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_pwm_init(void)
{
    uint32_t reg_addr;

    /* 禁止所有PWM通道 */
    PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, PWM_ENABLE_REG, 0x00);
    /* 设置PWM周期为默认值 */
    PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, PWM_PERIOD_REG, DEFAULT_PWM_PERIOD);
    /* 设置比较寄存器的值为0 */
    for(reg_addr=PWM_COMPARE0_REG; reg_addr<=PWM_COMPARE15_REG; reg_addr+=4) {
        PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, reg_addr, 0x00);
    }

    return ERR_OK;
}

/**
 * @function nane:axi_pwm_set_period
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              period PWM周期
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 配置PWM输出周期，此配置作用于所有通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_pwm_set_period(uint32_t period)
{
    /* 配置PWM周期寄存器 */
    PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, PWM_PERIOD_REG, period);

    return ERR_OK;
}

/**
 * @function nane: axi_pwm_enable
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 通道掩码，对应位为1表示选中相应通道
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 使能PWM通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_pwm_enable(uint32_t channel)
{
    uint32_t data;

    if((channel > PWM_CHANNEL_MASK) || (channel == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 读取通道使能寄存器 */
    data = PWM_OUTPUT_mReadReg(PWM_OUTPUT_BASEADDR, PWM_ENABLE_REG);
    /* 修改寄存器的值 */
    data |= channel;
    /* 将修改后的值写回寄存器 */
    PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, PWM_ENABLE_REG, data);

    return ERR_OK;
}

/**
 * @function nane: axi_pwm_disable
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 通道掩码，对应位为1表示选中相应通道
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 禁止PWM通道
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_pwm_disable(uint32_t channel)
{
    uint32_t data;

    if((channel > PWM_CHANNEL_MASK) || (channel == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 读取通道使能寄存器 */
    data = PWM_OUTPUT_mReadReg(PWM_OUTPUT_BASEADDR, PWM_ENABLE_REG);
    /* 修改寄存器的值 */
    data &= ~channel;
    /* 将修改后的值写回寄存器 */
    PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, PWM_ENABLE_REG, data);

    return ERR_OK;
}

/**
 * @function nane:
 * @author: liaofei
 * @data: 2021年5月6日
 * @input param:
 *              channel 通道掩码，对应位为1表示选中相应通道
 *              compare 比较寄存器的值，决定PWM的占空比
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description:
 * 设置PWM的占空比
 * @modeify log:
 * 2021年5月6日 liaofei 初次创建
 */
int32_t axi_pwm_set_compare(uint32_t channel, uint32_t compare)
{
    int32_t i;
    int32_t temp_channel;
    uint32_t reg_addr;

    if((channel > PWM_CHANNEL_MASK) || (channel == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    for(i=0; i<16; i++)
    {
        /* 获取一个有效的通道 */
        temp_channel = device_ffs(channel);
        if(temp_channel < 0) {
            break;
        }
        /* 计算通道比较寄存器偏移 */
        reg_addr = PWM_COMPARE0_REG + (temp_channel * 4);

        /* 设置比较寄存器的值 */
        PWM_OUTPUT_mWriteReg(PWM_OUTPUT_BASEADDR, reg_addr, compare);

        /* 忽略已经配置过的通道 */
        channel &= ~(0x01 << temp_channel);
    }

    return ERR_OK;
}

#endif /* USING_AXI_PWM */
