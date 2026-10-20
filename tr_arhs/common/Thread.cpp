/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "../../tr_arhs/common/Thread.h"

using namespace rt_cpp;


Thread::Thread(uint32_t stack_size,
               uint8_t  priority,
               uint32_t tick,
               const char *name)
    : _entry(NULL), _param(NULL), started(false)
{
	_event = xEventGroupCreate();

	xTaskCreate((TaskFunction_t)func,
			      name,
				  stack_size,
				  this,
				  priority,
				  &_thread);

	vTaskStartScheduler();
}

Thread::Thread(void (*entry)(void *p),
               void *p,
               uint32_t stack_size,
               uint8_t  priority,
               uint32_t tick,
               const char *name)
    : _entry(entry), _param(p), started(false)
{
      _event = xEventGroupCreate();

	  xTaskCreate((TaskFunction_t)func,
				  name,
				  stack_size,
				  this,
				  priority,
				  &_thread);

	  vTaskStartScheduler();

	  vTaskSuspend(_thread);
}

Thread::~Thread()
{
	vEventGroupDelete(_event);
	vTaskDelete(_thread);
}

bool Thread::start()
{
    vTaskResume(_thread);

    started = true;

    return started;
}

void Thread::sleep(int32_t millisec)
{
    int32_t tick;

    if (millisec < 0)
        tick = 1;
    else
        tick = pdMS_TO_TICKS(millisec);

    vTaskDelay(tick);
}

void Thread::func(Thread *pThis)
{
    if (pThis->_entry != NULL)
    {
    	pThis->_entry(pThis->_param);
    }
    else
    {
    	pThis->run(pThis->_param);
    }

    xEventGroupSetBits(pThis->_event,1);

}

void Thread::run(void *parameter)
{
    /* please overload this method */
}

int Thread::wait(int32_t millisec)
{
    return join(millisec);
}

int Thread::join(int32_t millisec)
{
    if (started)
    {
        int32_t tick;

        if (millisec < 0)
            tick = -1;
        else
            tick = pdMS_TO_TICKS(millisec);
        return xEventGroupWaitBits(_event,1,pdTRUE,pdFALSE,tick);
    }
    else
    {
        return -ENOSYS;
    }
}
