/*
 * antenna_v0_hal.h
 *
 *  Created on: 2021年4月19日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_HAL_H_
#define __ANTENNA_V0_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "antenna_v0_ctrl.h"

#ifdef USING_ANTENNA_V0

int32_t antenna_v0_rf_vcc_on(uint8_t TorR);
int32_t antenna_v0_rf_vcc_off(uint8_t TorR);
int32_t antenna_v0_rf_vcc_state(uint8_t TorR);

int32_t antenna_v0_set_freq(uint8_t TorR, float freq);
int32_t antenna_v0_get_freq(uint8_t TorR, float *freq);
int32_t antenna_v0_set_polarity(uint8_t TorR, uint8_t polarity_type);
int32_t antenna_v0_get_polarity(uint8_t TorR, uint8_t *polarity_type);
int32_t antenna_v0_set_point(uint8_t TorR, float polarity, float phi, float theta);
int32_t antenna_v0_get_point(uint8_t TorR, float *polarity, float *phi, float *theta);
int32_t antenna_v0_update_beam(uint8_t TorR, const antenna_v0_beam_t *beam);

int32_t antenna_v0_selfcheck(uint8_t TorR, uint8_t subset);

int32_t antenna_v0_init(void);

#endif /* USING_ANTENNA_V0 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V0_HAL_H_ */
