/*
 * mavlink_amp.h
 *
 *  Created on: 2021年9月23日
 *      Author: Administrator
 */

#ifndef __MAVLINK_AMP_H_
#define __MAVLINK_AMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../mavlink_chan/mavlink_chan.h"
#include "../config_manage/config_manage.h"
#include "../log_manage/log_manage.h"

#define APP_VERSION  "v1.0.0.220418"

typedef mavlink_report_version_t version_t;
typedef mavlink_set_beacon_t beacon_parameter_t;
typedef mavlink_set_transducer_t transducer_parameter_t;
typedef mavlink_set_beam_point_t beam_point_t;
typedef mavlink_self_check_result_t check_result_t;
typedef mavlink_beam_parameter_t beam_parameter_t;
typedef mavlink_tracking_parameter_t tracking_parameter_t;
typedef mavlink_work_parameter_t work_parameter_t;
typedef mavlink_issue_earth_point_local_t local_earth_point_t;

typedef mavlink_passthrough_22b_pack_t antenna_22b_pack_t;
typedef mavlink_passthrough_286b_pack_t antenna_286b_pack_t;
typedef mavlink_beacon_pack_t beacon_pack_t;
typedef mavlink_transducer_pack_t transducer_pack_t;
typedef mavlink_arm_22b_pack_t arm_22b_pack_t;
typedef mavlink_arm_286b_pack_t arm_286b_pack_t;

typedef mavlink_currect_yaw_t currect_yaw_t;
typedef mavlink_currect_yaw_complete_t currect_yaw_complete_t;
typedef mavlink_issue_earth_point_t issue_earth_point_t;
typedef mavlink_save_earth_point_t save_earth_point_t;

typedef mavlink_report_beacon_state_t beacon_state_t;
typedef mavlink_report_imu_state_t imu_state_t;
typedef mavlink_report_antenna_state_t antenna_state_t;
typedef mavlink_report_transducer_state_t transducer_state_t;
typedef mavlink_report_machine_state_t machine_state_t;
typedef mavlink_report_satellite_info_t satellite_info_t;

int32_t amp_init(const char *ocm_name);

//通用指令
int32_t amp_firmware_ready(void);
int32_t amp_correct_imu(void);
int32_t amp_reboot(uint8_t mode);
int32_t amp_update_firmware(uint8_t mode, const char *pack);
int32_t amp_switch_firmware(uint8_t mode);
int32_t amp_read_version(uint8_t mode, version_t *version);
int32_t amp_switch_antenna(uint8_t TorR, uint8_t subset, uint8_t mode);
int32_t amp_set_report(uint8_t type, uint32_t interval);
int32_t amp_set_beacon(beacon_parameter_t *beacon_parameter);
int32_t amp_set_transducer(transducer_parameter_t *transducer_parameter);
int32_t amp_set_beam_point(beam_point_t *beam_point);
int32_t amp_self_check(uint8_t mode, check_result_t *check_result);
int32_t amp_start_tracking(void);
int32_t amp_stop_tracking(void);
int32_t amp_set_monitor(uint8_t mode, uint8_t state);
int32_t amp_set_product_info(product_info_t *product_info);
int32_t amp_new_log_file(void);

//高轨指令
int32_t amp_config_beam_parameter(const beam_parameter_t *beam);
int32_t amp_config_tracking_parameter(const tracking_parameter_t *tracking);
int32_t amp_set_work_parameter(const work_parameter_t *work_parameter);
int32_t amp_update_tle(const char *path);
int32_t amp_issue_earth_point_local(const local_earth_point_t *issue_earth_point);
int32_t amp_set_satellite_info(const satellite_info_t *info);

//暗室测试指令
int32_t amp_antenna_22b_pack_write(const antenna_22b_pack_t *antenna_22b_pack);
int32_t amp_antenna_22b_pack_read(antenna_22b_pack_t *antenna_22b_pack);
int32_t amp_antenna_286b_pack_write(const antenna_286b_pack_t *antenna_286b_pack);
int32_t amp_antenna_286b_pack_read(antenna_286b_pack_t *antenna_286b_pack);
int32_t amp_beacon_pack_write(const beacon_pack_t *beacon_pack);
int32_t amp_beacon_pack_read(beacon_pack_t *beacon_pack);
int32_t amp_transducer_pack_write(const transducer_pack_t *transducer_pack);
int32_t amp_transducer_pack_read(transducer_pack_t *transducer_pack);
int32_t amp_arm_22b_pack_write(const arm_22b_pack_t *arm_22b_pack);
int32_t amp_arm_22b_pack_read(arm_22b_pack_t *arm_22b_pack);
int32_t amp_arm_286b_pack_write(const arm_286b_pack_t *arm_286b_pack);
int32_t amp_arm_286b_pack_read(arm_286b_pack_t *arm_286b_pack);

//低轨指令
int32_t amp_currect_yaw(currect_yaw_t *currect);
int32_t amp_currect_yaw_complete(currect_yaw_complete_t* currect_yaw_complete);
int32_t amp_issue_earth_point(issue_earth_point_t *earth_point);
int32_t amp_save_earth_point(save_earth_point_t *earth_point);
int32_t amp_clear_earth_point(void);

//数据上报指令
void amp_read_beacon_state(beacon_state_t *beacon);
void amp_read_imu_state(imu_state_t *imu);
void amp_read_antenna_state(antenna_state_t *antenna);
void amp_read_transducer_state(transducer_state_t *transducer);
void amp_read_whole_machine_state(machine_state_t *work);
void amp_read_satellite_info(satellite_info_t *info);

//调试输出指令
ssize_t amp_log_read(char *str, uint32_t size);

//OpenAMIP指令
ssize_t amp_openAmip_read(char *str, size_t size);
ssize_t amp_openAmip_write(const char *str, size_t size);

//读取需要广播到上位机的msg
int32_t amp_broadcast_msg_read(mavlink_message_t *msg);

//获取配置参数
int32_t amp_get_tarcking_parameter(tracking_parameter_t *tracking_parameter);
int32_t amp_get_work_parameter(work_parameter_t *work_parameter);
int32_t amp_get_beam_parameter(beam_parameter_t *beam_parameter);
int32_t amp_get_imu_drift(imu_drift_t *imu_drift);

int32_t amp_get_new_transducer_parameter(mavlink_new_transducer_result_t *new_transducer);
int32_t amp_set_new_transducer(const mavlink_set_new_transducer_t *para);

int32_t amp_switch_imu(const mavlink_switch_imu_hardware_t *para);

//设置传输回调函数
int32_t amp_set_transmit_callbk(mavlink_chan_callbk_t *callbk);

#ifdef __cplusplus
}
#endif

#endif /* __MAVLINK_AMP_H_ */
