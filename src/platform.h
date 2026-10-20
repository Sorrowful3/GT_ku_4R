/*
 * platform.h
 *
 *  Created on: 2021年3月1日
 *      Author: Administrator
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "./driver/driver.h"
#include "./misc/device_monitor.h"
#include "./misc/fan_control.h"
#include "./satmov/satmov_port.h"
#include "./user-interface/mavlink_top.h"
#include "./tr_arhs/adis16465_port.h"


int32_t platform_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _PLATFORM_H_ */
