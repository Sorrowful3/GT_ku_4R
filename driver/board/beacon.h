/*
 * beacon.h
 *
 *  Created on: 2021年4月6日
 *      Author: Administrator
 */

#ifndef __BEACON_H_
#define __BEACON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "power.h"
#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_BEACON

#define MAX_FRAME_LENGHT        22

typedef void (*beacon_receive_callbk)(device_t *device, uint8_t *command, uint32_t lenght);

//解算后的多模机数据
typedef struct{
    float cnr_power;        //载噪比or功率电平
    float filter_cnr;       //滤波后的载噪比or功率电平
    float freq;             //载波中心频率
    float sym;              //符号速率
    uint8_t rang;           //信标捕获范围
    uint8_t mode;           //工作模式
}beacon_data_t;

//初始化配置结构体
typedef struct {
    const char   *uart_name;    //设备所使用的串口
    const char  *dev_name;      //设备名称
    uint32_t    power;          //电源
}beacon_config_t;

//设备结构体
typedef struct {
    device_t              device;                             //继承device对象

    device_t              *uart;                              //所使用的串口
    beacon_config_t       *config;                            //配置信息

    uint8_t               receive_buffer[MAX_FRAME_LENGHT];   //接收缓存，用于缓存配置命令的返回值
    beacon_data_t         data;                               //已经解算的数据
    uint8_t mode;                                             //工作模式
    float dvb_freq;                                           //DVB频率
    float dvb_sym;                                            //DVB符号速率
    float detection_freq;                                     //检波频率
    float detection_sym;                                      //检波符号速率
    float beacon_freq;                                        //信标频率
    uint8_t beacon_rang;                                      //信标捕获范围

    SemaphoreHandle_t     lock;                               //互斥锁
    SemaphoreHandle_t     sync;                               //同步信号量
    biquad_filter_t       filter;                             //低通滤波器
    beacon_receive_callbk callbk;                             //命令接收完成回调函数
}beacon_t;


int32_t beacon_init(void);

int32_t beacon_set_mode(device_t *device, uint8_t mode);
int32_t beacon_get_mode(device_t *device, uint8_t *mode);

int32_t beacon_set_DVB(device_t *device, float freq, float sym);
int32_t beacon_get_DVB(device_t *device, float *freq, float *sym);

int32_t beacon_set_freq(device_t *device, float freq);
int32_t beacon_get_freq(device_t *device, float *freq);
int32_t beacon_set_range(device_t *device, uint8_t rang);
int32_t beacon_get_range(device_t *device, uint8_t *rang);
int32_t beacon_set_refresh(device_t *device, uint8_t period);

int32_t beacon_set_detection(device_t *device, float freq, float sym);
int32_t beacon_get_detection(device_t *device, float *freq, float *sym);

int32_t beacon_set_voltage_map(device_t *device, uint8_t on_off);
int32_t beacon_set_feed(device_t *device, uint8_t level);
int32_t beacon_set_single_tone(device_t *device, uint8_t on_off);

int32_t beacon_read_version(device_t *device, uint32_t *version);

int32_t beacon_read_data(device_t *device, beacon_data_t *data);

int32_t beacon_check(device_t *device);

int32_t beacon_reset(device_t *device, int32_t level);

int32_t beacon_write_command(device_t *device, const uint8_t *command, uint32_t lenght);
int32_t beacon_set_receive_callbk(device_t *device, beacon_receive_callbk callbk);

#endif /* USING_BEACON */

#ifdef __cplusplus
}
#endif

#endif /* __BEACON_H_ */
