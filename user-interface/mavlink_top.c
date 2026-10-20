/*
 * mavlink_top.c
 *
 *  Created on: 2022年1月23日
 *      Author: Administrator
 */

#include "mavlink_top.h"

#define DEBUG_TAG "mavlink_top,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../driver/core/debug.h"


static int32_t chan_open(mavlink_port_t *port)
{
    stream_channel_t *pri = (stream_channel_t*)(port->private_data);

    pri->device = device_open(pri->device_name, DEVICE_RDWR);
    if(!pri->device) {
        LOG_E("open device failed\r\n");
        return ERR_EMPTY;
    }
    return ERR_OK;
}

static ssize_t chan_read(struct mavlink_port *port, void *buffer, size_t lenght, TickType_t timeout)
{
    int32_t result;
    stream_channel_t *pri = (stream_channel_t*)port->private_data;

    if(timeout != pri->timeout) {
        result = device_ctrl(pri->device, DEVICE_SET_TIMEOUT, &timeout);
        if(result != ERR_OK) {
            LOG_E("set receive timeout failed\r\n");
            return result;
        }
        pri->timeout = timeout;
    }
    return device_read(pri->device, buffer, lenght);
}

static ssize_t chan_write(struct mavlink_port *port, const void *buffer, size_t lenght)
{
    stream_channel_t *pri = (stream_channel_t*)port->private_data;

    return device_write(pri->device, buffer, lenght);
}

static int32_t chan_close(mavlink_port_t *port)
{
    stream_channel_t *pri = (stream_channel_t*)port->private_data;

    return device_close(pri->device);
}

static stream_channel_t uart_chan = {
    .device_name = (char*)"axi_uart4",
    .device = NULL,
    .timeout = portMAX_DELAY,
};
static stream_channel_t ocm_chan = {
    .device_name = (char*)"ps_ocm",
    .device = NULL,
    .timeout = portMAX_DELAY,
};
static mavlink_port_t uart_port =
{
    .open = chan_open,
    .read = chan_read,
    .write = chan_write,
    .close = chan_close,
    .private_data =  (void *)&uart_chan,
};
static mavlink_port_t ocm_port =
{
    .open = chan_open,
    .read = chan_read,
    .write = chan_write,
    .close = chan_close,
    .private_data = (void *)&ocm_chan,
};
int32_t mavlink_top_init(void)
{
    int32_t result;
    mavlink_interval_t interval;

    //初始化通道管理
    result = mavlink_chan_init();
    if(result != ERR_OK) {
        LOG_E("mavlink chan init failed");
        return result;
    }

    //初始化远端通信
    result = mavlink_remote_init();
    if(result != ERR_OK) {
        LOG_E("mavlink remote init failed");
        return result;
    }

//    //创建串口远端通信
//    interval.antenna_interval = 50;
//    interval.beacon_interval = 50;
//    interval.imu_interval = 50;
//    interval.transducer_interval = 100;
//    interval.work_interval = 100;
//    result = mavlink_remote_create(&uart_port, SERIAL_TYPE, &interval);
//    if(result != ERR_OK) {
//        LOG_E("mavlink remote create failed");
//        return result;
//    }

    //创建ocm远端通信
    interval.antenna_interval = 5;
    interval.beacon_interval = 5;
    interval.imu_interval = 5;
    interval.transducer_interval = 1000;
    interval.work_interval = 1000;
    result = mavlink_remote_create(&ocm_port, AMP_TYPE, &interval);
    if(result != ERR_OK) {
        LOG_E("mavlink remote create failed");
        return result;
    }

    return ERR_OK;
}
