
#include "EKF2.h"


//using namespace time_literals;
using math::constrain;
using matrix::Eulerf;
using matrix::Quatf;
using matrix::Vector3f;

EKF2::EKF2():_tr_accel_sub(ORB_ID::sensor_accel),
		     _tr_gyro_sub(ORB_ID::sensor_gyro)
{
   //advertise immediately to keep instance numbering in sync
   _attitude_pub.advertise();
}

int EKF2::print_status()
{

	return 0;
}

int EKF2::Imu_updated(vehicle_imu_s &imu)
{
  sensor_accel_s	acc_ekf_s;
  sensor_gyro_s     gryo_ekf_s;

  _tr_accel_sub.updated();
  _tr_accel_sub.copy(&acc_ekf_s);
  _tr_gyro_sub.updated();
  _tr_gyro_sub.copy(&gryo_ekf_s);

  hrt_abstime _hrt_imu_temp = hrt_absolute_time();
  imu.timestamp = acc_ekf_s.timestamp;
  imu.timestamp_sample = (acc_ekf_s.timestamp_sample+gryo_ekf_s.timestamp_sample)/2UL;
  imu.accel_device_id = acc_ekf_s.device_id;
  imu.gyro_device_id =  gryo_ekf_s.device_id;
  imu.delta_angle_dt =  (uint32_t)math::min(_hrt_imu_temp - _hrt_imu_last,(uint64_t)20000);
  imu.delta_velocity_dt = (uint32_t)math::min(_hrt_imu_temp - _hrt_imu_last,(uint64_t)20000);
  imu.delta_angle[0] =  gryo_ekf_s.x*imu.delta_angle_dt*1.e-6f;
  imu.delta_angle[1] =  gryo_ekf_s.y*imu.delta_angle_dt*1.e-6f;
  imu.delta_angle[2] =  gryo_ekf_s.z*imu.delta_angle_dt*1.e-6f;

  imu.delta_velocity[0] =  acc_ekf_s.x*imu.delta_angle_dt*1.e-6f;
  imu.delta_velocity[1] =  acc_ekf_s.y*imu.delta_angle_dt*1.e-6f;
  imu.delta_velocity[2] =  acc_ekf_s.z*imu.delta_angle_dt*1.e-6f;

  _hrt_imu_last = _hrt_imu_temp;

  return true;
}

void EKF2::Run()
{
	bool imu_updated = false;
	imuSample imu_sample_new {};
	hrt_abstime imu_dt = 0; // for tracking time slip later
	vehicle_imu_s imu={0};

    imu_updated = Imu_updated(imu);

	imu_sample_new.time_us = imu.timestamp_sample;
	imu_sample_new.delta_ang_dt = imu.delta_angle_dt * 1.e-6f;
	imu_sample_new.delta_ang = Vector3f{imu.delta_angle};
	imu_sample_new.delta_vel_dt = imu.delta_velocity_dt * 1.e-6f;
	imu_sample_new.delta_vel = Vector3f{imu.delta_velocity};

	if (imu.delta_velocity_clipping > 0) {
		imu_sample_new.delta_vel_clipping[0] = imu.delta_velocity_clipping & vehicle_imu_s::CLIPPING_X;
		imu_sample_new.delta_vel_clipping[1] = imu.delta_velocity_clipping & vehicle_imu_s::CLIPPING_Y;
		imu_sample_new.delta_vel_clipping[2] = imu.delta_velocity_clipping & vehicle_imu_s::CLIPPING_Z;
	}

	imu_dt = imu.delta_angle_dt;

	if (imu_updated) {

		const hrt_abstime now = imu_sample_new.time_us;

		// push imu data into estimator
		_ekf.setIMUData(imu_sample_new);

		PublishAttitude(now); // publish attitude immediately (uses quaternion from output predictor)

		// integrate time to monitor time slippage
		if (_start_time_us > 0) {
			_integrated_time_us += imu_dt;
			_last_time_slip_us = (imu_sample_new.time_us - _start_time_us) - _integrated_time_us;

		} else {
			_start_time_us = imu_sample_new.time_us;
			_last_time_slip_us = 0;
		}

		// ekf2_timestamps (using 0.1 ms relative timestamps)
		ekf2_timestamps_s ekf2_timestamps {
			now,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			ekf2_timestamps_s::RELATIVE_TIMESTAMP_INVALID,
			0
		};

		UpdateGpsSample(ekf2_timestamps);

		if (_ekf.update()) {

			//PublishLocalPosition(now);
			//PublishGlobalPosition(now);
			//PublishSensorBias(now);
		}
		// publish ekf2_timestamps
		//_ekf2_timestamps_pub.publish(ekf2_timestamps);
	}
}


void EKF2::UpdateGpsSample(ekf2_timestamps_s &ekf2_timestamps)
{
	// EKF GPS message
	    vehicle_gps_position_s vehicle_gps_position={0};

   //  if (_vehicle_gps_position_sub.update(&vehicle_gps_position)) {
			gps_message gps_msg  {
				vehicle_gps_position.timestamp,
				vehicle_gps_position.lat,
			    vehicle_gps_position.lon,
				vehicle_gps_position.alt,
				vehicle_gps_position.heading,
				vehicle_gps_position.heading_offset,
				vehicle_gps_position.fix_type,
				vehicle_gps_position.eph,
				vehicle_gps_position.epv,
				vehicle_gps_position.s_variance_m_s,
				vehicle_gps_position.vel_m_s,
				Vector3f{
					vehicle_gps_position.vel_n_m_s,
					vehicle_gps_position.vel_e_m_s,
					vehicle_gps_position.vel_d_m_s
				},
				vehicle_gps_position.vel_ned_valid,
				vehicle_gps_position.satellites_used,
				sqrtf(vehicle_gps_position.hdop *vehicle_gps_position.hdop
					      + vehicle_gps_position.vdop * vehicle_gps_position.vdop),
			};
			_ekf.setGpsData(gps_msg);

			_gps_time_usec = gps_msg.time_usec;
			_gps_alttitude_ellipsoid = vehicle_gps_position.alt_ellipsoid;
		//}

}
void EKF2::PublishAttitude(const hrt_abstime &timestamp)
{
	// generate vehicle attitude quaternion data
	vehicle_attitude_s att;
	att.timestamp_sample = timestamp;
	const Quatf q{_ekf.calculate_quaternion()};
	q.copyTo(att.q);

	 const Vector3f axirate {_ekf.calculate_axirate()};
     att.x_rate = axirate(0);
     att.y_rate = axirate(1);
     att.z_rate = axirate(2);
	 _ekf.get_quat_reset(&att.delta_q_reset[0], &att.quat_reset_counter);
	 att.timestamp =  hrt_absolute_time();
     _attitude_pub.publish(att);

}
int EKF2::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

EKF2 *EKF2::ekf2_inst = nullptr;

int EKF2::task_spawn()
{

  ekf2_inst = new EKF2();
  while(1)
  {
	ekf2_inst->Run();
  }

    return 0;
}

int EKF2::print_usage(const char *reason)
{
	return 0;
}

void  ekf2_main(void *arg)
{

   EKF2::task_spawn();

}
