/*
 * antenna_hal.h
 *
 *  Created on: 2021年9月22日
 *      Author: Administrator
 */

#ifndef __ANTENNA_HAL_H_
#define __ANTENNA_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "antenna_v0/antenna_v0_port.h"
#include "antenna_v0/antenna_v0_msg.h"
#include "antenna_v0/antenna_v0_ctrl.h"
#include "antenna_v0/antenna_v0_hal.h"

#include "antenna_v1/antenna_v1_port.h"
#include "antenna_v1/antenna_v1_msg.h"
#include "antenna_v1/antenna_v1_ctrl.h"
#include "antenna_v1/antenna_v1_hal.h"

#include "../driver_config.h"

#ifdef USING_ANTENNA_HAL

//天线波束指向
typedef struct{
    uint8_t polarity_type;      //极化参考
    float polarity;             //极化角
    float theta;                //离轴角
    float phi;                  //方位角
    uint8_t weight;             //加权
    float freq;                 //频率
    uint8_t nbr;                //波束号
}antenna_beam_t;

//天线功率
typedef struct {
    float total_electricity;    //总电流
    float total_voltage;        //总电压
    float total_power;          //总功率
}antenna_power_t;

typedef struct {
    int8_t board_temperature[3];
}antenna_temperature_t;

int32_t antenna_rf_vcc_on(uint8_t TorR);
int32_t antenna_rf_vcc_off(uint8_t TorR);
int32_t antenna_rf_vcc_state(uint8_t TorR);

int32_t antenna_set_freq(uint8_t TorR, float freq);
int32_t antenna_get_freq(uint8_t TorR, float *freq);
int32_t antenna_set_polarity(uint8_t TorR, uint8_t polarity_type);
int32_t antenna_get_polarity(uint8_t TorR, uint8_t *polarity_type);
int32_t antenna_set_point(uint8_t TorR, float polarity, float phi, float theta);
int32_t antenna_get_point(uint8_t TorR, float *polarity, float *phi, float *theta);
int32_t antenna_update_beam(uint8_t TorR, const antenna_beam_t *beam);

int32_t antenna_power_on(uint8_t TorR, uint8_t subset);
int32_t antenna_power_off(uint8_t TorR, uint8_t subset);
int32_t antenna_power_state(uint8_t TorR, uint8_t subset);
int32_t antenna_read_power(uint8_t TorR, uint8_t subset, antenna_power_t *power);
int32_t antenna_read_temperature(uint8_t TorR, uint8_t subset, antenna_temperature_t *temperature);
int32_t antenna_selfcheck(uint8_t TorR, uint8_t subset);
int32_t antenna_read_version(uint8_t TorR, uint8_t subset, uint32_t *version);

int32_t antenna_init(void);

#endif /* USING_ANTENNA_HAL */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_HAL_H_ */
