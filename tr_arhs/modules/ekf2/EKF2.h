#pragma once

#include <float.h>

#include <../../lib/ecl_c/EKF/ekf.h>
#include <../mathlib/mathlib.h>
#include "hrt.h"
#include "Subscription.hpp"
#include "Publication.hpp"

#include "topics/sensor_accel.h"
#include "topics/sensor_gyro.h"
#include "topics/vehicle_attitude.h"

#include "freertos.h"
#include "task.h"

struct ekf2_timestamps_s {

	uint64_t timestamp;
	int16_t airspeed_timestamp_rel;
	int16_t distance_sensor_timestamp_rel;
	int16_t optical_flow_timestamp_rel;
	int16_t vehicle_air_data_timestamp_rel;
	int16_t vehicle_magnetometer_timestamp_rel;
	int16_t visual_odometry_timestamp_rel;
	uint8_t _padding0[4]; // required for logger

	static constexpr int16_t RELATIVE_TIMESTAMP_INVALID = 32767;
};

struct vehicle_gps_position_s {
	uint64_t timestamp;
	uint64_t time_utc_usec;
	int32_t lat;
	int32_t lon;
	int32_t alt;
	int32_t alt_ellipsoid;
	float s_variance_m_s;
	float c_variance_rad;
	float eph;
	float epv;
	float hdop;
	float vdop;
	int32_t noise_per_ms;
	int32_t jamming_indicator;
	float vel_m_s;
	float vel_n_m_s;
	float vel_e_m_s;
	float vel_d_m_s;
	float cog_rad;
	int32_t timestamp_time_relative;
	float heading;
	float heading_offset;
	uint8_t fix_type;
	uint8_t jamming_state;
	bool vel_ned_valid;
	uint8_t satellites_used;
	uint8_t selected;
	uint8_t _padding0[3]; // required for logger

};

struct vehicle_imu_s {
	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t accel_device_id;
	uint32_t gyro_device_id;
	float delta_angle[3];
	float delta_velocity[3];
	uint32_t delta_angle_dt;
	uint32_t delta_velocity_dt;
	uint8_t delta_velocity_clipping;
	uint8_t calibration_count;
	uint8_t _padding0[2]; // required for logger
	static constexpr uint8_t CLIPPING_X = 1;
	static constexpr uint8_t CLIPPING_Y = 2;
	static constexpr uint8_t CLIPPING_Z = 4;
};

class EKF2
{
public:
	EKF2();

   // ~EKF2();
	/** @see ModuleBase */
	static int task_spawn();

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	int print_status();

	int Imu_updated(vehicle_imu_s &imu);
private:

	void Run();

	void PublishAttitude(const hrt_abstime &timestamp);
	void UpdateGpsSample(ekf2_timestamps_s &ekf2_timestamps);

	static EKF2 *ekf2_inst;

	// time slip monitoring
	uint64_t  _hrt_imu_last=0;
	uint64_t _integrated_time_us = 0;	///< integral of gyro delta time from start (uSec)
	uint64_t _start_time_us = 0;		///< system time at EKF start (uSec)
	int64_t _last_time_slip_us = 0;		///< Last time slip (uSec)

	uint64_t _gps_time_usec{0};
	int32_t _gps_alttitude_ellipsoid{0};			///< altitude in 1E-3 meters (millimeters) above ellipsoid

	uORB::Subscription<sensor_accel_s>   _tr_accel_sub;
	uORB::Subscription<sensor_gyro_s>    _tr_gyro_sub;

	uORB::Publication<vehicle_attitude_s> _attitude_pub{ORB_ID::vehicle_attitude};

	Ekf _ekf;
};

extern "C"  void ekf2_main(void *arg);

