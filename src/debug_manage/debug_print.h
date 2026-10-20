/*
 * debug_print.h
 *
 *  Created on: 2022年4月12日
 *      Author: Administrator
 */

#ifndef __DEBUG_PRINT_H_
#define __DEBUG_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "../mavlink_amp/mavlink_amp.h"

/**
 * 调试输出等级
 * 需要用户在包含debug.h文件前定义DEBUG_LEVEL，若未定义则默认为WARNING_OUTPUT
 */
#define DEBUG_OUTPUT    3
#define INFO_OUTPUT     2
#define WARNING_OUTPUT  1
#define ERROR_OUTPUT    0

int32_t debug_print_init(const char *path, uint16_t port);
void debug_print_raw(int32_t level, const char *fmt, ...);
void debug_print_line(int32_t level, const char *lvl_name, const char *tag, const char *fmt, ...);
void debug_print_set_level(int32_t level);
void debug_print_new_file(void);

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_PRINT_H_ */
