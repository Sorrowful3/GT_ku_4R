/*
 * ina226.h
 *
 *  Created on: 2021年8月11日
 *      Author: Administrator
 */

#ifndef __INA226_H_
#define __INA226_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../chip/chip_include.h"
#include "../driver_config.h"

#ifdef USING_INA226

typedef struct{
    const char      *bus_name;
    const char      *dev_name;
    const uint16_t  slave_addr;
    const float     r_sampling;
}ina226_config_t;

typedef struct{
    float power;        //功率
    float electricity;  //电流
    float voltage;      //电压
}ina226_data_t;

typedef struct{
    device_t        device;     //继承设备对象
    device_t        *i2c;       //所使用的I2C总线

    ina226_config_t *config;    //ina220配置参数
}ina226_t;

int32_t ina226_init(void);

#endif /* USING_INA226 */

#ifdef __cplusplus
}
#endif

#endif /* __INA226_H_ */
