/*
 * ADIS16465.hpp
 *
 *  Created on: 2021��4��13��
 *      Author: admin
 */

#ifndef SRC_DRIVERS_IMU_ADIS16465_HPP_
#define SRC_DRIVERS_IMU_ADIS16465_HPP_

#include "sleep.h"
#include "xtime_l.h"
#include "log.h"
#include "accelerometer.h"
#include "gyroscope.h"

//#include "freertos.h"
#include "task.h"
#include "timers.h"
#include "../../driver/driver.h"

class ADIS16465
{
public:
	ADIS16465(void);
	virtual ~ADIS16465();

	static ADIS16465 *instantiate(void);
		static void print_usage();

		int		        init();

		void			RunImpl();
	protected:
		 int		    probe();
		//void exit_and_cleanup() override;

	private:

		device_t *adis16465;
		Accelerometer	_tr_accel;
		Gyroscope		_tr_gyro;

		//perf_counter_t		_sample_perf;
		//perf_counter_t		_bad_transfers;

	    TimerHandle_t    _AutoReload_Adis16465_Handle;   //ע�����ڶ�ʱ�����ؾ��

		const uint32_t   _drdy_gpio{0};

	#pragma pack(push, 1)
		// Report conversation with in the ADIS16465, including command byte and interrupt status.
		struct ADISReport {
			uint16_t	cmd;
			uint16_t	diag_stat;
			int16_t		gyro_x;
			int16_t		gyro_y;
			int16_t		gyro_z;
			int16_t		accel_x;
			int16_t		accel_y;
			int16_t		accel_z;
			uint16_t	temp;
//			uint16_t	DATA_CNTR;
			uint8_t		checksum;
			uint8_t		_padding; // 16 bit SPI mode
		};
	#pragma pack(pop)

		struct ADISReport32_t {

			int32_t		gyro_x;
			int32_t		gyro_y;
			int32_t		gyro_z;
			int32_t		accel_x;
			int32_t		accel_y;
			int32_t		accel_z;
			uint16_t	temp;
		};
	#pragma pack(pop)

		/**
		 * Start automatic measurement.
		 */
		void		start();

		/**
		 * Reset chip.
		 *
		 * Resets the chip and measurements ranges, but not scale and offset.
		 */
		int		    reset();

		static int	data_ready_interrupt(int irq, void *context, void *arg);

		/**
		 * Fetch measurements from the sensor and update the report buffers.
		 */
		int			measure();

		uint16_t	read_reg16(uint8_t reg);

		int			write_reg(uint8_t reg, uint8_t value);
		void		write_reg16(uint8_t reg, uint16_t value);

		// ADIS16465 onboard self test
		bool 		self_test_memory();
		bool 		self_test_sensor();

};


extern "C" void  ADIS16465_main(void *arg);
extern "C" void ADIS16465_Mesure(TimerHandle_t xTimer);

#endif /* SRC_DRIVERS_IMU_ADIS16465_HPP_ */
