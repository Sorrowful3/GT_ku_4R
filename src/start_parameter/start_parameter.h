/*
 * start_parameter.h
 *
 *  Created on: 2022Äê1ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef __START_PARAMETER_H_
#define __START_PARAMETER_H_

#ifdef __cplusplus
extern"C"{
#endif

#include"../set/set.h"

#define DEFAULT_START_PARAMETER							\
{														\
	.offset_load_address = 536870912,					\
	.offset_file_name = {"/usr/xphasedApp/VU5555.bin"},	\
	.firmware_load_addrss = 0x38000000,					\
	.ocm_device_name = {"/dev/ps_ocm"},					\
	.config_path = {"./antenna_config"},				\
	.log_path = {"./antenna_log"},						\
	.openAMIP_net = {"eth0:0"},							\
	.upper_net = {"eth0:1"},							\
	.work_path = {"/home/root/"},						\
	.system_ip = {"192.168.0.254"},						\
	.system_mask = {"255.255.255.0"},					\
}

typedef struct{
	uint32_t offset_load_address;
	char offset_file_name[64];
	uint32_t firmware_load_addrss;
	char ocm_device_name[64];
	char config_path[64];
	char log_path[64];
	char openAMIP_net[64];
	char upper_net[64];
	char work_path[64];
	char system_ip[64];
	char system_mask[64];
}start_parameter_t;

int32_t start_parameter_init(const char *path);

int32_t start_parameter_get(start_parameter_t *start_parameter);

#ifdef __cplusplus
}
#endif

#endif /* __START_PARAMETER_H_ */
