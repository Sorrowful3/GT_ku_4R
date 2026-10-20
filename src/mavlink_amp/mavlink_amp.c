/*
 * mavlink_amp.c
 *
 *  Created on: 2021年9月23日
 *      Author: Administrator
 */

#include "mavlink_amp.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "mavlink_amp.c,%d", __LINE__
#include "../debug_manage/debug.h"

typedef struct {
	//命令锁
	pthread_mutex_t mutex;
	//命令同步信号量
	sem_t semaphore;
	//命令结果
	uint8_t result;
}command_common_t;

static mavlink_new_transducer_result_t g_new_transducer_result;

//命令公共部分
//通用指令
static command_common_t correct_imu;
static command_common_t reboot_firmware;
static command_common_t update_firmware;
static command_common_t switch_firmware;
static command_common_t read_version;
static command_common_t switch_antenna;
static command_common_t set_report;
static command_common_t set_beacon;
static command_common_t request_new_transducer;
static command_common_t set_new_transducer;
static command_common_t switch_imu_hardware;
static command_common_t set_transducer;
static command_common_t set_beam_point;
static command_common_t self_check;
static command_common_t start_tracking;
static command_common_t stop_tracking;
static command_common_t set_monitor;
//高轨指令
static command_common_t config_beam_info;
static command_common_t config_tracking_parameter;
static command_common_t set_work_mode;
static command_common_t update_tle;
//低轨指令
static command_common_t currect_yaw;;
static command_common_t save_earth_point;
static command_common_t clear_earth_point;

//全局变量锁
static pthread_mutex_t variable_mutex;
//返回命令结果的消息队列
static mqd_t selfcheck_queue;
static mqd_t version_queue;
static mqd_t antenna_22b_pack_queue;
static mqd_t antenna_286b_pack_queue;
static mqd_t arm_22b_pack_queue;
static mqd_t arm_286b_pack_queue;
static mqd_t beacon_pack_queue;
static mqd_t transducer_pack_queue;
static mqd_t openamip_queue;
static mqd_t log_queue;
static mqd_t currect_complete_queue;
static mqd_t broadcast_queue;

static int ocm_fd;
static mavlink_channel_t chan;

static mavlink_firmware_reday_t firmware_reday;
static mavlink_report_beacon_state_t beacon_state;
static mavlink_report_imu_state_t imu_state;
static mavlink_report_antenna_state_t antenna_state;
static mavlink_report_transducer_state_t transducer_state;
static mavlink_report_machine_state_t machine_state;
static mavlink_report_satellite_info_t satellite_info;

static int32_t command_init(command_common_t *command)
{
	//创建同步信号量
	if(sem_init(&command->semaphore, 0, 0) < 0) {
		return -1;
	}
	//创建互斥锁
	if(pthread_mutex_init(&command->mutex, NULL) < 0) {
		return -1;
	}
	command->result = 0;

	return 0;
}

//等待命令执行完成
static int32_t command_wait(command_common_t *command, int32_t timeout)
{
	int32_t result;
	int32_t tmp1, tmp2;
	struct timespec ts;

	//等待命令回复
	if(timeout == 0) {
		result = sem_trywait(&command->semaphore);
	}
	else if(timeout > 0) {
		if(clock_gettime(CLOCK_REALTIME, &ts) < 0) {
			return -1;
		}
		tmp1 = timeout % 1000;
		tmp2 = timeout / 1000;
		ts.tv_nsec += tmp1 * 1000 * 1000;
		if(ts.tv_nsec > 1*1000*1000*1000) {
			ts.tv_nsec -= 1*1000*1000*1000;
			ts.tv_sec += 1;
		}
		ts.tv_sec += tmp2;
		result =  sem_timedwait(&command->semaphore, &ts);
	}
	else {
		result =  sem_wait(&command->semaphore);
	}
	//判断命令是否回复
	if(result < 0) {
		return -1;
	}
	//检查命令执行结果
	if(command->result != 0x00) {
		return -1;
	}

	return 0;
}

//命令执行完成
static int32_t command_completion(command_common_t *command, uint8_t result)
{
	command->result = result;
	return sem_post(&command->semaphore);
}

//命令参数复位
static void command_reset(command_common_t *command)
{
	while(sem_trywait(&command->semaphore) == 0);
	command->result = 0;
}

//加锁
static int32_t command_lock(command_common_t *command)
{
	int32_t result;

	result = pthread_mutex_lock(&command->mutex);
	if(result >= 0) {
		command_reset(command);
	}
	return result;
}

//解锁
static int32_t command_unlock(command_common_t *command)
{
	return pthread_mutex_unlock(&command->mutex);
}

static ssize_t mq_read(mqd_t mq, char *buffer, size_t size, int32_t timeout)
{
	int result;
	ssize_t lenght;
	struct timespec ts;
	int32_t tmp1, tmp2;

	if(timeout < 0) {
		lenght = mq_receive(mq, buffer, size, NULL);
		if(lenght < 0) {
			return lenght;
		}
	}
	else {
		result = clock_gettime(CLOCK_REALTIME, &ts);
		if(result != 0) {
			return result;
		}
		if(timeout > 0) {
			tmp1 = timeout % 1000;
			tmp2 = timeout / 1000;
			ts.tv_nsec += tmp1 * 1000 * 1000;
			if(ts.tv_nsec > 1*1000*1000*1000) {
				ts.tv_nsec -= 1*1000*1000*1000;
				ts.tv_sec += 1;
			}
			ts.tv_sec += tmp2;
		}
		lenght = mq_timedreceive(mq, buffer, size, NULL, &ts);
		if(lenght < 0) {
			return lenght;
		}
	}
	return lenght;
}

static int32_t mq_send_timeout(mqd_t mq, const char *buffer, size_t size, int32_t timeout)
{
	int result;
	struct timespec ts;
	int32_t tmp1, tmp2;

	if(timeout < 0) {
		result = mq_send(mq, buffer, size, 0);
		if(result < 0) {
			return result;
		}
	}
	else {
		result = clock_gettime(CLOCK_REALTIME, &ts);
		if(result != 0) {
			return result;
		}
		if(timeout > 0) {
			tmp1 = timeout % 1000;
			tmp2 = timeout / 1000;
			ts.tv_nsec += tmp1 * 1000 * 1000;
			if(ts.tv_nsec > 1*1000*1000*1000) {
				ts.tv_nsec -= 1*1000*1000*1000;
				ts.tv_sec += 1;
			}
			ts.tv_sec += tmp2;
		}
		result = mq_timedsend(mq, buffer, size, 0, &ts);
		if(result < 0) {
			return result;
		}
	}
	return 0;
}

static int32_t variable_lock(void)
{
	return pthread_mutex_lock(&variable_mutex);
}

static int32_t variable_unlock(void)
{
	return pthread_mutex_unlock(&variable_mutex);
}

static int32_t net_mask_check(const struct in_addr *mask)
{
	int32_t i;
	int32_t result;
	uint32_t net_mask = ntohl(mask->s_addr);

	if(net_mask == 0xFFFFFFFF) {
		return -1;
	}

	result = 0;
	for(i=31; i>=0; i--) {
		if(((net_mask >> i) & 0x01) != 0x01) {
			break;
		}
	}
	for(; i>=0; i--) {
		if(((net_mask >> i) & 0x01) == 0x01) {
			result = -1;
			break;
		}
	}

	return result;
}

static int32_t tcp_port_test(uint16_t port)
{
	int tcp_sock;
	int optval;
	struct sockaddr_in address;

	//申请一个TCP类型的套接字
	tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_sock < 0) {
		return -1;
	}
	//允许重用本地地址和端口
	optval = 1;
	if(setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		close(tcp_sock);
		return -1;
	}
	//尝试绑定本地IP和端口
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(bind(tcp_sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
		close(tcp_sock);
		return -1;
	}
	//尝试开启监听
	if(listen(tcp_sock, 15) == -1) {
		close(tcp_sock);
		return -1;
	}

	//测试结束需要关闭套接字
	close(tcp_sock);
	return 0;
}

static void *parse_thread(void *arg)
{
	uint8_t result;
	int32_t i;
	int32_t lenght;
	mavlink_message_t tx_msg;
	mavlink_message_t rx_msg;
	mavlink_status_t rx_state;
	//通用指令
	mavlink_correct_imu_result_t correct_imu_result;
	mavlink_request_imu_drift_t request_imu_drift;
	mavlink_imu_drift_t report_imu_drift;
	mavlink_restart_result_t restart_result;
	mavlink_update_firmware_result_t update_firmware_result;
	mavlink_switch_firmware_result_t switch_firmware_result;
	mavlink_read_version_t request_version;
	mavlink_report_version_t report_version;
	mavlink_switch_antenna_result_t switch_antenna_result;
	mavlink_set_report_result_t set_report_result;
	mavlink_set_beacon_result_t set_beacon_result;
	mavlink_set_transducer_result_t set_transducer_result;
	mavlink_set_beam_point_result_t set_beam_point_result;
	mavlink_self_check_result_t self_check_result;
	mavlink_start_tracking_result_t start_tracking_result;
	mavlink_stop_tracking_result_t stop_tracking_result;
	mavlink_set_monitor_result_t set_monitor_result;
	mavlink_set_product_info_t set_product_info;
	//高轨（闭环跟踪）
	mavlink_request_beam_parameter_t request_beam_parameter;
	mavlink_set_beam_parameter_result_t set_beam_parameter_result;
	mavlink_request_tracking_parameter_t request_tracking_parameter;
	mavlink_set_tracking_parameter_result_t set_tracking_parameter_result;
	mavlink_request_work_parameter_t request_work_parameter;
	mavlink_set_work_parameter_result_t set_work_parameter_result;
	//透传
	mavlink_passthrough_22b_pack_t antenna_22b_pack;
	mavlink_passthrough_286b_pack_t antenna_286b_pack;
	mavlink_beacon_pack_t beacon_pack;
	mavlink_transducer_pack_t transducer_pack;
	mavlink_arm_22b_pack_t arm_22b_pack;
	mavlink_arm_286b_pack_t arm_286b_pack;
	//低轨开环
	mavlink_currect_yaw_result_t currect_yaw_result;
	mavlink_currect_yaw_complete_t currect_yaw_complete;
	mavlink_save_earth_point_result_t save_earth_point_result;
	mavlink_clear_earth_point_result_t clear_earth_point_result;
	//ACU上报的调试信息
	mavlink_log_string_t log_string;
	//openamip字符串
	mavlink_openamip_string_t openamip_string;
	//配置参数
	tracking_info_t tracking_info;
	work_info_t work_info;
	beam_info_t beam_info;
	imu_drift_t imu_drift;
	product_info_t product_info;
	//接收缓存
	uint8_t rx_buffer[128];
	//缓存软件日期
	char software_data[32];

	memset(&rx_msg, 0, sizeof(rx_msg));
	memset(&rx_state, 0, sizeof(rx_state));
	while(1)
	{
		//接收数据
		memset(rx_buffer, 0, sizeof(rx_buffer));
		lenght = mavlink_chan_read(chan, rx_buffer, sizeof(rx_buffer));
		if(lenght <= 0) {
			continue;
		}
		for(i=0; i<lenght; i++) {
			//解析一个字节
			result = mavlink_parse_char(chan, rx_buffer[i], &rx_msg, &rx_state);
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
				case MAVLINK_MSG_ID_FIRMWARE_REDAY:
					//固件就绪
					mavlink_msg_firmware_reday_decode(&rx_msg, &firmware_reday);
					LOG_I_RAW("FIRMWARE_REDAY:"
							"antenna_name=%s;antenna_orb=%s;"
							"firmware_data=%s;firmware_version=%s;"
							"firmware_type=%d\r\n",
							firmware_reday.antenna_name, firmware_reday.antenna_orb,
							firmware_reday.firmware_data, firmware_reday.firmware_version,
							firmware_reday.firmware_type);
					break;
				case MAVLINK_MSG_ID_CORRECT_IMU_RESULT:
					//零漂校准启动结果
					mavlink_msg_correct_imu_result_decode(&rx_msg, &correct_imu_result);
					command_completion(&correct_imu, correct_imu_result.result);
					if(correct_imu_result.result == 0xff) {
						LOG_E("correct imu failed");
					}
					LOG_I_RAW("CORRECT_IMU_RESULT:result=%d\r\n", correct_imu_result.result);
					break;
				case MAVLINK_MSG_ID_REQUEST_IMU_DRIFT:
					//请求IMU零漂
					mavlink_msg_request_imu_drift_decode(&rx_msg, &request_imu_drift);
					if(config_manage_get_imu_drift(&imu_drift) < 0) {
						config_manage_default_imu_drift(&imu_drift);
					}
					mavlink_msg_imu_drift_pack_chan(0, 0, chan, &tx_msg,
							imu_drift.valid, imu_drift.gyro_x,
							imu_drift.gyro_y, imu_drift.gyro_z);
					mavlink_chan_write_pack(chan, &tx_msg);
					LOG_I_RAW("CORRECT_IMU_RESULT:mode=%d\r\n", request_imu_drift.mode);
					LOG_I_RAW("IMU_DRIFT:valid=%d,gyro_x=%0.12f,gyro_z=%0.12f,gyro_y=%0.12f\r\n",
							imu_drift.valid, imu_drift.gyro_x, imu_drift.gyro_y, imu_drift.gyro_z);
					break;
				case MAVLINK_MSG_ID_IMU_DRIFT:
					//上报零漂校准结果
					mavlink_msg_imu_drift_decode(&rx_msg, &report_imu_drift);
					if(report_imu_drift.valid == 0x00) {
						LOG_E("correct imu failed");
					}
					else {
						imu_drift.valid = 0x01;
						imu_drift.gyro_x = report_imu_drift.gyro_x;
						imu_drift.gyro_y = report_imu_drift.gyro_y;
						imu_drift.gyro_z = report_imu_drift.gyro_z;
						config_manage_set_imu_drift(&imu_drift);
					}
					LOG_I_RAW("IMU_DRIFT:valid=%d,gyro_x=%0.12f,gyro_z=%0.12f,gyro_y=%0.12f\r\n",
							imu_drift.valid, imu_drift.gyro_x, imu_drift.gyro_y, imu_drift.gyro_z);
					break;
				case MAVLINK_MSG_ID_RESTART_RESULT:
					//复位结果
					mavlink_msg_restart_result_decode(&rx_msg, &restart_result);
					command_completion(&reboot_firmware, restart_result.result);
					if(restart_result.result == 0xff) {
						LOG_E("reset failed");
					}
					LOG_I_RAW("RESTART_RESULT:result=%d\r\n", restart_result.result);
					break;
				case MAVLINK_MSG_ID_UPDATE_FIRMWARE_RESULT:
					//固件更新结果
					mavlink_msg_update_firmware_result_decode(&rx_msg, &update_firmware_result);
					command_completion(&update_firmware, update_firmware_result.result);
					if(update_firmware_result.result == 0xff) {
						LOG_E("reset failed");
					}
					LOG_I_RAW("UPDATE_FIRMWARE_RESULT:result=%d\r\n", update_firmware_result.result);
					break;
				case MAVLINK_MSG_ID_SWITCH_FIRMWARE_RESULT:
					//固件切换结果
					mavlink_msg_switch_firmware_result_decode(&rx_msg, &switch_firmware_result);
					command_completion(&switch_firmware, switch_firmware_result.result);
					if(switch_firmware_result.result == 0xff) {
						LOG_E("switch firmware failed");
					}
					LOG_I_RAW("SWITCH_FIRMWARE_RESULT:result=%d\r\n", switch_firmware_result.result);
					break;
				case MAVLINK_MSG_ID_READ_VERSION:
					//读取软件版本，core1的固件可以读取APP的版本
					mavlink_msg_read_version_decode(&rx_msg, &request_version);
					if(request_version.mode != 0x01) {
						//这里只能读取APP版本
						mavlink_msg_report_version_pack_chan(0, 0, chan, &tx_msg,
															0xff, "faile", "faile", "faile", "faile", 0x01);
					}
					else {
						//返回APP版本
						config_manage_get_product_info(&product_info);
						snprintf(software_data, sizeof(software_data), "%s;%s", __DATE__, __TIME__);
						mavlink_msg_report_version_pack_chan(0, 0, chan, &tx_msg,
								0x00, product_info.antenna_name,
								product_info.antenna_orb, software_data,
								APP_VERSION, 0x01);
					}
					mavlink_chan_write_pack(chan, &tx_msg);
					LOG_I_RAW("READ_VERSION:mode=%d\r\n", request_version.mode);
					break;
				case MAVLINK_MSG_ID_REPORT_VERSION:
					//上报版本
					mavlink_msg_report_version_decode(&rx_msg, &report_version);
					mq_send_timeout(version_queue, (char*)&report_version, sizeof(mavlink_report_version_t), 0);
					command_completion(&read_version, 0x00);
					if(report_version.result == 0xff) {
						LOG_E("read version failed");
					}
					LOG_I_RAW("REPORT_VERSION:result=%d\r\n", report_version.result);
					break;
				case MAVLINK_MSG_ID_SWITCH_ANTENNA_RESULT:
					//天线电源控制结果
					mavlink_msg_switch_antenna_result_decode(&rx_msg, &switch_antenna_result);
					command_completion(&switch_antenna, switch_antenna_result.result);
					if(switch_antenna_result.result == 0xff) {
						LOG_E("switch antenna failed");
					}
					LOG_I_RAW("SWITCH_ANTENNA_RESULT:result=%d\r\n", switch_antenna_result.result);
					break;
				case MAVLINK_MSG_ID_SET_REPORT_RESULT:
					//数据上报周期配置结果
					mavlink_msg_set_report_result_decode(&rx_msg, &set_report_result);
					command_completion(&set_report, set_report_result.result);
					if(set_report_result.result == 0xff) {
						LOG_E("set report failed");
					}
					LOG_I_RAW("SET_REPORT_RESULT:result=%d\r\n", set_report_result.result);
					break;
				case MAVLINK_MSG_ID_SET_BEACON_RESULT:
					//设置信标机结果
					mavlink_msg_set_beacon_result_decode(&rx_msg, &set_beacon_result);
					command_completion(&set_beacon, set_beacon_result.result);
					if(set_beacon_result.result == 0xff) {
						LOG_E("set beacon failed");
					}
					LOG_I_RAW("SET_BEACON_RESULT:result=%d\r\n", set_beacon_result.result);
					break;
				case MAVLINK_MSG_ID_SET_TRANSDUCER_RESULT:
					//设置变频器结果
					mavlink_msg_set_transducer_result_decode(&rx_msg, &set_transducer_result);
					command_completion(&set_transducer, set_transducer_result.result);
					if(set_transducer_result.result == 0xff) {
						LOG_E("set transducer failed");
					}
					LOG_I_RAW("SET_TRANSDUCER_RESULT:result=%d\r\n", set_transducer_result.result);
					break;
				case MAVLINK_MSG_ID_SET_BEAM_POINT_RESULT:
					//设置波束指向结果
					mavlink_msg_set_beam_point_result_decode(&rx_msg, &set_beam_point_result);
					command_completion(&set_beam_point, set_beam_point_result.result);
					if(set_beam_point_result.result == 0xff) {
						LOG_E("set beam point failed");
					}
					LOG_I_RAW("SET_BEAM_POINT_RESULT:result=%d\r\n", set_beam_point_result.result);
					break;
				case MAVLINK_MSG_ID_SELF_CHECK_RESULT:
					//自检结果
					mavlink_msg_self_check_result_decode(&rx_msg, &self_check_result);
					mq_send_timeout(selfcheck_queue, (char*)&self_check_result, sizeof(mavlink_self_check_result_t), 0);
					command_completion(&self_check, 0x00);
					if(self_check_result.result == 0xff) {
						LOG_E("self failed failed");
					}
					if(self_check_result.result == 0xfe) {
						printf("no device\n");
					}
					LOG_I_RAW("SELF_CHECK_RESULT:result=%d\r\n", self_check_result.result);
					break;
				case MAVLINK_MSG_ID_START_TRACKING_RESULT:
					//自动跟踪启动结果
					mavlink_msg_start_tracking_result_decode(&rx_msg, &start_tracking_result);
					command_completion(&start_tracking, start_tracking_result.result);
					if(start_tracking_result.result == 0xff) {
						LOG_E("start tracking failed");
					}
					LOG_I_RAW("START_TRACKING_RESULT:result=%d\r\n", start_tracking_result.result);
					break;
				case MAVLINK_MSG_ID_STOP_TRACKING_RESULT:
					//自动跟踪停止结果
					mavlink_msg_stop_tracking_result_decode(&rx_msg, &stop_tracking_result);
					command_completion(&stop_tracking, stop_tracking_result.result);
					if(stop_tracking_result.result == 0xff) {
						LOG_E("stop tracking failed");
					}
					LOG_I_RAW("STOP_TRACKING_RESULT:result=%d\r\n", stop_tracking_result.result);
					break;
				case MAVLINK_MSG_ID_SET_MONITOR_RESULT:
					//监测模式设置结果
					mavlink_msg_set_monitor_result_decode(&rx_msg, &set_monitor_result);
					command_completion(&set_monitor, set_monitor_result.result);
					if(set_monitor_result.result == 0xff) {
						LOG_E("set monitor failed");
					}
					LOG_I_RAW("SET_MONITOR_RESULT:result=%d\r\n", set_monitor_result.result);
					break;
				case MAVLINK_MSG_ID_SET_PRODUCT_INFO:
					//配置产品序列号和设备名称，其中设备名称仅用于APP
					mavlink_msg_set_product_info_decode(&rx_msg, &set_product_info);
					//存储产品信息
					strncpy(product_info.antenna_name, set_product_info.antenna_name, sizeof(product_info.antenna_name));
					strncpy(product_info.antenna_orb, set_product_info.antenna_orb, sizeof(product_info.antenna_orb));
					if(amp_set_product_info(&product_info) < 0) {
						//返回设置失败指令
						mavlink_msg_set_product_info_result_pack_chan(0, 0, chan, &tx_msg, 0xff);
					}
					else {
						//返回设置成功指令
						mavlink_msg_set_product_info_result_pack_chan(0, 0, chan, &tx_msg, 0x00);
					}
					mavlink_chan_write_pack(chan, &tx_msg);
					LOG_I_RAW("SET_PRODUCT_INFO:antenna_name=%s,antenna_orb=%s\r\n",
							product_info.antenna_name, product_info.antenna_orb);
					break;

				//高轨指令
				case MAVLINK_MSG_ID_REQUEST_BEAM_PARAMETER:
					mavlink_msg_request_beam_parameter_decode(&rx_msg, &request_beam_parameter);
					//请求卫星波束信息
					if(config_manage_get_beam_info(&beam_info) < 0) {
						config_manage_default_beam_info(&beam_info);
					}
					mavlink_msg_beam_parameter_pack_chan(0, 0, chan, &tx_msg,
							beam_info.sat_longitude, beam_info.sat_name,
							beam_info.rx_freq, beam_info.rx_polarity_type,
							beam_info.tx_freq, beam_info.tx_polarity_type,
							beam_info.rx_osc, beam_info.rx_gain,
							beam_info.tx_osc, beam_info.tx_gain,
							beam_info.sym, beam_info.rang,
							0x01);
					mavlink_chan_write_pack(chan, &tx_msg);
					LOG_I_RAW("BEAM_PARAMETER:sat_longitude=%f,sat_name=%s,rx_freq=%f,"
							"rx_polarity_type=%d,tx_freq=%f,tx_polarity_type=%d,"
							"rx_osc=%f,rx_gain=%d,tx_osc=%f,"
							"tx_gain=%d,sym=%f,rang=%d,"
							"issue_mode=%d\r\n",
							beam_info.sat_longitude, beam_info.sat_name, beam_info.rx_freq,
							beam_info.rx_polarity_type, beam_info.tx_freq, beam_info.tx_polarity_type,
							beam_info.rx_osc, beam_info.rx_gain, beam_info.tx_osc,
							beam_info.tx_gain, beam_info.sym, beam_info.rang,
							0x01);
					LOG_I_RAW("REQUEST_BEAM_PARAMETER:mode=%d\r\n", request_beam_parameter.mode);
					break;
				case MAVLINK_MSG_ID_SET_BEAM_PARAMETER_RESULT:
					//卫星波束信息设置结果
					mavlink_msg_set_beam_parameter_result_decode(&rx_msg, &set_beam_parameter_result);
					command_completion(&config_beam_info, set_beam_parameter_result.result);
					if(set_beam_parameter_result.result == 0xff) {
						LOG_E("set beam info failed");
					}
					LOG_I_RAW("SET_BEAM_PARAMETER_RESULT:result=%d\r\n", set_beam_parameter_result.result);
					break;
				case MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT:
					mavlink_msg_new_transducer_result_decode(&rx_msg, &g_new_transducer_result);
					command_completion(&request_new_transducer, 0);
					LOG_I_RAW("MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT\r\n");
					break;
				case MAVLINK_MSG_ID_REQUEST_TRACKING_PARAMETER:
					//请求自动跟踪参数
					mavlink_msg_request_tracking_parameter_decode(&rx_msg, &request_tracking_parameter);
					if(config_manage_get_tracking_info(&tracking_info) < 0) {
						config_manage_default_tracking_info(&tracking_info);
					}
					mavlink_msg_tracking_parameter_pack_chan(0, 0, chan, &tx_msg,
							tracking_info.beamwidth, tracking_info.dvb_coarse_threshold,
							tracking_info.dvb_refined_threshold, tracking_info.dvb_tracking_threshold,
							tracking_info.detection_coarse_threshold, tracking_info.detection_refined_threshold,
							tracking_info.detection_tracking_threshold, tracking_info.beacon_coarse_threshold,
							tracking_info.beacon_refined_threshold, tracking_info.beacon_tracking_threshold,
							tracking_info.coarse_linger_factor, tracking_info.refined_linger_factor,
							tracking_info.tracking_linger_factor, tracking_info.kp_az,
							tracking_info.kp_el, tracking_info.kp_pol,
							tracking_info.delta_az, tracking_info.delta_el,
							tracking_info.delta_pol, 0x01);
					mavlink_chan_write_pack(chan, &tx_msg);
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
							tracking_info.beamwidth, tracking_info.dvb_coarse_threshold,
							tracking_info.dvb_refined_threshold, tracking_info.dvb_tracking_threshold,
							tracking_info.detection_coarse_threshold, tracking_info.detection_refined_threshold,
							tracking_info.detection_tracking_threshold, tracking_info.beacon_coarse_threshold,
							tracking_info.beacon_refined_threshold, tracking_info.beacon_tracking_threshold,
							tracking_info.coarse_linger_factor, tracking_info.refined_linger_factor,
							tracking_info.tracking_linger_factor, tracking_info.kp_az,
							tracking_info.kp_el, tracking_info.kp_pol,
							tracking_info.delta_az, tracking_info.delta_el,
							tracking_info.delta_pol, 0x01);
					LOG_I_RAW("REQUEST_TRACKING_PARAMETER:mode=%d\r\n", request_tracking_parameter.mode);
					break;
				case MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT:
					//自动跟踪参数配置结果
					mavlink_msg_set_tracking_parameter_result_decode(&rx_msg, &set_tracking_parameter_result);
					command_completion(&config_tracking_parameter, set_tracking_parameter_result.result);
					if(set_tracking_parameter_result.result == 0xff) {
						LOG_E("set tracking parameter failed");
					}
					LOG_I_RAW("SET_TRACKING_PARAMETER_RESULT:result=%d\r\n", set_tracking_parameter_result.result);
					break;
				case MAVLINK_MSG_ID_REQUEST_WORK_PARAMETER:
					//请求工作模式
					mavlink_msg_request_work_parameter_decode(&rx_msg, &request_work_parameter);
					if(config_manage_get_work_info(&work_info) < 0) {
						config_manage_default_work_info(&work_info);
					}
					mavlink_msg_work_parameter_pack_chan(0, 0, chan, &tx_msg,
							work_info.work_mode, work_info.openamip_ip,
							work_info.openamip_mask, work_info.openamip_port,
							work_info.rolloff_factor, work_info.servicer_ip,
							work_info.servicer_mask, work_info.servicer_port,
							work_info.tracking_mode, work_info.manual_gps,
							work_info.manual_longitude, work_info.manual_latitude,
							work_info.manual_height, 0x01);
					mavlink_chan_write_pack(chan, &tx_msg);
					LOG_I_RAW("TRACKING_PARAMETER:work_mode=%d,openamip_ip=%s,"
							"openamip_mask=%s,openamip_port=%d,"
							"rolloff_factor=%f,servicer_ip=%s,"
							"servicer_mask=%s,servicer_port=%d,"
							"tracking_mode=%d,manual_gps=%d,"
							"manual_longitude=%f,manual_latitude=%f,"
							"manual_height=%f,issue_mode=%d\r\n",
							work_info.work_mode, work_info.openamip_ip,
							work_info.openamip_mask, work_info.openamip_port,
							work_info.rolloff_factor, work_info.servicer_ip,
							work_info.servicer_mask, work_info.servicer_port,
							work_info.tracking_mode, work_info.manual_gps,
							work_info.manual_longitude, work_info.manual_latitude,
							work_info.manual_height, 0x01);
					LOG_I_RAW("REQUEST_WORK_PARAMETER:mode=%d\r\n", request_work_parameter.mode);
					break;
				case MAVLINK_MSG_ID_SET_WORK_PARAMETER_RESULT:
					//工作模式配置结果
					mavlink_msg_set_work_parameter_result_decode(&rx_msg, &set_work_parameter_result);
					command_completion(&set_work_mode, set_work_parameter_result.result);
					if(set_work_parameter_result.result == 0xff) {
						LOG_E("set work_mode failed");
					}
					LOG_I_RAW("SET_WORK_PARAMETER_RESULT:result=%d\r\n", set_work_parameter_result.result);
					break;

				//暗室测试
				case MAVLINK_MSG_ID_PASSTHROUGH_22B_PACK:
					//固件上报的22B天线控制命令
					mavlink_msg_passthrough_22b_pack_decode(&rx_msg, &antenna_22b_pack);
					mq_send_timeout(antenna_22b_pack_queue, (char*)&antenna_22b_pack, sizeof(antenna_22b_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				case MAVLINK_MSG_ID_PASSTHROUGH_286B_PACK:
					//固件上报的286B天线控制命令
					mavlink_msg_passthrough_286b_pack_decode(&rx_msg, &antenna_286b_pack);
					mq_send_timeout(antenna_286b_pack_queue, (char*)&antenna_286b_pack, sizeof(antenna_286b_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				case MAVLINK_MSG_ID_BEACON_PACK:
					//固件上报的信标机控制命令
					mavlink_msg_beacon_pack_decode(&rx_msg, &beacon_pack);
					mq_send_timeout(beacon_pack_queue, (char*)&beacon_pack, sizeof(beacon_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				case MAVLINK_MSG_ID_TRANSDUCER_PACK:
					//固件上报的变频器控制命令
					mavlink_msg_transducer_pack_decode(&rx_msg, &transducer_pack);
					mq_send_timeout(transducer_pack_queue, (char*)&transducer_pack, sizeof(transducer_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				case MAVLINK_MSG_ID_ARM_22B_PACK:
					//固件上报的22B ARM控制命令
					mavlink_msg_arm_22b_pack_decode(&rx_msg, &arm_22b_pack);
					mq_send_timeout(arm_22b_pack_queue, (char*)&arm_22b_pack, sizeof(arm_22b_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				case MAVLINK_MSG_ID_ARM_286B_PACK:
					//固件上报的286B ARM控制命令
					mavlink_msg_arm_286b_pack_decode(&rx_msg, &arm_286b_pack);
					mq_send_timeout(antenna_286b_pack_queue, (char*)&arm_286b_pack, sizeof(arm_286b_pack), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;

				//低轨
				case MAVLINK_MSG_ID_CURRECT_YAW_RESULT:
					//天线航向角校准结果
					mavlink_msg_currect_yaw_result_decode(&rx_msg, &currect_yaw_result);
					command_completion(&currect_yaw, currect_yaw_result.result);
					if(currect_yaw_result.result == 0xff) {
						LOG_E("currect yaw failed");
					}
					LOG_I_RAW("CURRECT_YAW_RESULT:result=%d\r\n", currect_yaw_result.result);
					break;
				case MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE:
					mavlink_msg_currect_yaw_complete_decode(&rx_msg, &currect_yaw_complete);
					mq_send_timeout(currect_complete_queue, (char*)&currect_yaw_complete, sizeof(currect_yaw_complete), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					LOG_I_RAW("CURRECT_YAW_COMPLETE:yaw=%f\r\n", currect_yaw_complete.yaw);
					break;
				case MAVLINK_MSG_ID_SAVE_EARTH_POINT_RESULT:
					//存储地理指向结果
					mavlink_msg_save_earth_point_result_decode(&rx_msg, &save_earth_point_result);
					command_completion(&save_earth_point, save_earth_point_result.result);
					if(save_earth_point_result.result == 0xff) {
						LOG_E("save earth point failed");
					}
					LOG_I_RAW("SAVE_EARTH_POINT_RESULT:result=%d\r\n", save_earth_point_result.result);
					break;
				case MAVLINK_MSG_ID_CLEAR_EARTH_POINT_RESULT:
					//清除地理指向结果
					mavlink_msg_clear_earth_point_result_decode(&rx_msg, &clear_earth_point_result);
					command_completion(&clear_earth_point, clear_earth_point_result.result);
					if(clear_earth_point_result.result == 0xff) {
						LOG_E("clear earth point failed");
					}
					LOG_I_RAW("CLEAR_EARTH_POINT_RESULT:result=%d\r\n", clear_earth_point_result.result);
					break;

				//数据上报
				case MAVLINK_MSG_ID_REPORT_BEACON_STATE:
					//信标机工作状态上报
					variable_lock();
					mavlink_msg_report_beacon_state_decode(&rx_msg, &beacon_state);
					variable_unlock();
					break;
				case MAVLINK_MSG_ID_REPORT_IMU_STATE:
					//IMU工作状态上报
					variable_lock();
					mavlink_msg_report_imu_state_decode(&rx_msg, &imu_state);
					variable_unlock();
					break;
				case MAVLINK_MSG_ID_REPORT_ANTENNA_STATE:
					//天线工作状态上报
					variable_lock();
					mavlink_msg_report_antenna_state_decode(&rx_msg, &antenna_state);
					variable_unlock();
					break;
				case MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE:
					//变频器工作状态上报
					variable_lock();
					mavlink_msg_report_transducer_state_decode(&rx_msg, &transducer_state);
					variable_unlock();
					break;
				case MAVLINK_MSG_ID_REPORT_MACHINE_STATE:
					//整机工作状态上报
					variable_lock();
					mavlink_msg_report_machine_state_decode(&rx_msg, &machine_state);
					variable_unlock();
					break;
				case MAVLINK_MSG_ID_REPORT_SATELLITE_INFO:
					//卫星信息
					variable_lock();
					mavlink_msg_report_satellite_info_decode(&rx_msg, &satellite_info);
					variable_unlock();
					break;

				//调试输出
				case MAVLINK_MSG_ID_LOG_STRING:
					//调试输出字符串上报
					mavlink_msg_log_string_decode(&rx_msg, &log_string);
					mq_send_timeout(log_queue, (char*)&log_string, sizeof(mavlink_log_string_t), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					LOG_I_RAW("%s", log_string.str);
					break;

				//OpenAMIP
				case MAVLINK_MSG_ID_OPENAMIP_STRING:
					//OpenAMIP字符串上报
					mavlink_msg_openamip_string_decode(&rx_msg, &openamip_string);
					mq_send_timeout(openamip_queue, (char*)&openamip_string, sizeof(mavlink_openamip_string_t), 0);
					//把msg放入广播消息队列
					mq_send_timeout(broadcast_queue, (char*)&rx_msg, sizeof(rx_msg), 0);
					break;
				default:
					break;
				}
			}
			memset(&rx_msg, 0, sizeof(rx_msg));
			memset(&rx_state, 0, sizeof(rx_state));
		}
	}
	return NULL;
}

int32_t amp_init(const char *ocm_name)
{
	uint32_t i;
	pthread_t thread_handle;
	struct mq_attr queue_attr;
	command_common_t *table[] = {&correct_imu, &reboot_firmware, &update_firmware,
			&switch_firmware, &read_version, &switch_antenna,
			&set_report, &set_beacon, &request_new_transducer, &set_new_transducer, &switch_imu_hardware, &set_transducer,
			&set_beam_point, &self_check, &start_tracking,
			&stop_tracking, &set_monitor, &config_beam_info,
			&config_tracking_parameter, &set_work_mode, &update_tle,
			&currect_yaw, &save_earth_point, &clear_earth_point};

	//固件未就绪
	firmware_reday.firmware_type = 0xff;

	//初始化命令的公共数据，即互斥量和信号量
	for(i=0; i<sizeof(table)/sizeof(table[0]); i++) {
		if(command_init(table[i]) < 0) {
			LOG_E("command initializes failed");
			return -1;
		}
	}

	//全局变量互斥锁
	if(pthread_mutex_init(&variable_mutex, NULL) < 0) {
		LOG_E("create mutex failed");
		return -1;
	}

	//初始化自检同步消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_self_check_result_t);
	selfcheck_queue = mq_open("/selfcheck_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)selfcheck_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化版本读取消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_report_version_t);
	version_queue = mq_open("/version_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)version_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_passthrough_22b_pack_t);
	antenna_22b_pack_queue = mq_open("/antenna_22b_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)antenna_22b_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_passthrough_286b_pack_t);
	antenna_286b_pack_queue = mq_open("/antenna_286b_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)antenna_286b_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_arm_22b_pack_t);
	arm_22b_pack_queue = mq_open("/arm_22b_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)arm_22b_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_arm_286b_pack_t);
	arm_286b_pack_queue = mq_open("/arm_286b_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)arm_286b_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_beacon_pack_t);
	beacon_pack_queue = mq_open("/beacon_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)beacon_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化测试数据返回消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_transducer_pack_t);
	transducer_pack_queue = mq_open("/transducer_pack_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)transducer_pack_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化openamip消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_openamip_string_t);
	openamip_queue = mq_open("/openamip_string", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)openamip_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//初始化log消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_log_string_t);
	log_queue = mq_open("/log_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)log_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 1;
	queue_attr.mq_msgsize = sizeof(mavlink_currect_yaw_complete_t);
	currect_complete_queue = mq_open("/currect_complete_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)broadcast_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//所有需要广播出去的msg都放入此消息队列
	queue_attr.mq_flags = 0;
	queue_attr.mq_maxmsg = 128;
	queue_attr.mq_msgsize = sizeof(mavlink_message_t);
	broadcast_queue = mq_open("/broadcast_queue", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &queue_attr);
	if((int)broadcast_queue == -1) {
		LOG_E("create msg queue failed");
		return -1;
	}

	//打开ocm驱动
	ocm_fd = open(ocm_name, O_RDWR);
	if(ocm_fd < 0) {
		LOG_E("open ocm driver file faile");
		return -1;
	}
	//初始化mavlink通道
	if(mavlink_chan_alloc(&chan, ocm_fd, NORMAL_FD) < 0) {
		close(ocm_fd);
		LOG_E("mavlink chan failed");
		return -1;
	}

	//创建协议解析线程
	if(pthread_create(&thread_handle, NULL, parse_thread, NULL) != 0) {
		close(ocm_fd);
		LOG_E("create thread failed");
		return -1;
	}
	//脱离线程
	if(pthread_detach(thread_handle) != 0) {
		close(ocm_fd);
		LOG_E("detach thread failed");
		return -1;
	}

	return 0;
}

int32_t amp_firmware_ready(void)
{
	if(firmware_reday.firmware_type != 0xff) {
		//固件已经启动
		return 0;
	}
	else {
		//固件还未启动
		LOG_E("firmware no ready");
		return -1;
	}
}

int32_t amp_correct_imu(void)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&correct_imu);

	//打包命令为msg
	mavlink_msg_correct_imu_pack_chan(0, 0, chan, &msg, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&correct_imu);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&correct_imu, 1000);
	if(result < 0) {
		command_unlock(&correct_imu);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&correct_imu);

	return result;
}

int32_t amp_reboot(uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;

	//重启APP
	if(mode == 0x01) {
		//清空磁盘IO
		system("sync");
		//退出进程
		exit(0);
		//此时进程退出
	}
	//重启整个系统
	else if(mode == 0x03) {
		//清空磁盘IO
		system("sync");
		//复位系统
		system("reboot -n");
		//退出进程
		exit(0);
		//此时进程退出
	}
	else {
		//加锁
		command_lock(&reboot_firmware);

		//打包命令为msg
		mavlink_msg_restart_pack_chan(0, 0, chan, &msg, mode);
		//发送数据到固件
		result = mavlink_chan_write_pack(chan, &msg);
		if(result < 0) {
			command_unlock(&reboot_firmware);
			LOG_E("write command failed");
			return result;
		}

		//等待固件回复
		result = command_wait(&reboot_firmware, 1000);
		if(result < 0) {
			command_unlock(&reboot_firmware);
			LOG_E("wait command failed");
			return result;
		}

		//解锁
		command_unlock(&reboot_firmware);
	}

	return result;
}

int32_t amp_update_firmware(uint8_t mode, const char *path)
{
	int32_t result;
	product_info_t product_info;
	char buffer[256];

	//不是升级固件，或文件路径无效，或文件不存在
	if((!path) || (access(path, R_OK) < 0)) {
		LOG_E("Invalid parameter");
		return -1;
	}

	if(mode == 0x00) {
		//升级APP
		snprintf(buffer, sizeof(buffer), "cp %s ./AMP_APP.elf", path);
		system(buffer);
		system("chmod 0777 ./AMP_APP.elf");
		return 0;
	}
	//获取产品名称
	config_manage_get_product_info(&product_info);
	if(mode == 0x01) {
		//高轨（闭环控制）固件
		snprintf(buffer, sizeof(buffer), "cp %s /mnt/%s.bin.CLC", path, product_info.antenna_name);
		result = 0;
	}
	else if(mode == 0x02) {
		//低轨（开环控制固件）
		snprintf(buffer, sizeof(buffer), "cp %s /mnt/%s.bin.OLC", path, product_info.antenna_name);
		result = 0;
	}
	else if(mode == 0x03) {
		//暗室测试固件
		snprintf(buffer, sizeof(buffer), "cp %s /mnt/%s.bin.test", path, product_info.antenna_name);
		result = 0;
	}
	else {
		LOG_E("Invalid parameter");
		result = -1;
	}

	if(result >= 0) {
		//加锁
		command_lock(&update_firmware);
		//执行升级命令
		system("mount /dev/mmcblk1p1 /mnt");
		system(buffer);
		system("umount /dev/mmcblk1p1");
		//解锁
		command_unlock(&update_firmware);
	}

	return result;
}

int32_t amp_switch_firmware(uint8_t mode)
{
	int32_t result;
	product_info_t product_info;
	char buffer[256];
	char file_name[256];

	//获取产品名称
	config_manage_get_product_info(&product_info);
	if(mode == 0x01) {
		//高轨（闭环控制）固件
		snprintf(buffer, sizeof(buffer), "cp /mnt/%s.bin.CLC /mnt/firmware.bin", product_info.antenna_name);
		snprintf(file_name, sizeof(file_name), "/mnt/%s.bin.CLC", product_info.antenna_name);
		result = 0;
	}
	else if(mode == 0x02) {
		//低轨（开环控制固件）
		snprintf(buffer, sizeof(buffer), "cp /mnt/%s.bin.OLC /mnt/firmware.bin", product_info.antenna_name);
		snprintf(file_name, sizeof(file_name), "/mnt/%s.bin.OLC", product_info.antenna_name);
		result = 0;
	}
	else if(mode == 0x03) {
		//暗室测试固件
		snprintf(buffer, sizeof(buffer), "cp /mnt/%s.bin.test /mnt/firmware.bin", product_info.antenna_name);
		snprintf(file_name, sizeof(file_name), "/mnt/%s.bin.test", product_info.antenna_name);
		result = 0;
	}
	else {
		LOG_E("Invalid parameter");
		result = -1;
	}

	if(result >= 0) {
		//加锁
		command_lock(&switch_firmware);
		//挂载固件所在文件系统
		system("mount /dev/mmcblk1p1 /mnt");
		//检查文件是否存在
		if(access(file_name, R_OK) >= 0) {
			//进行固件切换
			system(buffer);
		}
		else {
			LOG_E("Invalid firmware");
			result = -1;
		}
		//卸载固件文件系统
		system("umount /dev/mmcblk1p1");
		//解锁
		command_unlock(&switch_firmware);
	}

	return result;
}

int32_t amp_read_version(uint8_t mode, version_t *version)
{
	int32_t xresult;
	mavlink_message_t msg;
	product_info_t product_info;

	if(!version) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//获取产品名称和序列号
	config_manage_get_product_info(&product_info);
	if(mode == 0x01) {
		//APP版本
		version->result = 0x00;
		strncpy(version->antenna_name, product_info.antenna_name, sizeof(version->antenna_name));
		strncpy(version->antenna_orb, product_info.antenna_orb, sizeof(version->antenna_orb));
		snprintf(version->software_data, sizeof(version->software_data), "%s;%s", __DATE__, __TIME__);
		strncpy(version->software_version, APP_VERSION, sizeof(version->software_version));
		version->software_type = 0x01;
	}
	else {
		//加锁
		command_lock(&read_version);

		//打包命令为msg
		mavlink_msg_read_version_pack_chan(0, 0, chan, &msg, mode);
		//发送数据到固件
		xresult = mavlink_chan_write_pack(chan, &msg);
		if(xresult < 0) {
			command_unlock(&read_version);
			LOG_E("write command failed");
			return xresult;
		}

		//等待固件回复
		xresult = command_wait(&read_version, 1000);
		if(xresult < 0) {
			command_unlock(&read_version);
			LOG_E("wait command failed");
			return xresult;
		}

		//从消息队列获取固件版本
		xresult = mq_read(version_queue, (char*)version, sizeof(version_t), 100);
		if(xresult < 0) {
			command_unlock(&read_version);
			LOG_E("read failed");
			return xresult;
		}
		//填充产品序列号，因为固件中没有存储序列号
		strncpy(version->antenna_orb, product_info.antenna_orb, sizeof(version->antenna_orb));
		//解锁
		command_unlock(&read_version);
	}
	return 0;
}

int32_t amp_switch_antenna(uint8_t TorR, uint8_t subset, uint8_t mode)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&switch_antenna);

	//打包命令为msg
	mavlink_msg_switch_antenna_pack_chan(0, 0, chan, &msg, TorR, subset, mode);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&switch_antenna);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&switch_antenna, 1000);
	if(result < 0) {
		command_unlock(&switch_antenna);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&switch_antenna);

	return 0;
}

int32_t amp_set_report(uint8_t type, uint32_t interval)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&set_report);

	//打包命令为msg
	mavlink_msg_set_report_pack_chan(0, 0, chan, &msg, type, interval);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_report);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_report, 1000);
	if(result < 0) {
		command_unlock(&set_report);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&set_report);

	return 0;
}

int32_t amp_set_beacon(beacon_parameter_t *beacon_parameter)
{
	int32_t result;
	mavlink_message_t msg;

	if(!beacon_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&set_beacon);

	//打包命令为msg
	mavlink_msg_set_beacon_pack_chan(0, 0, chan, &msg,
			beacon_parameter->freq, beacon_parameter->sym,
			beacon_parameter->rang, beacon_parameter->mode);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_beacon);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_beacon, 1000);
	if(result < 0) {
		command_unlock(&set_beacon);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&set_beacon);

	return 0;
}

int32_t amp_set_transducer(transducer_parameter_t *transducer_parameter)
{
	int32_t result;
	mavlink_message_t msg;

	if(!transducer_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&set_transducer);

	//打包命令为msg
	mavlink_msg_set_transducer_pack_chan(0, 0, chan, &msg,
			transducer_parameter->rx_osc, transducer_parameter->rx_gain,
			transducer_parameter->tx_osc, transducer_parameter->tx_gain);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_transducer);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_transducer, 1000);
	if(result < 0) {
		command_unlock(&set_transducer);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&set_transducer);

	return 0;
}

int32_t amp_set_beam_point(beam_point_t *beam_point)
{
	int32_t result;
	mavlink_message_t msg;

	if(!beam_point) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&set_beam_point);

	//打包命令为msg
	mavlink_msg_set_beam_point_pack_chan(0, 0, chan, &msg,
			beam_point->TorR, beam_point->polarity_type,
			beam_point->theta, beam_point->phi,
			beam_point->polarity, beam_point->freq);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_beam_point);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_beam_point, 1000);
	if(result < 0) {
		command_unlock(&set_beam_point);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&set_beam_point);

	return 0;
}

int32_t amp_self_check(uint8_t mode, check_result_t *check_result)
{
	int32_t result;
	ssize_t lenght;
	mavlink_message_t msg;

	if(!check_result) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&self_check);

	//打包命令为msg
	mavlink_msg_self_check_pack_chan(0, 0, chan, &msg, mode);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&self_check);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&self_check, 1000);
	if(result < 0) {
		command_unlock(&self_check);
		LOG_E("wait command failed");
		return result;
	}

	//从消息队列获取自检结果
	lenght = mq_read(selfcheck_queue, (char*)check_result, sizeof(check_result_t), 100);
	if(lenght != sizeof(check_result_t)) {
		command_unlock(&self_check);
		LOG_E("read failed");
		return -1;
	}

	//解锁
	command_unlock(&self_check);

	return 0;
}

int32_t amp_start_tracking(void)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&start_tracking);

	//打包命令为msg
	mavlink_msg_start_tracking_pack_chan(0, 0, chan, &msg, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&start_tracking);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&start_tracking, 1000);
	if(result < 0) {
		command_unlock(&start_tracking);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&start_tracking);

	return 0;
}

int32_t amp_stop_tracking(void)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&stop_tracking);

	//打包命令为msg
	mavlink_msg_stop_tracking_pack_chan(0, 0, chan, &msg, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&stop_tracking);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&stop_tracking, 1000);
	if(result < 0) {
		command_unlock(&stop_tracking);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&stop_tracking);

	return 0;
}

int32_t amp_set_monitor(uint8_t mode, uint8_t state)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&set_monitor);

	//打包命令为msg
	mavlink_msg_set_monitor_pack_chan(0, 0, chan, &msg, mode, state);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_monitor);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_monitor, 1000);
	if(result < 0) {
		command_unlock(&set_monitor);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&set_monitor);

	return 0;
}


int32_t amp_set_product_info(product_info_t *product_info)
{
	int32_t result;
	char sys_cmd[256];

	if(!product_info){
		LOG_E("Invalid parameter");
		return -1;
	}

	result = config_manage_set_product_info(product_info);
	if(result >= 0) {
		//修改固件名称
		system("mount /dev/mmcblk1p1 /mnt");
		snprintf(sys_cmd, sizeof(sys_cmd), "cp /mnt/*.bin.CLC /mnt/%s.bin.CLC", product_info->antenna_name);
		system(sys_cmd);
		snprintf(sys_cmd, sizeof(sys_cmd), "cp /mnt/*.bin.OLC /mnt/%s.bin.OLC", product_info->antenna_name);
		system(sys_cmd);
		snprintf(sys_cmd, sizeof(sys_cmd), "cp /mnt/*.bin.test /mnt/%s.bin.test", product_info->antenna_name);
		system(sys_cmd);
		system("umount /dev/mmcblk1p1");
	}

	return result;
}

int32_t amp_new_log_file(void)
{
	log_manage_new_file();

	return 0;
}

int32_t amp_config_beam_parameter(const beam_parameter_t *beam)
{
	int32_t result;
	mavlink_message_t msg;
	beam_info_t beam_info;

	if(!beam) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&config_beam_info);

	//打包命令为msg
	mavlink_msg_beam_parameter_pack_chan(0, 0, chan, &msg,
			beam->sat_longitude, beam->sat_name,
			beam->rx_freq, beam->rx_polarity_type,
			beam->tx_freq, beam->tx_polarity_type,
			beam->rx_osc, beam->rx_gain,
			beam->tx_osc, beam->tx_gain,
			beam->sym, beam->rang,
			0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&config_beam_info);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&config_beam_info, 1000);
	if(result < 0) {
		command_unlock(&config_beam_info);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&config_beam_info);

	//存储波束信息
	beam_info.sat_longitude = beam->sat_longitude;
	strncpy(beam_info.sat_name, beam->sat_name, sizeof(beam_info.sat_name));
	beam_info.rx_freq = beam->rx_freq;
	beam_info.rx_polarity_type = beam->rx_polarity_type;
	beam_info.tx_freq = beam->tx_freq;
	beam_info.tx_polarity_type = beam->tx_polarity_type;
	beam_info.rx_osc = beam->rx_osc;
	beam_info.rx_gain = beam->rx_gain;
	beam_info.tx_osc = beam->tx_osc;
	beam_info.tx_gain = beam->tx_gain;
	beam_info.sym = beam->sym;
	beam_info.rang = beam->rang;
	result = config_manage_set_beam_info(&beam_info);
	if(result < 0) {
		LOG_E("save beam info failed");
		return result;
	}

	return 0;
}

int32_t amp_switch_imu(const mavlink_switch_imu_hardware_t *para)
{
	int32_t result;
	mavlink_message_t msg;

	if(!para) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&switch_imu_hardware);

	//打包命令为msg
	mavlink_msg_switch_imu_hardware_pack_chan(0, 0, chan, &msg,	para->hardware);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&switch_imu_hardware);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	// result = command_wait(&switch_imu_hardware, 1000);
	// if(result < 0) {
	// 	command_unlock(&switch_imu_hardware);
	// 	LOG_E("wait command failed");
	// 	return result;
	// }

	command_unlock(&switch_imu_hardware);

	return 0;
}

int32_t amp_set_new_transducer(const mavlink_set_new_transducer_t *para)
{
	int32_t result;
	mavlink_message_t msg;

	if(!para) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&set_new_transducer);

	//打包命令为msg
	mavlink_msg_set_new_transducer_pack_chan(0, 0, chan, &msg,
			para->rx_osc, para->rx_gain, para->tx_osc, para->tx_gain, para->cal_freq, para->channel);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_new_transducer);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	// result = command_wait(&set_new_transducer, 1000);
	// if(result < 0) {
	// 	command_unlock(&set_new_transducer);
	// 	LOG_E("wait command failed");
	// 	return result;
	// }

	command_unlock(&set_new_transducer);

	return 0;
}

int32_t amp_config_tracking_parameter(const tracking_parameter_t *tracking)
{
	int32_t result;
	mavlink_message_t msg;
	tracking_info_t tracking_parameter;

	if(!tracking) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&config_tracking_parameter);

	//打包命令为msg
	mavlink_msg_tracking_parameter_pack_chan(0, 0, chan, &msg,
			tracking->beamwidth, tracking->dvb_coarse_threshold,
			tracking->dvb_refined_threshold, tracking->dvb_tracking_threshold,
			tracking->detection_coarse_threshold, tracking->detection_refined_threshold,
			tracking->detection_tracking_threshold, tracking->beacon_coarse_threshold,
			tracking->beacon_refined_threshold, tracking->beacon_tracking_threshold,
			tracking->coarse_linger_factor, tracking->refined_linger_factor,
			tracking->tracking_linger_factor, tracking->kp_az,
			tracking->kp_el, tracking->kp_pol,
			tracking->delta_az, tracking->delta_el,
			tracking->delta_pol, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&config_tracking_parameter);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&config_tracking_parameter, 1000);
	if(result < 0) {
		command_unlock(&config_tracking_parameter);
		LOG_E("wait command failed");
		return result;
	}

	command_unlock(&config_tracking_parameter);

	//存储跟踪参数
	tracking_parameter.beamwidth = tracking->beamwidth;
	tracking_parameter.dvb_coarse_threshold = tracking->dvb_coarse_threshold;
	tracking_parameter.dvb_refined_threshold = tracking->dvb_refined_threshold;
	tracking_parameter.dvb_tracking_threshold = tracking->dvb_tracking_threshold;
	tracking_parameter.detection_coarse_threshold = tracking->detection_coarse_threshold;
	tracking_parameter.detection_refined_threshold = tracking->detection_refined_threshold;
	tracking_parameter.detection_tracking_threshold = tracking->detection_tracking_threshold;
	tracking_parameter.beacon_coarse_threshold = tracking->beacon_coarse_threshold;
	tracking_parameter.beacon_refined_threshold = tracking->beacon_refined_threshold;
	tracking_parameter.beacon_tracking_threshold = tracking->beacon_tracking_threshold;
	tracking_parameter.coarse_linger_factor = tracking->coarse_linger_factor;
	tracking_parameter.refined_linger_factor = tracking->refined_linger_factor;
	tracking_parameter.tracking_linger_factor = tracking->tracking_linger_factor;
	tracking_parameter.kp_az = tracking->kp_az;
	tracking_parameter.kp_el = tracking->kp_el;
	tracking_parameter.kp_pol = tracking->kp_pol;
	tracking_parameter.delta_az = tracking->delta_az;
	tracking_parameter.delta_el = tracking->delta_el;
	tracking_parameter.delta_pol = tracking->delta_pol;
	result = config_manage_set_tracking_info(&tracking_parameter);
	if(result < 0) {
		LOG_E("save beam tracking failed");
		return result;
	}

	return 0;
}

int32_t amp_set_work_parameter(const work_parameter_t *work_parameter)
{
	int32_t result;
	mavlink_message_t msg;
	work_info_t work_mode;
	struct in_addr openamip_ip;
	struct in_addr openamip_mask;
	struct in_addr servicer_ip;
	struct in_addr servicer_mask;

	if(!work_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//获取以前的配置
	if(config_manage_get_work_info(&work_mode) < 0) {
		config_manage_default_work_info(&work_mode);
	}

	//检查IP是否有效，并提取IP
	if((inet_aton(work_parameter->openamip_ip, &openamip_ip) == 0) ||
			(inet_aton(work_parameter->openamip_mask, &openamip_mask) == 0) ||
			(inet_aton(work_parameter->servicer_ip, &servicer_ip) == 0) ||
			(inet_aton(work_parameter->servicer_mask, &servicer_mask) == 0)) {
		LOG_E("IP Invalid");
		return -1;
	}
	//检查子网掩码是否合法
	if((net_mask_check(&openamip_mask) < 0) || (net_mask_check(&servicer_mask) < 0)) {
		LOG_E("network mask Invalid");
		return -1;
	}
	//OpenAMIP端口和上位机端口是否冲突
	if(work_parameter->openamip_port == work_parameter->servicer_port) {
		LOG_E("port conflict");
		return -1;
	}
	//OpenAMIP端口与系统中的其他端口是否冲突，与之前的配置不一样才检测
	if(work_parameter->openamip_port != work_mode.openamip_port) {
		if(tcp_port_test(work_parameter->openamip_port) < 0) {
			LOG_E("openamip port conflict");
			return -1;
		}
	}
	//上位机端口与系统中的其他端口是否冲突，与之前的配置不一样才检测
	if(work_parameter->servicer_port != work_mode.servicer_port) {
		if(tcp_port_test(work_parameter->servicer_port) < 0) {
			LOG_E("servicer port conflict");
			return -1;
		}
	}
	//检查IP是否冲突
	if((openamip_ip.s_addr == servicer_ip.s_addr) &&
			(openamip_mask.s_addr == servicer_mask.s_addr)) {
		LOG_E("IP conflict");
		return -1;
	}

	//加锁
	command_lock(&set_work_mode);

	//打包命令为msg
	mavlink_msg_work_parameter_pack_chan(0, 0, chan, &msg,
			work_parameter->work_mode, work_parameter->openamip_ip,
			work_parameter->openamip_mask, work_parameter->openamip_port,
			work_parameter->rolloff_factor, work_parameter->servicer_ip,
			work_parameter->servicer_mask, work_parameter->servicer_port,
			work_parameter->tracking_mode, work_parameter->manual_gps,
			work_parameter->manual_longitude, work_parameter->manual_latitude,
			work_parameter->manual_height, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&set_work_mode);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&set_work_mode, 1000);
	if(result < 0) {
		command_unlock(&set_work_mode);
		LOG_E("wait command failed");
		return result;
	}

	command_unlock(&set_work_mode);

	//存储工作模式
	work_mode.work_mode = work_parameter->work_mode;
	strncpy(work_mode.openamip_ip, work_parameter->openamip_ip, sizeof(work_mode.openamip_ip));
	strncpy(work_mode.openamip_mask, work_parameter->openamip_mask, sizeof(work_mode.openamip_mask));
	work_mode.openamip_port = work_parameter->openamip_port;
	work_mode.rolloff_factor = work_parameter->rolloff_factor;
	strncpy(work_mode.servicer_ip, work_parameter->servicer_ip, sizeof(work_mode.servicer_ip));
	strncpy(work_mode.servicer_mask, work_parameter->servicer_mask, sizeof(work_mode.servicer_mask));
	work_mode.servicer_port = work_parameter->servicer_port;
	work_mode.tracking_mode = work_parameter->tracking_mode;
	work_mode.manual_gps = work_parameter->manual_gps;
	work_mode.manual_longitude = work_parameter->manual_longitude;
	work_mode.manual_latitude = work_parameter->manual_latitude;
	work_mode.manual_height = work_parameter->manual_height;
	result = config_manage_set_work_info(&work_mode);
	if(result < 0) {
		LOG_E("save work info failed");
		return result;
	}

	return 0;
}

int32_t amp_update_tle(const char *path)
{
	if(!path) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//待实现
	return 0;
}

int32_t amp_issue_earth_point_local(const local_earth_point_t *earth_point)
{
	int32_t result;
	mavlink_message_t msg;

	if(!earth_point) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_issue_earth_point_local_pack_chan(0, 0, chan, &msg,
			earth_point->valid, earth_point->pitch,
			earth_point->phi, earth_point->polarity);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_set_satellite_info(const satellite_info_t *info)
{
	if(!info) {
		LOG_E("Invalid parameter");
		return -1;
	}

	variable_lock();
	satellite_info = *info;
	variable_unlock();

	return 0;
}

int32_t amp_antenna_22b_pack_write(const antenna_22b_pack_t *antenna_22b_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!antenna_22b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_passthrough_22b_pack_pack_chan(0, 0, chan, &msg, antenna_22b_pack->cmd);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_antenna_22b_pack_read(antenna_22b_pack_t *antenna_22b_pack)
{
	ssize_t lenght;

	if(!antenna_22b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(antenna_22b_pack_queue, (char*)antenna_22b_pack, sizeof(antenna_22b_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_antenna_286b_pack_write(const antenna_286b_pack_t *antenna_286b_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!antenna_286b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_passthrough_286b_pack_pack_chan(0, 0, chan, &msg,
			antenna_286b_pack->frameCnt, antenna_286b_pack->FCODE, antenna_286b_pack->cmd);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_antenna_286b_pack_read(antenna_286b_pack_t *antenna_286b_pack)
{
	ssize_t lenght;

	if(!antenna_286b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(antenna_286b_pack_queue, (char*)antenna_286b_pack, sizeof(antenna_286b_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_beacon_pack_write(const beacon_pack_t *beacon_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!beacon_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_beacon_pack_pack_chan(0, 0, chan, &msg, beacon_pack->cmd, beacon_pack->size);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_beacon_pack_read(beacon_pack_t *beacon_pack)
{
	ssize_t lenght;

	if(!beacon_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(beacon_pack_queue, (char*)beacon_pack, sizeof(beacon_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_transducer_pack_write(const transducer_pack_t *transducer_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!transducer_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_transducer_pack_pack_chan(0, 0, chan, &msg, transducer_pack->cmd, transducer_pack->size);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_transducer_pack_read(transducer_pack_t *transducer_pack)
{
	ssize_t lenght;

	if(!transducer_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(transducer_pack_queue, (char*)transducer_pack, sizeof(transducer_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_arm_22b_pack_write(const arm_22b_pack_t *arm_22b_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!arm_22b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_arm_22b_pack_pack_chan(0, 0, chan, &msg, arm_22b_pack->cmd);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_arm_22b_pack_read(arm_22b_pack_t *arm_22b_pack)
{
	ssize_t lenght;

	if(!arm_22b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(arm_22b_pack_queue, (char*)arm_22b_pack, sizeof(arm_22b_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_arm_286b_pack_write(const arm_286b_pack_t *arm_286b_pack)
{
	int32_t result;
	mavlink_message_t msg;

	if(!arm_286b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_arm_286b_pack_pack_chan(0, 0, chan, &msg,
			arm_286b_pack->frame_count, arm_286b_pack->code, arm_286b_pack->cmd);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_arm_286b_pack_read(arm_286b_pack_t *arm_286b_pack)
{
	ssize_t lenght;

	if(!arm_286b_pack) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(arm_286b_pack_queue, (char*)arm_286b_pack, sizeof(arm_286b_pack_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_currect_yaw(currect_yaw_t *currect)
{
	int32_t result;
	mavlink_message_t msg;

	if(!currect) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&currect_yaw);

	//打包命令为msg
	mavlink_msg_currect_yaw_pack_chan(0, 0, chan, &msg,
			currect->sat_longitude, currect->freq,
			currect->osc, currect->gain,
			currect->polarity_type, currect->sym,
			currect->rang, currect->mode,
			currect->beamwidth, currect->threshold_cnr,
			currect->linger_factor, currect->yaw);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&currect_yaw);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&currect_yaw, 5000);
	if(result < 0) {
		command_unlock(&currect_yaw);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&currect_yaw);

	return 0;
}

int32_t amp_currect_yaw_complete(currect_yaw_complete_t* currect_yaw_complete)
{
	ssize_t lenght;

	if(!currect_yaw_complete) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取数据包
	lenght = mq_read(currect_complete_queue, (char*)currect_yaw_complete, sizeof(currect_yaw_complete_t), -1);
	if(lenght < 0) {
		LOG_E("read failed");
		return lenght;
	}

	return 0;
}

int32_t amp_issue_earth_point(issue_earth_point_t *earth_point)
{
	int32_t result;
	mavlink_message_t msg;

	if(!earth_point) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//打包命令为msg
	mavlink_msg_issue_earth_point_pack_chan(0, 0, chan, &msg,
			earth_point->elevation, earth_point->azimuth,
			earth_point->polarity, earth_point->rx_freq,
			earth_point->rx_osc, earth_point->rx_gain,
			earth_point->rx_polarity_type, earth_point->tx_freq,
			earth_point->tx_osc, earth_point->tx_gain,
			earth_point->tx_polarity_type);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return 0;
}

int32_t amp_save_earth_point(save_earth_point_t *earth_point)
{
	int32_t result;
	mavlink_message_t msg;

	if(!earth_point) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//加锁
	command_lock(&save_earth_point);

	//打包命令为msg
	mavlink_msg_save_earth_point_pack_chan(0, 0, chan, &msg,
			earth_point->elevation, earth_point->azimuth,
			earth_point->polarity, earth_point->rx_freq,
			earth_point->rx_osc, earth_point->rx_gain,
			earth_point->rx_polarity_type, earth_point->tx_freq,
			earth_point->tx_osc, earth_point->tx_gain,
			earth_point->tx_polarity_type, earth_point->gps_week,
			earth_point->gps_second);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&save_earth_point);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&save_earth_point, 1000);
	if(result < 0) {
		command_unlock(&save_earth_point);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&save_earth_point);

	return 0;
}

int32_t amp_clear_earth_point(void)
{
	int32_t result;
	mavlink_message_t msg;

	//加锁
	command_lock(&clear_earth_point);

	//打包命令为msg
	mavlink_msg_clear_earth_point_pack_chan(0, 0, chan, &msg, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&clear_earth_point);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&clear_earth_point, 1000);
	if(result < 0) {
		command_unlock(&clear_earth_point);
		LOG_E("wait command failed");
		return result;
	}

	//解锁
	command_unlock(&clear_earth_point);

	return 0;
}

void amp_read_beacon_state(beacon_state_t *beacon)
{
	if(!beacon) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*beacon = beacon_state;
	variable_unlock();
}

void amp_read_imu_state(imu_state_t *imu)
{
	if(!imu) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*imu = imu_state;
	variable_unlock();
}

void amp_read_antenna_state(antenna_state_t *antenna)
{
	if(!antenna) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*antenna = antenna_state;
	variable_unlock();
}

void amp_read_transducer_state(transducer_state_t *transducer)
{
	if(!transducer) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*transducer = transducer_state;
	variable_unlock();
}

void amp_read_whole_machine_state(machine_state_t *work)
{
	if(!work) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*work = machine_state;
	variable_unlock();
}

void amp_read_satellite_info(satellite_info_t *info)
{
	if(!info) {
		LOG_E("Invalid parameter");
		return ;
	}

	variable_lock();
	*info = satellite_info;
	variable_unlock();
}

ssize_t amp_log_read(char *str, uint32_t size)
{
	int32_t result;
	mavlink_log_string_t log_string;

	if(!str) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取调试字符串
	result = mq_read(log_queue, (char*)&log_string, sizeof(log_string), -1);
	if(result < 0) {
		LOG_E("read failed");
		return result;
	}
	//拷贝字符串
	if(size > log_string.size) {
		size = log_string.size;
	}
	strncpy(str, log_string.str, size);

	return (int32_t)size;
}

ssize_t amp_openAmip_read(char *str, size_t size)
{
	int32_t result;
	mavlink_openamip_string_t openamip_string;

	if(!str) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取openAMIP字符串
	result = mq_read(openamip_queue, (char*)&openamip_string, sizeof(openamip_string), -1);
	if(result < 0) {
		LOG_E("read failed");
		return result;
	}
	//拷贝字符串
	if(size > openamip_string.size) {
		size = openamip_string.size;
	}
	strncpy(str, openamip_string.str, size);

	return (ssize_t)size;
}

ssize_t amp_openAmip_write(const char *str, size_t size)
{
	int32_t result;
	mavlink_message_t msg;

	if(!str) {
		LOG_E("Invalid parameter");
		return -1;
	}

	if(size > 200) {
		size = 200;
	}
	//打包命令为msg
	mavlink_msg_openamip_string_pack_chan(0, 0, chan, &msg, str, size);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		LOG_E("write command failed");
		return result;
	}

	return (ssize_t)size;
}

int32_t amp_broadcast_msg_read(mavlink_message_t *msg)
{
	int32_t result;

	if(!msg) {
		LOG_E("Invalid parameter");
		return -1;
	}

	//从消息队列获取需要广播的msg
	result = mq_read(broadcast_queue, (char*)msg, sizeof(mavlink_message_t), -1);
	if(result < 0) {
		LOG_E("read failed");
		return result;
	}

	return 0;
}

int32_t amp_get_new_transducer_parameter(mavlink_new_transducer_result_t *new_transducer)
{
	int32_t result;
	mavlink_message_t msg;

	if(!new_transducer) {
		LOG_E("Invalid parameter");
		return -1;
	}
	// config_manage_get_new_transducer_info(&para);

	//加锁
	command_lock(&request_new_transducer);

	new_transducer->rx_osc = 1;//g_new_transducer_result.rx_osc;
	new_transducer->cal_freq = 2;//g_new_transducer_result.cal_freq;
	new_transducer->state_lock = 0;//g_new_transducer_result.state_lock;

	//打包命令为msg
	mavlink_msg_request_new_transducer_pack_chan(0, 0, chan, &msg, 0x00);
	//发送数据到固件
	result = mavlink_chan_write_pack(chan, &msg);
	if(result < 0) {
		command_unlock(&request_new_transducer);
		LOG_E("write command failed");
		return result;
	}

	//等待固件回复
	result = command_wait(&request_new_transducer, 2000);
	if(result < 0) {
		command_unlock(&request_new_transducer);
		LOG_E("wait command failed");
		return result;
	}

	new_transducer->rx_osc = g_new_transducer_result.rx_osc;
	new_transducer->cal_freq = g_new_transducer_result.cal_freq;
	new_transducer->state_lock = g_new_transducer_result.state_lock;

	//解锁
	command_unlock(&request_new_transducer);

	return 0;
}

int32_t amp_get_tarcking_parameter(tracking_parameter_t *tracking_parameter)
{
	tracking_info_t tracking_info;

	if(!tracking_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	if(config_manage_get_tracking_info(&tracking_info) < 0) {
		config_manage_default_tracking_info(&tracking_info);
	}

	tracking_parameter->beamwidth = tracking_info.beamwidth;
	tracking_parameter->dvb_coarse_threshold = tracking_info.dvb_coarse_threshold;
	tracking_parameter->dvb_refined_threshold = tracking_info.dvb_refined_threshold;
	tracking_parameter->dvb_tracking_threshold = tracking_info.dvb_tracking_threshold;
	tracking_parameter->detection_coarse_threshold = tracking_info.detection_coarse_threshold;
	tracking_parameter->detection_refined_threshold = tracking_info.detection_refined_threshold;
	tracking_parameter->detection_tracking_threshold = tracking_info.detection_tracking_threshold;
	tracking_parameter->beacon_coarse_threshold = tracking_info.beacon_coarse_threshold;
	tracking_parameter->beacon_refined_threshold = tracking_info.beacon_refined_threshold;
	tracking_parameter->beacon_tracking_threshold = tracking_info.beacon_tracking_threshold;
	tracking_parameter->coarse_linger_factor = tracking_info.coarse_linger_factor;
	tracking_parameter->refined_linger_factor = tracking_info.refined_linger_factor;
	tracking_parameter->tracking_linger_factor = tracking_info.tracking_linger_factor;
	tracking_parameter->kp_az = tracking_info.kp_az;
	tracking_parameter->kp_el = tracking_info.kp_el;
	tracking_parameter->kp_pol = tracking_info.kp_pol;
	tracking_parameter->delta_az = tracking_info.delta_az;
	tracking_parameter->delta_el = tracking_info.delta_el;
	tracking_parameter->delta_pol = tracking_info.delta_pol;
	tracking_parameter->issue_mode = 0x00;

	return 0;
}

int32_t amp_get_work_parameter(work_parameter_t *work_parameter)
{
	work_info_t work_info;

	if(!work_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	if(config_manage_get_work_info(&work_info) < 0) {
		config_manage_default_work_info(&work_info);
	}

	work_parameter->work_mode = work_info.work_mode;
	strncpy(work_parameter->openamip_ip, work_info.openamip_ip, sizeof(work_info.openamip_ip));
	strncpy(work_parameter->openamip_mask, work_info.openamip_mask, sizeof(work_info.openamip_mask));
	work_parameter->openamip_port = work_info.openamip_port;
	work_parameter->rolloff_factor = work_info.rolloff_factor;
	strncpy(work_parameter->servicer_ip, work_info.servicer_ip, sizeof(work_info.servicer_ip));
	strncpy(work_parameter->servicer_mask, work_info.servicer_mask, sizeof(work_info.servicer_mask));
	work_parameter->servicer_port = work_info.servicer_port;
	work_parameter->tracking_mode = work_info.tracking_mode;
	work_parameter->manual_gps = work_info.manual_gps;
	work_parameter->manual_longitude = work_info.manual_longitude;
	work_parameter->manual_latitude = work_info.manual_latitude;
	work_parameter->manual_height = work_info.manual_height;
	work_parameter->issue_mode = 0x00;

	return 0;
}

int32_t amp_get_beam_parameter(beam_parameter_t *beam_parameter)
{
	beam_info_t beam_info;

	if(!beam_parameter) {
		LOG_E("Invalid parameter");
		return -1;
	}

	if(config_manage_get_beam_info(&beam_info) < 0) {
		config_manage_default_beam_info(&beam_info);
	}

	beam_parameter->sat_longitude = beam_info.sat_longitude;
	beam_parameter->rx_freq = beam_info.rx_freq;
	beam_parameter->tx_freq = beam_info.tx_freq;
	beam_parameter->rx_osc = beam_info.rx_osc;
	beam_parameter->tx_osc = beam_info.tx_osc;
	beam_parameter->sym = beam_info.sym;
	strncpy(beam_parameter->sat_name, beam_info.sat_name, sizeof(beam_parameter->sat_name));
	beam_parameter->rx_polarity_type = beam_info.rx_polarity_type;
	beam_parameter->tx_polarity_type = beam_info.tx_polarity_type;
	beam_parameter->rx_gain = beam_info.rx_gain;
	beam_parameter->tx_gain = beam_info.tx_gain;
	beam_parameter->rang = beam_info.rang;
	beam_parameter->issue_mode = 0x00;

	return 0;
}

int32_t amp_get_imu_drift(imu_drift_t *imu_drift)
{
	if(!imu_drift) {
		LOG_E("Invalid parameter");
		return -1;
	}

	return  config_manage_get_imu_drift(imu_drift);
}

int32_t amp_set_transmit_callbk(mavlink_chan_callbk_t *callbk)
{
	return  mavlink_chan_set_callbk(chan, callbk);
}
