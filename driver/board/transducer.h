/*
 * transducer.h
 *
 *  Created on: 2021年3月24日
 *      Author: Administrator
 */

#ifndef __TRANSDUCER_H_
#define __TRANSDUCER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "power.h"
#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_TRANSDUCER

typedef void (*transducer_receive_callbk)(device_t *device, const uint8_t *command, uint32_t lenght);

typedef struct{
    uint8_t     reset_state;    //变频复位状态，0xff复位成功
    uint32_t    version;        //变频器版本号
    float       temperature;    //变频器温度
    float       electricity;    //电流值
    float       vlotage;        //电压值
#ifdef FREQUENCY_POINT
    uint8_t     down_Lo;        //下变频器本振
#else
    uint32_t     down_Lo;        //下变频器本振
#endif
    uint8_t     mode;           //工作模式
    uint8_t     fault_code;     //异常上报值，0位表示电流异常，1位表示电压异常,2位表示温度异常
    float       if_Value;       //中频信号电压值
    float       rf_Value;       //中频信号电压值
    uint8_t     lock_state;     //锁定状态
    float       gain;       //中频增益
    uint8_t     clok_mode;           //0：内参考  1：外参考
}transducer_data_t;

typedef struct{
    const char  *uart_name; //串口名称
    const char  *dev_name;  //设备名称
    uint32_t    power12v;   //电源
    uint32_t    power5v5;   //电源
}transducer_config_t;

typedef struct{
    device_t                  device;         //继承设备对象

    device_t                  *uart;          //变频器串口
    transducer_config_t       *config;        //变频器配置参数
    transducer_data_t         data;           //变频器数据

    SemaphoreHandle_t         lock;           //互斥锁
    SemaphoreHandle_t         sync;           //同步信号量
    transducer_receive_callbk callbk;         //接收命令回调函数
}transducer_t;


int32_t transducer_init(void);

int32_t transducer_read_version(device_t *device, uint32_t *version);

int32_t transducer_read_state(device_t *device, float *temperature, float *electricity, float *vlotage);

int32_t transducer_set_gain(device_t *device, float gain);

int32_t transducer_set_lo(device_t *device, float lo);
int32_t transducer_get_lo(device_t *device, float *lo);

int32_t transducer_set_mode(device_t *device, uint8_t mode);
int32_t transducer_get_mode(device_t *device, uint8_t *mode);

int32_t transducer_get_fault_code(device_t *device, uint8_t *fault_code);

int32_t transducer_get_if_value(device_t *device, float *if_Value);
int32_t transducer_get_rf_value(device_t *device, float *rf_Value);

int32_t transducer_get_lock_state(device_t *device, uint8_t *lock_state);

int32_t transducer_check(device_t *device, uint8_t *fault_code);

int32_t transducer_restart(device_t *device);

int32_t transducer_write_command(device_t *device, const uint8_t *command, uint32_t lenght);
int32_t transducer_set_receive_callbk(device_t *device, transducer_receive_callbk callbk);

#endif /* USING_TRANSDUCER */

#ifdef __cplusplus
}
#endif

#endif /* __TRANSDUCER_H_ */
