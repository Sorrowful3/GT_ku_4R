/*
 * mavlink_chan.h
 *
 *  Created on: 2021年9月23日
 *      Author: Administrator
 */

#ifndef __MAVLINK_CHAN_H_
#define __MAVLINK_CHAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include "../mavlink/SATMOV_v2/mavlink.h"

//描述符类型
#define UNKNOWN_FD	0x00
#define VIRTUAL_FD	0x01
#define NORMAL_FD	0x02
#define SOCKET_FD	0x03

typedef struct{
	void (*read_callbk)(const uint8_t *buffer, size_t lenght);
	void (*write_callbk)(const uint8_t *buffer, size_t lenght);
}mavlink_chan_callbk_t;

int32_t mavlink_chan_init(void);
int32_t mavlink_chan_alloc(mavlink_channel_t *chan, int fd, uint8_t fd_type);
int32_t mavlink_chan_free(mavlink_channel_t chan);
ssize_t mavlink_chan_read(mavlink_channel_t chan, uint8_t *data, size_t size);
ssize_t mavlink_chan_write(mavlink_channel_t chan, const uint8_t *data, size_t size);
int32_t mavlink_all_chan_write(const uint8_t *data, size_t size, uint8_t fd_type);
int32_t mavlink_chan_write_pack(mavlink_channel_t chan, const mavlink_message_t *msg);
int32_t mavlink_all_chan_write_pack(const mavlink_message_t *msg, uint8_t fd_type);
int32_t mavlink_chan_set_callbk(mavlink_channel_t chan, mavlink_chan_callbk_t *callbk);

#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_CHAN_H_ */
