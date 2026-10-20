/*
 * mavlink_server.h
 *
 *  Created on: 2021Äê10ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef __MAVLINK_SERVER_H_
#define __MAVLINK_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

#include "../mavlink_chan/mavlink_chan.h"
#include "mavlink_client.h"

int32_t mavlink_server_init(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif /* __TCP_SERVER_H_ */
