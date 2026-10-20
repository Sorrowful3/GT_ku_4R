/*
 * antenna_v0_offset1.h
 *
 *  Created on: 2021年10月22日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_OFFSET1_H_
#define __ANTENNA_V0_OFFSET1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "antenna_V0_offset.h"
#include "../../core/core_include.h"

#ifdef USING_ANTENNA_V0
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET

int32_t antenna_v0_offset1_find(uint8_t TorR, const antenna_v0_point_t *point, const antenna_v0_offset_info_t *data_info, antenna_v0_offset_t *data);

#endif  /* ANTENNA_V0_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V0 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V0_OFFSET1_H_ */
