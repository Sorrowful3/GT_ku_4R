/*
 * accelerometer.hpp
 *
 *  Created on: 2021��4��22��
 *      Author: admin
 */

#ifndef SRC_DRIVERS_ACCELEROMETER_ACCELEROMETER_HPP_
#define SRC_DRIVERS_ACCELEROMETER_ACCELEROMETER_HPP_

#pragma once

#include "geo/geo.h"
#include "Publication.hpp"
#include "topics/sensor_accel_fifo.h"
#include "topics/sensor_accel.h"
#include "EKF2.h"

class Accelerometer
{
public:
	Accelerometer(uint32_t device_id);
	~Accelerometer();

	uint32_t get_device_id() const { return _device_id; }

	int32_t get_max_rate_hz() const { return _imu_gyro_rate_max; }

	void set_device_id(uint32_t device_id) { _device_id = device_id; }
	void set_error_count(uint32_t error_count) { _error_count = error_count; }
	void increase_error_count() { _error_count++; }
	void set_range(float range) { _range = range; UpdateClipLimit(); }
	void set_scale(float scale) { _scale = scale; UpdateClipLimit(); }
	void set_temperature(float temperature) { _temperature = temperature; }

	void update(const hrt_abstime &timestamp_sample, float x, float y, float z);

	void updateFIFO(sensor_accel_fifo_s &sample);

private:
	void Publish(const hrt_abstime &timestamp_sample, float x, float y, float z, uint8_t clip_count[3],
		     uint8_t samples = 1);
	void UpdateClipLimit();

	uORB::Publication<sensor_accel_s> _sensor_pub{ORB_ID::sensor_accel};
	uORB::Publication<sensor_accel_fifo_s>  _sensor_fifo_pub{ORB_ID::sensor_accel_fifo};

	uint32_t		_device_id{0};
	int32_t			_imu_gyro_rate_max{0}; // match gyro max rate

	float			_range{16 * CONSTANTS_ONE_G};
	float			_scale{1.f};
	float			_temperature{NAN};

	float			_clip_limit{_range / _scale};

	uint32_t		_error_count{0};

	int16_t			_last_sample[3] {};
};




#endif /* SRC_DRIVERS_ACCELEROMETER_ACCELEROMETER_HPP_ */
