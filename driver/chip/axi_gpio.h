/*
 * axi_gpio.h
 *
 *  Created on: 2021年3月5日
 *      Author: Administrator
 */

#ifndef __AXI_GPIO_H_
#define __AXI_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_GPIO

#include "xgpio.h"

//AXI GPIO电平状态
#define AXI_GPIO_LOW            0x00
#define AXI_GPIO_HIGH           0x01

//AXI GPIO工作模式
#define AXI_GPIO_MODE_OUTPUT    0x00
#define AXI_GPIO_MODE_INPUT     0x01

//AXI GPIO通道号
#define AXI_GPIO_CHANNEL1       XGPIO_IR_CH1_MASK
#define AXI_GPIO_CHANNEL2       XGPIO_IR_CH2_MASK

//AXI GPIO中断触发方式
#define AXI_GPIO_IRQ_HIGH       0x01
#define AXI_GPIO_IRQ_LOW        0x00

//AXI GPIO中断回调函数
typedef void (*axi_gpio_handler)(void *arg);

int32_t axi_gpio_config(uint32_t number, uint32_t mode);
int32_t axi_gpio_write(uint32_t number, uint32_t value);
int32_t axi_gpio_read(uint32_t number);

int32_t axi_gpio_attach_irq(uint32_t number, uint32_t level, axi_gpio_handler irq_handler, void *arg);
int32_t axi_gpio_detach_irq(uint32_t number);
int32_t axi_gpio_irq_enable(uint32_t channel);
int32_t axi_gpio_irq_disable(uint32_t channel);

int32_t axi_gpio_init(void);

#endif /* USING_AXI_GPIO */

#ifdef __cplusplus
}
#endif

#endif /* __AXI_GPIO_H_ */
