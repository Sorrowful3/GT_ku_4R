/*
 * hrt.c
 *
 *  Created on: 2021��4��22��
 *      Author: admin
 */

#include "../../tr_arhs/common/hrt.h"


hrt_abstime
hrt_absolute_time(void)
{
	XTime timestamp_sample ;
	XTime_GetTime(&timestamp_sample);
	timestamp_sample =(double)timestamp_sample/ XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ *2000000UL;

	return timestamp_sample;

}


