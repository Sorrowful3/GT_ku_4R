/*
 * antenna_define.h
 *
 *  Created on: 2021年9月27日
 *      Author: Administrator
 */

#ifndef __ANTENNA_DEFINE_H_
#define __ANTENNA_DEFINE_H_

#include "../driver_config.h"

/* 阵面编号定义 */
//接收
#define ANTENNA_RX                               0x00
//发射
#define ANTENNA_TX                               0xFF
//ACU
#define ANTENNA_ACU                              0xEE
//子阵0~3
#define ANTENNA_SUBSET_0                         0x01
#define ANTENNA_SUBSET_1                         0x02
#define ANTENNA_SUBSET_2                         0x03
#define ANTENNA_SUBSET_3                         0x04
//全部子阵
#define ANTENNA_SUBSET_ALL                       0x80

/* 极化参考 */
//线极化垂直参考
#define ANTENNA_LINEAR_POLARITY_VERTICAL         0x00
//线极化水平参考
#define ANTENNA_LINEAR_POLARITY_HORIZONTALL      0x01
//线极化仅垂直
#define ANTENNA_LINEAR_POLARITY_VERTICAL_OLAY    0x02
//线极化仅水平
#define ANTENNA_LINEAR_POLARITY_HORIZONTAL_OLAY  0x03
//左极化
#define ANTENNA_LEFT_HAND_CIRCULAR_POLARITY      0x04
//右极化
#define ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY     0x05

#endif /* __ANTENNA_DEFINE_H_ */
