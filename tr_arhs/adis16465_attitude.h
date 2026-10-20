/*
 * adis16465_attitude.h
 *
 *  Created on: 2022年4月26日
 *      Author: Administrator
 */

#ifndef __ADIS16465_ATTITUDE_H_
#define __ADIS16465_ATTITUDE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    struct {
        float x;
        float y;
        float z;
    };
    float axis[3];
}axis3_t;

typedef struct{
    float roll;
    float pitch;
    float yaw;
}euler_angle_t;

typedef struct{
    axis3_t gyro;
    euler_angle_t euler_angle;
}adis16465_attitude_t;

#ifdef __cplusplus
}
#endif

#endif /* __ADIS16465_ATTITUDE_H_ */
