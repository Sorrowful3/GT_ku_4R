/*
 * antenna_v0_config.h
 *
 *  Created on: 2020年8月27日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_CONFIG_H_
#define __ANTENNA_V0_CONFIG_H_

#include "../../driver_config.h"
#include "../antenna_define.h"

//接收阵面的数量
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
#define ANTENNA_V0_RX_TOTAL                                 ANTENNA_RX_TOTAL
#else
#define ANTENNA_V0_RX_TOTAL                                 0
#endif
//发射阵面的数量
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
#define ANTENNA_V0_TX_TOTAL                                 ANTENNA_TX_TOTAL
#else
#define ANTENNA_V0_TX_TOTAL                                 0
#endif

//接收阵面直流供电
#define ANTENNA_V0_R0_VCC                                   ANTENNA_R0_VCC
#define ANTENNA_V0_R1_VCC                                   ANTENNA_R1_VCC
#define ANTENNA_V0_R2_VCC                                   ANTENNA_R2_VCC
#define ANTENNA_V0_R3_VCC                                   ANTENNA_R3_VCC
//发射阵面直流供电
#define ANTENNA_V0_T0_VCC                                   ANTENNA_T0_VCC
#define ANTENNA_V0_T1_VCC                                   ANTENNA_T1_VCC
#define ANTENNA_V0_T2_VCC                                   ANTENNA_T2_VCC
#define ANTENNA_V0_T3_VCC                                   ANTENNA_T3_VCC

//接收
#define ANTENNA_V0_RX                                       ANTENNA_RX
//发射
#define ANTENNA_V0_TX                                       ANTENNA_TX
//ACU
#define ANTENNA_V0_ACU                                      ANTENNA_ACU

//子阵0~3
#define ANTENNA_V0_SUBSET_0                                 ANTENNA_SUBSET_0
#define ANTENNA_V0_SUBSET_1                                 ANTENNA_SUBSET_1
#define ANTENNA_V0_SUBSET_2                                 ANTENNA_SUBSET_2
#define ANTENNA_V0_SUBSET_3                                 ANTENNA_SUBSET_3
//全部子阵
#define ANTENNA_V0_SUBSET_ALL                               ANTENNA_SUBSET_ALL

/* 极化参考 */
//线极化垂直参考
#define ANTENNA_V0_LINEAR_POLARITY_VERTICAL                 ANTENNA_LINEAR_POLARITY_VERTICAL
//线极化水平参考
#define ANTENNA_V0_LINEAR_POLARITY_HORIZONTALL              ANTENNA_LINEAR_POLARITY_HORIZONTALL
//线极化仅垂直
#define ANTENNA_V0_LINEAR_POLARITY_VERTICAL_OLAY            ANTENNA_LINEAR_POLARITY_VERTICAL_OLAY
//线极化仅水平
#define ANTENNA_V0_LINEAR_POLARITY_HORIZONTAL_OLAY          ANTENNA_LINEAR_POLARITY_HORIZONTAL_OLAY
//左极化
#define ANTENNA_V0_LEFT_HAND_CIRCULAR_POLARITY              ANTENNA_LEFT_HAND_CIRCULAR_POLARITY
//右极化
#define ANTENNA_V0_RIGHT_HAND_CIRCULAR_POLARITY             ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY

/* 极化方式 */
//左极化
#define ANTENNA_V0_LEFT_POLARZA_MODE    0x00
//右极化
#define ANTENNA_V0_RIGHT_POLARZA_MODE   0x01
//全极化
#define ANTENNA_V0_COMPLETE_POLARZA_MODE  0x02

//天线有两种长度的数据帧，分别是286B和22B
#define ANTENNA_V0_BIG_PACK_LENGHT                          286
#define ANTENNA_V0_SMALL_PACK_LENGHT                        22

//接收阵面芯片数量
#define ANTENNA_V0_RX_CHIP_TOTAL                            192
//发射阵面芯片数量
#define ANTENNA_V0_TX_CHIP_TOTAL                            128

//自检数据最大回复帧数
#define ANTENNA_V0_MAX_SELFCHECK_FRAME                      6

//使能极化补偿
#define ANTENNA_V0_USING_POLARITY_OFFSET

//使能调试匹配输出功能
#define ANTENNA_V0_ENABLE_MATE_DEBUG

#endif /* _ANTENNA_V0_CONFIG_H_ */
