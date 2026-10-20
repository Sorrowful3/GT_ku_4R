/*
 * mavlink_top.h
 *
 *  Created on: 2022年1月23日
 *      Author: Administrator
 */

#ifndef __MAVLINK_TOP_H_
#define __MAVLINK_TOP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver/driver.h"
#include "mavlink_chan.h"
#include "mavlink_remote.h"

typedef struct stream_channel{
    char       *device_name;
    device_t   *device;
    TickType_t timeout;
}stream_channel_t;

int32_t mavlink_top_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_TOP_H_ */
