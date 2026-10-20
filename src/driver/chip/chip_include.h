/*
 * chip_include.h
 *
 *  Created on: 2021年5月20日
 *      Author: Administrator
 */

#ifndef __CHIP_INCLUDE_H_
#define __CHIP_INCLUDE_H_

#include "../driver_config.h"

#ifdef USING_AXI_CAPTURE
#include "axi_capture.h"
#endif
#ifdef USING_AXI_DMA
#include "axi_dma.h"
#endif
#ifdef USING_AXI_GPIO
#include "axi_gpio.h"
#endif
#ifdef USING_AXI_PWM
#include "axi_pwm.h"
#endif
#ifdef USING_AXI_UART550
#include "axi_uart550.h"
#endif
#ifdef USING_AXI_UARTLITE
#include "axi_uartlite.h"
#endif
#include "gic.h"
#ifdef USING_PS_GPIO
#include "ps_gpio.h"
#endif
#ifdef USING_PS_I2C
#include "ps_i2c.h"
#endif
#ifdef USING_PS_OCM
#include "ps_ocm.h"
#endif
#ifdef USING_PS_WDT
#include "ps_wdt.h"
#endif
#ifdef USING_PS_SPI
#include "ps_spi.h"
#endif

#endif /* __CHIP_INCLUDE_H_ */
