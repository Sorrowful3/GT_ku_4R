/*
 * ADIS16465_main.cpp
 *
 *  Created on: 2021��4��13��
 *      Author: admin
 */
#include "ADIS16465.h"

ADIS16465 *ADIS16465::instantiate(void)
{
	ADIS16465 *instance = new ADIS16465();

	if (!instance) {
		printf("alloc failed");
		return nullptr;
	}

	if (0 != instance->init()) {
		delete instance;
		return nullptr;
	}

	return instance;
}

static ADIS16465* instance_bus_0_dev_1;

void  ADIS16465_main(void *arg)
{
   //bus��0  device��adis16465  freq��1M    mode�� mode3
	instance_bus_0_dev_1=ADIS16465::instantiate();

    while(1)
    {
    	vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void ADIS16465_Mesure(TimerHandle_t xTimer)
{
	ADIS16465 * p_this = ( ADIS16465* ) pvTimerGetTimerID( xTimer );
	p_this->RunImpl();
}


