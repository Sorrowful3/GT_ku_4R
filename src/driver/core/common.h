/**
 * @file    common.h
 * @brief   实现了一些常用的公共函数
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "define.h"

#define BIG32_2_LITTLE32(dst, src) byte_swap_4(dst, src)
#define LITTLE32_2_BIG32(dst, src) byte_swap_4(dst, src)
#define BIG16_2_LITTLE16(dst, src) byte_swap_2(dst, src)
#define LITTLE16_2_BIG16(dst, src) byte_swap_2(dst, src)

int32_t device_ffs(uint32_t value);

void byte_swap_4(void *dst, const void *src);
void byte_swap_2(void *dst, const void *src);

uint16_t crc16(const uint8_t *data, uint32_t lenght, uint16_t init_value);

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H_ */
