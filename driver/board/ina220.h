/*
 * ina220.h
 *
 *  Created on: 2021年4月26日
 *      Author: Administrator
 */

#ifndef __INA220_H_
#define __INA220_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../chip/chip_include.h"
#include "../driver_config.h"

#ifdef USING_INA220

typedef struct{
    const char      *bus_name;
    const char      *dev_name;
    const uint16_t  slave_addr;
    const float     r_sampling;
}ina220_config_t;

typedef struct{
    float power;        //功率
    float electricity;  //电流
    float voltage;      //电压
}ina220_data_t;

typedef struct{
    device_t        device;     //继承设备对象
    device_t        *i2c;       //所使用的I2C总线

    ina220_config_t *config;    //ina220配置参数
}ina220_t;

int32_t ina220_init(void);

#endif /* USING_INA220 */

#ifdef __cplusplus
}
#endif

#endif /* __INA220_H_ */
