/*
 * mavlink_client.c
 *
 *  Created on: 2021年10月29日
 *  Author: Administrator
 */

#include "mavlink_client.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "mavlink_client.c,%d", __LINE__
#include "../debug_manage/debug.h"

typedef struct{
	int sock;
	int32_t report_exit;
	uint32_t beacon_interval;
	uint32_t imu_interval;
	uint32_t antenna_interval;
	uint32_t transducer_interval;
	uint32_t work_interval;
	uint32_t sat_interval;
	mavlink_channel_t chan;
}client_info_t;

typedef struct {
	uint8_t fcode;
	uint8_t data[286];
}antenna_286b_cmd_t;

static pthread_mutex_t mutex;

static void client_correct_imu(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;

	//启动零漂校准
	result = amp_correct_imu();

	//打包命令
	if(result < 0) {
		mavlink_msg_correct_imu_result_pack_chan(0, 0, client->chan, &msg, 0xff);
	}
	else {
		mavlink_msg_correct_imu_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_request_imu_drift(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;
	imu_drift_t imu_drift;

	//读取IMU零漂
	result = amp_get_imu_drift(&imu_drift);

	//打包
	if(result < 0) {
		mavlink_msg_imu_drift_pack_chan(0, 0, client->chan, &msg,
				0x00, 0.0f,
				0.0f, 0.0f);
	}
	else {
		mavlink_msg_imu_drift_pack_chan(0, 0, client->chan, &msg,
				imu_drift.valid, imu_drift.gyro_x,
				imu_drift.gyro_y, imu_drift.gyro_z);
	}
	//发送
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_reboot(client_info_t *client, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;

	//执行重启操作
	result = amp_reboot(mode);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_restart_result_pack_chan(0, 0, client->chan, &msg, 0xff);
	}
	else {
		mavlink_msg_restart_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_update_firmware(client_info_t *client, uint8_t mode, const char *path)
{
	int32_t result;
	mavlink_message_t msg;

	//更新固件
	result = amp_update_firmware(mode, path);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_update_firmware_result_pack_chan(0, 0, client->chan, &msg, 0xff);
	}
	else {
		mavlink_msg_update_firmware_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_switch_firmware(client_info_t *client, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;

	//切换固件
	result = amp_switch_firmware(mode);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_switch_firmware_result_pack_chan(0, 0, client->chan, &msg, 0xff);
	}
	else {
		mavlink_msg_switch_firmware_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_read_version(client_info_t *client, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;
	version_t version;

	//读取版本
	result = amp_read_version(mode, &version);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_report_version_pack_chan(0, 0, client->chan, &msg,
				0xff, "failed",
				"failed", "failed",
				"failed", 0xff);
	}
	else {
		mavlink_msg_report_version_pack_chan(0, 0, client->chan, &msg,
				version.result, version.antenna_name,
				version.antenna_orb, version.software_data,
				version.software_version, version.software_type);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_switch_antenna(client_info_t *client, uint8_t TorR, uint8_t subset, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;

	//控制天线电源
	result = amp_switch_antenna(TorR, subset, mode);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_switch_antenna_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_switch_antenna_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_report(client_info_t *client, uint8_t type, uint32_t interval)
{
	int32_t result;
	mavlink_message_t msg;

	//设置数据上报周期
	if(type == 0x00) {
		client->beacon_interval = interval;
		client->imu_interval = interval;
		client->antenna_interval = interval;
		client->transducer_interval = interval;
		client->work_interval = interval;
		client->sat_interval = interval;
		result = 0;
	}
	else if(type == 0x01) {
		client->work_interval = interval;
		result = 0;
	}
	else if(type == 0x02) {
		client->transducer_interval = interval;
		result = 0;
	}
	else if(type == 0x03) {
		client->antenna_interval = interval;
		result = 0;
	}
	else if(type == 0x04) {
		client->imu_interval = interval;
		result = 0;
	}
	else if(type == 0x05) {
		client->beacon_interval = interval;
		result = 0;
	}
	else if(type == 0x06) {
		client->sat_interval = interval;
		result = 0;
	}
	else {
		result = -1;
	}

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_report_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_report_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_beacon(client_info_t *client, beacon_parameter_t *beacon_parameter)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_set_beacon(beacon_parameter);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_beacon_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_beacon_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_transducer(client_info_t *client, transducer_parameter_t *transducer_parameter)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_set_transducer(transducer_parameter);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_transducer_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_transducer_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_beam_point(client_info_t *client, beam_point_t *beam_point)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_set_beam_point(beam_point);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_beam_point_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_beam_point_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_self_check(client_info_t *client, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;
	check_result_t check_result;

	result = amp_self_check(mode, &check_result);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_self_check_result_pack_chan(0, 0, client->chan, &msg, 0xFF, "self check failed");
	}
	else {
		mavlink_msg_self_check_result_pack_chan(0, 0, client->chan, &msg, check_result.result, check_result.describe);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_start_tracking(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_start_tracking();

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_start_tracking_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_start_tracking_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_stop_tracking(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_stop_tracking();

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_stop_tracking_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_stop_tracking_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_monitor(client_info_t *client, uint8_t mode, uint8_t state)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_set_monitor(mode, state);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_monitor_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_monitor_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_new_log_file(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_new_log_file();

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_new_log_file_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_new_log_file_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}


static void client_request_beam_parameter(client_info_t *client)
{
	beam_parameter_t beam_parameter;
	mavlink_message_t msg;

	amp_get_beam_parameter(&beam_parameter);

	mavlink_msg_beam_parameter_pack_chan(0, 0, client->chan, &msg,
			beam_parameter.sat_longitude, beam_parameter.sat_name,
			beam_parameter.rx_freq, beam_parameter.rx_polarity_type,
			beam_parameter.tx_freq, beam_parameter.tx_polarity_type,
			beam_parameter.rx_osc, beam_parameter.rx_gain,
			beam_parameter.tx_osc, beam_parameter.tx_gain,
			beam_parameter.sym, beam_parameter.rang,
			0x01);
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_config_beam_parameter(client_info_t *client, beam_parameter_t *beam_parametere)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_config_beam_parameter(beam_parametere);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_beam_parameter_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_beam_parameter_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_request_new_transducer_parameter(client_info_t *client)
{
	mavlink_new_transducer_result_t new_transducer;
	mavlink_message_t msg;

	amp_get_new_transducer_parameter(&new_transducer);

	mavlink_msg_new_transducer_result_pack_chan(0, 0, client->chan, &msg, new_transducer.rx_osc, new_transducer.cal_freq, new_transducer.state_lock);
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_config_set_new_transducer(client_info_t *client, mavlink_set_new_transducer_t *para)
{
	// int32_t result;
	// mavlink_message_t msg;

	// result =
	amp_set_new_transducer(para);

	//打包命令为msg
	// if(result < 0) {
	// 	mavlink_msg_set_tracking_parameter_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	// }
	// else {
	// 	mavlink_msg_set_tracking_parameter_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	// }
	// //发送数据到客户端
	// mavlink_chan_write_pack(client->chan, &msg);
}

static void client_request_tracking_parameter(client_info_t *client)
{
	tracking_parameter_t tracking_parameter;
	mavlink_message_t msg;

	amp_get_tarcking_parameter(&tracking_parameter);

	mavlink_msg_tracking_parameter_pack_chan(0, 0, client->chan, &msg,
			tracking_parameter.beamwidth, tracking_parameter.dvb_coarse_threshold,
			tracking_parameter.dvb_refined_threshold, tracking_parameter.dvb_tracking_threshold,
			tracking_parameter.detection_coarse_threshold, tracking_parameter.detection_refined_threshold,
			tracking_parameter.detection_tracking_threshold, tracking_parameter.beacon_coarse_threshold,
			tracking_parameter.beacon_refined_threshold, tracking_parameter.beacon_tracking_threshold,
			tracking_parameter.coarse_linger_factor, tracking_parameter.refined_linger_factor,
			tracking_parameter.tracking_linger_factor, tracking_parameter.kp_az,
			tracking_parameter.kp_el, tracking_parameter.kp_pol,
			tracking_parameter.delta_az, tracking_parameter.delta_el,
			tracking_parameter.delta_pol, 0x01);
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_config_tracking_parameter(client_info_t *client, tracking_parameter_t *tracking)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_config_tracking_parameter(tracking);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_tracking_parameter_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_tracking_parameter_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_request_work_parameter(client_info_t *client)
{
	work_parameter_t work_parameter;
	mavlink_message_t msg;

	amp_get_work_parameter(&work_parameter);

	mavlink_msg_work_parameter_pack_chan(0, 0, client->chan, &msg,
			work_parameter.work_mode, work_parameter.openamip_ip,
			work_parameter.openamip_mask, work_parameter.openamip_port,
			work_parameter.rolloff_factor, work_parameter.servicer_ip,
			work_parameter.servicer_mask, work_parameter.servicer_port,
			work_parameter.tracking_mode, work_parameter.manual_gps,
			work_parameter.manual_longitude, work_parameter.manual_latitude,
			work_parameter.manual_height, 0x01);
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_set_work_parameter(client_info_t *client, work_parameter_t *work_parameter)
{
	int32_t result;
	mavlink_message_t msg;


	result = amp_set_work_parameter(work_parameter);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_set_work_parameter_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_set_work_parameter_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_update_tle(client_info_t *client, const char *path)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_update_tle(path);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_update_tle_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_update_tle_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_antenna_22b_pack_write(const antenna_22b_pack_t *antenna_22b_pack)
{
	amp_antenna_22b_pack_write(antenna_22b_pack);
}

static void client_antenna_286b_pack_write(antenna_286b_cmd_t *antenna_286b_cmd)
{
	antenna_286b_pack_t antenna_286b_pack;

	//加锁
	pthread_mutex_lock(&mutex);

	//分两次发送
	antenna_286b_pack.FCODE = antenna_286b_cmd->fcode;
	antenna_286b_pack.frameCnt = 1;
	memcpy(antenna_286b_pack.cmd, &antenna_286b_cmd->data[0], 143);
	amp_antenna_286b_pack_write(&antenna_286b_pack);

	antenna_286b_pack.FCODE = antenna_286b_cmd->fcode;
	antenna_286b_pack.frameCnt = 2;
	memcpy(antenna_286b_pack.cmd, &antenna_286b_cmd->data[143], 143);
	amp_antenna_286b_pack_write(&antenna_286b_pack);

	//解锁
	pthread_mutex_unlock(&mutex);
}

static void client_beacon_pack_write(const beacon_pack_t *beacon_pack)
{
	amp_beacon_pack_write(beacon_pack);
}

static void client_transducer_pack_write(const transducer_pack_t *transducer_pack)
{
	amp_transducer_pack_write(transducer_pack);
}

static void client_currect_yaw(client_info_t *client, currect_yaw_t *currect)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_currect_yaw(currect);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_currect_yaw_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_currect_yaw_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_issue_earth_point(issue_earth_point_t *earth_point)
{
	amp_issue_earth_point(earth_point);
}

static void client_save_earth_point(client_info_t *client, save_earth_point_t *earth_point)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_save_earth_point(earth_point);

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_save_earth_point_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_save_earth_point_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_clear_earth_point(client_info_t *client)
{
	int32_t result;
	mavlink_message_t msg;

	result = amp_clear_earth_point();

	//打包命令为msg
	if(result < 0) {
		mavlink_msg_clear_earth_point_result_pack_chan(0, 0, client->chan, &msg, 0xFF);
	}
	else {
		mavlink_msg_clear_earth_point_result_pack_chan(0, 0, client->chan, &msg, 0x00);
	}
	//发送数据到客户端
	mavlink_chan_write_pack(client->chan, &msg);
}

static void client_openAmip_write(const char *str, uint32_t size)
{
	amp_openAmip_write(str, size);
}

//上报设备工作状态
static void *report_thread(void *arg)
{
	uint32_t tick;
	client_info_t *client_info;
	mavlink_message_t tx_msg;

	beacon_state_t beacon;
	imu_state_t imu;
	antenna_state_t antenna;
	transducer_state_t transducer;
	machine_state_t machine;
	satellite_info_t sat;

	client_info = arg;
	tick = 0;
	while(!client_info->report_exit)
	{
		if((client_info->beacon_interval > 0) && (tick % client_info->beacon_interval == 0)) {
			//读取多模机数据
			amp_read_beacon_state(&beacon);
			mavlink_msg_report_beacon_state_pack_chan(0, 0, client_info->chan, &tx_msg,
					beacon.cnr_power, beacon.freq,
					beacon.sym, beacon.rang,
					beacon.mode);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}
		if((client_info->imu_interval > 0) && (tick % client_info->imu_interval == 0)) {
			//读取IMU数据
			amp_read_imu_state(&imu);
			mavlink_msg_report_imu_state_pack_chan(0, 0, client_info->chan, &tx_msg,
					imu.state, imu.pitch,
					imu.roll, imu.yaw,
					imu.x, imu.y,
					imu.z, imu.longitude,
					imu.latitude, imu.height,
					imu.speed, imu.temperature,
					imu.week, imu.second);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}
		if((client_info->antenna_interval > 0) && (tick % client_info->antenna_interval == 0)) {
			//读取天线数据
			amp_read_antenna_state(&antenna);
			mavlink_msg_report_antenna_state_pack_chan(0, 0, client_info->chan, &tx_msg,
					antenna.rx_phi, antenna.rx_theta,
					antenna.rx_polarity, antenna.rx_freq,
					antenna.rx_polarity_type, antenna.tx_phi,
					antenna.tx_theta, antenna.tx_polarity,
					antenna.tx_freq, antenna.tx_polarity_type,
					antenna.earth_azimuth, antenna.earth_elevation,
					antenna.earth_polarity);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}
		if((client_info->transducer_interval > 0) && (tick % client_info->transducer_interval == 0)) {
			//读取变频器数据
			amp_read_transducer_state(&transducer);
			mavlink_msg_report_transducer_state_pack_chan(0, 0, client_info->chan, &tx_msg,
					transducer.rx_osc, transducer.rx_gain,
					transducer.tx_osc, transducer.tx_gain,
					transducer.if_Value, transducer.rf_Value,
					transducer.lock_state, transducer.temperature,
					transducer.voltage, transducer.electricity);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}
		if((client_info->work_interval > 0) && (tick % client_info->work_interval == 0)) {
			//读取整机状态
			amp_read_whole_machine_state(&machine);
			mavlink_msg_report_machine_state_pack_chan(0, 0, client_info->chan, &tx_msg,
					machine.fault_code, machine.tracking,
					machine.rx_rf_state, machine.tx_rf_state,
					machine.acu_temperature, machine.acu_voltage,
					machine.acu_electricity, machine.tx_temperature,
					machine.tx_voltage, machine.tx_electricity,
					machine.rx_temperature, machine.rx_voltage,
					machine.rx_electricity);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}
		if((client_info->sat_interval > 0) && (tick % client_info->sat_interval == 0)) {
			//读取卫星信息
			amp_read_satellite_info(&sat);
			mavlink_msg_report_satellite_info_pack_chan(0, 0, client_info->chan, &tx_msg,
					sat.name, sat.longitude,
					sat.latitude, sat.altitude);
			mavlink_chan_write_pack(client_info->chan, &tx_msg);
		}

		usleep(1000);

		tick++;
	}
	return NULL;
}

//处理命令
static void *client_thread(void *arg)
{
	uint8_t result;
	int32_t i;
	int32_t lenght;
	void *xresult;
	client_info_t *client_info;
	pthread_t report;
	mavlink_message_t rx_msg;
	mavlink_status_t rx_state;
	//公共指令
	mavlink_request_imu_drift_t request_imu_drift;
	mavlink_restart_t restart;
	mavlink_update_firmware_t update_firmware;
	mavlink_switch_firmware_t switch_firmware;
	mavlink_read_version_t read_version;
	mavlink_switch_antenna_t switch_antenna;
	mavlink_set_report_t set_report;
	mavlink_set_beacon_t set_beacon;
	mavlink_set_transducer_t set_transducer;
	mavlink_set_beam_point_t set_beam_point;
	mavlink_self_check_t self_check;
	mavlink_set_monitor_t set_monitor;
	//高轨指令
	mavlink_request_beam_parameter_t request_beam_parameter;
	mavlink_beam_parameter_t beam_info;
	mavlink_request_new_transducer_t request_new_transducer;
	mavlink_switch_imu_hardware_t switch_imu;
	mavlink_set_new_transducer_t set_new_transducer;
	mavlink_request_tracking_parameter_t request_tracking_parameter;
	mavlink_tracking_parameter_t tracking_parameter;
	mavlink_request_work_parameter_t request_work_parameter;
	mavlink_work_parameter_t work_mode;
	mavlink_update_tle_t update_tle;
	//测试指令
	mavlink_passthrough_22b_pack_t antenna_22b_pack;
	mavlink_passthrough_286b_pack_t antenna_286b_pack;
	antenna_286b_cmd_t antenna_286b_cmd;
	mavlink_beacon_pack_t beacon_pack;
	mavlink_transducer_pack_t transducer_pack;
	//低轨指令
	mavlink_currect_yaw_t currect_yaw;
	mavlink_issue_earth_point_t issue_earth_point;
	mavlink_save_earth_point_t save_earth_point;
	//OpenAMIP
	mavlink_openamip_string_t openamip_string;
	//接收缓存
	uint8_t buffer[128];

	xresult = NULL;
	client_info = arg;
	memset(&rx_msg, 0, sizeof(rx_msg));
	memset(&rx_state, 0, sizeof(rx_state));

	//创建数据上报线程
	if(pthread_create(&report, NULL, report_thread, client_info) != 0) {
		xresult = (void *)-1;
		LOG_E("create report_thread failed");
		goto ERROR;
	}
	while(1)
	{
		//接收数据
		memset(buffer, 0, sizeof(buffer));
		lenght = mavlink_chan_read(client_info->chan, buffer, sizeof(buffer));
		//返回0表示对方关闭
		if(lenght == 0) {
			xresult = (void *)0;
			break;
		}
		//小于0表示出错，忽略超时错误
		if((lenght < 0) && (errno != EWOULDBLOCK)) {
			xresult = (void *)-1;
			LOG_E("read data failed");
			break;
		}

		for(i=0; i<lenght; i++) {
			//解析一个字节
			result = mavlink_parse_char(client_info->chan, buffer[i], &rx_msg, &rx_state);
			//正在接收命令
			if(result == MAVLINK_FRAMING_INCOMPLETE) {
				continue;
			}
			//命令错误
			if(result == MAVLINK_FRAMING_BAD_CRC || result == MAVLINK_FRAMING_BAD_SIGNATURE) {
				LOG_E("Command format error\n");
			}
			//接收完一帧命令
			if(result == MAVLINK_FRAMING_OK) {
				switch(rx_msg.msgid) {
				//通用指令
				case MAVLINK_MSG_ID_CORRECT_IMU:
					//校准IMU
					client_correct_imu(client_info);
					LOG_I_RAW("CORRECT_IMU\r\n");
					break;
				case MAVLINK_MSG_ID_REQUEST_IMU_DRIFT:
					//请求IMU零漂
					mavlink_msg_request_imu_drift_decode(&rx_msg, &request_imu_drift);
					client_request_imu_drift(client_info);
					LOG_I_RAW("REQUEST_IMU_DRIFT:mode=%d\r\n", request_imu_drift.mode);
					break;
				case MAVLINK_MSG_ID_RESTART:
					//复位
					mavlink_msg_restart_decode(&rx_msg, &restart);
					client_reboot(client_info, restart.mode);
					LOG_I_RAW("RESTART:mode=%d\r\n", restart.mode);
					break;
				case MAVLINK_MSG_ID_UPDATE_FIRMWARE:
					//更新固件
					mavlink_msg_update_firmware_decode(&rx_msg, &update_firmware);
					client_update_firmware(client_info, update_firmware.mode, update_firmware.path);
					LOG_I_RAW("UPDATE_FIRMWARE:mode=%d,path=%s\r\n", update_firmware.mode, update_firmware.path);
					break;
				case MAVLINK_MSG_ID_SWITCH_FIRMWARE:
					//切换固件
					mavlink_msg_switch_firmware_decode(&rx_msg, &switch_firmware);
					client_switch_firmware(client_info, switch_firmware.mode);
					LOG_I_RAW("SWITCH_FIRMWARE:mode=%d\r\n", switch_firmware.mode);
					break;
				case MAVLINK_MSG_ID_READ_VERSION:
					//读版本
					mavlink_msg_read_version_decode(&rx_msg, &read_version);
					client_read_version(client_info, read_version.mode);
					LOG_I_RAW("READ_VERSION:mode=%d\r\n", read_version.mode);
					break;
				case MAVLINK_MSG_ID_SWITCH_ANTENNA:
					//切换天线电源
					mavlink_msg_switch_antenna_decode(&rx_msg, &switch_antenna);
					client_switch_antenna(client_info, switch_antenna.TorR, switch_antenna.subset, switch_antenna.mode);

					LOG_I_RAW("SWITCH_ANTENNA:TorR=%d,subset=%d,mode=%d\r\n",
							switch_antenna.TorR, switch_antenna.subset, switch_antenna.mode);
					break;
				case MAVLINK_MSG_ID_SET_REPORT:
					//设置上报周期
					mavlink_msg_set_report_decode(&rx_msg, &set_report);
					client_set_report(client_info, set_report.type, set_report.interval);
					LOG_I_RAW("SET_REPORT:type=%d,interval=%d\r\n", set_report.type, set_report.interval);
					break;
				case MAVLINK_MSG_ID_SET_BEACON:
					//设置信标机
					mavlink_msg_set_beacon_decode(&rx_msg, &set_beacon);
					client_set_beacon(client_info, &set_beacon);
					LOG_I_RAW("SET_BEACON:freq=%f,sym=%f,rang=%d,mode=%d\r\n",
							set_beacon.freq, set_beacon.sym, set_beacon.rang, set_beacon.mode);
					break;
				case MAVLINK_MSG_ID_SET_TRANSDUCER:
					//设置变频器
					mavlink_msg_set_transducer_decode(&rx_msg, &set_transducer);
					client_set_transducer(client_info, &set_transducer);
					LOG_I_RAW("SET_TRANSDUCER:rx_osc=%f,rx_gain=%d,tx_osc=%f,tx_gain=%d\r\n",
							set_transducer.rx_osc, set_transducer.rx_gain,
							set_transducer.tx_osc, set_transducer.tx_gain);
					break;
				case MAVLINK_MSG_ID_SET_BEAM_POINT:
					//设置波束指向
					mavlink_msg_set_beam_point_decode(&rx_msg, &set_beam_point);
					client_set_beam_point(client_info, &set_beam_point);
					LOG_I_RAW("SET_BEAM_POINT:TorR=%d,polarity_type=%d,theta=%f,"
							"phi=%f,polarity=%f,freq=%f\r\n",
							set_beam_point.TorR, set_beam_point.polarity_type, set_beam_point.theta,
							set_beam_point.phi, set_beam_point.polarity, set_beam_point.freq);
					break;
				case MAVLINK_MSG_ID_SELF_CHECK:
					//自检
					mavlink_msg_self_check_decode(&rx_msg, &self_check);
					client_self_check(client_info, self_check.mode);
					LOG_I_RAW("SELF_CHECK:mode=%d\r\n", self_check.mode);
					break;
				case MAVLINK_MSG_ID_START_TRACKING:
					//启动跟踪
					client_start_tracking(client_info);
					LOG_I_RAW("START_TRACKING\r\n");
					break;
				case MAVLINK_MSG_ID_STOP_TRACKING:
					//停止跟踪
					client_stop_tracking(client_info);
					LOG_I_RAW("STOP_TRACKING\r\n");
					break;
				case MAVLINK_MSG_ID_SET_MONITOR:
					//设置监测模式
					mavlink_msg_set_monitor_decode(&rx_msg, &set_monitor);
					client_set_monitor(client_info, set_monitor.mode, set_monitor.state);
					LOG_I_RAW("SET_MONITOR:mode=%d,state=%d\r\n", set_monitor.mode, set_monitor.state);
					break;
				case MAVLINK_MSG_ID_SET_PRODUCT_INFO:
					//不能通过网络设置产品信息
					break;
				case MAVLINK_MSG_ID_NEW_LOG_FILE:
					//创建新的日志文件
					client_new_log_file(client_info);
					LOG_I_RAW("NEW_LOG_FILE\r\n");
					break;
				//高轨
				case MAVLINK_MSG_ID_REQUEST_BEAM_PARAMETER:
					//请求波束信息
					mavlink_msg_request_beam_parameter_decode(&rx_msg, &request_beam_parameter);
					client_request_beam_parameter(client_info);
					LOG_I_RAW("REQUEST_BEAM_PARAMETER:mode=%d\r\n", request_beam_parameter.mode);
					break;
				case MAVLINK_MSG_ID_BEAM_PARAMETER:
					//配置波束信息
					mavlink_msg_beam_parameter_decode(&rx_msg, &beam_info);
					client_config_beam_parameter(client_info, &beam_info);
					LOG_I_RAW("BEAM_PARAMETER:sat_longitude=%f,sat_name=%s,rx_freq=%f,"
							"rx_polarity_type=%d,tx_freq=%f,tx_polarity_type=%d,"
							"rx_osc=%f,rx_gain=%d,tx_osc=%f,"
							"tx_gain=%d,sym=%f,rang=%d,"
							"issue_mode=%d\r\n",
							beam_info.sat_longitude, beam_info.sat_name, beam_info.rx_freq,
							beam_info.rx_polarity_type, beam_info.tx_freq, beam_info.tx_polarity_type,
							beam_info.rx_osc, beam_info.rx_gain, beam_info.tx_osc,
							beam_info.tx_gain, beam_info.sym, beam_info.rang,
							beam_info.issue_mode);
					break;
				case MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE:
					LOG_E("--MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE\n");
					mavlink_msg_switch_imu_hardware_decode(&rx_msg, &switch_imu);
					amp_switch_imu(&switch_imu);
				break;
				case MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER:
					LOG_E("--REQUEST_NEW_TRANSDUCER\r\n");
					mavlink_msg_request_new_transducer_decode(&rx_msg, &request_new_transducer);
					client_request_new_transducer_parameter(client_info);
					LOG_E("REQUEST_NEW_TRANSDUCER:mode=%d\r\n", request_new_transducer.mode);
					break;
				case MAVLINK_MSG_ID_SET_NEW_TRANSDUCER:
					LOG_E("--SET_NEW_TRANSDUCER:\n");
					mavlink_msg_set_new_transducer_decode(&rx_msg, &set_new_transducer);
					client_config_set_new_transducer(client_info, &set_new_transducer);
					LOG_E("SET_NEW_TRANSDUCER:rx_osc=%f,tx_osc=%f,cal_freq=%f,rx_gain=%d,tx_gain=%d,channel=%d",
									set_new_transducer.rx_osc, set_new_transducer.tx_osc, set_new_transducer.cal_freq,
									set_new_transducer.rx_gain, set_new_transducer.tx_gain, set_new_transducer.channel);
					break;
				case MAVLINK_MSG_ID_REQUEST_TRACKING_PARAMETER:
					//请求跟踪参数
					mavlink_msg_request_tracking_parameter_decode(&rx_msg, &request_tracking_parameter);
					client_request_tracking_parameter(client_info);
					LOG_I_RAW("REQUEST_TRACKING_PARAMETER:mode=%d\r\n", request_tracking_parameter.mode);
					break;
				case MAVLINK_MSG_ID_TRACKING_PARAMETER:
					//配置跟踪参数
					mavlink_msg_tracking_parameter_decode(&rx_msg, &tracking_parameter);
					client_config_tracking_parameter(client_info, &tracking_parameter);
					LOG_I_RAW("TRACKING_PARAMETER:beamwidth=%f,dvb_coarse_threshold=%f,"
							"dvb_refined_threshold=%f,dvb_tracking_threshold=%f,"
							"detection_coarse_threshold=%f,detection_refined_threshold=%f,"
							"detection_tracking_threshold=%f,beacon_coarse_threshold=%f,"
							"beacon_refined_threshold=%f,beacon_tracking_threshold=%f,"
							"coarse_linger_factor=%f,refined_linger_factor=%f,"
							"tracking_linger_factor=%f,kp_az=%f,"
							"kp_el=%f,kp_pol=%f,"
							"delta_az=%f,delta_el=%f,"
							"delta_pol=%f,issue_mode=%d\r\n",
							tracking_parameter.beamwidth, tracking_parameter.dvb_coarse_threshold,
							tracking_parameter.dvb_refined_threshold, tracking_parameter.dvb_tracking_threshold,
							tracking_parameter.detection_coarse_threshold, tracking_parameter.detection_refined_threshold,
							tracking_parameter.detection_tracking_threshold, tracking_parameter.beacon_coarse_threshold,
							tracking_parameter.beacon_refined_threshold, tracking_parameter.beacon_tracking_threshold,
							tracking_parameter.coarse_linger_factor, tracking_parameter.refined_linger_factor,
							tracking_parameter.tracking_linger_factor, tracking_parameter.kp_az,
							tracking_parameter.kp_el, tracking_parameter.kp_pol,
							tracking_parameter.delta_az, tracking_parameter.delta_el,
							tracking_parameter.delta_pol, tracking_parameter.issue_mode);
					break;
				case MAVLINK_MSG_ID_REQUEST_WORK_PARAMETER:
					//请求工作模式
					mavlink_msg_request_work_parameter_decode(&rx_msg, &request_work_parameter);
					client_request_work_parameter(client_info);
					LOG_I_RAW("REQUEST_WORK_PARAMETER:mode=%d\r\n", request_work_parameter.mode);
					break;
				case MAVLINK_MSG_ID_WORK_PARAMETER:
					//配置工作模式
					mavlink_msg_work_parameter_decode(&rx_msg, &work_mode);
					client_set_work_parameter(client_info, &work_mode);
					LOG_I_RAW("TRACKING_PARAMETER:work_mode=%d,openamip_ip=%s,"
							"openamip_mask=%s,openamip_port=%d,"
							"rolloff_factor=%f,servicer_ip=%s,"
							"servicer_mask=%s,servicer_port=%d,"
							"tracking_mode=%d,manual_gps=%d,"
							"manual_longitude=%f,manual_latitude=%f,"
							"manual_height=%f,issue_mode=%d\r\n",
							work_mode.work_mode, work_mode.openamip_ip,
							work_mode.openamip_mask, work_mode.openamip_port,
							work_mode.rolloff_factor, work_mode.servicer_ip,
							work_mode.servicer_mask, work_mode.servicer_port,
							work_mode.tracking_mode, work_mode.manual_gps,
							work_mode.manual_longitude, work_mode.manual_latitude,
							work_mode.manual_height, work_mode.issue_mode);
					break;
				case MAVLINK_MSG_ID_UPDATE_TLE:
					//更新TLE文件
					mavlink_msg_update_tle_decode(&rx_msg, &update_tle);
					client_update_tle(client_info, update_tle.path);
					LOG_I_RAW("UPDATE_TLE:path=%d\r\n", update_tle.path);
					break;
				//暗室测试
				case MAVLINK_MSG_ID_PASSTHROUGH_22B_PACK:
					//22B透传命令
					mavlink_msg_passthrough_22b_pack_decode(&rx_msg, &antenna_22b_pack);
					client_antenna_22b_pack_write(&antenna_22b_pack);
					break;
				case MAVLINK_MSG_ID_PASSTHROUGH_286B_PACK:
					//286B透传命令
					mavlink_msg_passthrough_286b_pack_decode(&rx_msg, &antenna_286b_pack);
					if(antenna_286b_pack.frameCnt == 1) {
						//第一次只收到了前143字节
						antenna_286b_cmd.fcode = antenna_286b_pack.FCODE;
						memset(antenna_286b_cmd.data, 0, 286);
						memcpy(&antenna_286b_cmd.data[0], antenna_286b_pack.cmd, 143);
					}
					else if((antenna_286b_pack.frameCnt == 2) && (antenna_286b_cmd.fcode == antenna_286b_pack.FCODE)) {
						//第二次收到了剩余的143字节
						memcpy(&antenna_286b_cmd.data[143], antenna_286b_pack.cmd, 143);
						//处理286B命令
						client_antenna_286b_pack_write(&antenna_286b_cmd);
					}
					break;
				case MAVLINK_MSG_ID_BEACON_PACK:
					//信标机透传命令
					mavlink_msg_beacon_pack_decode(&rx_msg, &beacon_pack);
					client_beacon_pack_write(&beacon_pack);
					break;
				case MAVLINK_MSG_ID_TRANSDUCER_PACK:
					//变频器透传命令
					mavlink_msg_transducer_pack_decode(&rx_msg, &transducer_pack);
					client_transducer_pack_write(&transducer_pack);
					break;
				//低轨
				case MAVLINK_MSG_ID_CURRECT_YAW:
					//校准航向角
					mavlink_msg_currect_yaw_decode(&rx_msg, &currect_yaw);
					client_currect_yaw(client_info, &currect_yaw);
					LOG_E("CURRECT_YAW:sat_longitude=%f,freq=%f,"
							"osc=%f,gain=%d,"
							"polarity_type=%d,sym=%f,"
							"rang=%d,mode=%d,"
							"beamwidth=%f,threshold_cnr=%f,"
							"linger_factor=%f,yaw=%f\r\n",
							currect_yaw.sat_longitude, currect_yaw.freq,
							currect_yaw.osc, currect_yaw.gain,
							currect_yaw.polarity_type, currect_yaw.sym,
							currect_yaw.rang, currect_yaw.mode,
							currect_yaw.beamwidth, currect_yaw.threshold_cnr,
							currect_yaw.linger_factor, currect_yaw.yaw);
					break;
				case MAVLINK_MSG_ID_ISSUE_EARTH_POINT:
					//下发地理指向及波束参数
					mavlink_msg_issue_earth_point_decode(&rx_msg, &issue_earth_point);
					client_issue_earth_point(&issue_earth_point);
					break;
				case MAVLINK_MSG_ID_SAVE_EARTH_POINT:
					//存储地理指向及波束信息
					mavlink_msg_save_earth_point_decode(&rx_msg, &save_earth_point);
					client_save_earth_point(client_info, &save_earth_point);
					break;
				case MAVLINK_MSG_ID_CLEAR_EARTH_POINT:
					client_clear_earth_point(client_info);
					LOG_I_RAW("CLEAR_EARTH_POINT\r\n");
					break;
				//OpenAMIP
				case MAVLINK_MSG_ID_OPENAMIP_STRING:
					//OpenAMIP字符串
					mavlink_msg_openamip_string_decode(&rx_msg, &openamip_string);
					client_openAmip_write(openamip_string.str, openamip_string.size);
					break;
				default:
					break;
				}
			}
			memset(&rx_msg, 0, sizeof(rx_msg));
			memset(&rx_state, 0, sizeof(rx_state));
		}
	}
	//停止数据上报
	client_info->report_exit = 1;
	//单独数据上报线程结束
	pthread_join(report, NULL);
ERROR:
	//释放通道
	mavlink_chan_free(client_info->chan);
	//关闭套接字
	close(client_info->sock);
	//释放内存
	free(client_info);

	return xresult;
}

//广播暗室测试数据、OpenAMIP字符串、log字符串
static void *broadcast_thread(void *arg)
{
	mavlink_message_t tx_msg;

	while(1)
	{
		//读取广播msg
		if(amp_broadcast_msg_read(&tx_msg) < 0) {
			continue;
		}
		//广播到客户端
		mavlink_all_chan_write_pack(&tx_msg, SOCKET_FD);
	}
	return NULL;
}

int32_t mavlink_create_client(int sock)
{
    int flag;
	pthread_t thread;
	struct timeval timeout;
	client_info_t *client_info;

    //关闭Nagle算法
    flag = 1;
    if(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *) &flag, sizeof(int)) != 0) {
		LOG_E("set socket Nagle failed");
    	goto ERROR1;
    }
	//设置套接字接收超时,50ms
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000*500;
	if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) != 0) {
		LOG_E("set socket timeout failed");
		goto ERROR1;
	}
	//分配内存
	client_info = malloc(sizeof(client_info_t));
	if(!client_info) {
		LOG_E("alloc memory failed");
		goto ERROR1;
	}
	client_info->report_exit = 0;
	client_info->beacon_interval = 5;
	client_info->imu_interval = 5;
	client_info->antenna_interval = 5;
	client_info->transducer_interval = 100;
	client_info->work_interval = 100;
	client_info->sat_interval = 100;
	client_info->sock = sock;
	//初始化mavlink通道
	if(mavlink_chan_alloc(&client_info->chan, sock, SOCKET_FD) < 0) {
		LOG_E("alloc channel failed");
		goto ERROR2;
	}
	//创建客户端线程
	if(pthread_create(&thread, NULL, client_thread, client_info) != 0) {
		LOG_E("create client_thread failed");
		goto ERROR3;
	}
	if(pthread_detach(thread) != 0) {
		LOG_E("detach client_thread failed");
		goto ERROR3;
	}
	return 0;

ERROR3:
	mavlink_chan_free(client_info->chan);
ERROR2:
	free(client_info);
ERROR1:
	return -1;
}

int32_t mavlink_client_init(void)
{
	pthread_t broadcast;

	//全局互斥锁
	if(pthread_mutex_init(&mutex, NULL) < 0) {
		LOG_E("create mutex failed\n");
		return -1;
	}

	//数据广播线程
	if(pthread_create(&broadcast, NULL, broadcast_thread, NULL) != 0) {
		LOG_E("create broadcast_thread failed");
		return -1;
	}
	if(pthread_detach(broadcast) != 0) {
		LOG_E("detach broadcast_thread failed");
		return -1;
	}

	return 0;
}
