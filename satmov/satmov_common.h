/*
 * satmov_common.h
 *
 *  Created on: 2020骞�10鏈�29鏃�
 *	  Author: Administrator
 */

#ifndef _SATMOV_COMMON_H_
#define _SATMOV_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "../driver/driver.h"



#define RADIAN_2_ANGLE(radian)  ((radian) * (180.0f / PI))
#define ANGLE_2_RADIAN(angle)   ((angle) / (180.0f / PI))

typedef struct{
    float       pitch;      //俯仰角，弧度
    float       roll;       //横滚角，弧度
    float       yaw;        //航向角，弧度

    float       rate_x;     //x轴角速度，弧度/s
    float       rate_y;     //y轴角速度，弧度/s
    float       rate_z;     //z轴角速度，弧度/s
    uint8_t     state;
}satmov_imu_t;

typedef struct{
    uint32_t    state;      //GPS状态
    float       longitude;  //经度，弧度
    float       latitude;   //纬度，弧度
    float       height;     //高度，m
    uint32_t    week;
     uint32_t    second;
}satmov_gps_t;

typedef struct{
    float   azimuth;        //方位角，弧度
    float   elevation;      //俯仰角，弧度
    float   polarity;
}satmov_point_t;

//地理指向、极化转载体指向、极化
//只计算了satmov_point_t中的azimuth和elevation
int32_t earth_2_body_point(const satmov_point_t *earth_point, const satmov_imu_t *imu,satmov_point_t *body_point);
int32_t earth_3_body_point(const satmov_point_t *earth_point, const satmov_imu_t *imu, float *best_yaw,satmov_point_t *body_point);

/**********************************************************************************************/

int32_t track_earth_2_body_point(const satmov_point_t *earth_point, const satmov_imu_t *imu, satmov_point_t *body_point);

/*********************************************************************************************/
//只计算了satmov_point_t中的polarity
int32_t earth_2_body_polarity(const satmov_point_t *earth_point, const satmov_imu_t *imu, satmov_point_t *body_polarity);

int32_t earth_3_body_polarity(const satmov_point_t *earth_point, const satmov_imu_t *imu, float *best_yaw,satmov_point_t *body_polarity);


//计算圆锥上某一点的指向
void calculate_conical_point(const satmov_point_t *centre_point, float deviate, float phase, satmov_point_t *conical_point);

//惯导姿态预测
void forecast_gesture(satmov_imu_t *imu, uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _SATMOV_COMMON_H_ */
