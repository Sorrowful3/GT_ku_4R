/*
 * antenna_v1_offset0.h
 *
 *  Created on: 2021年10月22日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V1_OFFSET0_H_
#define __ANTENNA_V1_OFFSET0_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "antenna_V1_offset.h"
#include "../../core/core_include.h"

#ifdef USING_ANTENNA_V1
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET

int32_t antenna_v1_offset0_find(uint8_t TorR, const antenna_v1_point_t *point, const antenna_v1_offset_info_t *data_info, antenna_v1_offset_t *data);

#endif  /* ANTENNA_V1_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V1 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V1_OFFSET0_H_ */
