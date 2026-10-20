/*
 * satmov_common.c
 *
 *  Created on: 2020年10月29日
 *	  Author: Administrator
 */

#include "satmov_common.h"

#define DEBUG_TAG "satmov_common,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../driver/core/debug.h"

float angle_mod(float angle)
{
    angle = fmodf(angle, 360.0f);
    if(angle < 0.0f) {
        return angle + 360.0f;
    }
    else {
        return angle;
    }
}

float radian_mod(float radian)
{
    radian = fmodf(radian, 2.0f*PI);
    if(radian < 0.0f) {
        return radian + 2.0f*PI;
    }
    else {
        return radian;
    }
}


int32_t earth_2_body_point(const satmov_point_t *earth_point,const satmov_imu_t *imu,satmov_point_t *body_point)
{
//	memcpy(body_point,earth_point,sizeof(satmov_point_t));
	float E = 0,A = 0,H = 0,R = 0,P = 0;
	float Aj_FZ = 0,Aj_FM = 0,Aj_Rad = 0,Ej_Rad = 0,Aj,Ej = 0;
	float cos_a=0,sin_a=0,cos_e=0,sin_e=0,cos_r=0,sin_r=0,cos_y=0,sin_y=0,cos_p=0,sin_p=0,K0=0,K1=0,K2=0;

//	A = ANGLE_2_RADIAN(earth_point->azimuth);
//	E = ANGLE_2_RADIAN(earth_point->elevation);
	A = earth_point->azimuth;
	E = 1.570796-earth_point->elevation;
	H = imu->yaw;
	H=H+PI;
	if(H>2.0f*PI){
	H=H-2.0f*PI;
	}
	R = imu->roll;
	P = imu->pitch;

//	LOG_E("A=%2.f,E=%2.f,yaw=%2.f,roll=%2.f,pitch=%2.f\n",RADIAN_2_ANGLE(earth_point->azimuth),RADIAN_2_ANGLE(earth_point->elevation),
//			RADIAN_2_ANGLE(imu->yaw),RADIAN_2_ANGLE(imu->roll),RADIAN_2_ANGLE(imu->pitch));


	//Aj_FZ = cosf(E) * ( cosf(R) * sinf(A - H) + sinf(R) * sinf(P) * cosf(A-H)) - sinf(R)* cosf(P)* sinf(E);
	//Aj_FM = cosf(P) * cosf(E) * cosf(A - H) + sinf(P) * sinf(E);
	//Aj_Rad = atan2f(Aj_FZ, Aj_FM);
	//Ej_Rad = asinf(sinf(R)* cosf(E)* sinf(A-H) - cosf( R)* sinf(P)*cosf( E)* cosf(A-H)+ cosf(R)* cosf(P)* sinf(E));
	//Aj = 180 * Aj_Rad / PI;

	cos_a=cosf(A);sin_a=sinf(A);

//	cos_e=cosf(E);sin_e=sinf(E);
	cos_e=cosf(-E);sin_e=sinf(-E);
	cos_r=cosf(R);sin_r=sinf(R);
	cos_y=cosf(H);sin_y=sinf(H);
	cos_p=cosf(P);sin_p=sinf(P);
	K0=cos_a*cos_e*cos_p*cos_y-sin_e*sin_p+cos_e*cos_p*sin_a*sin_y;
	K1=cos_e*sin_a*(cos_r*cos_y+sin_p*sin_r*sin_y)-cos_a*cos_e*(cos_r*sin_y-cos_y*sin_p*sin_r)+cos_p*sin_e*sin_r;
	K2=cos_a*cos_e*(sin_r*sin_y+cos_r*cos_y*sin_p)-cos_e*sin_a*(cos_y*sin_r-cos_r*sin_p*sin_y)+cos_p*cos_r*sin_e;
	Aj_Rad = (-atan2f(K1, K0))-(PI/2.0);
	Ej_Rad = asinf(K2)+(PI/2.0);
	Aj = 180 * Aj_Rad / PI;
	Ej = 180 * Ej_Rad / PI;

	if(Ej >= 80)
	{   /*Ej是俯仰角，如果是负的，就在方位+180读，如果方位大于360，方位对360取余*/
//		Ej =80.0f;
		Ej =180.0f-Ej;
	}
	Aj = angle_mod(Aj);


	body_point->azimuth = ANGLE_2_RADIAN(Aj);
	body_point->elevation = ANGLE_2_RADIAN(Ej);
	body_point->polarity = 0;

////	LOG_E("Aj=%.2f,Ej=%.2f\n",Aj,Ej);
//	LOG_I("Aj=%.2f,Ej=%.2f\n",Aj,body_point->elevation);
    return ERR_OK;
}

//    lutao modifiy 2022-- 4R is error
int32_t earth_3_body_point(const satmov_point_t *earth_point,const satmov_imu_t *imu,float *best_yaw,satmov_point_t *body_point)
{
//	memcpy(body_point,earth_point,sizeof(satmov_point_t));
	float E = 0,A = 0,H = 0,R = 0,P = 0;
	float Aj_FZ = 0,Aj_FM = 0,Aj_Rad = 0,Ej_Rad = 0,Aj,Ej = 0;
	float cos_a=0,sin_a=0,cos_e=0,sin_e=0,cos_r=0,sin_r=0,cos_y=0,sin_y=0,cos_p=0,sin_p=0,K0=0,K1=0,K2=0;

//	A = ANGLE_2_RADIAN(earth_point->azimuth);
//	E = ANGLE_2_RADIAN(earth_point->elevation);
	A = earth_point->azimuth;
	E = earth_point->elevation;//    lutao modifiy 2022-- 4R is error
	//E = earth_point->elevation;//    lutao modifiy 2022-- 4R is error
	H = *best_yaw;//imu->yaw;    //    lutao modifiy 2022-- 4R is error
	H=H+PI;
		if(H>2.0f*PI){
		H=H-2.0f*PI;
		}



	R = imu->roll;
	P = imu->pitch;

//	LOG_E("A=%2.f,E=%2.f,yaw=%2.f,roll=%2.f,pitch=%2.f\n",RADIAN_2_ANGLE(earth_point->azimuth),RADIAN_2_ANGLE(earth_point->elevation),
//			RADIAN_2_ANGLE(imu->yaw),RADIAN_2_ANGLE(imu->roll),RADIAN_2_ANGLE(imu->pitch));


	//Aj_FZ = cosf(E) * ( cosf(R) * sinf(A - H) + sinf(R) * sinf(P) * cosf(A-H)) - sinf(R)* cosf(P)* sinf(E);
	//Aj_FM = cosf(P) * cosf(E) * cosf(A - H) + sinf(P) * sinf(E);
	//Aj_Rad = atan2f(Aj_FZ, Aj_FM);
	//Ej_Rad = asinf(sinf(R)* cosf(E)* sinf(A-H) - cosf( R)* sinf(P)*cosf( E)* cosf(A-H)+ cosf(R)* cosf(P)* sinf(E));
	//Aj = 180 * Aj_Rad / PI;

	cos_a=cosf(A);sin_a=sinf(A);

//	cos_e=cosf(E);sin_e=sinf(E);
	cos_e=cosf(-E);sin_e=sinf(-E);
	cos_r=cosf(R);sin_r=sinf(R);
	cos_y=cosf(H);sin_y=sinf(H);
	cos_p=cosf(P);sin_p=sinf(P);
	K0=cos_a*cos_e*cos_p*cos_y-sin_e*sin_p+cos_e*cos_p*sin_a*sin_y;
	K1=cos_e*sin_a*(cos_r*cos_y+sin_p*sin_r*sin_y)-cos_a*cos_e*(cos_r*sin_y-cos_y*sin_p*sin_r)+cos_p*sin_e*sin_r;
	K2=cos_a*cos_e*(sin_r*sin_y+cos_r*cos_y*sin_p)-cos_e*sin_a*(cos_y*sin_r-cos_r*sin_p*sin_y)+cos_p*cos_r*sin_e;
	Aj_Rad = (-atan2f(K1, K0))-(PI/2.0);
	Ej_Rad = asinf(K2)+(PI/2.0);
	Aj = 180 * Aj_Rad / PI;
	Ej = 180 * Ej_Rad / PI;

	if(Ej >= 80)
	{   /*Ej是俯仰角，如果是负的，就在方位+180读，如果方位大于360，方位对360取余*/
//		Ej =80.0f;
		Ej =180.0f-Ej;
	}
	Aj = angle_mod(Aj);
//	if (Aj < 0)
//		Aj += 360;

//
//	if(Ej < 0)
//	{   /*Ej是俯仰角，如果是负的，就在方位+180读，如果方位大于360，方位对360取余*/
//		Ej = -Ej;
//		Ej = (90 - Ej);
//		Aj += 180;
//		if(Aj >= 360.0f)
//		{
//			Aj = angle_mod(Aj);
//		}
//	}else{
//		/*Ej是俯仰角，是正的，转化为离轴角*/
//		Ej = (90 - Ej);
//	}
//
//	if(Ej >= 80)
//	{   /*Ej是俯仰角，如果是负的，就在方位+180读，如果方位大于360，方位对360取余*/
//		Ej =80.0f;
//	}


	body_point->azimuth = ANGLE_2_RADIAN(Aj);
	body_point->elevation = ANGLE_2_RADIAN(Ej);
	body_point->polarity = 0;






//	LOG_E("Aj=%.2f,Ej=%.2f\n",Aj,Ej);
	LOG_I("Aj=%.2f,Ej=%.2f\n",Aj,body_point->elevation);
    return ERR_OK;
}


/********************************************************************************************/
//int32_t track_earth_2_body_point(const satmov_point_t *earth_point,const satmov_imu_t *imu,satmov_point_t *body_point)
//{
////	memcpy(body_point,earth_point,sizeof(satmov_point_t));
//	float E = 0,A = 0,H = 0,R = 0,P = 0;
//	float Aj_FZ = 0,Aj_FM = 0,Aj_Rad = 0,Ej_Rad = 0,Aj,Ej = 0;
//	float cos_a=0,sin_a=0,cos_e=0,sin_e=0,cos_r=0,sin_r=0,cos_y=0,sin_y=0,cos_p=0,sin_p=0,K0=0,K1=0,K2=0;
//
////	A = ANGLE_2_RADIAN(earth_point->azimuth);
////	E = ANGLE_2_RADIAN(earth_point->elevation);
//	A = earth_point->azimuth;
//	E = earth_point->elevation;
//	H = imu->yaw;
//	R = imu->roll;
//	P = imu->pitch;
//
//	LOG_I("A=%2.f,E=%2.f,yaw=%2.f,roll=%2.f,pitch=%2.f\n",RADIAN_2_ANGLE(earth_point->azimuth),RADIAN_2_ANGLE(earth_point->elevation),
//			RADIAN_2_ANGLE(imu->yaw),RADIAN_2_ANGLE(imu->roll),RADIAN_2_ANGLE(imu->pitch));
//
//	//Aj_FZ = cosf(E) * ( cosf(R) * sinf(A - H) + sinf(R) * sinf(P) * cosf(A-H)) - sinf(R)* cosf(P)* sinf(E);
//	//Aj_FM = cosf(P) * cosf(E) * cosf(A - H) + sinf(P) * sinf(E);
//	//Aj_Rad = atan2f(Aj_FZ, Aj_FM);
//	//Ej_Rad = asinf(sinf(R)* cosf(E)* sinf(A-H) - cosf( R)* sinf(P)*cosf( E)* cosf(A-H)+ cosf(R)* cosf(P)* sinf(E));
//	//Aj = 180 * Aj_Rad / PI;
//
//	cos_a=cosf(A);sin_a=sinf(A);
//
////	cos_e=cosf(E);sin_e=sinf(E);
//	cos_e=cosf(-E);sin_e=sinf(-E);
//	cos_r=cosf(R);sin_r=sinf(R);
//	cos_y=cosf(H);sin_y=sinf(H);
//	cos_p=cosf(P);sin_p=sinf(P);
//	K0=cos_a*cos_e*cos_p*cos_y-sin_e*sin_p+cos_e*cos_p*sin_a*sin_y;
//	K1=cos_e*sin_a*(cos_r*cos_y+sin_p*sin_r*sin_y)-cos_a*cos_e*(cos_r*sin_y-cos_y*sin_p*sin_r)+cos_p*sin_e*sin_r;
//	K2=cos_a*cos_e*(sin_r*sin_y+cos_r*cos_y*sin_p)-cos_e*sin_a*(cos_y*sin_r-cos_r*sin_p*sin_y)+cos_p*cos_r*sin_e;
//
//	    body_point->azimuth = -atan2f(K1,K0);
//		body_point->azimuth =body_point->azimuth + (PI/2.0f);
//		body_point->elevation = asinf(K2);
//		body_point->elevation = body_point->elevation + (PI/2.0f);
//
////		if(body_point->elevation > ANGLE_2_RADIAN(80.0f)) {
////			body_point->elevation = ANGLE_2_RADIAN(80.0f);
////		}
//		body_point->azimuth = radian_mod(body_point->azimuth);
//
//
//    return ERR_OK;
//}
//
///**********************************************************************************/





int32_t earth_2_body_polarity(const satmov_point_t *earth_point, const satmov_imu_t *imu, satmov_point_t *body_polarity)
{
   //memcpy(body_polarity,earth_point,sizeof(satmov_point_t));
   //return ERR_OK;
	float temp;
	satmov_imu_t tmp_imu;

	//tmp_imu = *imu;
	//satmov_point_t tmp_body;//计算水平投影下的方位角tmp _imu.yaw = imu->yaw;tmp_imu.roll =0.0f;tmp_imu.pitch = 0.0f;l
	//earth_2_body_point(earth_point, &tmp_imu, &tmp_body);
	//将地理极化转换为载体极化
	temp = radian_mod(earth_point->polarity + body_polarity->azimuth);
	body_polarity->polarity = fmodf(temp,PI);

	//float az;
	// Float pol;//float pitch;1 float roll;
	// float yaw;
	// satmov_point_t tmp_earth;1/satmov_gps_t BpS3
	//gps.height=500.of;
	//gps.latitude = ANGLE_2_RADIAN(30.58千);Bps.longitude = ANGLE_2_RADIAN(104.05f);·gPS.state= 0x02;
	//calculate_earth _point(&gps，ANGLE 2 RADIAN(134.0f)，&tmp_earth);
	//pitch = imu->pitch;
	//roll=imu->roll;

	return ERR_OK;



}



int32_t earth_3_body_polarity(const satmov_point_t *earth_point, const satmov_imu_t *imu, float *best_yaw,satmov_point_t *body_polarity)
{
   //memcpy(body_polarity,earth_point,sizeof(satmov_point_t));
   //return ERR_OK;
	float temp;
	satmov_imu_t tmp_imu;

	tmp_imu = *imu;
	tmp_imu.yaw = *best_yaw;

	satmov_point_t tmp_body;//计算水平投影下的方位角tmp _imu.yaw = imu->yaw;tmp_imu.roll =0.0f;tmp_imu.pitch = 0.0f;l
	earth_2_body_point(earth_point, &tmp_imu, &tmp_body);
	//将地理极化转换为载体极化
	temp = radian_mod(earth_point->polarity + tmp_body.azimuth);
	body_polarity->polarity = fmodf(temp,PI);

	//float az;
	// Float pol;//float pitch;1 float roll;
	// float yaw;
	// satmov_point_t tmp_earth;1/satmov_gps_t BpS3
	//gps.height=500.of;
	//gps.latitude = ANGLE_2_RADIAN(30.58千);Bps.longitude = ANGLE_2_RADIAN(104.05f);·gPS.state= 0x02;
	//calculate_earth _point(&gps，ANGLE 2 RADIAN(134.0f)，&tmp_earth);
	//pitch = imu->pitch;
	//roll=imu->roll;

	return ERR_OK;



}

void calculate_conical_point(const satmov_point_t *centre_point, float deviate, float phase, satmov_point_t *conical_point)
{

}

void forecast_gesture(satmov_imu_t *imu, uint32_t ms)
{

}
