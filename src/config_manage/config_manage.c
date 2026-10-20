/*
 * config_manage.c
 *
 *  Created on: 2021年9月29日
 *      Author: Administrator
 */

#include "config_manage.h"

#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_TAG "config_manage.c,%d", __LINE__
#include "../debug_manage/debug.h"

static set_t set;
static pthread_mutex_t mutex;

static int32_t _config_manage_get_tracking_info(tracking_info_t *tracking_parameter)
{
	uint32_t type;
	data_type_t data;
	tracking_info_t default_info = DEFAULT_TRACKING_PARAMETER;

	//设置默认值，若没有匹配到相应的关键字则使用默认值
	*tracking_parameter = default_info;

	//天线波束宽度,度；盲扫、螺旋扫描时使用
	if((set_get(&set, BEAMWIDTH_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->beamwidth = data.f32;
	}

	//DVB粗对准阈值
	if((set_get(&set, DVB_COARSE_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->dvb_coarse_threshold = data.f32;
	}
	//DVB精对准阈值
	if((set_get(&set, DVB_REFINED_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->dvb_refined_threshold = data.f32;
	}
	//DVB动态跟踪阈值
	if((set_get(&set, DVB_TRACKING_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->dvb_tracking_threshold = data.f32;
	}

	//检波粗对准阈值
	if((set_get(&set, DETECTION_COARSE_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->detection_coarse_threshold = data.f32;
	}
	//检波精对准阈值
	if((set_get(&set, DETECTION_REFINED_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->detection_refined_threshold = data.f32;
	}
	//检波动态跟踪阈值
	if((set_get(&set, DETECTION_TRACKING_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->detection_tracking_threshold = data.f32;
	}

	//信标粗对准阈值
	if((set_get(&set, BEACON_COARSE_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->beacon_coarse_threshold = data.f32;
	}
	//信标精对准阈值
	if((set_get(&set, BEACON_REFINED_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->beacon_refined_threshold = data.f32;
	}
	//信标动态跟踪阈值
	if((set_get(&set, BEACON_TRACKING_THRESHOLD_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->beacon_tracking_threshold = data.f32;
	}

	//粗对准波束驻留时间
	if((set_get(&set, COARSE_LINGER_FACTOR_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->coarse_linger_factor = data.f32;
	}
	//精对准波束驻留时间
	if((set_get(&set, REFINED_LINGER_FACTOR_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->refined_linger_factor = data.f32;
	}
	//动态跟踪波束驻留时间
	if((set_get(&set, TRACKING_LINGER_FACTOR_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->tracking_linger_factor = data.f32;
	}

	//动态跟踪比例系数
	if((set_get(&set, KP_AZ_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->kp_az = data.f32;
	}
	//动态跟踪比例系数
	if((set_get(&set, KP_EL_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->kp_el = data.f32;
	}
	//动态跟踪比例系数
	if((set_get(&set, KP_POL_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->kp_pol = data.f32;
	}

	//动态跟踪偏离角度
	if((set_get(&set, DELTA_AZ_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->delta_az = data.f32;
	}
	//动态跟踪偏离角度
	if((set_get(&set, DELTA_EL_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->delta_el = data.f32;
	}
	//动态跟踪偏离角度
	if((set_get(&set, DELTA_POL_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		tracking_parameter->delta_pol = data.f32;
	}

	return 0;
}

static int32_t _config_manage_set_tracking_info(const tracking_info_t *tracking_parameter)
{
	data_type_t data;

	//天线波束宽度,度；盲扫、螺旋扫描时使用
	data.f32 = tracking_parameter->beamwidth;
	if(set_add_revise(&set, BEAMWIDTH_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", BEAMWIDTH_KEY);
		return -1;
	}

	//DVB粗对准阈值
	data.f32 = tracking_parameter->dvb_coarse_threshold;
	if(set_add_revise(&set, DVB_COARSE_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DVB_COARSE_THRESHOLD_KEY);
		return -1;
	}
	//DVB精对准阈值
	data.f32 = tracking_parameter->dvb_refined_threshold;
	if(set_add_revise(&set, DVB_REFINED_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DVB_REFINED_THRESHOLD_KEY);
		return -1;
	}
	//DVB动态跟踪阈值
	data.f32 = tracking_parameter->dvb_tracking_threshold;
	if(set_add_revise(&set, DVB_TRACKING_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DVB_TRACKING_THRESHOLD_KEY);
		return -1;
	}

	//检波粗对准阈值
	data.f32 = tracking_parameter->detection_coarse_threshold;
	if(set_add_revise(&set, DETECTION_COARSE_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DETECTION_COARSE_THRESHOLD_KEY);
		return -1;
	}
	//检波精对准阈值
	data.f32 = tracking_parameter->detection_refined_threshold;
	if(set_add_revise(&set, DETECTION_REFINED_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DETECTION_REFINED_THRESHOLD_KEY);
		return -1;
	}
	//检波动态跟踪阈值
	data.f32 = tracking_parameter->detection_tracking_threshold;
	if(set_add_revise(&set, DETECTION_TRACKING_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DETECTION_TRACKING_THRESHOLD_KEY);
		return -1;
	}

	//信标粗对准阈值
	data.f32 = tracking_parameter->beacon_coarse_threshold;
	if(set_add_revise(&set, BEACON_COARSE_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", BEACON_COARSE_THRESHOLD_KEY);
		return -1;
	}
	//信标精对准阈值
	data.f32 = tracking_parameter->beacon_refined_threshold;
	if(set_add_revise(&set, BEACON_REFINED_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", BEACON_REFINED_THRESHOLD_KEY);
		return -1;
	}
	//信标动态跟踪阈值
	data.f32 = tracking_parameter->beacon_tracking_threshold;
	if(set_add_revise(&set, BEACON_TRACKING_THRESHOLD_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", BEACON_TRACKING_THRESHOLD_KEY);
		return -1;
	}

	//粗对准波束驻留时间
	data.f32 = tracking_parameter->coarse_linger_factor;
	if(set_add_revise(&set, COARSE_LINGER_FACTOR_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", COARSE_LINGER_FACTOR_KEY);
		return -1;
	}
	//精对准波束驻留时间
	data.f32 = tracking_parameter->refined_linger_factor;
	if(set_add_revise(&set, REFINED_LINGER_FACTOR_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", REFINED_LINGER_FACTOR_KEY);
		return -1;
	}
	//动态跟踪波束驻留时间
	data.f32 = tracking_parameter->tracking_linger_factor;
	if(set_add_revise(&set, TRACKING_LINGER_FACTOR_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", TRACKING_LINGER_FACTOR_KEY);
		return -1;
	}

	//动态跟踪比例系数
	data.f32 = tracking_parameter->kp_az;
	if(set_add_revise(&set, KP_AZ_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", KP_AZ_KEY);
		return -1;
	}
	//动态跟踪比例系数
	data.f32 = tracking_parameter->kp_el;
	if(set_add_revise(&set, KP_EL_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", KP_EL_KEY);
		return -1;
	}
	//动态跟踪比例系数
	data.f32 = tracking_parameter->kp_pol;
	if(set_add_revise(&set, KP_POL_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", KP_POL_KEY);
		return -1;
	}

	//动态跟踪比例系数
	data.f32 = tracking_parameter->delta_az;
	if(set_add_revise(&set, DELTA_AZ_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DELTA_AZ_KEY);
		return -1;
	}
	//动态跟踪比例系数
	data.f32 = tracking_parameter->delta_el;
	if(set_add_revise(&set, DELTA_EL_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DELTA_EL_KEY);
		return -1;
	}
	//动态跟踪比例系数
	data.f32 = tracking_parameter->delta_pol;
	if(set_add_revise(&set, DELTA_POL_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", DELTA_POL_KEY);
		return -1;
	}

	//将数据同步到文件
	if(set_sync(&set) < 0) {
		LOG_E("sync tracking info failed");
		return -1;
	}

	return 0;
}

static int32_t _config_manage_get_work_info(work_info_t *work_mode)
{
	uint32_t type;
	data_type_t data;
	work_info_t default_info = DEFAULT_WORK_MODE;

	//先给一个默认值
	*work_mode = default_info;
	
	//天线工作模式
	if((set_get(&set, WORK_MODE_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		work_mode->work_mode = data.u8;
	}

	//OpenAMIP IP
	if((set_get(&set, OPENAMIP_IP_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(work_mode->openamip_ip, data.str, sizeof(work_mode->openamip_ip));
	}
	//OpenAMIP mask
	if((set_get(&set, OPENAMIP_MASK_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(work_mode->openamip_mask, data.str, sizeof(work_mode->openamip_mask));
	}
	//OpenAMIP 端口
	if((set_get(&set, OPENAMIP_PORT_KEY, &type, &data) >= 0) && (type == TYPE_U16)) {
		work_mode->openamip_port = data.u16;
	}
	//滚降系数
	if((set_get(&set, ROLLOFF_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		work_mode->rolloff_factor = data.f32;
	}

	//天线IP
	if((set_get(&set, SERVICER_IP_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(work_mode->servicer_ip, data.str, sizeof(work_mode->servicer_ip));
	}
	//天线mask
	if((set_get(&set, SERVICER_MASK_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(work_mode->servicer_mask, data.str, sizeof(work_mode->servicer_mask));
	}
	//天线端口
	if((set_get(&set, SERVICER_PORT_KEY, &type, &data) >= 0) && (type == TYPE_U16)) {
		work_mode->servicer_port = data.u16;
	}

	//跟踪模式，0DVB，1检波，2信标，3组合模式
	if((set_get(&set, TRACKING_MODE_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		work_mode->tracking_mode = data.u8;
	}

	//手动GPS
	if((set_get(&set, MANUAL_GPS_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		work_mode->manual_gps = data.u8;
	}
	//经度
	if((set_get(&set, MANUAL_LONGITUDE_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		work_mode->manual_longitude = data.f32;
	}
	//纬度
	if((set_get(&set, MANUAL_LATITUDE_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		work_mode->manual_latitude = data.f32;
	}
	//高度
	if((set_get(&set, MANUAL_HEIGHT_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		work_mode->manual_height = data.f32;
	}

	return 0;
}

static int32_t _config_manage_set_work_info(const work_info_t *work_mode)
{
	data_type_t data;
	
	//天线工作模式
	data.u8 = work_mode->work_mode;
	if(set_add_revise(&set, WORK_MODE_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", WORK_MODE_KEY);
		return -1;
	}

	//OpenAMIP IP
	strncpy(data.str, work_mode->openamip_ip, sizeof(data.str));
	if(set_add_revise(&set, OPENAMIP_IP_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", OPENAMIP_IP_KEY);
		return -1;
	}
	//OpenAMIP mask
	strncpy(data.str, work_mode->openamip_mask, sizeof(data.str));
	if(set_add_revise(&set, OPENAMIP_MASK_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", DELTA_POL_KEY);
		return -1;
	}
	//OpenAMIP 端口
	data.u16 = work_mode->openamip_port;
	if(set_add_revise(&set, OPENAMIP_PORT_KEY, TYPE_U16, &data) < 0) {
		LOG_E("set %s failed", OPENAMIP_PORT_KEY);
		return -1;
	}
	//滚降系数
	data.f32 = work_mode->rolloff_factor;
	if(set_add_revise(&set, ROLLOFF_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", ROLLOFF_KEY);
		return -1;
	}

	//天线IP
	strncpy(data.str, work_mode->servicer_ip, sizeof(data.str));
	if(set_add_revise(&set, SERVICER_IP_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", SERVICER_IP_KEY);
		return -1;
	}
	//天线mask
	strncpy(data.str, work_mode->servicer_mask, sizeof(data.str));
	if(set_add_revise(&set, SERVICER_MASK_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", SERVICER_MASK_KEY);
		return -1;
	}
	//天线端口
	data.u16 = work_mode->servicer_port;
	if(set_add_revise(&set, SERVICER_PORT_KEY, TYPE_U16, &data) < 0) {
		LOG_E("set %s failed", SERVICER_PORT_KEY);
		return -1;
	}

	//跟踪模式，0DVB，1检波，2信标，3组合
	data.u8 = work_mode->tracking_mode;
	if(set_add_revise(&set, TRACKING_MODE_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", TRACKING_MODE_KEY);
		return -1;
	}

	//手动GPS
	data.u8 = work_mode->manual_gps;
	if(set_add_revise(&set, MANUAL_GPS_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", MANUAL_GPS_KEY);
		return -1;
	}
	//经度
	data.f32 = work_mode->manual_longitude;
	if(set_add_revise(&set, MANUAL_LONGITUDE_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", MANUAL_LONGITUDE_KEY);
		return -1;
	}
	//纬度
	data.f32 = work_mode->manual_latitude;
	if(set_add_revise(&set, MANUAL_LATITUDE_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", MANUAL_LATITUDE_KEY);
		return -1;
	}
	//高度
	data.f32 = work_mode->manual_height;
	if(set_add_revise(&set, MANUAL_HEIGHT_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", MANUAL_HEIGHT_KEY);
		return -1;
	}

	//将数据同步到文件
	if(set_sync(&set) < 0) {
		LOG_E("sync work info failed");
		return -1;
	}

	return 0;
}

static int32_t _config_manage_get_beam_info(beam_info_t *beam_info)
{
	uint32_t type;
	data_type_t data;
	beam_info_t default_info = DEFAULT_BEAM_INFO;

	//向给一个默认值
	*beam_info = default_info;

	//卫星经度
	if((set_get(&set, SAT_LONGITUDE_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->sat_longitude = data.f32;
	}
	//卫星名称
	if((set_get(&set, SAT_NAME_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(beam_info->sat_name, data.str, sizeof(beam_info->sat_name));
	}

	//下行频率(MHz)
	if((set_get(&set, RX_FREQ_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->rx_freq = data.f32;
	}
	//下行极化参考
	if((set_get(&set, RX_POLARITY_TYPE_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		beam_info->rx_polarity_type = data.u8;
	}

	//上行频率(MHz)
	if((set_get(&set, TX_FREQ_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->tx_freq = data.f32;
	}
	//上行极化参考
	if((set_get(&set, TX_POLARITY_TYPE_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		beam_info->tx_polarity_type = data.u8;
	}

	//下行本振(MHz)
	if((set_get(&set, RX_OSC_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->rx_osc = data.f32;
	}
	//下行增益
	if((set_get(&set, RX_GAIN_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		beam_info->rx_gain = data.u8;
	}
	//上行本振(MHz)
	if((set_get(&set, TX_OSC_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->tx_osc = data.f32;
	}
	//上行增益
	if((set_get(&set, TX_GAIN_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		beam_info->tx_gain = data.u8;
	}

	//下行符号速率(Ksps)，DVB或检波模式有效
	if((set_get(&set, SYM_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		beam_info->sym = data.f32;
	}
	//信标机捕获范围(KHZ)，检波模式或信标模式有效
	if((set_get(&set, RANG_KEY, &type, &data) >= 0) && (type == TYPE_U8)) {
		beam_info->rang = data.u8;
	}

	return 0;
}

static int32_t _config_manage_set_beam_info(const beam_info_t *beam_info)
{
	data_type_t data;

	//卫星经度
	data.f32 = beam_info->sat_longitude;
	if(set_add_revise(&set, SAT_LONGITUDE_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", SAT_LONGITUDE_KEY);
		return -1;
	}
	//卫星名称
	strncpy(data.str, beam_info->sat_name, sizeof(data.str));
	if(set_add_revise(&set, SAT_NAME_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", SAT_NAME_KEY);
		return -1;
	}

	//下行频率(MHz)
	data.f32 = beam_info->rx_freq;
	if(set_add_revise(&set, RX_FREQ_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", RX_FREQ_KEY);
		return -1;
	}
	//下行极化参考
	data.u8 = beam_info->rx_polarity_type;
	if(set_add_revise(&set, RX_POLARITY_TYPE_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", RX_POLARITY_TYPE_KEY);
		return -1;
	}

	//上行频率(MHz)
	data.f32 = beam_info->tx_freq;
	if(set_add_revise(&set, TX_FREQ_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", TX_FREQ_KEY);
		return -1;
	}
	//上行极化参考
	data.u8 = beam_info->tx_polarity_type;
	if(set_add_revise(&set, TX_POLARITY_TYPE_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", TX_POLARITY_TYPE_KEY);
		return -1;
	}

	//下行本振(MHz)
	data.f32 = beam_info->rx_osc;
	if(set_add_revise(&set, RX_OSC_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", RX_OSC_KEY);
		return -1;
	}
	//下行增益
	data.u8 = beam_info->rx_gain;
	if(set_add_revise(&set, RX_GAIN_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", RX_GAIN_KEY);
		return -1;
	}
	//上行本振(MHz)
	data.f32 = beam_info->tx_osc;
	if(set_add_revise(&set, TX_OSC_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", TX_OSC_KEY);
		return -1;
	}
	//上行增益
	data.u8 = beam_info->tx_gain;
	if(set_add_revise(&set, TX_GAIN_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", TX_GAIN_KEY);
		return -1;
	}

	//下行符号速率(Ksps)，DVB或检波模式有效
	data.f32 = beam_info->sym;
	if(set_add_revise(&set, SYM_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", SYM_KEY);
		return -1;
	}
	//信标机捕获范围(KHZ)，检波模式或信标模式有效
	data.u8 = beam_info->rang;
	if(set_add_revise(&set, RANG_KEY, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", RANG_KEY);
		return -1;
	}

	//将数据同步到文件
	if(set_sync(&set) < 0) {
		LOG_E("sync work beam failed");
		return -1;
	}

	return 0;
}

static int32_t _config_manage_get_imu_drift(imu_drift_t *imu_drift)
{
	uint32_t type;
	data_type_t data;
	imu_drift_t default_imu_drift = DEFAULT_IMU_DRIFT;

	//先给一个默认值
	*imu_drift = default_imu_drift;

	//零漂有效标志
	if((set_get(&set, GYRO_VALID, &type, &data) >= 0) && (type == TYPE_U8)) {
		imu_drift->valid = data.u8;
	}

	//零漂值
	if((set_get(&set, GYRO_X_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		imu_drift->gyro_x = data.f32;
	}
	if((set_get(&set, GYRO_Y_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		imu_drift->gyro_y = data.f32;
	}
	if((set_get(&set, GYRO_Z_KEY, &type, &data) >= 0) && (type == TYPE_F32)) {
		imu_drift->gyro_z = data.f32;
	}

	return 0;
}

static int32_t _config_manage_set_imu_drift(const imu_drift_t *imu_drift)
{
	data_type_t data;

	//零漂有效标志
	data.u8 = imu_drift->valid;
	if(set_add_revise(&set, GYRO_VALID, TYPE_U8, &data) < 0) {
		LOG_E("set %s failed", GYRO_VALID);
		return -1;
	}
	//陀螺仪零漂
	data.f32 = imu_drift->gyro_x;
	if(set_add_revise(&set, GYRO_X_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", GYRO_X_KEY);
		return -1;
	}
	data.f32 = imu_drift->gyro_y;
	if(set_add_revise(&set, GYRO_Y_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", GYRO_Y_KEY);
		return -1;
	}
	data.f32 = imu_drift->gyro_z;
	if(set_add_revise(&set, GYRO_Z_KEY, TYPE_F32, &data) < 0) {
		LOG_E("set %s failed", GYRO_Z_KEY);
		return -1;
	}

	//将数据同步到文件
	if(set_sync(&set) < 0) {
		LOG_E("sync work imu drift");
		return -1;
	}

	return 0;
}

static int32_t _config_manage_get_product_info(product_info_t *product_info)
{
	uint32_t type;
	data_type_t data;
	product_info_t default_product_info = DEFAULT_PRODUCT_INFO;

	//先给一个默认值
	*product_info = default_product_info;

	//天线名称
	if((set_get(&set, ANTENNA_NANME_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(product_info->antenna_name, data.str, sizeof(product_info->antenna_name));
	}
	//天线序列号
	if((set_get(&set, ANTENNA_ORB_KEY, &type, &data) >= 0) && (type == TYPE_STR)) {
		strncpy(product_info->antenna_orb, data.str, sizeof(product_info->antenna_orb));
	}

	return 0;
}

static int32_t _config_manage_set_product_info(const product_info_t *product_info)
{
	data_type_t data;

	//产品名称
	strncpy(data.str, product_info->antenna_name, sizeof(data.str));
	if(set_add_revise(&set, ANTENNA_NANME_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", ANTENNA_NANME_KEY);
		return -1;
	}
	//产品序列号
	strncpy(data.str, product_info->antenna_orb, sizeof(data.str));
	if(set_add_revise(&set, ANTENNA_ORB_KEY, TYPE_STR, &data) < 0) {
		LOG_E("set %s failed", ANTENNA_ORB_KEY);
		return -1;
	}

	//将数据同步到文件
	if(set_sync(&set) < 0) {
		LOG_E("sync work product failed");
		return -1;
	}

	return 0;
}

int32_t config_manage_init(const char *path)
{
	if(!path) {
		LOG_E("Invalid parameter");
	}

	if(pthread_mutex_init(&mutex, NULL) < 0) {
		return -1;
	}
	set.work_path = path;
	set.max_entry = 200;
	return set_init(&set);
}

int32_t config_manage_get_tracking_info(tracking_info_t *tracking_info)
{
	int32_t result;

	if(!tracking_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_get_tracking_info(tracking_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_set_tracking_info(const tracking_info_t *tracking_info)
{
	int32_t result;

	if(!tracking_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_set_tracking_info(tracking_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_default_tracking_info(tracking_info_t *tracking_info)
{
	tracking_info_t default_info = DEFAULT_TRACKING_PARAMETER;

	if(!tracking_info) {
		LOG_E("Invalid parameter");
	}

	*tracking_info = default_info;
	return 0;
}

int32_t config_manage_get_work_info(work_info_t *work_info)
{
	int32_t result;

	if(!work_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_get_work_info(work_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_set_work_info(const work_info_t *work_info)
{
	int32_t result;

	if(!work_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_set_work_info(work_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_default_work_info(work_info_t *work_info)
{
	work_info_t default_info = DEFAULT_WORK_MODE;

	if(!work_info) {
		LOG_E("Invalid parameter");
	}

	*work_info = default_info;

	return 0;
}

int32_t config_manage_get_beam_info(beam_info_t *beam_info)
{
	int32_t result;

	if(!beam_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_get_beam_info(beam_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_set_beam_info(const beam_info_t *beam_info)
{
	int32_t result;

	if(!beam_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_set_beam_info(beam_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_default_beam_info(beam_info_t *beam_info)
{
	beam_info_t default_info = DEFAULT_BEAM_INFO;

	if(!beam_info) {
		LOG_E("Invalid parameter");
	}

	*beam_info = default_info;
	return 0;
}

int32_t config_manage_get_imu_drift(imu_drift_t *imu_drift)
{
	int32_t result;

	if(!imu_drift) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_get_imu_drift(imu_drift);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_set_imu_drift(const imu_drift_t *imu_drift)
{
	int32_t result;

	if(!imu_drift) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_set_imu_drift(imu_drift);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_default_imu_drift(imu_drift_t *imu_drift)
{
	imu_drift_t default_imu_drift = DEFAULT_IMU_DRIFT;

	if(!imu_drift) {
		LOG_E("Invalid parameter");
	}

	*imu_drift = default_imu_drift;
	return 0;
}

int32_t config_manage_get_product_info(product_info_t *product_info)
{
	int32_t result;

	if(!product_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_get_product_info(product_info);

	pthread_mutex_unlock(&mutex);

	return result;
}
int32_t config_manage_set_product_info(const product_info_t *product_info)
{
	int32_t result;

	if(!product_info) {
		LOG_E("Invalid parameter");
	}

	pthread_mutex_lock(&mutex);

	result = _config_manage_set_product_info(product_info);

	pthread_mutex_unlock(&mutex);

	return result;
}

int32_t config_manage_default_product_info(product_info_t *product_info)
{
	product_info_t default_product_info = DEFAULT_PRODUCT_INFO;

	if(!product_info) {
		LOG_E("Invalid parameter");
	}

	*product_info = default_product_info;
	return 0;
}

int32_t config_manage_restore_default(void)
{
	int32_t result;
	imu_drift_t imu_drift = DEFAULT_IMU_DRIFT;
	beam_info_t beam_info = DEFAULT_BEAM_INFO;
	work_info_t work_info = DEFAULT_WORK_MODE;
	tracking_info_t tracking_info = DEFAULT_TRACKING_PARAMETER;

	result = config_manage_set_tracking_info(&tracking_info);
	if(result < 0) {
		return result;
	}
	result = config_manage_set_work_info(&work_info);
	if(result < 0) {
		return result;
	}
	result = config_manage_set_beam_info(&beam_info);
	if(result < 0) {
		return result;
	}
	result = config_manage_set_imu_drift(&imu_drift);
	if(result < 0) {
		return result;
	}

	return 0;
}
