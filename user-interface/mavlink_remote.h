/*
 * mavlink_parse.h
 *
 *  Created on: 2022年1月23日
 *      Author: Administrator
 */

#ifndef __MAVLINK_PARSE_H_
#define __MAVLINK_PARSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver/driver.h"
#include "../misc/device_monitor.h"
#include "../satmov/satmov_port.h"
#include "../tr_arhs/adis16465_port.h"
#include "mavlink_chan.h"
#include "mavlink_top.h"

#define ERUPT_THREAD_NUMBER 5
#define LOG_BUFFER_LENGHT 512

//#define FIRWARE_VERSION  "v2.0.0.220415"
#define PRODUCT_NAME     "MU0055-BA1"
#define PRODUCT_ORB      "12345678"

#define DEFAULT_WORK_PARAMETER              \
{                                           \
    .work_mode = 0x00,      				\
    .rolloff = 0.15f,                       \
    .manual_gps = 0x00,                     \
    .manual_longitude = 104.05f,            \
    .manual_latitude = 30.58f,              \
    .manual_height = 500.0f,                \
}

typedef struct{
    uint32_t beacon_interval;
    uint32_t imu_interval;
    uint32_t antenna_interval;
    uint32_t transducer_interval;
    uint32_t work_interval;
}mavlink_interval_t;

typedef struct {
	uint8_t work_mode;
	float rolloff;
	uint8_t tracking_mode;
	uint8_t manual_gps;
	float manual_longitude;
	float manual_latitude;
	float manual_height;
}work_parameter_t;

int32_t mavlink_read_imu_drift(uint8_t *valid, float *gyro_x, float *gyro_y, float *gyro_z);
int32_t mavlink_write_imu_drift(uint8_t valid, float gyro_x, float gyro_y, float gyro_z);
void mavlink_print(const char *str);
void mavlink_vprint(const char *fmt, va_list args);
void mavlink_beacon_broadcast(const uint8_t *buffer, size_t size);
void mavlink_transducer_broadcast(const uint8_t *buffer, size_t size);
void mavlink_antenna_broadcast(const uint8_t *buffer, size_t size);
void mavlink_currect_yaw_complete(float yaw);

int32_t mavlink_remote_create(mavlink_port_t *chan_ops, uint8_t chan_type, mavlink_interval_t *interval);
int32_t mavlink_remote_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_PARSE_H_ */
