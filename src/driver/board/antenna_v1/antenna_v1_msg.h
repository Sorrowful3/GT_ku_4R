/*
 * antenna_v1_msg.h
 *
 *  Created on: 2021年9月5日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V1_MSG_H_
#define __ANTENNA_V1_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "antenna_v1_config.h"
#include "antenna_v1_port.h"

#ifdef USING_ANTENNA_V1

typedef enum{
    ANTENNA_V1_PARSE_OK = 0,          //解析完成
    ANTENNA_V1_PARSE_INCOMPLETE = 1,  //正在解析
    ANTENNA_V1_PARSE_BAD = 2,         //解析出错
}antenna_v1_parse_state_t;

typedef struct{
    uint16_t    current_lenght;                     //当前接收到的字节数
    uint16_t    frame_lenght;                       //当前帧的长度
    uint8_t     msg_id;                             //命令码
    uint8_t     load[ANTENNA_V1_BIG_PACK_LENGHT];   //命令载荷
}antenna_v1_msg_t;


/* 初始化状态查询 */
typedef struct{
    uint8_t TorR;   //收发
    uint8_t subset; //子阵号
}antenna_v1_init_check_tx_t;

typedef struct{
    uint8_t TorR;       //收发
    uint8_t subset;     //子阵号
    uint8_t state;      //状态, 0x00初始化完成，0x01正在初始化
    uint8_t init_flag;  //初始化标志，bit0 cmos初始化未完成，bit1 表项初始化未完成
}antenna_v1_init_check_rx_t;


/* 通讯错误应答 */
typedef struct{
    uint8_t TorR;       //收发
    uint8_t subset;     //子阵号
    uint8_t fault_code; //故障码，bit0 crc错误， bit1 帧长度错误1
}antenna_v1_comm_error_rx_t;


/* 波控复位 */
typedef struct{
    uint8_t reserved;
}antenna_v1_reset_tx_t;

typedef struct{
    uint8_t TorR;       //收发
    uint8_t subset;     //子阵号
}antenna_v1_reset_rx_t;


/* 版本读取 */
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v1_versions_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t major;          //主版本号
    uint8_t minor;          //次版本号
    uint8_t amend;          //修正版本号
    uint32_t data;          //日期
    uint8_t antenna_type;   //阵面类型
    uint8_t work_freq;      //工作频段
    uint8_t work_type;      //工作类型
    uint16_t channel_nbr;   //通道数量
    uint8_t large_version;  //大版本
    uint8_t small_edition;  //小版本
}antenna_v1_versions_rx_t;


/* 总功率读取 */
typedef struct{
    uint8_t TorR;
    uint8_t subset;
}antenna_v1_power_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t total;              //总帧数
    uint8_t current;            //当前帧数
    uint16_t total_electricity; //总电流，发射阵面第一帧有，ma
    uint16_t total_voltage;     //总电压，发射阵面第一帧有，mv
    uint16_t xadc_voltage[4];   //XADC采集的电压，第一帧为XADC1、2，第二帧为XADC3~6，mv
}antenna_v1_power_rx_t;


/* 波束控制 */
typedef struct{
    float polarity;         //极化角
    float theta;            //离轴角
    float phi;              //方位角
    uint8_t weight;         //加权
    uint8_t TorR;           //收发
    float freq;             //频率
}antenna_v1_beam_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t result;     //结果，0x00成功，0x01失败
}antenna_v1_beam_rx_t;


/* 波控配置 */
typedef struct{
    uint8_t TorR;
    uint8_t polarity_type; //极化类型，取值参考宏定义"极化方式"
}antenna_v1_config_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t result;     //结果，0x00成功，0x01失败
}antenna_v1_config_rx_t;

/* 配置异常返回帧 */
typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t state; 	//当前状态，0x00 正在初始化，0x01 初始化完成，0x02上电前波束更新状态，0x03 配置完成，0x04上电完成，0x05上电后波束更新状态
    uint8_t error; 	//错误信息，0x01 正在初始化，0x02 重复配置，0x03 波束设置失败，0x04 波束回读失败，0x05 射频上电失败
    uint8_t hint;	//提示，0x01 需要初始化，0x02 需要配置，0x03 需要设置波束，0x04需要上电
}antenna_v1_config_abnormal_rx_t;


/* 射频电源控制 */
typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t sw_state;      //开关状态，0xff通道供电开，0x01 稀布阵供电开，0x00通道供电关
}antenna_v1_rf_vcc_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint8_t sw_state;   //开关状态，0xff通道供电开，0x01 稀布阵供电开，0x00通道供电关
    uint8_t result;     //结果，0x00成功，0x01失败
}antenna_v1_rf_vcc_rx_t;


/* 温度读取 */
typedef struct{
    uint8_t TorR;
    uint8_t subset;
    uint16_t chip_number; //芯片号
}antenna_v1_temperature_tx_t;

typedef struct{
    uint8_t TorR;
    uint8_t subset;
    int8_t board_temperature[3];    //板子温度
    int8_t chip_temperature;        //芯片温度
}antenna_v1_temperature_rx_t;

/* 极化补偿 */
typedef struct{
    uint8_t TorR;   //收发
    float range;    //幅度
    float phase;    //相位
}antenna_v1_offset_tx_t;

antenna_v1_parse_state_t antenna_v1_parse_byte(antenna_v1_msg_t *msg, uint8_t data);
ssize_t antenna_v1_msg_2_buffer(antenna_v1_msg_t *msg, uint8_t *buffer);
int32_t antenna_v1_msg_send(antenna_v1_msg_t *msg);

int32_t antenna_v1_init_check_pack(const antenna_v1_init_check_tx_t *init_check, antenna_v1_msg_t *msg);
int32_t antenna_v1_init_check_send(const antenna_v1_init_check_tx_t *init_check);
int32_t antenna_v1_init_check_decode(const antenna_v1_msg_t *msg, antenna_v1_init_check_rx_t *init_check);

int32_t antenna_v1_comm_error_decode(const antenna_v1_msg_t *msg, antenna_v1_comm_error_rx_t *comm_error);

int32_t antenna_v1_reset_pack(const antenna_v1_reset_tx_t *reset, antenna_v1_msg_t *msg);
int32_t antenna_v1_reset_send(const antenna_v1_reset_tx_t *reset);
int32_t antenna_v1_reset_decode(const antenna_v1_msg_t *msg, antenna_v1_reset_rx_t *reset);

int32_t antenna_v1_versions_pack(const antenna_v1_versions_tx_t *versions, antenna_v1_msg_t *msg);
int32_t antenna_v1_versions_send(const antenna_v1_versions_tx_t *versions);
int32_t antenna_v1_versions_decode(const antenna_v1_msg_t *msg, antenna_v1_versions_rx_t *versions);

int32_t antenna_v1_power_pack(const antenna_v1_power_tx_t *power, antenna_v1_msg_t *msg);
int32_t antenna_v1_power_send(const antenna_v1_power_tx_t *power);
int32_t antenna_v1_power_decode(const antenna_v1_msg_t *msg, antenna_v1_power_rx_t *power);

int32_t antenna_v1_beam_pack(const antenna_v1_beam_tx_t *beam, antenna_v1_msg_t *msg);
int32_t antenna_v1_beam_send(const antenna_v1_beam_tx_t *beam);
int32_t antenna_v1_beam_decode(const antenna_v1_msg_t *msg, antenna_v1_beam_rx_t *beam);

int32_t antenna_v1_config_pack(const antenna_v1_config_tx_t *config, antenna_v1_msg_t *msg);
int32_t antenna_v1_config_send(const antenna_v1_config_tx_t *config);
int32_t antenna_v1_config_decode(const antenna_v1_msg_t *msg, antenna_v1_config_rx_t *config);

int32_t antenna_v1_config_abnormal_decode(const antenna_v1_msg_t *msg, antenna_v1_config_abnormal_rx_t *config);

int32_t antenna_v1_rf_vcc_pack(const antenna_v1_rf_vcc_tx_t *rf_vcc, antenna_v1_msg_t *msg);
int32_t antenna_v1_rf_vcc_send(const antenna_v1_rf_vcc_tx_t *rf_vcc);
int32_t antenna_v1_rf_vcc_decode(const antenna_v1_msg_t *msg, antenna_v1_rf_vcc_rx_t *rf_vcc);

int32_t antenna_v1_temperature_pack(const antenna_v1_temperature_tx_t *temperature, antenna_v1_msg_t *msg);
int32_t antenna_v1_temperature_send(const antenna_v1_temperature_tx_t *temperature);
int32_t antenna_v1_temperature_decode(const antenna_v1_msg_t *msg, antenna_v1_temperature_rx_t *temperature);

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
int32_t antenna_v1_offset_pack(const antenna_v1_offset_tx_t *offset, antenna_v1_msg_t *msg);
int32_t antenna_v1_offset_send(const antenna_v1_offset_tx_t *offset);
#endif


#endif /* USING_ANTENNA_V1 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V1_MSG_H_ */
