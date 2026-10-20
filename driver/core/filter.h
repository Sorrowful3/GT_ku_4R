/**
 * @file    filter.h
 * @brief   实现滤波器相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年4月27日
 */

#ifndef __FILTER_H_
#define __FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "define.h"

#define FILTER_LPF      0
#define FILTER_NOTCH    1

/* bandwidth in octaves */
#define BIQUAD_BANDWIDTH        1.9f
/* quality factor - butterworth*/
#define BIQUAD_Q                (1.0f / sqrtf(2.0f))

/* this holds the data required to update samples thru a filter */
typedef struct biquad_filter_s {
    float b0, b1, b2, a1, a2;
    float d1, d2;
} biquad_filter_t;

int32_t biquad_filter_init(biquad_filter_t *filter, float sampling_freq, float filter_freq, float Q, uint32_t filter_type);
int32_t biquad_filter_init_notch(biquad_filter_t *filter, float sampling_freq, float filter_freq, float cutoff_Hz);
int32_t biquad_filter_init_LPF(biquad_filter_t *filter, float sampling_freq, float filter_freq);
float biquad_filter_apply(biquad_filter_t *filter, float sample);

#ifdef __cplusplus
}
#endif

#endif /* __FILTER_H_ */
