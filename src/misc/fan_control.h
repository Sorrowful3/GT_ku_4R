/*
 * fan_control.h
 *
 *  Created on: 2021年5月6日
 *      Author: Administrator
 */

#ifndef __FAN_CONTROL_H_
#define __FAN_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "device_monitor.h"
#include "../driver/driver.h"


#define PWM_CLK_MHZ         100
#define CAPTURE_CLK_MHZ     100
#define PWM_PERIOD          500

#define FAN_CONTROL_PARAMETER               \
{                                           \
    {                                       \
        .temperature = 45.0f,               \
        .duty_cycle = 25.0f,                \
    },                                      \
    {                                       \
        .temperature = 50.0f,               \
        .duty_cycle = 35.0f,                \
    },                                      \
    {                                       \
        .temperature = 55.0f,               \
        .duty_cycle = 45.0f,                \
    },                                      \
    {                                       \
        .temperature = 60.0f,               \
        .duty_cycle = 70.0f,                \
    },                                      \
    {                                       \
        .temperature = 65.0f,               \
        .duty_cycle = 100.0f,               \
    },                                      \
}

#define FAN_CONTROL_INFO                    \
{                                           \
    {                                       \
        .number = 0,                        \
        .pwm_channel = PWM_CHANNEL0,        \
        .capture_channel = INPUT_CHANNEL0,  \
        .ctrl_foc = 0.0f,                   \
        .speed = 0,                         \
    },                                      \
    {                                       \
        .number = 1,                        \
        .pwm_channel = PWM_CHANNEL1,        \
        .capture_channel = INPUT_CHANNEL1,  \
        .ctrl_foc = 0.0f,                   \
        .speed = 0,                         \
    },                                      \
}

typedef struct{
    float temperature;  //温度
    float duty_cycle;   //对应温度所使用的占空比
}fan_control_parameter_t;

typedef struct{
    uint32_t number;            //风扇编号，按顺序编号
    uint32_t pwm_channel;       //PWM通道
    uint32_t capture_channel;   //输入捕获通道
    float ctrl_foc;             //当前控制量
    uint32_t speed;             //风扇转速
 }fan_control_info_t;

int32_t fan_control_init(void);
int32_t fan_get_speed(uint32_t number, uint32_t *speed);

#ifdef __cplusplus
}
#endif

#endif /* __FAN_CONTROL_H_ */
