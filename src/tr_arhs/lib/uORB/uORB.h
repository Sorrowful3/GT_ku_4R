/*
 * uORB.h
 *
 *  Created on: 2021Äê4ÔÂ21ÈÕ
 *      Author: admin
 */

#ifndef SRC_LIB_UORB_UORB_H_
#define SRC_LIB_UORB_UORB_H_
#include <errno.h>
#include <sys/_stdint.h>

enum class ORB_ID : uint8_t {
	sensor_accel = 1,
	sensor_accel_fifo = 2,
	sensor_gps = 3,
	sensor_gyro = 4,
	sensor_gyro_fft = 5,
	sensor_gyro_fifo = 6,
	vehicle_imu = 7,
	vehicle_attitude = 8,

	INVALID
};

/**
 * ORB topic advertiser handle.
 *
 * Advertiser handles are global; once obtained they can be shared freely
 * and do not need to be closed or released.
 *
 * This permits publication from interrupt context and other contexts where
 * a file-descriptor-based handle would not otherwise be in scope for the
 * publisher.
 */
typedef void 	*orb_advert_t;

extern int uorb_start(void);
extern orb_advert_t orb_advertise(const ORB_ID o_id_t,unsigned int queue_size);
extern int orb_unadvertise(orb_advert_t handle);
extern int  orb_publish( orb_advert_t handle, const void *data);
extern orb_advert_t  orb_subscribe(const ORB_ID orb_id);

#endif /* SRC_LIB_UORB_UORB_H_ */
