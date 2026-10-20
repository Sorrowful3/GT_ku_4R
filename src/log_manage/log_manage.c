/*
 * log_manage.c
 *
 *  Created on: 2022年2月14日
 *      Author: Administrator
 */

#include "log_manage.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "log_manage.c,%d", __LINE__
#include "../debug_manage/debug.h"

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
			snprintf(file_name, sizeof(file_name), "%s/%s.hex", log_path, str.str);

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

		//文件大于30M则创建一个新的文件
		if(file_size > 1024*1024*30) {
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
			//只考虑.hex文件
			temp = strstr(dirent->d_name, ".hex");
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
			strncpy(str.str, dirent->d_name, strstr(dirent->d_name, ".hex")-dirent->d_name);
			//将时间字符串转换为时间戳
			tmep_time = string_2_time(&str);

			//记录最先创建的文件以及创建时间
			if((uint32_t)old_time > (uint32_t)tmep_time) {
				old_time = tmep_time;
				strncpy(old_name, file_name, sizeof(old_name));
			}
		}
		closedir(dir);

		//累计大小超出600MB，则删除最先创建的文件
		if((total_size > (1024*1024*600)) && (old_time != 0xffffffff)) {
			unlink(old_name);
		}
	}

	return NULL;
}

static int32_t _log_write(const uint8_t *buffer, size_t lenght, uint8_t type)
{
	ssize_t result;
	log_frame_head_t frame_head;
	log_frame_stern_t frame_stern;

	//写入帧头
	frame_head.frame_head = 0xAA;
	frame_head.type = type;
	frame_head.lenght = lenght;
	result = write(pipe_fd[1], &frame_head, sizeof(frame_head));
	if(result != sizeof(frame_head)) {
		LOG_E("write frame head failed");
		return result;
	}

	//写入数据
	result = write(pipe_fd[1], buffer, lenght);
	if(result != lenght) {
		LOG_E("write data failed");
		return result;
	}

	//写入帧尾
	frame_stern.frame_stern = 0x55;
	result = write(pipe_fd[1], &frame_stern, sizeof(frame_stern));
	if(result != sizeof(frame_stern)) {
		LOG_E("write frame stern failed");
		return result;
	}

	return 0;
}

int32_t log_manage_init(const char *path)
{
	int result;
	DIR *dir;
	pthread_t thread;
	char file_name[300];
	char sys_cmd[256];

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
			LOG_E("create directory failed");
			return -1;
		}
	}
	//关闭打开的目录
	closedir(dir);

	//暂存log存储目录路径
	strncpy(log_path, path, sizeof(log_path));

	//打开存储GPS时间的文件
	snprintf(file_name, sizeof(file_name), "%s/log_gps_time.dat", log_path);
	gps_time_fd = open(file_name, O_RDWR|O_CREAT, S_IRWXU);
	if(gps_time_fd < 0) {
		LOG_E("create log_gps_time.dat file failed");
		return gps_time_fd;
	}

	//创建缓存日志数据的FIFO
	result = pipe(pipe_fd);
	if(result < 0) {
		LOG_E("create pipe failed");
		return result;
	}

	//创建写FIFO的互斥锁
	if(pthread_mutex_init(&pipe_lock, NULL) < 0) {
		LOG_E("create pipe lock failed");
		return -1;
	}

	//创建存储日志的线程
	if(pthread_create(&thread, NULL, save_thread, NULL) < 0) {
		LOG_E("create save_thread failed");
		return -1;
	}
	if(pthread_detach(thread) < 0) {
		LOG_E("detach save_thread failed");
		return -1;
	}
	//创建清理过期日志的线程
	if(pthread_create(&thread, NULL, clear_thread, NULL) < 0) {
		LOG_E("create clear_thread failed");
		return -1;
	}
	if(pthread_detach(thread) < 0) {
		LOG_E("detach clear_thread failed");
		return -1;
	}
	return 0;
}

int32_t log_write(const uint8_t *buffer, size_t lenght, uint8_t type)
{
	int32_t result;

	//获取互斥锁
	pthread_mutex_lock(&pipe_lock);

	//将数据写入FIFO
	result = _log_write(buffer, lenght, type);

	//释放互斥锁
	pthread_mutex_unlock(&pipe_lock);

	return result;
}

void log_manage_receive_save(const uint8_t *buffer, size_t lenght)
{
	log_write(buffer, lenght, 0x01);
}

void log_manage_send_save(const uint8_t *buffer, size_t lenght)
{
	log_write(buffer, lenght, 0x00);
}

void log_manage_new_file(void)
{
	new_flag = 1;
}
