
#pragma once


#include "uORB.h"


struct  sensor_accel_fifo_s {

	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t device_id;
	float dt;
	float scale;
	int16_t x[32];
	int16_t y[32];
	int16_t z[32];
	uint8_t samples;
};
