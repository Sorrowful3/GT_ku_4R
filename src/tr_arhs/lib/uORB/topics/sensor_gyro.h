
#pragma once


#include "uORB.h"


struct  sensor_gyro_s {

	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t device_id;
	float x;
	float y;
	float z;
	float temperature;
	uint32_t error_count;
	uint8_t samples;
};
