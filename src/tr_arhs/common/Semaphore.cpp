/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "../../tr_arhs/common/Semaphore.h"

using namespace rt_cpp;

Semaphore::Semaphore(int32_t max_count, int32_t count)
{
	mID= xSemaphoreCreateCounting(max_count,count);
}

bool Semaphore::wait(int32_t millisec)
{
    int32_t tick;

    if (millisec < 0)
        tick = -1;
    else
        tick = pdMS_TO_TICKS(millisec);

    return xSemaphoreTake(mID, tick) == pdPASS;
}

void Semaphore::release(void)
{
    xSemaphoreGive(mID);
}

Semaphore::~Semaphore()
{
	vSemaphoreDelete(mID);
}
