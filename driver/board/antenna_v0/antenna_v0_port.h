/*
 * antenna_v0_port.h
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V0_LL_H_
#define __ANTENNA_V0_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/core_include.h"
#include "../../driver_config.h"

#ifdef USING_ANTENNA_V0

void antenna_v0_port_write_start(void);
void antenna_v0_port_write_end(void);

ssize_t antenna_v0_port_read(void *buffer, ssize_t lenght, TickType_t xTicksToWait);
ssize_t antenna_v0_port_write(const void *buffer, ssize_t lenght);

int32_t antenna_v0_port_init(void);

#endif /* USING_ANTENNA_V0 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V0_LL_H_ */
