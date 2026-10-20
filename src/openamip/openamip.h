/*
 * openamip.h
 *
 *  Created on: 2022Äê1ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef __OPENAMIP_H_
#define __OPENAMIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>

#include "../mavlink_amp/mavlink_amp.h"

int32_t openamip_init(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif /* __OPENAMIP_H_ */
