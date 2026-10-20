/*
 * antenna_v1_ctrl.h
 *
 *  Created on: 2021年9月11日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V1_CTRL_H_
#define __ANTENNA_V1_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../power.h"
#include "antenna_v1_config.h"
#include "antenna_v1_msg.h"
#include "antenna_v1_port.h"
#include "antenna_v1_offset.h"

#ifdef USING_ANTENNA_V1

typedef void (*antenna_v1_receive_callbk)(const uint8_t *command, size_t lenght);

//版本信息结构体
typedef struct{
    uint8_t major;          //主版本号
    uint8_t minor;          //次版本号
    uint8_t amend;          //修正版本号
    uint32_t data;          //日期
}antenna_v1_version_t;

//天线波束指向
typedef struct{
    uint8_t polarity_type;      //极化参考
    float polarity;             //极化角
    float theta;                //离轴角
    float phi;                  //方位角
    uint8_t weight;             //加权
    float freq;                 //频率
    uint8_t nbr;                //波束号
    uint8_t polarity_mode;		//极化方式
}antenna_v1_beam_t;

//天线温度
typedef struct{
    int8_t chip_temperature;        //芯片温度
    int8_t board_temperature[3];    //板子温度
}antenna_v1_temperature_t;

//天线功率
typedef struct {
    float total_electricity;    //总电流
    float total_voltage;        //总电压
    float total_power;          //总功率
}antenna_v1_power_t;

//直流电源控制
int32_t antenna_v1_power_on(uint8_t TorR, uint8_t subset);
int32_t antenna_v1_power_on_nodelay(uint8_t TorR, uint8_t subset);
int32_t antenna_v1_power_off(uint8_t TorR, uint8_t subset);
int32_t antenna_v1_power_state(uint8_t TorR, uint8_t subset);

//波控指令实现
int32_t antenna_v1_init_check(uint8_t TorR, uint8_t subset);
int32_t antenna_v1_reset(void);
int32_t antenna_v1_read_versions(uint8_t TorR, uint8_t subset, antenna_v1_version_t *versions);
int32_t antenna_v1_read_power(uint8_t TorR, uint8_t subset, antenna_v1_power_t *power);
int32_t antenna_v1_set_beam(uint8_t TorR, const antenna_v1_beam_t *beam, uint8_t block_flag);
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
int32_t antenna_v1_offset(uint8_t TorR, const antenna_v1_beam_t *beam);
#endif
int32_t antenna_v1_config(uint8_t TorR, uint8_t polarity_mode);
int32_t antenna_v1_ctrl_rf_vcc(uint8_t TorR,  uint8_t subset, uint8_t sw_state);
int32_t antenna_v1_read_temperature(uint8_t TorR, uint8_t subset, uint16_t chip_number, antenna_v1_temperature_t *temperature);
int32_t antenna_v1_set_callbk(antenna_v1_receive_callbk callbk);

int32_t antenna_v1_ctrl_init(void);

#endif /* USING_ANTENNA_V1 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V1_CTRL_H_ */
