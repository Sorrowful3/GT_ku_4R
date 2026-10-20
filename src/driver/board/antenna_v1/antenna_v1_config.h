/*
 * antenna_v1_config.h
 *
 *  Created on: 2020年9月5日
 *      Author: Administrator
 */

#ifndef _ANTENNA_V1_CONFIG_H_
#define _ANTENNA_V1_CONFIG_H_

#include "../../driver_config.h"
#include "../antenna_define.h"

//接收阵面的数量
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V1
#define ANTENNA_V1_RX_TOTAL                                 ANTENNA_RX_TOTAL
#else
#define ANTENNA_V1_RX_TOTAL                                 0
#endif
//发射阵面的数量
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V1
#define ANTENNA_V1_TX_TOTAL                                 ANTENNA_TX_TOTAL
#else
#define ANTENNA_V1_TX_TOTAL                                 0
#endif

//接收阵面直流供电
#define ANTENNA_V1_R0_VCC                                   ANTENNA_R0_VCC
#define ANTENNA_V1_R1_VCC                                   ANTENNA_R1_VCC
#define ANTENNA_V1_R2_VCC                                   ANTENNA_R2_VCC
#define ANTENNA_V1_R3_VCC                                   ANTENNA_R3_VCC
//发射阵面直流供电
#define ANTENNA_V1_T0_VCC                                   ANTENNA_T0_VCC
#define ANTENNA_V1_T1_VCC                                   ANTENNA_T1_VCC
#define ANTENNA_V1_T2_VCC                                   ANTENNA_T2_VCC
#define ANTENNA_V1_T3_VCC                                   ANTENNA_T3_VCC

//接收
#define ANTENNA_V1_RX                                       ANTENNA_RX
//发射
#define ANTENNA_V1_TX                                       ANTENNA_TX
//ACU
#define ANTENNA_V1_ACU                                      ANTENNA_ACU

//子阵0~3
#define ANTENNA_V1_SUBSET_0                                 ANTENNA_SUBSET_0
#define ANTENNA_V1_SUBSET_1                                 ANTENNA_SUBSET_1
#define ANTENNA_V1_SUBSET_2                                 ANTENNA_SUBSET_2
#define ANTENNA_V1_SUBSET_3                                 ANTENNA_SUBSET_3
//全部子阵
#define ANTENNA_V1_SUBSET_ALL                               ANTENNA_SUBSET_ALL

/* 极化参考 */
//线极化垂直参考
#define ANTENNA_V1_LINEAR_POLARITY_VERTICAL                 ANTENNA_LINEAR_POLARITY_VERTICAL
//线极化水平参考
#define ANTENNA_V1_LINEAR_POLARITY_HORIZONTALL              ANTENNA_LINEAR_POLARITY_HORIZONTALL
//线极化仅垂直
#define ANTENNA_V1_LINEAR_POLARITY_VERTICAL_OLAY            ANTENNA_LINEAR_POLARITY_VERTICAL_OLAY
//线极化仅水平
#define ANTENNA_V1_LINEAR_POLARITY_HORIZONTAL_OLAY          ANTENNA_LINEAR_POLARITY_HORIZONTAL_OLAY
//左极化
#define ANTENNA_V1_LEFT_HAND_CIRCULAR_POLARITY              ANTENNA_LEFT_HAND_CIRCULAR_POLARITY
//右极化
#define ANTENNA_V1_RIGHT_HAND_CIRCULAR_POLARITY             ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY

/* 极化方式 */
//左旋极化
#define ANTENNA_V1_LEFT_POLARITY_MODE                       0x00
//右旋极化
#define ANTENNA_V1_RIGHT_POLARITY_MODE                      0x01
//线极化
#define ANTENNA_V1_LINEAR_POLARITY_MODE                     0x02
//水平极化
#define ANTENNA_V1_HORIZONTALL_POLARITY_MODE                0x03
//垂直极化
#define ANTENNA_V1_VERTICAL_POLARITY_MODE                   0x04

/* 射频电源状态 */
//开电源
#define ANTENNA_V1_ON                                       0xFF
//关电源
#define ANTENNA_V1_OFF                                      0x00

/* 数据包长度 */
//天线有两种长度的数据帧，分别是286B和22B
#define ANTENNA_V1_BIG_PACK_LENGHT                          286
#define ANTENNA_V1_SMALL_PACK_LENGHT                        22

//使能匹配调试输出
#define ANTENNA_V1_ENABLE_MATE_DEBUG

/* 阵面芯片数量  */
//接收阵面芯片数量
#define ANTENNA_V1_RX_CHIP_TOTAL                            192
//发射阵面芯片数量
#define ANTENNA_V1_TX_CHIP_TOTAL                            192

/* 定义多帧回复命令的最大回复帧数 */
//总功率读取命令最大回复帧数
#define ANTENNA_V1_MAX_POWER_FRAME                          2
//自检命令最大回复帧数
#define ANTENNA_V1_MAX_SELFCHECK_FRAME                      6

//#define ANTENNA_V1_USING_POLARITY_OFFSET

//使用全局互斥锁
#define USING_GLOBAL_LOCK
#endif /* _ANTENNA_V1_CONFIG_H_ */
