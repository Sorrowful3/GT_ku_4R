/*
 * adis16465_port.h
 *
 *  Created on: 2022年4月26日
 *      Author: Administrator
 */

#ifndef __ADIS16465_PORT_H_
#define __ADIS16465_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "../driver/driver.h"
#include "adis16465_attitude.h"

extern "C"  int32_t adis16465_imu1_init(const char *adis16465_name);

extern "C"  int32_t adis16465_imu1_get_attitude(adis16465_attitude_t *attitude);

#ifdef __cplusplus
}
#endif

#endif /* __ADIS16465_PORT_H_ */
