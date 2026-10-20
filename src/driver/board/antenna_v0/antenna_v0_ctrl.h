/*
 * antenna_V0_ctrl.h
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_CTRL_H_
#define __ANTENNA_V0_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../power.h"
#include "antenna_v0_config.h"
#include "antenna_v0_msg.h"
#include "antenna_v0_port.h"
#include "antenna_v0_offset.h"

#ifdef USING_ANTENNA_V0

typedef void (*antenna_v0_receive_callbk)(const uint8_t *command, size_t lenght);

//天线波束指向
typedef struct{
    uint8_t polarity_type;      //极化参考
    float polarity;             //极化角
    float theta;                //离轴角
    float phi;                  //方位角
    uint8_t weight;             //加权
    uint8_t polarity_mode;      //极化方式
    float freq;                 //频率
}antenna_v0_beam_t;

//版本信息结构体
typedef struct{
    uint32_t version;
    uint8_t baud;
}antenna_v0_version_t;

//天线电源控制
typedef struct{
    uint8_t state;      //电源状态
    uint8_t polarity;   //极化选择
}antenna_v0_rf_vcc_t;

//波控自检
typedef struct{
    uint16_t data[ANTENNA_V0_MAX_SELFCHECK_FRAME*4];
    uint32_t lenght;
}antenna_v0_selfcheck_t;

//天线温度
typedef struct{
    int8_t board_temperature[3];    //板子温度
    int8_t chip_temperature[9];     //芯片温度
}antenna_v0_temperature_t;

//天线功率
typedef struct {
    float total_electricity;    //总电流
    float total_voltage;        //总电压
    float total_power;          //总功率
}antenna_v0_power_t;

//天线电压
typedef struct{
    float vdd1_00;
    float vdd1_01;
    float maam_vdd;
    float maam_vg;
}antenna_v0_voltage_t;

int32_t antenna_v0_power_on(uint8_t TorR, uint8_t subset);
int32_t antenna_v0_power_on_nodelay(uint8_t TorR, uint8_t subset);
int32_t antenna_v0_power_off(uint8_t TorR, uint8_t subset);
int32_t antenna_v0_power_state(uint8_t TorR, uint8_t subset);

int32_t antenna_v0_set_beam(uint8_t TorR, const antenna_v0_beam_t *beam, uint8_t block_flag);
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
int32_t antenna_v0_offset(uint8_t TorR, const antenna_v0_beam_t *beam);
#endif
int32_t antenna_v0_read_versions(uint8_t TorR, uint8_t subset, antenna_v0_version_t *version);
int32_t antenna_v0_ctrl_rf_vcc(uint8_t TorR, const antenna_v0_rf_vcc_t *rf_vcc);
int32_t antenna_v0_reboot(void);
int32_t antenna_v0_check_channel(uint8_t TorR, uint8_t subset, antenna_v0_selfcheck_t *selfcheck);
int32_t antenna_v0_read_temperature(uint8_t TorR, uint8_t subset, const uint8_t chip_number[9], antenna_v0_temperature_t *temperature);
int32_t antenna_v0_read_power(uint8_t TorR, uint8_t subset, antenna_v0_power_t *power);
int32_t antenna_v0_read_voltage(uint8_t TorR, uint8_t subset, antenna_v0_voltage_t *voltage);
int32_t antenna_v0_set_callbk(antenna_v0_receive_callbk callbk);

int32_t antenna_v0_ctrl_init(void);

#endif /* USING_ANTENNA_V0 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V0_CTRL_H_ */
