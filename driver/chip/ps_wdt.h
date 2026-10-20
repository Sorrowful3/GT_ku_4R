/*
 * ps_wdt.h
 *
 *  Created on: 2020年12月14日
 *      Author: Administrator
 */

#ifndef _PS_WDT_H_
#define _PS_WDT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_PS_WDT

#include "xparameters.h"
#include "xscuwdt.h"


#define WDT_LOAD_VALUE		0xFFFF0000

int32_t ps_wdt_init(void);
int32_t ps_wdt_start(void);

#endif /* USING_PS_WDT */

#ifdef __cplusplus
}
#endif

#endif /* __WDT_H_ */
