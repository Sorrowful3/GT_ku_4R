/*
 * device_monitor.h
 *
 *  Created on: 2021年4月28日
 *      Author: Administrator
 */

#ifndef __DEVICE_MONITOR_H_
#define __DEVICE_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver/driver.h"

#define ACU_TEMP_ANOMALY            0x0001  //ACU温度异常
#define ACU_ELECTRICITY_ANOMALY     0x0002  //ACU电流异常
#define ACU_VOLTAGE_ANOMALY         0x0004  //ACU电压异常
#define ACU_CTRL_ANOMALY            0x0008  //ACU控制异常

#define TRANSDUCER_CTRL_ANOMALY     0x0010  //变频器控制异常
#define TRANSDUCER_STATE_ANOMALY    0x0020  //变频器状态异常
#define IMU_CTRL_ANOMALY            0x0040  //IMU控制异常
#define BEAM_CTRL_ANOMALY           0x0080  //信标机控制异常

#define TX_CTRL_ANOMALY             0x0100  //发射阵面控制异常
#define TX_TEMP_ANOMALY             0x0200  //发射阵面温度异常
#define TX_ELECTRICITY_ANOMALY      0x0400  //发射阵面电流异常
#define TX_VOLTAGE_ANOMALY          0x0800  //发射阵面电压异常

#define RX_CTRL_ANOMALY             0x1000  //接收阵面控制异常
#define RX_TEMP_ANOMALY             0x2000  //接收阵面温度异常
#define RX_ELECTRICITY_ANOMALY      0x4000  //接收阵面电流异常
#define RX_VOLTAGE_ANOMALY          0x8000  //接收阵面电压异常

#define ACU_UPPER_LIMIT_TEMP        80.0f   //ACU温度上限
#define ACU_UPPER_LIMIT_ELECTRICITY 40.0f   //ACU电流上限
#define ACU_LOWER_LIMIT_VOLTAGE     0.0f    //ACU电压下限
#define ACU_UPPER_LIMIT_VOLTAGE     35.0f   //ACU电压上限

//接收天线没有电流电压传感器
#define RX_UPPER_LIMIT_TEMP         80.0f   //接收天线温度上限
#define RX_UPPER_LIMIT_ELECTRICITY  25.0f	//接收天线电流上限
#define RX_LOWER_LIMIT_VOLTAGE      0.0f    //接收天线电压下限
#define RX_UPPER_LIMIT_VOLTAGE      15.0f	//接收天线电压上限

#define TX_UPPER_LIMIT_TEMP         80.0f   //发射天线温度上限
#define TX_UPPER_LIMIT_ELECTRICITY  25.0f   //发射天线电流上限
#define TX_LOWER_LIMIT_VOLTAGE      0.0f    //发射天线电压下限
#define TX_UPPER_LIMIT_VOLTAGE      15.0f   //发射天线电压上限

//#define USING_INA22x_1
#define INAx0_NAME  "ina226_0"
#define INAx1_NAME  "ina220_1"

//typedef ina226_data_t inax_data_t;

typedef struct {
    float       acu_temperature;                    //ACU温度
    float       acu_electricity;                    //ACU电流
    float       acu_voltage;                        //ACU电压

    float       tx_temperature[ANTENNA_TX_TOTAL];   //发射天线温度
    float       tx_electricity[ANTENNA_TX_TOTAL];   //发射天线电流
    float       tx_voltage[ANTENNA_TX_TOTAL];       //发射天线电压

    float       rx_temperature[ANTENNA_RX_TOTAL];   //接收天线温度
    float       rx_electricity[ANTENNA_RX_TOTAL];   //接收天线电流
    float       rx_voltage[ANTENNA_RX_TOTAL];       //接收天线电压

    float       tr_temperature;                     //变频器温度
    float       tr_electricity;                     //变频器线电流
    float       tr_voltage;                         //变频器线电压

    float       max_temperature;                    //设备最热点温度
    uint32_t    global_fault_code;                  //设备异常码
}device_state_t;

int32_t device_monitor_init(void);
int32_t get_device_state(device_state_t *device_state);
int32_t device_monitor_set(uint8_t mode, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_MONITOR_H_ */
