/*
 * crt.cpp
 *
 *  Created on: 2021��4��23��
 *      Author: admin
 */
#include "../../tr_arhs/common/crt.h"

void *operator new(size_t size)
{
 return pvPortMalloc(size);
}

void *operator new[](size_t size)
{
 return pvPortMalloc(size);
}

void operator delete(void *ptr)
{
 vPortFree(ptr);
}

void operator delete[](void *ptr)
{
 vPortFree(ptr);
}

