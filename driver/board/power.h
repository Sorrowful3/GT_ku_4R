/*
 * power.h
 *
 *  Created on: 2021年3月19日
 *      Author: Administrator
 */

#ifndef __POWER_H_
#define __POWER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../chip/chip_include.h"
#include "../driver_config.h"

#ifdef USING_POWER

#define AXI_GPIO_CTRL   0X01
#define PS_GPIO_CTRL    0X02

#define HIGH_LEVEL_VALID    0X01
#define LOW_LEVEL_VALID     0X00


typedef struct{
    uint32_t power_number;  //电源网络编号，方便阅读
    uint32_t is_axi_ctrl;   //是否是AXI GPIO控制
    uint32_t gpio_number;   //AXI GPIO或PS GPIO的引脚编号
    uint32_t polarity;      //有效电平极性，高电平或低电平时开启电源
}power_info_t;

int32_t power_init(void);
int32_t power_on(uint32_t number);
int32_t power_off(uint32_t number);

#else

#define power_init()
#define power_on(number)
#define power_off(number)

#endif /* USING_POWER */

#ifdef __cplusplus
}
#endif

#endif /* __POWER_H_ */
