/*
+ * aADIS16465.cpp
 *
 *  Created on: 2021��4��13��
 *      Author: admin
 */
#include "ADIS16465.h"
#include <iostream>
#include "xgpiops.h"
#define DIR_READ				0x00
#define DIR_WRITE				0x80

////  ADIS16465 registers
//static constexpr uint8_t DIAG_STAT	= 0x02; // Output, system error flags
//static constexpr uint8_t FILT_CTRL	= 0x5C;
//static constexpr uint8_t MSC_CTRL	= 0x60;
//static constexpr uint8_t DEC_RATE	= 0x64;
//static constexpr uint8_t GLOB_CMD	= 0x68;
//static constexpr uint8_t PROD_ID	= 0x72;

static constexpr uint8_t IMU_ADDR[13] =
{
  0x04,0x06,  //X_GYRO_LOW  X_GYRO_OUT
  0x08,0x0A,  //Y_GYRO_LOW  Y_GYRO_OUT
  0x0c,0x0e,  //Z_GYRO_LOW  Z_GYRO_OUT
  0x10,0x12,  //X_ACCL_LOW  X_ACCL_OUT
  0x14,0x16,  //Y_ACCL_LOW  Y_ACCL_OUT
  0x18,0x1A,  //Z_ACCL_LOW  Z_ACCL_OUT
  0x1c        //TEMP_OUT
};

static constexpr uint16_t PROD_ID_ADIS16465 = 0x4051;	// ADIS16465 Identification, device number

// Stall time between SPI transfers
static constexpr uint8_t T_STALL = 16;

static constexpr uint32_t ADIS16465_DEFAULT_RATE = 1000;


ADIS16465::ADIS16465(void):
		   _tr_accel(1),
		   _tr_gyro(1)

{
	_tr_accel.set_scale(0.25f/65536.0f * CONSTANTS_ONE_G / 1000.0f); // accel 0.25 mg/LSB
	_tr_gyro.set_scale(math::radians(1.0f/2621440.0f)); // gyro 0.025 ��/sec/LSB

}

ADIS16465::~ADIS16465()
{
	// delete the perf counters

}

int
ADIS16465::init()
{
	start();
 return 0;
}

int ADIS16465::reset()
{
	return 0;
}

int
ADIS16465::probe()
{
	return 0;
}

bool
ADIS16465::self_test_memory()
{
	return true;
}

bool
ADIS16465::self_test_sensor()
{
	return true;
}

uint16_t
ADIS16465::read_reg16(uint8_t reg)
{
	return 0;
}

int
ADIS16465::write_reg(uint8_t reg, uint8_t val)
{
	return 0;
}

void
ADIS16465::write_reg16(uint8_t reg, uint16_t value)
{
}

void
ADIS16465::start()
{
    adis16465 = device_open("adis16465_0", DEVICE_RDWR);
    if(!adis16465) {
        return;
    }

    _AutoReload_Adis16465_Handle =xTimerCreate((const char *          )"ADIS16465MeasureAutoload",
                                              (TickType_t             )pdMS_TO_TICKS(2),
                                              (UBaseType_t            )pdTRUE,
                                              (void *                 )this,
                                              (TimerCallbackFunction_t)ADIS16465_Mesure);
    xTimerStart(_AutoReload_Adis16465_Handle,0);
}



int
ADIS16465::data_ready_interrupt(int irq, void *context, void *arg)
{

	return 0;
}

void
ADIS16465::RunImpl()
{
	// make another measurement
	measure();
}

int
ADIS16465::measure()
{
//    ssize_t lenght;
//    ADISReport32_t adis_report32_t{0};
//    adis16465_original_t adis16465_original;
//    XTime timestamp_sample = hrt_absolute_time();
//
//    lenght = device_read(adis16465, &adis16465_original, sizeof(adis16465_original));
//    if(lenght < 0) {
//        return -EIO;
//    }
//
//	adis_report32_t.gyro_x = adis16465_original.gyro_x;
//	adis_report32_t.gyro_y = adis16465_original.gyro_y;
//	adis_report32_t.gyro_z = adis16465_original.gyro_z;
//
//	adis_report32_t.accel_x = adis16465_original.acc_x;
//	adis_report32_t.accel_y = adis16465_original.acc_y;
//	adis_report32_t.accel_z = adis16465_original.acc_z;
//
//	adis_report32_t.temp = adis16465_original.temp;
//
//
//	// temperature 1 LSB = 0.1��C
//	const float temperature = adis_report32_t.temp * 0.1f;
//
//	_tr_accel.set_temperature(temperature);
//	_tr_gyro.set_temperature(temperature);
//    //printf("x:    %d       y:     %d       z:    %d\r\n",adis_report.accel_x, adis_report.accel_y, adis_report.accel_z);
//	_tr_accel.update(timestamp_sample, adis_report32_t.accel_x, adis_report32_t.accel_y, adis_report32_t.accel_z);
//	_tr_gyro.update(timestamp_sample, adis_report32_t.gyro_x, adis_report32_t.gyro_y, adis_report32_t.gyro_z);
//
//	return 0;
}


