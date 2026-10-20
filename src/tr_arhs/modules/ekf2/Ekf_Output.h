/*
 * Ekf_Output.hpp
 *
 *  Created on: 2021��4��26��
 *      Author: admin
 */

#ifndef SRC_MODULES_EKF2_EKF_OUTPUT_HPP_
#define SRC_MODULES_EKF2_EKF_OUTPUT_HPP_

#include <float.h>

#include <../ecl_c/EKF/ekf.h>
#include <../mathlib/mathlib.h>
#include <xuartps.h>
#include "hrt.h"
#include "Subscription.hpp"

#include "topics/vehicle_attitude.h"

#include "freertos.h"
#include "task.h"

#include "../../adis16465_attitude.h"

class EKF_OutPut
{
public:
	EKF_OutPut();
    ~EKF_OutPut();

    void Attitude_Output();
    void Init();

    static EKF_OutPut *ekf_output_inst;
    static XUartPs  *XUartPs_Out;
private:
	uORB::Subscription<vehicle_attitude_s>   _tr_attitude_sub;
#pragma pack(push, 1)
	struct AHRS_Output
	{
	  uint8_t   head_L;
	  uint8_t   head_H;
	  uint8_t   pitch_L;
	  uint8_t   pitch_H;
	  uint8_t   roll_L;
	  uint8_t   roll_H;
	  uint8_t   yaw_L;
	  uint8_t   yaw_H;
	  uint8_t   x_rate_L;
	  uint8_t   x_rate_H;
	  uint8_t   y_rate_L;
	  uint8_t   y_rate_H;
	  uint8_t   z_rate_L;
	  uint8_t   z_rate_H;
	  uint8_t   cheak;
	};
#pragma pack(pop)

};

extern "C" void Vehivle_EKF_OUT(void *arg);
extern "C" void Vehivle_EKF_read(adis16465_attitude_t *attitude);

#endif /* SRC_MODULES_EKF2_EKF_OUTPUT_HPP_ */
