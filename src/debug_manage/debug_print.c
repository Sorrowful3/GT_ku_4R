/*
 * debug_print.c
 *
 *  Created on: 2022年4月12日
 *      Author: Administrator
 */

#include "debug_print.h"

/* 全局调试输出等级 */
static int32_t debug_level = DEBUG_OUTPUT;

//GPS时间起点;1980-1-6 0:0:0
#define GPS_TIME_START  315964800LL

typedef struct{
	char str[50];
}time_str_t;

//日志工作目录
static char log_path[256];
//FIFO文件描述符
static int pipe_fd[2];
//闰秒
static uint64_t leap_second = 17LL;
//写FIFO的互斥锁
static pthread_mutex_t pipe_lock;
//存储GPS时间的文件
static int gps_time_fd = -1;
//是否创建新文件
static int32_t new_flag = 1;
//初始标志
static int32_t init_flag = 0;
//TCP套接字列表
static struct pollfd sock_fds[16];
//描述符列表中可能的最大条目
static nfds_t nfds = 0;
//服务器套接字
static int server_socket = -1;
//TCP写操作互斥量
pthread_mutex_t	w_mutex[sizeof(sock_fds)/sizeof(sock_fds[0])];

static int32_t fds_init(void)
{
	int32_t i;

	nfds = 0;
	for(i=0; i<sizeof(sock_fds)/sizeof(sock_fds[0]); i++) {
		sock_fds[i].fd = -1;
		if(pthread_mutex_init(&w_mutex[i], NULL) != 0) {
			fprintf(stderr, "init write_mutex failed");
			return -1;
		}
	}
	return 0;
}

static int32_t fds_add(int socket, short events)
{
	int32_t i;

	for(i=0; i<sizeof(sock_fds)/sizeof(sock_fds[0]); i++) {
		if(sock_fds[i].fd < 0) {
			sock_fds[i].fd = socket;
			sock_fds[i].events = events;
			if((i+1) > nfds) {
				nfds = i + 1;
			}
			return 0;
		}
	}
	return -1;
}

static int32_t fds_del(int socket)
{
	int32_t i;

	//不能删除服务器套接字
	if(socket == server_socket) {
		return -1;
	}

	for(i=0; i<sizeof(sock_fds)/sizeof(sock_fds[0]); i++) {
		if(sock_fds[i].fd == socket) {
			//确保写操作完成
			pthread_mutex_lock(&w_mutex[i]);

			//关闭套接字
			close(sock_fds[i].fd);

			//释放fds
			sock_fds[i].events = 0x00;
			sock_fds[i].fd = -1;

			//操作完成释放互斥量
			pthread_mutex_unlock(&w_mutex[i]);
			return 0;
		}
	}
	return -1;
}

static time_t get_time(void)
{
	uint64_t time_ms;
	ssize_t result;
	time_t time;
	struct tm tm;
	imu_state_t imu;

	//读取IMU数据
	amp_read_imu_state(&imu);
	if((imu.state & 0x90) == 0x90) {
		//将GPS时间转换尾时间戳
		//转换为ms
		time_ms = ((GPS_TIME_START - leap_second) * 1000LL);
		time_ms += (imu.week * 604800LL) *1000LL;
		time_ms += imu.second;
		//转换为s
		time = (time_t)(time_ms / 1000.0 + 0.5);
	}
	else {
		//从文件读取GPS时间
		lseek(gps_time_fd, 0, SEEK_SET);
		result = read(gps_time_fd, &time, sizeof(time_t));
		if(result != sizeof(time_t)) {
			//读取失败则使用默认时间
			tm.tm_year = 100 + 22;
			tm.tm_mon = 2;
			tm.tm_mday = 15;
			tm.tm_hour = 11;
			tm.tm_min = 18;
			tm.tm_sec = 50;
			time = mktime(&tm);
		}
		else {
			//累加1s，确保下次文件不重名
			time += 1;
		}
	}

	//存储时间到文件
	lseek(gps_time_fd, 0, SEEK_SET);
	write(gps_time_fd, &time, sizeof(time_t));
	fsync(gps_time_fd);

	return time;
}

static void time_2_string(const time_t *time, time_str_t *str)
{
	struct tm *p_tm;

	memset(str->str, 0, sizeof(time_str_t));
	p_tm = localtime(time);
	if(!p_tm) {
		return;
	}
	snprintf(str->str, sizeof(str->str), "%02d-%02d-%02d;%02d.%02d.%02d",\
				p_tm->tm_year - 100, p_tm->tm_mon + 1, p_tm->tm_mday, \
				p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
}

static time_t string_2_time(time_str_t *str)
{
	time_t time;
	struct tm tm;

	sscanf(str->str, "%2d-%2d-%2d;%2d.%2d.%2d",\
			&tm.tm_year, &tm.tm_mon, &tm.tm_mday, \
			&tm.tm_hour, &tm.tm_min, &tm.tm_sec);
	tm.tm_year += 100;
	tm.tm_mon += 1;
	time = mktime(&tm);

	return time;
}

static int32_t tcp_printf(const char *str, size_t lengt)
{
	int i;

	for(i=0; i<sizeof(sock_fds)/sizeof(sock_fds[0]); i++) {
		//描述符有效，且非服务器描述符
		if((sock_fds[i].fd >= 0) && (sock_fds[i].fd != server_socket)) {
			//获取写互斥量
			pthread_mutex_lock(&w_mutex[i]);

			//通过套接字发送数据
			write(sock_fds[i].fd, str, lengt);

			//操作完成释放互斥量
			pthread_mutex_unlock(&w_mutex[i]);
		}
	}

	return 0;
}

static void *save_thread(void *arg)
{
	int fd;
	time_t time;
	time_str_t str;
	ssize_t result;
	off_t file_size;
	char file_name[300];
	uint8_t buffer[512];

	fd = -1;
	new_flag = 1;
	while(1)
	{
		if(new_flag) {
			//关闭之前的文件
			if(fd >= 0) {
				close(fd);
				fd = -1;
			}

			//读取GPS时间
			time = get_time();

			//将时间转换为字符串
			time_2_string(&time, &str);

			//用时间字符串组合文件名
			snprintf(file_name, sizeof(file_name), "%s/%s.txt", log_path, str.str);

			//创建新文件
			fd = open(file_name, O_RDWR|O_CREAT|O_APPEND, S_IRWXU);
			if(fd < 0) {
				sleep(1);
				continue;
			}

			//下次无需创建新文件
			new_flag = 0;
		}

		//从管道读取数据
		result = read(pipe_fd[0], buffer, sizeof(buffer));
		if(result <= 0) {
			continue;
		}

		//将数据写入文件
		result = write(fd, buffer, result);
		if(result < 0) {
			new_flag = 1;
			continue;
		}

		//获取文件大小
		file_size = lseek(fd, 0, SEEK_END);
		if(file_size < 0) {
			new_flag = 1;
			continue;
		}

		//文件大于5M则创建一个新的文件
		if(file_size > 1024*1024*5) {
			new_flag = 1;
		}
	}

	return NULL;
}

static void *clear_thread(void *arg)
{
	int result;
	char *temp;
	DIR *dir;
	struct dirent *dirent;
	time_t old_time;
	time_t tmep_time;
	time_str_t str;
	off_t total_size;
	struct stat buf;
	char old_name[300];
	char file_name[300];

	while(1)
	{
		//每个10s清理一次
		sleep(10);

		//打开目录
		dir = opendir(log_path);
		if(!dir) {
			continue;
		}

		total_size = 0;
		old_time = 0xffffffff;
		//遍历目录项
		for(dirent=readdir(dir); dirent; dirent=readdir(dir)) {
			//只考虑.txt文件
			temp = strstr(dirent->d_name, ".txt");
			if(!temp) {
				continue;
			}

			//从目录属性提取文件名，组合文件路径
			snprintf(file_name, sizeof(file_name), "%s/%s", log_path, dirent->d_name);

			//读取文件属性
			result = stat(file_name, &buf);
			if (result < 0) {
				continue;
			}
			//累计普通文件的大小
			total_size += buf.st_size;

			//提取文件名中的时间字符串
			strncpy(str.str, dirent->d_name, strstr(dirent->d_name, ".txt")-dirent->d_name);
			//将时间字符串转换为时间戳
			tmep_time = string_2_time(&str);

			//记录最先创建的文件以及创建时间
			if((uint32_t)old_time > (uint32_t)tmep_time) {
				old_time = tmep_time;
				strncpy(old_name, file_name, sizeof(old_name));
			}
		}
		closedir(dir);

		//累计大小超出60MB，则删除最先创建的文件
		if((total_size > (1024*1024*60)) && (old_time != 0xffffffff)) {
			unlink(old_name);
		}
	}

	return NULL;
}

static void *tcp_server_thread(void *arg)
{
	int i;
	int flag;
	int flags;
	int client_sock;
	int restul;
	char buffer[32];

	//将套接字设置为非阻塞模式
	flags = fcntl(server_socket, F_GETFL);
	fcntl(server_socket, F_SETFL, flags|O_NONBLOCK);
	while(1)
	{
		//通过poll同时监听多个描述符
		restul = poll(sock_fds, nfds, -1);
		if(restul < 0) {
			usleep(1000*100);
			continue;
		}

		//依次处理监听到的事件
		for(i=0; i<sizeof(sock_fds)/sizeof(sock_fds[0]); i++) {
			//如果可读则执行一下操作
			if((sock_fds[i].fd >= 0) && (sock_fds[i].revents & POLLIN)) {
				if(sock_fds[i].fd == server_socket) {
					//从服务器套接字获取一个新连接
					client_sock = accept(server_socket, NULL, NULL);
					if(client_sock < 0) {
						continue;
					}
					//将套接字设置为非阻塞模式
					flags = fcntl(client_sock, F_GETFL);
					fcntl(client_sock, F_SETFL, flags|O_NONBLOCK);
					//关闭Nagle算法
					flag = 1;
					if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (void *) &flag, sizeof(int)) != 0) {
						close(client_sock);
						continue;
					}
					//将描述符加入描述符列表
					restul = fds_add(client_sock, POLLIN);
					if(restul == -1) {
						close(client_sock);
						continue;
					}
				}
				else {
					//读取客户端数据
					memset(buffer, 0, sizeof(buffer));
					restul = read(sock_fds[i].fd, buffer, sizeof(buffer)-1);
					//读取出错(忽略EWOULDBLOCK错误)，或对方关闭连接
					if(((restul < 0) && (errno != EWOULDBLOCK)) || (restul == 0)) {
						fds_del(sock_fds[i].fd);
					}
				}
			}
		}
	}
	return NULL;
}

int32_t debug_print_init(const char *path, uint16_t port)
{
	int result;
	int optval;
	DIR *dir;
	pthread_t thread;
	struct sockaddr_in address;
	char file_name[300];
	char sys_cmd[256];

	if(!path) {
		fprintf(stderr, "Invalid parameter\r\n");
	}

	//创建日志存储目录
	dir = opendir(path);
	if(!dir) {
		//创建目录
		snprintf(sys_cmd, sizeof(sys_cmd), "mkdir -p %s", path);
		system(sys_cmd);
		snprintf(sys_cmd, sizeof(sys_cmd), "chmod 0777 %s", path);
		system(sys_cmd);
		//检查创建是否成功
		dir = opendir(path);
		if(!dir) {
			fprintf(stderr, "create directory failed\r\n");
			return -1;
		}
	}
	//关闭打开的目录
	closedir(dir);

	//暂存log存储目录路径
	strncpy(log_path, path, sizeof(log_path));

	//打开存储GPS时间的文件
	snprintf(file_name, sizeof(file_name), "%s/debug_gps_time.dat", log_path);
	gps_time_fd = open(file_name, O_RDWR|O_CREAT, S_IRWXU);
	if(gps_time_fd < 0) {
		fprintf(stderr, "create debug_gps_time.dat file failed\r\n");
		return gps_time_fd;
	}

	//创建缓存日志数据的FIFO
	result = pipe(pipe_fd);
	if(result < 0) {
		fprintf(stderr, "create pipe failed\r\n");
		return result;
	}

	//创建写FIFO的互斥锁
	if(pthread_mutex_init(&pipe_lock, NULL) < 0) {
		fprintf(stderr, "create pipe lock failed\r\n");
		return -1;
	}

	//创建存储日志的线程
	if(pthread_create(&thread, NULL, save_thread, NULL) < 0) {
		fprintf(stderr, "create save_thread failed\r\n");
		return -1;
	}
	if(pthread_detach(thread) < 0) {
		fprintf(stderr, "detach save_thread failed\r\n");
		return -1;
	}
	//创建清理过期日志的线程
	if(pthread_create(&thread, NULL, clear_thread, NULL) < 0) {
		fprintf(stderr, "create clear_thread failed\r\n");
		return -1;
	}
	if(pthread_detach(thread) < 0) {
		fprintf(stderr, "detach clear_thread failed\r\n");
		return -1;
	}

	//初始化描述符列表
	if(fds_init() < 0) {
		fprintf(stderr, "fds list init failed\r\n");
	}
	//创建套接字
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0) {
		fprintf(stderr, "apply socket failed\r\n");
		return -1;
	}
	//允许重用本地地址和端口
	optval = 1;
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		close(server_socket);
		return -1;
	}
	//绑定本地IP和端口
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
		close(server_socket);
		fprintf(stderr, "bind socket failed\r\n");
		return -1;
	}
	//开启监听
	if(listen(server_socket, 15) == -1) {
		close(server_socket);
		fprintf(stderr, "listen socket failed\r\n");
		return -1;
	}
	//添加到fds列表
	if(fds_add(server_socket, POLLIN) == -1) {
		close(server_socket);
		fprintf(stderr, "add socket to fds failed\r\n");
		return -1;
	}

	//创建服务器线程，处理客户端连接
	if(pthread_create(&thread, NULL, tcp_server_thread, NULL) < 0) {
		close(server_socket);
		fprintf(stderr, "create tcp_server_thread failed\r\n");
		return -1;
	}
	if(pthread_detach(thread) < 0) {
		close(server_socket);
		fprintf(stderr, "detach tcp_server_thread failed\r\n");
		return -1;
	}

	//置位初始化标志
	init_flag = 1;

	return 0;
}

void debug_print_raw(int32_t level, const char *fmt, ...)
{
	int lenght;
	char *dest_buffer;
	time_t time;
	time_str_t time_str;
	va_list args;

	//检查参数是否合法
	if(!fmt) {
		fprintf(stderr, "Invalid parameter\r\n");
		return;
	}

	/* 检查调试输出等级优先级是否高于全局输出等级，值越小，优先级越高 */
	if(debug_level < level) {
		return;
	}

	/* 计算字符串长度 */
	va_start(args, fmt);
	lenght = vsnprintf(NULL, 0, fmt, args) + 1;
	va_end(args);

	/* 分配内存 */
	dest_buffer = malloc(lenght);
	if(!dest_buffer) {
		fprintf(stderr, "alloc memory failed\r\n");
		return;
	}

	/* 将字符串格式到内存中 */
	va_start(args, fmt);
	vsnprintf(dest_buffer, lenght, fmt, args);
	va_end(args);

	if(init_flag) {
		/* 写入时间戳 */
		time = get_time();
		time_2_string(&time, &time_str);
		write(pipe_fd[1], time_str.str, strlen(time_str.str));
		write(pipe_fd[1], "\r\n", strlen("\r\n"));
		/* 写入调试字符串 */
		write(pipe_fd[1], dest_buffer, strlen(dest_buffer));
		/* 通过TCP输出数据 */
		tcp_printf(dest_buffer, strlen(dest_buffer));
	}
	/* 通过标准输出输出数据 */
	if(level <= ERROR_OUTPUT) {
		fprintf(stderr, "%s\r\n", dest_buffer);
	}

	//释放内存
	free(dest_buffer);
}

void debug_print_line(int32_t level, const char *lvl_name, const char *tag, const char *fmt, ...)
{
	int lenght;
	char *fmt_buffer;
	char *dest_buffer;
	time_t time;
	time_str_t time_str;
	va_list args;

	//检查参数是否合法
	if((!lvl_name) || (!tag) || (!fmt)) {
		fprintf(stderr, "Invalid parameter\r\n");
		return;
	}

	/* 检查调试输出等级优先级是否高于全局输出等级，值越小，优先级越高 */
	if(debug_level < level) {
		return;
	}

	//计算格式字符串长度
	lenght = snprintf(NULL, 0, "[%s/%s]%s\r\n", lvl_name, tag, fmt) + 1;

	//分配存储格式字符串的保持
	fmt_buffer = malloc(lenght);
	if(!fmt_buffer) {
		fprintf(stderr, "alloc memory failed\r\n");
		return;
	}

	//格式化格式字符串
	snprintf(fmt_buffer, lenght, "[%s/%s]%s\r\n", lvl_name, tag, fmt);

	//计算输出字符串长度
	va_start(args, fmt);
	lenght = vsnprintf(NULL, 0, fmt_buffer, args) + 1;
	va_end(args);

	//分配存储输出字符串的保持
	dest_buffer = malloc(lenght);
	if(!dest_buffer) {
		free(fmt_buffer);
		fprintf(stderr, "alloc memory failed\r\n");
		return;
	}

	/* 将字符串格式到内存中 */
	va_start(args, fmt);
	vsnprintf(dest_buffer, lenght, fmt_buffer, args);
	va_end(args);

	if(init_flag) {
		/* 写入时间戳 */
		time = get_time();
		time_2_string(&time, &time_str);
		write(pipe_fd[1], time_str.str, strlen(time_str.str));
		write(pipe_fd[1], "\r\n", strlen("\r\n"));
		/* 写入调试字符串 */
		write(pipe_fd[1], dest_buffer, strlen(dest_buffer));
		/* 通过TCP输出数据 */
		tcp_printf(dest_buffer, strlen(dest_buffer));
	}
	/* 通过标准输出输出数据 */
	if(level <= ERROR_OUTPUT) {
		fprintf(stderr, "%s", dest_buffer);
	}

	//释放内存
	free(fmt_buffer);
	free(dest_buffer);
}

void debug_print_set_level(int32_t level)
{
	debug_level = level;
}

void debug_print_new_file(void)
{
	new_flag = 1;
}
