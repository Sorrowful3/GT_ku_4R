/*
 * wdt.c
 *
 *  Created on: 2020年12月14日
 *      Author: Administrator
 */

#include "ps_wdt.h"

#define DEBUG_TAG "ps_wdt,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_PS_WDT

static XScuWdt Watchdog;

static void wdt_thread(void *arg)
{
	//启动看门狗
	XScuWdt_Start(&Watchdog);

	while(1)
	{
		vTaskDelay(100);
		//复位看门狗
		XScuWdt_RestartWdt(&Watchdog);
	}
}

int32_t ps_wdt_init(void)
{
	int32_t result;
	XScuWdt_Config *ConfigPtr;

	//初始化scuwdt
	ConfigPtr = XScuWdt_LookupConfig(XPAR_SCUWDT_0_DEVICE_ID);
	result = XScuWdt_CfgInitialize(&Watchdog, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (result != XST_SUCCESS) {
        LOG_E("config wdt failed");
		return ERR_IO;
	}
	//设置scuwdt为看门狗模式
	XScuWdt_SetWdMode(&Watchdog);
	//设置初始值
	XScuWdt_LoadWdt(&Watchdog, WDT_LOAD_VALUE);

	return ERR_OK;
}

int32_t ps_wdt_start(void)
{
	TaskHandle_t handle;
	portBASE_TYPE result;

	//创建启动线程
	result = xTaskCreate(wdt_thread,
				 	 	 "wdt_thread",
						 configMINIMAL_STACK_SIZE,
						 NULL,
						 configMAX_PRIORITIES - 4,
					 	 &handle );
	if(result != pdPASS) {
		LOG_E("create task start_thread failed");
		return ERR_NOMEM;
	}

	return ERR_OK;
}

#endif /* USING_PS_WDT */
