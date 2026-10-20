/*
 * mavlink_chan.c
 *
 *  Created on: 2021年9月23日
 *      Author: Administrator
 */

#include "mavlink_chan.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "mavlink_chan.c,%d", __LINE__
#include "../debug_manage/debug.h"

typedef struct{
	uint8_t					use_flag;
	int						fd;
	uint8_t					fd_type;
	pthread_mutex_t 		r_mutex;
	pthread_mutex_t 		w_mutex;
	void (*read_callbk)(const uint8_t *buffer, size_t lenght);
	void (*write_callbk)(const uint8_t *buffer, size_t lenght);
}mavlink_chan_t;

static mavlink_chan_t mavlink_chan[MAVLINK_COMM_NUM_BUFFERS];
static pthread_mutex_t alloc_mutex;

int32_t mavlink_chan_init(void)
{
	int32_t i;

	if(pthread_mutex_init(&alloc_mutex, NULL) < 0) {
		LOG_E("create alloc mutex failed");
		return -1;
	}
	for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
		mavlink_chan[i].use_flag = 0;
		mavlink_chan[i].fd = -1;
		mavlink_chan[i].fd_type = UNKNOWN_FD;
		mavlink_chan[i].read_callbk = NULL;
		mavlink_chan[i].write_callbk = NULL;
		//创建互斥锁
		if(pthread_mutex_init(&mavlink_chan[i].r_mutex, NULL) < 0) {
			LOG_E("create read mutex failed");
			return -1;
		}
		if(pthread_mutex_init(&mavlink_chan[i].w_mutex, NULL) < 0) {
			LOG_E("create write mutex failed");
			return -1;
		}
	}
	return 0;
}

int32_t mavlink_chan_alloc(mavlink_channel_t *chan, int fd, uint8_t fd_type)
{
	int32_t i;

	if(!chan) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//获取通道分配保护锁
	pthread_mutex_lock(&alloc_mutex);

	for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
		//判断通道是否空闲
		if(!mavlink_chan[i].use_flag) {
			//返回获取到的通道号
			*chan = (mavlink_channel_t)i;
			//获取相应通道
			mavlink_chan[i].use_flag = 1;
			mavlink_chan[i].fd = fd;
			mavlink_chan[i].fd_type = fd_type;
			//通道回调函数默认为空
			mavlink_chan[i].read_callbk = NULL;
			mavlink_chan[i].write_callbk = NULL;
			break;
		}
	}

	//释放通道分配保护锁
	pthread_mutex_unlock(&alloc_mutex);

	if(i >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) {
		LOG_E("alloc channel failed");
		return -1;
	}
	return 0;
}

int32_t mavlink_chan_free(mavlink_channel_t chan)
{
	if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (chan < 0)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//获取通道分配保护锁
	pthread_mutex_lock(&alloc_mutex);
	//获取读写锁，确保读写操作完成
	pthread_mutex_lock(&mavlink_chan[chan].r_mutex);
	pthread_mutex_lock(&mavlink_chan[chan].w_mutex);

	//复位通道
	mavlink_chan[chan].use_flag = 0;
	mavlink_chan[chan].fd = -1;
	mavlink_chan[chan].fd_type = UNKNOWN_FD;
	mavlink_chan[chan].read_callbk = NULL;
	mavlink_chan[chan].write_callbk = NULL;

	//释放读写锁
	pthread_mutex_unlock(&mavlink_chan[chan].r_mutex);
	pthread_mutex_unlock(&mavlink_chan[chan].w_mutex);
	//释放通道分配保护锁
	pthread_mutex_unlock(&alloc_mutex);

	return 0;
}

ssize_t mavlink_chan_read(mavlink_channel_t chan, uint8_t *data, size_t size)
{
	ssize_t result;

	if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (chan < 0) || (!data) || (size == 0)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	pthread_mutex_lock(&mavlink_chan[chan].r_mutex);

	if(mavlink_chan[chan].use_flag) {
		if((mavlink_chan[chan].fd >= 0) &&
				(mavlink_chan[chan].fd_type != UNKNOWN_FD) &&
				(mavlink_chan[chan].fd_type != VIRTUAL_FD)) {
			//执行读操作
			result = read(mavlink_chan[chan].fd, data, size);
			//执行读取回调函数
			if((mavlink_chan[chan].read_callbk) && (result > 0)) {
				mavlink_chan[chan].read_callbk(data, result);
			}
		}
		else if(mavlink_chan[chan].fd_type == VIRTUAL_FD) {
			result = 0;
		}
		else {
			LOG_E("file descriptor Invalid");
			result = -1;
		}
	}
	else {
		LOG_E("channel no enable failed");
		result = -1;
	}

	pthread_mutex_unlock(&mavlink_chan[chan].r_mutex);

	return result;
}

ssize_t mavlink_chan_write(mavlink_channel_t chan, const uint8_t *data, size_t size)
{
	ssize_t result;

	if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (chan < 0) || (!data) || (size == 0)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	pthread_mutex_lock(&mavlink_chan[chan].w_mutex);
	
	if(mavlink_chan[chan].use_flag) {
		if((mavlink_chan[chan].fd >= 0) &&
				(mavlink_chan[chan].fd_type != UNKNOWN_FD) &&
				(mavlink_chan[chan].fd_type != VIRTUAL_FD)) {
			//执行写操作
			result = write(mavlink_chan[chan].fd, data, size);
			//执行写入回调函数
			if((mavlink_chan[chan].write_callbk) && (result > 0)) {
				mavlink_chan[chan].write_callbk(data, result);
			}
		}
		else if(mavlink_chan[chan].fd_type == VIRTUAL_FD) {
			result = 0;
		}
		else {
			LOG_E("file descriptor Invalid");
			result = -1;
		}
	}
	else {
		LOG_E("channel no enable failed");
		result = -1;
	}
	
	pthread_mutex_unlock(&mavlink_chan[chan].w_mutex);
	
	return result;
}

int32_t mavlink_all_chan_write(const uint8_t *data, size_t size, uint8_t fd_type)
{
	int32_t i;

	if((!data) || (size == 0)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
		if(mavlink_chan[i].fd_type == fd_type) {
			mavlink_chan_write(i, data, size);
		}
	}
	return 0;
}

int32_t mavlink_chan_write_pack(mavlink_channel_t chan, const mavlink_message_t *msg)
{
	size_t lenght = 0;
	uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

	if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (chan < 0) || (!msg)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	//序列化
	lenght = mavlink_msg_to_send_buffer(buffer, msg);
	//发送
	if(mavlink_chan_write(chan, buffer, lenght) != lenght) {
		LOG_E("write pack failed");
		return -1;
	}
	else {
		return 0;
	}
}

int32_t mavlink_all_chan_write_pack(const mavlink_message_t *msg, uint8_t fd_type)
{
	size_t lenght = 0;
	uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

	if(!msg) {
		LOG_E("Invalid parameter");
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	//序列化
	lenght = mavlink_msg_to_send_buffer(buffer, msg);
	//发送
	return mavlink_all_chan_write(buffer, lenght, fd_type);
}

int32_t mavlink_chan_set_callbk(mavlink_channel_t chan, mavlink_chan_callbk_t *callbk)
{
	if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (chan < 0) || (!callbk)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	mavlink_chan[chan].read_callbk = callbk->read_callbk;
	mavlink_chan[chan].write_callbk = callbk->write_callbk;

	return 0;
}
