/*
 * antenna_v1_port.c
 *
 *  Created on: 2021年9月5日
 *      Author: Administrator
 */


#include "antenna_v1_port.h"

#define DEBUG_TAG   "antenna_v1_port,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V1

static device_t *device;
static TickType_t timeout = portMAX_DELAY;

void antenna_v1_port_write_start(void)
{

}

void antenna_v1_port_write_end(void)
{

}

ssize_t antenna_v1_port_read(void *buffer, ssize_t lenght, TickType_t xTicksToWait)
{
    int32_t result;

    if(timeout != xTicksToWait) {
        result = device_ctrl(device, DEVICE_SET_TIMEOUT, &xTicksToWait);
        if(result != ERR_OK) {
            timeout = portMAX_DELAY;
            LOG_E("set rx timeout failed");
            return result;
        }
        timeout = xTicksToWait;
    }

    return device_read(device, buffer, lenght);
}

ssize_t antenna_v1_port_write(const void *buffer, ssize_t lenght)
{
    return device_write(device, buffer, lenght);
}

int32_t antenna_v1_port_init(void)
{
    int32_t result;

    //打开设备
    device = device_open(ANTENNA_V1_PORT_DEVICE, DEVICE_RDWR);
    if(!device) {
        LOG_E("open device failed");
        return ERR_EMPTY;
    }
    //获取设备超时时间
    result = device_ctrl(device, DEVICE_GET_TIMEOUT, &timeout);
    if(result != ERR_OK) {
        LOG_E("set rx timeout failed");
        return result;
    }

    return ERR_OK;
}

#endif /* USING_ANTENNA_V1 */



