/*
 * main.c
 *
 *  Created on: 2021年9月23日
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "offset_load/offset_load.h"
#include "start_parameter/start_parameter.h"
#include "mavlink_chan/mavlink_chan.h"
#include "config_manage/config_manage.h"
#include "mavlink_amp/mavlink_amp.h"
#include "mavlink_socket/mavlink_server.h"
#include "openamip/openamip.h"
#include "log_manage/log_manage.h"
#include "debug_manage/debug_print.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "main.c,%d", __LINE__
#include "debug_manage/debug.h"

static mavlink_chan_callbk_t amp_transmit_callbk = {
    .read_callbk = log_manage_receive_save,
	.write_callbk = log_manage_send_save,
};

int main(int argc, char *argv[])
{
	work_parameter_t work_parameter;
	start_parameter_t start_parameter;
	imu_state_t imu;
	struct timespec time;
	char mac_addr[256];
	char sys_cmd[256];
	const char *start_config = "/home/root/XPHASED/start_parameter/";

	//获取启动参数配置文件路径
	if(argc > 1) {
		start_config = argv[1];
	}

	LOG_RAW("--linux APP start succeed, config file name %s\r\n", start_config);

	//忽略管道破裂信号
	if(signal(SIGPIPE, SIG_IGN) == (void*)SIG_ERR) {
		LOG_E("set signal failed");
		return -1;
	}

	//随机生成一组mac地址
	clock_gettime(CLOCK_REALTIME, &time);
	srand(time.tv_nsec);
	snprintf(mac_addr, sizeof(mac_addr), "ifconfig eth0 hw ether 00:0A:35:00:%02x:%02x", rand()%199+1, rand()%199+1);
	system("ifconfig eth0 down");
	system(mac_addr);
	system("ifconfig eth0 up");

	//加载启动参数
	if(start_parameter_init(start_config) < 0) {
		LOG_E("start parameter init failed");
		return -1;
	}

	start_parameter_get(&start_parameter);

	//配置系统IP
	snprintf(sys_cmd, sizeof(sys_cmd), "ifconfig eth0 %s netmask %s up",
			start_parameter.system_ip, start_parameter.system_mask);
	LOG_RAW("run:%s\r\n", sys_cmd);
	system(sys_cmd);

	//改变当前工作目录
	chdir(start_parameter.work_path);

	//初始化调试输出
	if(debug_print_init(start_parameter.log_path, 65500) < 0) {
		LOG_E("debug print init failed");
		return -1;
	}

	//加载交叉极化补偿数据
	if(strcmp("none", start_parameter.offset_file_name)) {
		if((argc <= 2) || ((argc > 2) && strcmp("restart", argv[2]))) {
			if(load_offse_data(start_parameter.offset_file_name, start_parameter.offset_load_address) < 0) {
				LOG_E("load offset data failed");
				return -1;
			}
		}
	}

	//加载驱动
	if(access(start_parameter.ocm_device_name, R_OK) != 0) {
		system("insmod  ocm_driver.ko");
		LOG_RAW("run:insmod  ocm_driver.ko\r\n");
	}

	//初始化配置参数管理
	if(config_manage_init(start_parameter.config_path) < 0) {
		LOG_E("config manage init failed");
		return -1;
	}

//	//复位系统配置
//	config_manage_restore_default();

	//初始化mavlink通道管理
	if(mavlink_chan_init() < 0) {
		LOG_E("mavlink chan init failed");
		return -1;
	}

	//初始化核间通信
	if(amp_init(start_parameter.ocm_device_name) < 0) {
		LOG_E("amp init failed");
		return -1;
	}

	//读取工作参数
	if(amp_get_work_parameter(&work_parameter) < 0) {
		LOG_E("get work parameter failed");
		return -1;
	}

	//配置OpenAMIP服务器IP
	if((strcmp(start_parameter.system_ip, work_parameter.openamip_ip) != 0) || (strcmp(start_parameter.system_mask, work_parameter.openamip_mask) != 0)) {
		snprintf(sys_cmd, sizeof(sys_cmd), "ifconfig %s %s netmask %s up",
				start_parameter.openAMIP_net, work_parameter.openamip_ip, work_parameter.openamip_mask);
		LOG_RAW("run:%s\r\n", sys_cmd);
		system(sys_cmd);
	}

	//配置上位机服务器IP
	if(((strcmp(start_parameter.system_ip, work_parameter.servicer_ip) != 0) || (strcmp(start_parameter.system_mask, work_parameter.servicer_mask) != 0)) &&
			((strcmp(work_parameter.openamip_ip, work_parameter.servicer_ip) != 0) || (strcmp(work_parameter.openamip_mask, work_parameter.servicer_mask) != 0))) {
		snprintf(sys_cmd, sizeof(sys_cmd), "ifconfig %s %s netmask %s up",
				start_parameter.upper_net, work_parameter.servicer_ip, work_parameter.servicer_mask);
		LOG_RAW("run:%s\r\n", sys_cmd);
		system(sys_cmd);
	}

	//初始化上位机服务器
	if(mavlink_server_init(work_parameter.servicer_port) < 0) {
		LOG_E("mavlink server init failed");
		return -1;
	}

	//初始化OpenAMIP服务器
	if(openamip_init(work_parameter.openamip_port) < 0) {
		LOG_E("mavlink server init failed");
		return -1;
	}

	//初始化日志管理
	if(log_manage_init(start_parameter.log_path) < 0) {
		LOG_E("log manage init failed");
		return -1;
	}
	//设置写日志函数
	if(amp_set_transmit_callbk(&amp_transmit_callbk) < 0) {
		LOG_E("set AMP transmit callbk failed");
		return -1;
	}

	//驱动CPU1的固件
	snprintf(sys_cmd, sizeof(sys_cmd), "./poke.out 0xfffffff0 0x%x", start_parameter.firmware_load_addrss);
	LOG_RAW("run:%s\r\n", sys_cmd);
	system(sys_cmd);

	//等待GPS就绪，然后重新创建日志文件
	for(; ; ) {
		amp_read_imu_state(&imu);
		if((imu.state & 0x90) == 0x90) {
			log_manage_new_file();
			break;
		}
		usleep(1000);
	}

	//主进程不退出
	while(1)
	{
		sleep(30);
	}
}
