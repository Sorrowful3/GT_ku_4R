/*
 * gyroscope.hpp
 *
 *  Created on: 2021��4��22��
 *      Author: admin
 */

#ifndef SRC_DRIVERS_GYROSCOPE_GYROSCOPE_HPP_
#define SRC_DRIVERS_GYROSCOPE_GYROSCOPE_HPP_

/****************************************************************************
 *
 *   Copyright (c) 2018-2020 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#pragma once

#include "log.h"
#include "Publication.hpp"
#include "EKF2.h"
#include "topics/sensor_gyro.h"
#include "topics/sensor_gyro_fifo.h"
#include "math/filter/MedianFilter.hpp"
class Gyroscope
{
public:
	Gyroscope(uint32_t device_id);
	~Gyroscope();

	uint32_t get_device_id() const { return _device_id; }

	int32_t get_max_rate_hz() const { return _imu_gyro_rate_max; }

	void set_device_id(uint32_t device_id) { _device_id = device_id; }
	void set_error_count(uint32_t error_count) { _error_count = error_count; }
	void increase_error_count() { _error_count++; }
	void set_range(float range) { _range = range; }
	void set_scale(float scale) { _scale = scale; }
	void set_temperature(float temperature) { _temperature = temperature; }

	void update(const hrt_abstime &timestamp_sample, float x, float y, float z);

	void updateFIFO(sensor_gyro_fifo_s &sample);

private:
	void Publish(const hrt_abstime &timestamp_sample, float x, float y, float z, uint8_t samples = 1);
	void gyroscpe_bais(float x, float y, float z);
    int  gyro_calibration(float x, float y ,float z);
	uORB::Publication<sensor_gyro_s> _sensor_pub{ORB_ID::sensor_gyro};
	uORB::Publication<sensor_gyro_fifo_s>  _sensor_fifo_pub{ORB_ID::sensor_gyro_fifo};

	math::MedianFilter<float, 9> *filter;

	uint32_t		_device_id{0};

	int32_t			_imu_gyro_rate_max{0};

	float			_range{math::radians(2000.f)};
	float			_scale{1.f};
	float			_temperature{NAN};

	uint32_t		_error_count{0};

	int16_t			_last_sample[3] {};

	bool            _isbais_init {false};

	float            offset[3]{0.0f};

	//float           _gyro_bais_x{-12.732611f};
	//float           _gyro_bais_y{-83.139908f};
	//float           _gyro_bais_z{-64.036392f};
	float           _gyro_bais_x{0.0f};
	float           _gyro_bais_y{0.0f};
	float           _gyro_bais_z{0.0f};

	const uint32_t  _avg_except{5000};
	uint32_t        _avg_cnt;

	bool            _calibrateion{false};
	bool            _init_calib;
};




#endif /* SRC_DRIVERS_GYROSCOPE_GYROSCOPE_HPP_ */
