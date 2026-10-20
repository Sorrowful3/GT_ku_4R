/*
 * crt.h
 *
 *  Created on: 2021Äê4ÔÂ23ÈÕ
 *      Author: admin
 */

#ifndef SRC_PALNFORM_FREERTOS_COMMON_CRT_H_
#define SRC_PALNFORM_FREERTOS_COMMON_CRT_H_

#include "stddef.h"
#include "FreeRTOS.h"

void *operator new(size_t size);
void *operator new[](size_t size);
void  operator delete(void *ptr);
void  operator delete(void *ptr);

#endif /* SRC_PALNFORM_FREERTOS_COMMON_CRT_H_ */
