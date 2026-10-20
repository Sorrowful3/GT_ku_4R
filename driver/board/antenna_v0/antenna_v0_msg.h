/*
 * antenna_v0_msg.h
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_MSG_H_
#define __ANTENNA_V0_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "antenna_v0_config.h"
#include "antenna_v0_port.h"

#ifdef USING_ANTENNA_V0

typedef enum{
    ANTENNA_PARSE_OK = 0,          //解析完成
    ANTENNA_PARSE_INCOMPLETE = 1,  //正在解析
    ANTENNA_PARSE_BAD = 2,         //解析出错
}antenna_v0_parse_state_t;

typedef struct {
    uint16_t    current_lenght;                     //当前接收到的字节数
    uint16_t    frame_lenght;                       //当前帧的长度
    uint8_t     msg_id;                             //命令码
    uint8_t     load[ANTENNA_V0_BIG_PACK_LENGHT];      //命令载荷
}antenna_v0_msg_t;

//命令命名规则，antenna_命令名称_命令方向_t，tx表示PS到PL，rx表示PL到PS

//波束控制
typedef struct{
    float polarity;         //极化角
    float theta;            //离轴角
    float phi;              //方位角
    uint8_t weight;         //加权
    uint8_t TorR;           //收发
    uint8_t polarity_mode;  //极化类型
    float freq;             //频率
}antenna_v0_beam_tx_t;

typedef struct{
    uint8_t TorR;   //收发
    uint8_t subset; //子阵号
    uint8_t error;  //错误码
}antenna_v0_beam_rx_t;


//版本回读
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v0_versions_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint32_t version;   //版本号
    uint8_t baud;       //通信速率
}antenna_v0_versions_rx_t;


//射频电源控制
typedef struct{
    uint8_t state;      //开关状态，0xff通道供电开，0x01 稀布阵供电开，0x00通道供电关
    uint8_t polarity;   //极化方式，0x01水平极化，0x00垂直极化，0xff双极化选择
    uint8_t TorR;
}antenna_v0_rf_vcc_tx_t;

typedef struct{
    uint8_t state;      //开关状态，0xff通道供电开，0x01 稀布阵供电开，0x00通道供电关
    uint8_t polarity;   //极化方式，0x01水平极化，0x00垂直极化，0xff双极化选择
    uint8_t TorR;
    uint8_t subset;
}antenna_v0_rf_vcc_rx_t;


//复位
typedef struct{
    uint8_t reserved;
}antenna_v0_reboot_tx_t;


//波束赋形芯片自检
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v0_selfcheck_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t total_frame;    //总帧数
    uint8_t current_frame;  //当前帧数
    uint16_t check_data[4]; //自检数据
}antenna_v0_selfcheck_rx_t;


//温度读取
typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t chip_number[9]; //芯片号
}antenna_v0_temperature_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    int8_t board_temperature[3];    //板子温度
    int8_t chip_temperature[9];     //芯片温度
}antenna_v0_temperature_rx_t;


//总电流电压读取
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v0_power_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    float total_electricity;//总电流
    float total_voltage;    //总电压
    float total_power;      //总功率
}antenna_v0_power_rx_t;


//关键电流电压读取
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v0_voltage_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    float vdd1_00;
    float vdd1_01;
    float maam_vdd;
    float maam_vg;
}antenna_v0_voltage_rx_t;

typedef struct{
    uint8_t TorR;   //收发
    float range;    //幅度
    float phase;    //相位
}antenna_v0_offset_tx_t;

antenna_v0_parse_state_t antenna_v0_parse_byte(antenna_v0_msg_t *msg, uint8_t data);

ssize_t antenna_v0_msg_2_buffer(antenna_v0_msg_t *msg, uint8_t *buffer);
int32_t antenna_v0_msg_send(antenna_v0_msg_t *msg);

int32_t antenna_v0_beam_pack(const antenna_v0_beam_tx_t *beam, antenna_v0_msg_t *msg);
int32_t antenna_v0_beam_send(const antenna_v0_beam_tx_t *beam);
int32_t antenna_v0_beam_decode(const antenna_v0_msg_t *msg, antenna_v0_beam_rx_t *beam);

int32_t antenna_v0_versions_pack(const antenna_v0_versions_tx_t *versions, antenna_v0_msg_t *msg);
int32_t antenna_v0_versions_send(const antenna_v0_versions_tx_t *versions);
int32_t antenna_v0_versions_decode(const antenna_v0_msg_t *msg, antenna_v0_versions_rx_t *versions);

int32_t antenna_v0_rf_vcc_pack(const antenna_v0_rf_vcc_tx_t *rf_vcc, antenna_v0_msg_t *msg);
int32_t antenna_v0_rf_vcc_send(const antenna_v0_rf_vcc_tx_t *rf_vcc);
int32_t antenna_v0_rf_vcc_decode(const antenna_v0_msg_t *msg, antenna_v0_rf_vcc_rx_t *rf_vcc);

int32_t antenna_v0_reboot_pack(const antenna_v0_reboot_tx_t *reboot, antenna_v0_msg_t *msg);
int32_t antenna_v0_reboot_send(const antenna_v0_reboot_tx_t *reboot);

int32_t antenna_v0_selfcheck_pack(const antenna_v0_selfcheck_tx_t *selfcheck, antenna_v0_msg_t *msg);
int32_t antenna_v0_selfcheck_send(const antenna_v0_selfcheck_tx_t *selfcheck);
int32_t antenna_v0_selfcheck_decode(const antenna_v0_msg_t *msg, antenna_v0_selfcheck_rx_t *selfcheck);

int32_t antenna_v0_temperature_pack(const antenna_v0_temperature_tx_t *temperature, antenna_v0_msg_t *msg);
int32_t antenna_v0_temperature_send(const antenna_v0_temperature_tx_t *temperature);
int32_t antenna_v0_temperature_decode(const antenna_v0_msg_t *msg, antenna_v0_temperature_rx_t *temperature);

int32_t antenna_v0_power_pack(const antenna_v0_power_tx_t *power, antenna_v0_msg_t *msg);
int32_t antenna_v0_power_send(const antenna_v0_power_tx_t *power);
int32_t antenna_v0_power_decode(const antenna_v0_msg_t *msg, antenna_v0_power_rx_t *power);

int32_t antenna_v0_voltage_pack(const antenna_v0_voltage_tx_t *voltage, antenna_v0_msg_t *msg);
int32_t antenna_v0_voltage_send(const antenna_v0_voltage_tx_t *voltage);
int32_t antenna_v0_voltage_decode(const antenna_v0_msg_t *msg, antenna_v0_voltage_rx_t *voltage);

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
int32_t antenna_v0_offset_pack(const antenna_v0_offset_tx_t *offset, antenna_v0_msg_t *msg);
int32_t antenna_v0_offset_send(const antenna_v0_offset_tx_t *offset);
#endif

#endif /* USING_ANTENNA_V0 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V0_MSG_H_ */
