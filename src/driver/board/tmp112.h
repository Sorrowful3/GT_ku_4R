/*
 * tmp112.h
 *
 *  Created on: 2021年4月26日
 *      Author: Administrator
 */

#ifndef __TMP112_H_
#define __TMP112_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../chip/chip_include.h"
#include "../driver_config.h"

#ifdef USING_TMP112

typedef struct{
    const char      *bus_name;
    const char      *dev_name;
    const uint16_t  slave_addr;
}tmp112_config_t;

typedef struct{
    device_t        device;         //继承设备对象
    device_t        *i2c;           //所使用的I2C总线

    tmp112_config_t *config;        //TMP112配置参数
}tmp112_t;

int32_t tmp112_init(void);

#endif /* USING_TMP112 */

#ifdef __cplusplus
}
#endif

#endif /* __TMP112_H_ */
