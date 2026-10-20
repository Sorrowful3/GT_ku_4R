
/*
 * log_manage.h
 *
 *  Created on: 2022年2月14日
 *      Author: Administrator
 */

#ifndef __LOG_MANAGE_H_
#define __LOG_MANAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>

#include "../mavlink_amp/mavlink_amp.h"

/*
 * 日志格式：
 * 帧头1byte 方向1byte 载荷长度2byte 数据载荷nbyte 帧尾1byte
 * 帧头：0xAA，1byte
 * 方向：0x00 linux到对星固件，0x01 对星固件到linux，1byte
 * 长度：载荷长度2byte
 * 载荷：mavlink信息包
 * 帧尾：0x55
 * */

typedef struct{
	uint8_t frame_head;
	uint8_t type;
	uint16_t lenght;
}__attribute__((packed)) log_frame_head_t;

typedef struct{
	uint8_t frame_stern;
}__attribute__((packed)) log_frame_stern_t;

int32_t log_manage_init(const char *path);
int32_t log_write(const uint8_t *buffer, size_t lenght, uint8_t type);
void log_manage_receive_save(const uint8_t *buffer, size_t lenght);
void log_manage_send_save(const uint8_t *buffer, size_t lenght);
void log_manage_new_file(void);

#ifdef __cplusplus
}
#endif

#endif /* __LOG_MANAGE_H_ */
