/*
 * offset_load.h
 *
 *  Created on: 2022Äê1ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef __OFFSET_LOAD_H_
#define __OFFSET_LOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


int32_t load_offse_data(const char *file_name, uint32_t load_addr);

#ifdef __cplusplus
}
#endif

#endif /* __OFFSET_LOAD_H_ */
