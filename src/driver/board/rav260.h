/*
 * rav260.h
 *
 *  Created on: 2021年3月19日
 *      Author: Administrator
 */

#ifndef __RAV260_H_
#define __RAV260_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "power.h"
#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_RAV260

#define RAV260_INERTIAL_NAVIGATION_STATE    0x01    //惯性导航
#define RAV260_INTEGRATED_NAVIGATION_STATE  0x02    //组合导航
#define RAV260_OTHER_STATE                  0x04    //其他状态

//IMU原始数据
typedef struct{
    uint8_t     current_state;  //状态
    int16_t     x_speed;        //x轴方向的角速度
    int16_t     y_speed;        //y轴方向的角速度
    int16_t     z_speed;        //z轴方向的角速度
    int16_t     pitch;          //俯仰角
    int16_t     roll;           //横滚角
    uint16_t    yaw;            //航向角
    int16_t     longitude;      //经度
    int16_t     latitude;       //纬度
    int16_t     height;         //高度
    uint8_t     speed;          //地速
    uint8_t     temperature;    //温度
    int32_t     second;         //GPS周秒
    uint16_t    week;           //GPS周
}rav260_orig_data_t;

typedef struct{
    uint32_t    valid_flag; //有效标志，0 无效，1 定位成功
    int32_t     second;     //GPS周秒
    uint16_t    week;       //GPS周
    uint32_t    time_s;     //GPS时间，采用UNIX格式
    uint64_t    time_ms;    //GPS时间，精细到ms
}rav260_time_t;

typedef struct{
    uint32_t    valid_flag; //有效标志，0 GPS无效，1 GPS定位成功
    float       longitude;  //经度
    float       latitude;   //纬度
    float       height;     //高度
    float       speed;      //地速
}rav260_gps_t;

typedef struct{
    uint32_t    valid_flag; //有效标志，0 INS无效，1 INS定位成功
    float       x_speed;    //x轴方向的角速度
    float       y_speed;    //y轴方向的角速度
    float       z_speed;    //z轴方向的角速度
    float       pitch;      //俯仰角
    float       roll;       //横滚角
    float       yaw;        //航向角
}rav260_ins_t;

typedef struct{
    uint8_t orig_state; //IMU当前的原始状态字
    uint8_t state;      //IMU当前导航状态
}rav260_state_t;

//配置结构体
typedef struct {
    const char  *uart_name; //IMU所连接的串口名称
    const char  *dev_name;  //IMU设备名称
    uint32_t    versions;   //IMU版本
    uint32_t    power;      //电源
}rav260_config_t;

//设备结构体
typedef struct {
    device_t            device;     //继承device对象

    rav260_config_t     *config;    //IMU配置信息
    device_t            *uart;      //IMU所使用的串口
    rav260_orig_data_t  orig_data;  //解算前的IMU数据

    int32_t             manual_gps;         //手动GPS标志
    float               manual_longitude;   //经度
    float               manual_latitude;    //纬度
    int16_t             manual_height;      //高度

    SemaphoreHandle_t   lock;       //配置IMU的互斥锁
    EventGroupHandle_t  sync;       //配置IMU的同步事件标志组
}rav260_t;

int32_t rav260_init(void);

int32_t rav260_leap_second(uint64_t second);

int32_t rav260_correct(device_t *device, uint8_t valid, float correct);
int32_t rav260_aim(device_t *device);
int32_t rav260_manual_gps(device_t *device, int32_t flag, float longitude, float latitude, int16_t height);

int32_t rav260_orig_data(device_t *device, rav260_orig_data_t *orig_data, uint8_t block_flag);

int32_t rav260_time(device_t *device, rav260_time_t *time);
int32_t rav260_gps(device_t *device, rav260_gps_t *gps);

int32_t rav260_ins(device_t *device, rav260_ins_t *ins);

int32_t rav260_state(device_t *device, rav260_state_t *state);

int32_t rav260_temperature(device_t *device, float *temperature);

int32_t rav260_check(device_t *device);

int32_t rav260_restart(device_t *device);

#endif /* USING_RAV260 */

#ifdef __cplusplus
}
#endif

#endif /* __RAV260_H_ */
