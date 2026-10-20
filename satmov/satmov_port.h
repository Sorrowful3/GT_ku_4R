/*
 * satmov_port.h
 *
 *  Created on: 2021年6月17日
 *      Author: Administrator
 */

#ifndef __SATMOV_PORT_H_
#define __SATMOV_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "satmov_tracking.h"
#include "../tr_arhs/adis16465_port.h"

typedef struct {
    float sat_longitude;        /*<  卫星经度,弧度*/

    float rx_freq;              /*<  下行频率(MHz)*/
    float rx_osc;               /*<  下行本振(MHz)*/
    uint8_t rx_gain;            /*<  下行增益(dB)*/
    uint8_t rx_polarity_type;   /*<  下行极化参考*/

    float linger_factor;        /*<  波束驻留时间系数*/
    float sym;                  /*<  下行符号速率(Ksps)*/
    uint8_t beacon_rang;        /*<  信标机捕获范围(KHZ)，信标模式有效*/
    uint8_t beacon_mode;        /*<  信标机模式*/
}satmov_geo_beam_info_t;

typedef struct {
    float azimuth;              //方位角，弧度
    float elevation;            //俯仰角，弧度
    float polarity;             //地理极化，弧度
    float rx_freq;              //下行频率(MHz)
    float rx_osc;               //下行本振(MHz)
    float tx_freq;              //上行频率(MHz)
    float tx_osc;               //上行本振(MHz)
    uint8_t rx_gain;            //下行增益
    uint8_t rx_polarity_type;   //下行极化参考
    uint8_t tx_gain;            //上行增益
    uint8_t tx_polarity_type;   //上行极化参考
}stamov_leo_beam_info_t;

typedef void (*satmov1_currect_complete)(float yaw);

//初始化对星程序
int32_t satmov1_init(void);
//启动自动对星
int32_t satmov1_start(void);
//启动跟踪
int32_t satmov1_tracking(void);
//暂停自动对星
int32_t satmov1_suspend(void);
//恢复自动对星
int32_t satmov1_resume(void);
//启动航向角校准
int32_t satmov1_currect_yaw(const satmov_geo_beam_info_t *beam_info, const satmov_currect_info_t *currect_info);
//获取当前工作状态
uint32_t satmov1_get_state(void);
//设置校准完成回调函数
int32_t satmov1_set_currect_callbk(satmov1_currect_complete callbk);
//设置地理指向
int32_t satmov1_set_leo_beam_info(stamov_leo_beam_info_t *beam_info);
//获取地理指向
int32_t satmov1_get_earth_point(satmov_point_t *earth_point);

int32_t read_gps_temp();
#ifdef __cplusplus
}
#endif

#endif /* __SATMOV_PORT_H_ */
