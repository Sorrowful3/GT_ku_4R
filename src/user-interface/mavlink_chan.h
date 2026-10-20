/*
 * mavlink_chan.h
 *
 *  Created on: 2020年9月3日
 *      Author: Administrator
 */

#ifndef __MAVLINK_CHAN_H_
#define __MAVLINK_CHAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver/driver.h"

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#define MAVLINK_SEND_UART_BYTES                 mavlink_chan_send_bytes
#define MAVLINK_START_UART_SEND(chan, length)   mavlink_chan_send_begin(chan, length)
#define MAVLINK_END_UART_SEND(chan, length)     mavlink_chan_send_end(chan, length)

#define DEFAULT_SYSID 0
#define DEFAULT_COMPID 0

#define MAVLINK_COMM_NUM_BUFFERS 5

#include "mavlink/mavlink_types.h"

//通道符类型
#define UNKNOWN_TYPE  0x00
#define VIRTUAL_TYPE  0x01
#define SERIAL_TYPE   0X02
#define AMP_TYPE      0x04
#define SOCKET_TYPE   0x10

typedef struct mavlink_port{
    int32_t (*open)(struct mavlink_port *port);
    ssize_t (*read)(struct mavlink_port *port, void *buffer, size_t lenght, TickType_t timeout);
    ssize_t (*write)(struct mavlink_port *port, const void *buffer, size_t lenght);
    int32_t (*close)(struct mavlink_port *port);
    void *private_data;
}mavlink_port_t;

static struct __mavlink_system mavlink_system = {
    DEFAULT_SYSID,
    DEFAULT_COMPID
};

int32_t mavlink_chan_init(void);
int32_t mavlink_chan_alloc(mavlink_channel_t *chan, mavlink_port_t *ops, uint8_t type);
int32_t mavlink_chan_free(mavlink_channel_t chan);
ssize_t mavlink_chan_read(mavlink_channel_t chan, void *data, size_t size, TickType_t timeout);
ssize_t mavlink_chan_write(mavlink_channel_t chan, const void *data, size_t size);
int32_t mavlink_type_chan_write(const void *data, size_t size, uint8_t type);
int32_t mavlink_all_chan_write(const void *data, size_t size);
int32_t mavlink_chan_write_pack(mavlink_channel_t chan, const mavlink_message_t *msg);
int32_t mavlink_type_chan_write_pack(const mavlink_message_t *msg, uint8_t type);
int32_t mavlink_all_chan_write_pack(const mavlink_message_t *msg);

void mavlink_chan_send_begin(mavlink_channel_t chan, uint16_t lenght);
void mavlink_chan_send_bytes(mavlink_channel_t chan, const void *data, uint16_t lenght);
void mavlink_chan_send_end(mavlink_channel_t chan, uint16_t lenght);

#include "mavlink/SATMOV_v2/mavlink.h"

#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_CHAN_H_ */
