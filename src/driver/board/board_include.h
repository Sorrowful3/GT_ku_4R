/*
 * board_include.h
 *
 *  Created on: 2021年5月20日
 *      Author: Administrator
 */

#ifndef __BOARD_INCLUDE_H_
#define __BOARD_INCLUDE_H_

#include "../driver_config.h"

#ifdef USING_ANTENNA_HAL
#include "antenna_hal.h"
#endif
#ifdef USING_BEACON
#include "beacon.h"
#endif
#ifdef USING_INA220
#include "ina220.h"
#endif
#ifdef USING_INA226
#include "ina226.h"
#endif
#include "power.h"
#ifdef USING_RAV260
#include "rav260.h"
#endif
#ifdef USING_TMP112
#include "tmp112.h"
#endif
#ifdef USING_TRANSDUCER
#include "transducer.h"
#endif
#ifdef USING_ADIS16465
#include "adis16465.h"
#endif
#ifdef USING_NOE_M8
#include "noe_m8.h"
#endif

#endif /* __BOARD_INCLUDE_H_ */
