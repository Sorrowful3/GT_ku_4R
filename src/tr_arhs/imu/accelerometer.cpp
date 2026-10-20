
#include "accelerometer.h"


static constexpr int32_t sum(const int16_t samples[16], uint8_t len)
{
	int32_t sum = 0;

	for (int n = 0; n < len; n++) {
		sum += samples[n];
	}

	return sum;
}

static constexpr uint8_t clipping(const int16_t samples[16], int16_t clip_limit, uint8_t len)
{
	unsigned clip_count = 0;

	for (int n = 0; n < len; n++) {
		if (abs(samples[n]) >= clip_limit) {
			clip_count++;
		}
	}

	return clip_count;
}

Accelerometer::Accelerometer(uint32_t device_id) :
	_device_id{device_id}
{
	 //advertise immediately to keep instance numbering in sync
	_sensor_pub.advertise();

}

Accelerometer::~Accelerometer()
{
	_sensor_pub.unadvertise();
	_sensor_fifo_pub.unadvertise();
}


void Accelerometer::update(const hrt_abstime &timestamp_sample, float x, float y, float z)
{
	// clipping
	uint8_t clip_count[3];
	clip_count[0] = (fabsf(x) >= _clip_limit);
	clip_count[1] = (fabsf(y) >= _clip_limit);
	clip_count[2] = (fabsf(z) >= _clip_limit);

	// publish
	Publish(timestamp_sample, x, y, z, clip_count);
}

void Accelerometer::updateFIFO(sensor_accel_fifo_s &sample)
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

		// clipping
		uint8_t clip_count[3] {
			clipping(sample.x, _clip_limit, N),
			clipping(sample.y, _clip_limit, N),
			clipping(sample.z, _clip_limit, N),
		};

		const float x = integral(0) / (float)N;
		const float y = integral(1) / (float)N;
		const float z = integral(2) / (float)N;

		// publish
		Publish(sample.timestamp_sample, x, y, z, clip_count, N);
	}
}

void Accelerometer::Publish(const hrt_abstime &timestamp_sample, float x, float y, float z, uint8_t clip_count[3],
			       uint8_t samples)
{
	float clipping_x = clip_count[0];
	float clipping_y = clip_count[1];
	float clipping_z = clip_count[2];

	sensor_accel_s report;

	report.timestamp_sample = timestamp_sample;
	report.device_id = _device_id;
	report.temperature = _temperature;
	report.error_count = _error_count;
	report.x = x * _scale;
	report.y = y * _scale;
	report.z = z * _scale;
	report.clip_counter[0] = fabsf(roundf(clipping_x));
	report.clip_counter[1] = fabsf(roundf(clipping_y));
	report.clip_counter[2] = fabsf(roundf(clipping_z));
	report.samples = samples;
	report.timestamp = hrt_absolute_time();

	//printf("x=%f   y=%f   z=%f  \r\n",report.x,report.y,report.z);
	_sensor_pub.publish(report);
}

void Accelerometer::UpdateClipLimit()
{
	// 99.9% of potential max
	_clip_limit = fmaxf((_range / _scale) * 0.999f, INT16_MAX);
}
