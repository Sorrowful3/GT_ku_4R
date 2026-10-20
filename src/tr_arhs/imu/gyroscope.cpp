/*
 * gyroscope.cpp
 *
 *  Created on: 2021Äê4ÔÂ22ÈÕ
 *      Author: admin
 */
#include "gyroscope.h"


static constexpr int32_t sum(const int16_t samples[16], uint8_t len)
{
	int32_t sum = 0;

	for (int n = 0; n < len; n++) {
		sum += samples[n];
	}

	return sum;
}

Gyroscope::Gyroscope(uint32_t device_id) :
	_device_id{device_id}
{
	// advertise immediately to keep instance numbering in sync
	_sensor_pub.advertise();

}

Gyroscope::~Gyroscope()
{
	_sensor_pub.unadvertise();
	_sensor_fifo_pub.unadvertise();
}
void Gyroscope::gyroscpe_bais(float x, float y, float z)
{
	_gyro_bais_x += x/(_avg_except/10) ;
	_gyro_bais_y += y/(_avg_except/10);
	_gyro_bais_z += z/(_avg_except/10) ;

	_avg_cnt ++;

	if(_avg_cnt>=_avg_except/10)
		_isbais_init = true;

}
void Gyroscope::update(const hrt_abstime &timestamp_sample, float x, float y, float z)
{
	// publish
	if(_isbais_init)
	{
	  //printf("x: %f  y:  %f   z:   %f\r\n",_gyro_bais_x,_gyro_bais_y,_gyro_bais_z);
	  gyro_calibration(x,y,z);
	  Publish(timestamp_sample, x, y, z);
	}
	else
	  gyroscpe_bais(x,y,z);
	  //gyro_calibration( x,  y , z);
}

void Gyroscope::updateFIFO(sensor_gyro_fifo_s &sample)
{
	// publish fifo
	sample.device_id = _device_id;
	sample.scale = _scale;
	sample.timestamp = hrt_absolute_time();
	_sensor_fifo_pub.publish(sample);

	{
		// trapezoidal integration (equally spaced, scaled by dt later)
		const uint8_t N = sample.samples;
		const Vector3f integral{
			(0.5f * (_last_sample[0] + sample.x[N - 1]) + sum(sample.x, N - 1)),
			(0.5f * (_last_sample[1] + sample.y[N - 1]) + sum(sample.y, N - 1)),
			(0.5f * (_last_sample[2] + sample.z[N - 1]) + sum(sample.z, N - 1)),
		};

		_last_sample[0] = sample.x[N - 1];
		_last_sample[1] = sample.y[N - 1];
		_last_sample[2] = sample.z[N - 1];

		const float x = integral(0) / (float)N;
		const float y = integral(1) / (float)N;
		const float z = integral(2) / (float)N;

		// publish
		Publish(sample.timestamp_sample, x, y, z, N);
	}
}

void Gyroscope::Publish(const hrt_abstime &timestamp_sample, float x, float y, float z, uint8_t samples)
{
	sensor_gyro_s report;
	report.timestamp_sample = timestamp_sample;
	report.device_id = _device_id;
	report.temperature = _temperature;
	report.error_count = _error_count;
	report.x = (x-_gyro_bais_x) * _scale;
	report.y = (y-_gyro_bais_y) * _scale;
	report.z = (z-_gyro_bais_z) * _scale;
	report.samples = samples;
	report.timestamp = hrt_absolute_time();
	//printf("x:    %f       y:     %f       z:    %f\r\n",report.x*180.0/3.1415926f, report.y*180.0/3.1415926f, report.z*180.0/3.1415926f);
	_sensor_pub.publish(report);
}

int  Gyroscope::gyro_calibration(float x, float y ,float z)
{
  if(!_calibrateion)
   {
	if(!_init_calib)
	{
	   _init_calib = true;
	   filter = new math::MedianFilter<float, 9> [3];
	}
	filter[0].insert(x - _gyro_bais_x);
	filter[1].insert(y - _gyro_bais_y);
	filter[2].insert(z - _gyro_bais_z);

	offset[0] += (x - _gyro_bais_x)/_avg_except;
	offset[1] += (y - _gyro_bais_y)/_avg_except;
	offset[2] += (z - _gyro_bais_z)/_avg_except;

	_avg_cnt ++;

	if(_avg_cnt>=_avg_except)
	{
		_avg_cnt = 0;

	   /* check offsets using a median filter */
	   float xdiff = filter[0].median() -  offset[0];
	   float ydiff = filter[1].median() -  offset[1];
	   float zdiff = filter[2].median() -  offset[2];
       //printf("%f   %f  %f\r\n",filter[0].median(),filter[1].median(),filter[2].median());
       //printf("%f   %f  %f\r\n",offset[0],offset[1],offset[2]);
	   /* maximum allowable calibration error */
	   static constexpr float maxoff = 80000.0f;

	   if ( fabsf(xdiff) > maxoff || fabsf(ydiff) > maxoff || fabsf(zdiff) > maxoff)
	   {
		   //TR_ERR("calibration error\r\n");
		   offset[0] =0.0f;
		   offset[1] =0.0f;
		   offset[2] =0.0f;
		   _init_calib = false;
		   delete [] filter;

		return false;

	    }
	   else
	   {
		//_gyro_bais_x +=  offset[0];
		//_gyro_bais_y +=  offset[1];
		_gyro_bais_z +=  offset[2];

		_calibrateion =true;
		_isbais_init = true;
	    _init_calib = false;
		offset[0] =0.0f;
		offset[1] =0.0f;
		offset[2] =0.0f;
        delete [] filter;
        TR_INFO("x: %f  y:  %f   z:   %f\r\n",_gyro_bais_x,_gyro_bais_y,_gyro_bais_z);
	   }
	}
   }
	/*
	else
	{
	  _calibrateion =true ;

	  filter = new math::MedianFilter<float, 9> [3];

	}*/
    return true;
}
