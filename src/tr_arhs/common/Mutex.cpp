/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "../../tr_arhs/common/Mutex.h"

using namespace rt_cpp;

Mutex::Mutex(const char *name)
{
	mID = xSemaphoreCreateMutex();
}

bool Mutex::lock(int32_t millisec)
{
    int32_t tick;

    if (millisec < 0)
        tick = -1;
    else
        tick = pdMS_TO_TICKS(millisec);

    return xSemaphoreTake(mID, tick) == pdPASS;
}

bool Mutex::trylock()
{
    return lock(0);
}

void Mutex::unlock()
{
    xSemaphoreGive(mID);
}

Mutex::~Mutex()
{
	vSemaphoreDelete(mID);
}
