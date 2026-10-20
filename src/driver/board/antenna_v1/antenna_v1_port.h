/*
 * antenna_v1_port.h
 *
 *  Created on: 2021年9月5日
 *      Author: Administrator
 */

#ifndef __ANTENNA_V1_PORT_H_
#define __ANTENNA_V1_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/core_include.h"
#include "../../driver_config.h"

#ifdef USING_ANTENNA_V1

void antenna_v1_port_write_start(void);
void antenna_v1_port_write_end(void);

ssize_t antenna_v1_port_read(void *buffer, ssize_t lenght, TickType_t xTicksToWait);
ssize_t antenna_v1_port_write(const void *buffer, ssize_t lenght);

int32_t antenna_v1_port_init(void);

#endif /* USING_ANTENNA_V1 */

#ifdef __cplusplus
}
#endif

#endif /* __ANTENNA_V1_0_PORT_H_ */
