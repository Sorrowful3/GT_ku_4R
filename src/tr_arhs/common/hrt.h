/*
 * hrt.h
 *
 *  Created on: 2021Äê4ÔÂ22ÈÕ
 *      Author: admin
 */

#ifndef SRC_PALNFORM_FREERTOS_COMMON_HRT_H_
#define SRC_PALNFORM_FREERTOS_COMMON_HRT_H_

#include "xtime_l.h"
#include <sys/types.h>

typedef uint64_t	hrt_abstime;

extern "C"  hrt_abstime  hrt_absolute_time(void);

#endif /* SRC_PALNFORM_FREERTOS_COMMON_HRT_H_ */
