/*
 * start_parameter.c
 *
 *  Created on: 2022年1月19日
 *      Author: Administrator
 */

#include "start_parameter.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "start_parameter.c,%d", __LINE__
#include "../debug_manage/debug.h"

static int32_t init_falg = 0;
static set_t set;

int32_t start_parameter_init(const char *path)
{
	int32_t result;

	set.work_path = path;
	set.max_entry = 200;
	result = set_init(&set);
	if(result < 0) {
		LOG_E("set init failed");
		return result;
	}

	init_falg = 1;
	return result;
}

static int32_t _start_parameter_get(start_parameter_t *start_parameter)
{
	uint32_t type;
	data_type_t data;
	start_parameter_t default_parameter = DEFAULT_START_PARAMETER;

	//设置默认值，若没有查找到相应的关键字则使用默认值
	*start_parameter = default_parameter;

	//补偿数据加载地址
	if((set_get(&set, "offset_load_address", &type, &data) >= 0) && (type == TYPE_U32)) {
		start_parameter->offset_load_address = data.u32;
	}
	//补偿数据文件
	if((set_get(&set, "offset_file_name", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->offset_file_name, data.str, sizeof(start_parameter->offset_file_name));
	}

	//固件加载地址
	if((set_get(&set, "firmware_load_addrss", &type, &data) >= 0) && (type == TYPE_U32)) {
		start_parameter->firmware_load_addrss = data.u32;
	}

	//ocm设备文件
	if((set_get(&set, "ocm_device_name", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->ocm_device_name, data.str, sizeof(start_parameter->ocm_device_name));
	}

	//天线配置数据路径
	if((set_get(&set, "config_path", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->config_path, data.str, sizeof(start_parameter->config_path));
	}

	//日志存储路径
	if((set_get(&set, "log_path", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->log_path, data.str, sizeof(start_parameter->log_path));
	}

	//上位机服务器网卡
	if((set_get(&set, "upper_computer_network_card", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->upper_net, data.str, sizeof(start_parameter->upper_net));
	}
	//OpenAMIP服务器网卡
	if((set_get(&set, "OpenAMIP_network_card", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->openAMIP_net, data.str, sizeof(start_parameter->openAMIP_net));
	}

	//程序工作目录
	if((set_get(&set, "work_path", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->work_path, data.str, sizeof(start_parameter->work_path));
	}

	//系统IP
	if((set_get(&set, "system_ip", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->system_ip, data.str, sizeof(start_parameter->system_ip));
	}
	//系统子网掩码
	if((set_get(&set, "system_mask", &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(start_parameter->system_mask, data.str, sizeof(start_parameter->system_mask));
	}

	return 0;
}

int32_t start_parameter_get(start_parameter_t *start_parameter)
{
	start_parameter_t default_parameter = DEFAULT_START_PARAMETER;

	if(!start_parameter) {
		LOG_E("parameter error");
		return -1;
	}

	if(init_falg) {
		if(_start_parameter_get(start_parameter) < 0) {
			*start_parameter = default_parameter;
		}
	}
	else {
		*start_parameter = default_parameter;
	}

	return 0;
}
