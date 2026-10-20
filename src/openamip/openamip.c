/*
 * openamip.c
 *
 *  Created on: 2022年1月23日
 *		Author: Administrator
 */

#include "openamip.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "openamip.c,%d", __LINE__
#include "../debug_manage/debug.h"

typedef struct{
	int				sock;
	pthread_mutex_t	r_mutex;
	pthread_mutex_t	w_mutex;
}socket_node_t;

static socket_node_t socket_list[32];
static pthread_mutex_t list_mutex;

static int32_t fds_init(void)
{
	uint32_t i;

	if(pthread_mutex_init(&list_mutex, NULL) != 0) {
		LOG_E("init list_mutex failed");
		return -1;
	}

	//初始化客户端套接字池
	for(i=0; i<sizeof(socket_list)/sizeof(socket_list[0]); i++) {
		socket_list[i].sock = -1;
		//创建互斥锁
		if(pthread_mutex_init(&socket_list[i].r_mutex, NULL) != 0) {
			LOG_E("init read_mutex failed");
			return -1;
		}
		if(pthread_mutex_init(&socket_list[i].w_mutex, NULL) != 0) {
			LOG_E("init write_mutex failed");
			return -1;
		}
	}

	 return 0;
}

static int32_t fds_add(int sock)
{
	int32_t i;

	//获取分配保护锁
	pthread_mutex_lock(&list_mutex);

	for(i=0; i<sizeof(socket_list)/sizeof(socket_list[0]); i++) {
		if(socket_list[i].sock < 0) {
			socket_list[i].sock = sock;
			break;
		}
	}

	//释放分配保护锁
	pthread_mutex_unlock(&list_mutex);

	if(i >= sizeof(socket_list)/sizeof(socket_list[0])) {
		LOG_E("add fds failed");
		return -1;
	}
	return i;
}

static int fds_remove(int32_t handle)
{
	if(handle < sizeof(socket_list)/sizeof(socket_list[0])) {
		//获取通道分配保护锁
		pthread_mutex_lock(&list_mutex);
		//获取读写锁，确保读写操作完成
		pthread_mutex_lock(&socket_list[handle].r_mutex);
		pthread_mutex_lock(&socket_list[handle].w_mutex);

		//关闭套接字
		close(socket_list[handle].sock);
		//释放节点
		socket_list[handle].sock = -1;

		//释放读写锁
		pthread_mutex_unlock(&socket_list[handle].r_mutex);
		pthread_mutex_unlock(&socket_list[handle].w_mutex);
		//释放通道分配保护锁
		pthread_mutex_unlock(&list_mutex);

		return 0;
	}

	LOG_E("remove failed");
	return -1;
}

static ssize_t fds_read(int32_t handle, uint8_t *data, size_t size)
{
	ssize_t result;

	if((handle < sizeof(socket_list)/sizeof(socket_list[0])) || (data) || (size != 0)) {
		//加读锁
		pthread_mutex_lock(&socket_list[handle].r_mutex);
		//读数据
		result = read(socket_list[handle].sock, data, size);
		//释放读锁
		pthread_mutex_unlock(&socket_list[handle].r_mutex);

		if(result < 0) {
			LOG_E("Failed to read data through TCP. Procedure");
		}
		return result;
	}

	LOG_E("Invalid parameter");
	return -1;
}

static ssize_t fds_write(int32_t handle, const uint8_t *data, size_t size)
{
	ssize_t result;

	if((handle < sizeof(socket_list)/sizeof(socket_list[0])) || (data) || (size != 0)) {
		//加写锁
		pthread_mutex_lock(&socket_list[handle].w_mutex);
		//写数据
		result = write(socket_list[handle].sock, data, size);
		//释放写锁
		pthread_mutex_unlock(&socket_list[handle].w_mutex);

		if(result < 0) {
			LOG_E("Failed to write data through TCP. Procedure");
		}

		return result;
	}

	LOG_E("Invalid parameter");
	return -1;
}

static int32_t fds_write_all(const uint8_t *data, size_t size)
{
	int32_t i;

	for(i=0; i<sizeof(socket_list)/sizeof(socket_list[0]); i++) {
		if(socket_list[i].sock >= 0) {
			fds_write(i, data, size);
		}
	}

	return 0;
}

//广播
static void *broadcast_thread(void *arg)
{
	ssize_t result;
	char str[201];

	while(1)
	{
		//读取固件上报的openAmip数据
		result = amp_openAmip_read(str, sizeof(str)-1);
		if(result < 0) {
			LOG_E("Failed to read data through AMP Procedure");
			continue;
		}

		//广播到客户端
		fds_write_all((uint8_t*)str, (size_t)result);
		LOG_I_RAW("%s\r\n", str);
	}
	return NULL;
}

static void* receive_thread(void *arg)
{
	int32_t sock_handle;
	ssize_t result;
	char str[201];

	sock_handle = (int32_t)arg;
	//发送连接成功命令
	amp_openAmip_write("new connect\n", strlen("new connect\n"));
	while(1)
	{
		//从套接字读数据
		memset(str, 0, sizeof(str));
		result = fds_read(sock_handle, (uint8_t*)str, (size_t)(sizeof(str)-1));
		if(result > 0) {
			//将数据发送给固件
			amp_openAmip_write(str, result);
			LOG_I_RAW("%s\r\n", str);
		}
		else {
			//关闭套接字
			fds_remove(sock_handle);
			break;
		}
	}

	return NULL;
}

static void *tcp_server_thread(void *arg)
{
	int sock;
	int server_sock;
	int32_t sock_handle;
	struct timeval timeout;
	pthread_t receive;

	server_sock = (int)arg;
	while(1)
	{
		//获取服务器连接
		sock = accept(server_sock, NULL, NULL);
		if(sock < 0) {
			continue;
		}

		//设置套接字接收超时
		timeout.tv_sec = 30;
		timeout.tv_usec = 0;
		if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) != 0) {
			close(sock);
			LOG_E("set socket timeout failed");
			continue;
		}

		//将客户端套接字加入套接字列表
		sock_handle = fds_add(sock);
		if(sock_handle < 0) {
			LOG_E("add socket to fds list failed");
			close(sock);
			continue;
		}

		//创建客户端线程
		if(pthread_create(&receive, NULL, receive_thread, (void*)sock_handle) < 0) {
			LOG_E("create receive_thread failed");
			close(sock);
			continue;
		}
		if(pthread_detach(receive)  < 0) {
			LOG_E("detach receive_thread failed");
			close(sock);
			continue;
		}
	}

	return NULL;
}

int32_t openamip_init(uint16_t port)
{
	int optval;
	int server_sock;
	pthread_t server;
	pthread_t broadcast;
	struct sockaddr_in address;

	//初始化描述符列表
	if(fds_init() < 0) {
		LOG_E("fds list init failed");
		return -1;
	}

	//申请一个TCP类型的套接字
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0) {
		LOG_E("alloc socket failed");
		return -1;
	}
	//允许重用本地地址和端口
	optval = 1;
	if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		close(server_sock);
		LOG_E("set socket revaluate failed");
		return -1;
	}
	//绑定本地IP和端口
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(bind(server_sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
		close(server_sock);
		LOG_E("bind socket failed");
		return -1;
	}
	//开启监听
	if(listen(server_sock, 15) == -1) {
		close(server_sock);
		LOG_E("listen socket failed");
		return -1;
	}

	//创建服务器线程
	if(pthread_create(&server, NULL, tcp_server_thread, (void*)server_sock) < 0) {
		close(server_sock);
		LOG_E("create tcp_server_thread failed");
		return -1;
	}
	if(pthread_detach(server) < 0) {
		close(server_sock);
		LOG_E("detach tcp_server_thread failed");
		return -1;
	}

	//数据广播线程
	if(pthread_create(&broadcast, NULL, broadcast_thread, NULL) != 0) {
		close(server_sock);
		LOG_E("create broadcast_thread failed");
		return -1;
	}
	if(pthread_detach(broadcast) != 0) {
		close(server_sock);
		LOG_E("detach broadcast_thread failed");
		return -1;
	}

	return 0;
}
