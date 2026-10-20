/*
 * correct_v1_data.h
 *
 *  Created on: 2021年6月5日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V1_OFFSET_H_
#define __ANTENNA_V1_OFFSET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "antenna_v1_config.h"
#include "../../core/core_include.h"

#ifdef USING_ANTENNA_V1
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET

typedef struct{
    float range;    //幅度
    float phase;    //相位
}antenna_v1_offset_t;

typedef struct{
    float polarity; 		//极化角
    float theta;    		//离轴角
    float phi;     		 	//方位角
    float freq;     		//频率
    uint8_t polarity_type;	//极化模式
}antenna_v1_point_t;

typedef struct{
    uint32_t load_addr;	//数据加载地址
    uint32_t size;		//数据大小
    uint16_t sum;		//校验和
    uint8_t  sum_flag;	//是否和校验
}antenna_v1_offset_info_t;

int32_t antenna_v1_offset_init(void);
int32_t antenna_v1_offset_find(uint8_t TorR, const antenna_v1_point_t *point, antenna_v1_offset_t *data);

#endif  /* ANTENNA_V1_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V1 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V1_OFFSET_H_ */
