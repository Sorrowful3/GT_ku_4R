/*
 * Ekf_Output.cpp
 *
 *  Created on: 2021��4��26��
 *      Author: admin
 */

#include "Ekf_Output.h"

static adis16465_attitude_t adis16465_attitude;

//EKF_OutPut::EKF_OutPut():_tr_attitude_sub(ORB_ID::vehicle_attitude)
//
//{
//
//}
//
//
//EKF_OutPut::~EKF_OutPut()
//
//{
//
//}
//
//
//void EKF_OutPut::Attitude_Output()
//{
//	vehicle_attitude_s  att;
//	AHRS_Output   ahrs_output{0};
//	_tr_attitude_sub.updated();
//	_tr_attitude_sub.copy(&att);
//
//    Vector3f attitude_angle =Eulerf((Quatf)att.q);
//
//    /*
//    printf("roll:%.2f,pitch:%.2f,yaw:%.2f\n",math::degrees(attitude_angle(0)),
//	                                   math::degrees(attitude_angle(1)),
//	                                   math::degrees(attitude_angle(2)));*/
//    //attitude_angle(0) = 3.05f;
//    //attitude_angle(1) = 1.23f;
//    //attitude_angle(2) = 6.19f;
//
//    ahrs_output.head_L = 0xAA;
//    ahrs_output.head_H = 0x55;
//
//    ahrs_output.pitch_L = (int8_t)(math::degrees(attitude_angle(1))*100);
//    ahrs_output.pitch_H = (int8_t)((int16_t)(math::degrees(attitude_angle(1))*100)>>8);
//    ahrs_output.roll_L  = (int8_t)(math::degrees(attitude_angle(0))*100);
//    ahrs_output.roll_H  = (int8_t)((int16_t)(math::degrees(attitude_angle(0))*100)>>8);
//    ahrs_output.yaw_L   = (int8_t)(math::degrees(attitude_angle(2))*100);
//    ahrs_output.yaw_H   = (int8_t)((int16_t)(math::degrees(attitude_angle(2))*100)>>8);
//
//    uint8_t *checksum_helper = (uint8_t *)&ahrs_output;
//    for(int i=0;i<8;i++)
//    {
//      ahrs_output.cheak += checksum_helper[i];
//    }
//
//    ahrs_output.cheak = (0- ahrs_output.cheak)&0xff;
//
//    XUartPs_Send(XUartPs_Out,(uint8_t *)&ahrs_output,sizeof(ahrs_output));
//
//}
//
//EKF_OutPut * EKF_OutPut::ekf_output_inst = nullptr;
//XUartPs  *EKF_OutPut::XUartPs_Out = nullptr;
//
//void Vehivle_EKF_OUT(void *arg)
//{
//	EKF_OutPut::ekf_output_inst = new EKF_OutPut;
//	EKF_OutPut::XUartPs_Out = new XUartPs;
//	EKF_OutPut::XUartPs_Out->Config.BaseAddress = XPAR_PS7_UART_1_BASEADDR;
//	EKF_OutPut::XUartPs_Out->IsReady = XIL_COMPONENT_IS_READY;
//	while(1)
//	{
//     EKF_OutPut::ekf_output_inst->Attitude_Output();
//	 vTaskDelay(pdMS_TO_TICKS(5));
//	}
//
//}


EKF_OutPut::EKF_OutPut():
            _tr_attitude_sub(ORB_ID::vehicle_attitude)

{

}


EKF_OutPut::~EKF_OutPut()

{

}

void EKF_OutPut::Init()
{
}

void EKF_OutPut::Attitude_Output()
{
	vehicle_attitude_s  att;
	AHRS_Output   ahrs_output{0};
	_tr_attitude_sub.updated();
	_tr_attitude_sub.copy(&att);

    Vector3f attitude_angle =Eulerf((Quatf)att.q);

//    ahrs_output.head_L = 0xAA;
//    ahrs_output.head_H = 0x55;
//
//    ahrs_output.pitch_L = (int8_t)(math::degrees(attitude_angle(1))*100);
//    ahrs_output.pitch_H = (int8_t)((int16_t)(math::degrees(attitude_angle(1))*100)>>8);
//    ahrs_output.roll_L  = (int8_t)(math::degrees(attitude_angle(0))*100);
//    ahrs_output.roll_H  = (int8_t)((int16_t)(math::degrees(attitude_angle(0))*100)>>8);
//    ahrs_output.yaw_L   = (int8_t)(math::degrees(attitude_angle(2))*100);
//    ahrs_output.yaw_H   = (int8_t)((int16_t)(math::degrees(attitude_angle(2))*100)>>8);
//
//    ahrs_output.x_rate_L = (int8_t)(math::degrees(att.x_rate)*100);
//    ahrs_output.x_rate_H = (int8_t)((int16_t)(math::degrees(att.x_rate)*100)>>8);
//    ahrs_output.y_rate_L  = (int8_t)(math::degrees(att.y_rate)*100);
//    ahrs_output.y_rate_H  = (int8_t)((int16_t)(math::degrees(att.y_rate)*100)>>8);
//    ahrs_output.z_rate_L   = (int8_t)(math::degrees(att.z_rate)*100);
//    ahrs_output.z_rate_H   = (int8_t)((int16_t)(math::degrees(att.z_rate)*100)>>8);
//
//
//    uint8_t *checksum_helper = (uint8_t *)&ahrs_output;
//    for(int i=0;i<14;i++)
//    {
//      ahrs_output.cheak += checksum_helper[i];
//    }
//
//    ahrs_output.cheak = (0- ahrs_output.cheak)&0xff;

    adis16465_attitude.gyro.x = att.x_rate;
    adis16465_attitude.gyro.y = att.y_rate;
    adis16465_attitude.gyro.z = att.z_rate;

    adis16465_attitude.euler_angle.pitch = math::degrees(attitude_angle(1));
    adis16465_attitude.euler_angle.roll = math::degrees(attitude_angle(0));
    adis16465_attitude.euler_angle.yaw = math::degrees(attitude_angle(2));

    //XUartPs_Send(XUartPs_Out,(uint8_t *)&ahrs_output,sizeof(ahrs_output));
//     write((uint8_t *)&ahrs_output,sizeof(ahrs_output),0);
    //printf("rool %f  pitch %f yaw %f\r\n",math::degrees(attitude_angle(0)),math::degrees(attitude_angle(1)),math::degrees(attitude_angle(2)));
    //printf("x:%f    y:%f   z:%f\r\n",math::degrees(att.x_rate),math::degrees(att.y_rate),math::degrees(att.z_rate));
}

EKF_OutPut * EKF_OutPut::ekf_output_inst = nullptr;
XUartPs  *EKF_OutPut::XUartPs_Out = nullptr;

void Vehivle_EKF_OUT(void *arg)
{
	EKF_OutPut::ekf_output_inst = new EKF_OutPut;

	EKF_OutPut::ekf_output_inst->Init();

	while(1)
	{
     EKF_OutPut::ekf_output_inst->Attitude_Output();
	 vTaskDelay(pdMS_TO_TICKS(2));
	}

}

void Vehivle_EKF_read(adis16465_attitude_t *attitude)
{
    *attitude = adis16465_attitude;
}
