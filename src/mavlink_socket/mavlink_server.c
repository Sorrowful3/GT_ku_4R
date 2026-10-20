/*
 * mavlink_server.c
 *
 *  Created on: 2021年10月29日
 *      Author: Administrator
 */

#include "mavlink_server.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "mavlink_server.c,%d", __LINE__
#include "../debug_manage/debug.h"

static int tcp_sock;

static void *tcp_server_thread(void *arg)
{
	int sock;

	while(1)
	{
		//获取服务器连接
		sock = accept(tcp_sock, NULL, NULL);
		if(sock < 0) {
			continue;
		}
		//创建mavlink客户端
		if(mavlink_create_client(sock) < 0) {
			close(sock);
		}
	}
	return NULL;
}

int32_t mavlink_server_init(uint16_t port)
{
	int optval;
	pthread_t tcp_thread;
	struct sockaddr_in address;

	//申请一个TCP类型的套接字
	tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_sock < 0) {
		LOG_E("apply socket failed");
		return -1;
	}
	//允许重用本地地址和端口
	optval = 1;
	if(setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		close(tcp_sock);
		LOG_E("set socket reused failed");
		return -1;
	}
	//绑定本地IP和端口
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(bind(tcp_sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
		close(tcp_sock);
		LOG_E("bind socket failed");
		return -1;
	}
	//开启监听
	if(listen(tcp_sock, 15) == -1) {
		close(tcp_sock);
		LOG_E("listen socket failed");
		return -1;
	}

	//初始化客户端资源
	if(mavlink_client_init() < 0) {
		close(tcp_sock);
		LOG_E("mavlink client init failed");
		return -1;
	}

	//创建服务器线程
	if(pthread_create(&tcp_thread, NULL, tcp_server_thread, NULL) < 0) {
		close(tcp_sock);
		LOG_E("create tcp_server_thread failed");
		return -1;
	}
	if(pthread_detach(tcp_thread) < 0) {
		close(tcp_sock);
		LOG_E("detach tcp_server_thread failed");
		return -1;
	}

	return 0;
}
