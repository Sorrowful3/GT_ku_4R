/*
 * vehicle_attitude.h
 *
 *  Created on: 2021Äê4ÔÂ26ÈÕ
 *      Author: admin
 */

#ifndef SRC_LIB_UORB_TOPICS_VEHICLE_ATTITUDE_H_
#define SRC_LIB_UORB_TOPICS_VEHICLE_ATTITUDE_H_

#pragma once


#include "uORB.h"

struct vehicle_attitude_s {

	uint64_t timestamp;
	uint64_t timestamp_sample;
	float q[4];
	float delta_q_reset[4];
	float x_rate;
	float y_rate;
	float z_rate;
	uint8_t quat_reset_counter;
	uint8_t _padding0[7]; // required for logger

};



#endif /* SRC_LIB_UORB_TOPICS_VEHICLE_ATTITUDE_H_ */
