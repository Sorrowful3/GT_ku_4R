/*
 * mavlink_client.h
 *
 *  Created on: 2021Äê10ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef __MAVLINK_CLIENT_H_
#define __MAVLINK_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mqueue.h>
#include <errno.h>
#include <netinet/tcp.h>

#include "../mavlink_amp/mavlink_amp.h"
#include "../mavlink_chan/mavlink_chan.h"

int32_t mavlink_create_client(int sock);
int32_t mavlink_client_init(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_CLIENT_H_ */
