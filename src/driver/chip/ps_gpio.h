/**
 * @file    ps_gpio.h
 * @brief   用于驱动PS端的GPIO外设
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月5日
 */

#ifndef __PS_GPIO_H_
#define __PS_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_PS_GPIO

#include "xgpiops.h"

/* PS GPIO电平状态 */
#define PS_GPIO_LOW         0x00
#define PS_GPIO_HIGH        0x01

/* PS GPIO工作模式 */
#define PS_GPIO_MODE_OUTPUT      0x01
#define PS_GPIO_MODE_INPUT       0x00

/* PS GPIO中断触发方式 */
#define PS_GPIO_IRQ_RISING          XGPIOPS_IRQ_TYPE_EDGE_RISING
#define PS_GPIO_IRQ_FALLING         XGPIOPS_IRQ_TYPE_EDGE_FALLING
#define PS_GPIO_IRQ_RISING_FALLING  XGPIOPS_IRQ_TYPE_EDGE_BOTH
#define PS_GPIO_IRQ_HIGH            XGPIOPS_IRQ_TYPE_LEVEL_HIGH
#define PS_GPIO_IRQ_LOW             XGPIOPS_IRQ_TYPE_LEVEL_LOW

/* PS GPIO中断回调函数 */
typedef void (*ps_gpio_handler)(void *arg);

int32_t ps_gpio_config(uint32_t gpio_number, uint32_t mode);
int32_t ps_gpio_write(uint32_t gpio_number, uint32_t value);
int32_t ps_gpio_read(uint32_t gpio_number);

int32_t ps_gpio_attach_irq(uint32_t gpio_number, uint8_t type, ps_gpio_handler irq_handler, void *arg);
int32_t ps_gpio_detach_irq(uint32_t gpio_number);

int32_t ps_gpio_init(void);

#endif /* USING_PS_GPIO */

#ifdef __cplusplus
}
#endif

#endif /* __PS_GPIO_H_ */
