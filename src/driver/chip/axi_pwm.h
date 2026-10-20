/**
 * @file    axi_pwm.c
 * @brief   实现PL端的PWM IP核的驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年5月6日
 */

#ifndef __AXI_PWM_H_
#define __AXI_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_PWM

#include "xparameters.h"
#include "xil_io.h"
#include "pwm_output.h"

/* PWM寄存器基地址 */
#define PWM_OUTPUT_BASEADDR XPAR_PWM_OUTPUT_0_S00_AXI_BASEADDR

/* PWM寄存器列表 */
#define PWM_ENABLE_REG      PWM_OUTPUT_S00_AXI_SLV_REG0_OFFSET
#define PWM_PERIOD_REG      PWM_OUTPUT_S00_AXI_SLV_REG1_OFFSET
#define PWM_COMPARE0_REG    PWM_OUTPUT_S00_AXI_SLV_REG2_OFFSET
#define PWM_COMPARE1_REG    PWM_OUTPUT_S00_AXI_SLV_REG3_OFFSET
#define PWM_COMPARE2_REG    PWM_OUTPUT_S00_AXI_SLV_REG4_OFFSET
#define PWM_COMPARE3_REG    PWM_OUTPUT_S00_AXI_SLV_REG5_OFFSET
#define PWM_COMPARE4_REG    PWM_OUTPUT_S00_AXI_SLV_REG6_OFFSET
#define PWM_COMPARE5_REG    PWM_OUTPUT_S00_AXI_SLV_REG7_OFFSET
#define PWM_COMPARE6_REG    PWM_OUTPUT_S00_AXI_SLV_REG8_OFFSET
#define PWM_COMPARE7_REG    PWM_OUTPUT_S00_AXI_SLV_REG9_OFFSET
#define PWM_COMPARE8_REG    PWM_OUTPUT_S00_AXI_SLV_REG10_OFFSET
#define PWM_COMPARE9_REG    PWM_OUTPUT_S00_AXI_SLV_REG11_OFFSET
#define PWM_COMPARE10_REG   PWM_OUTPUT_S00_AXI_SLV_REG12_OFFSET
#define PWM_COMPARE11_REG   PWM_OUTPUT_S00_AXI_SLV_REG13_OFFSET
#define PWM_COMPARE12_REG   PWM_OUTPUT_S00_AXI_SLV_REG14_OFFSET
#define PWM_COMPARE13_REG   PWM_OUTPUT_S00_AXI_SLV_REG15_OFFSET
#define PWM_COMPARE14_REG   PWM_OUTPUT_S00_AXI_SLV_REG16_OFFSET
#define PWM_COMPARE15_REG   PWM_OUTPUT_S00_AXI_SLV_REG17_OFFSET

/* PWM 通道掩码 */
#define PWM_CHANNEL0        0x0001
#define PWM_CHANNEL1        0x0002
#define PWM_CHANNEL2        0x0004
#define PWM_CHANNEL3        0x0008
#define PWM_CHANNEL4        0x0010
#define PWM_CHANNEL5        0x0020
#define PWM_CHANNEL6        0x0040
#define PWM_CHANNEL7        0x0080
#define PWM_CHANNEL8        0x0100
#define PWM_CHANNEL9        0x0200
#define PWM_CHANNEL10       0x0400
#define PWM_CHANNEL11       0x0800
#define PWM_CHANNEL12       0x1000
#define PWM_CHANNEL13       0x2000
#define PWM_CHANNEL14       0x4000
#define PWM_CHANNEL15       0x8000
#define PWM_CHANNEL_MASK    0xffff

int32_t axi_pwm_init(void);
int32_t axi_pwm_set_period(uint32_t period);
int32_t axi_pwm_enable(uint32_t channel);
int32_t axi_pwm_disable(uint32_t channel);
int32_t axi_pwm_set_compare(uint32_t channel, uint32_t compare);

#endif /* USING_AXI_PWM */

#ifdef __cplusplus
}
#endif

#endif /* __AXI_PWM_H_ */
