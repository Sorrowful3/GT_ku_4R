/*
*config_manage.h
*
*	Createdon:2021年9月29日
*		Author:Administrator
*/

#ifndef __CONFIG_MANAGE_H_
#define __CONFIG_MANAGE_H_

#ifdef __cplusplus
extern"C"{
#endif

#include"../set/set.h"

#define BEAMWIDTH_KEY						"beamwidth"
#define COARSE_CNR_KEY						"coarse_cnr"
#define REFINED_CNR_KEY						"refined_cnr"
#define TRACKING_CNR_KEY					"tracking_cnr"
#define DVB_COARSE_THRESHOLD_KEY			"dvb_coarse_threshold"
#define DVB_REFINED_THRESHOLD_KEY			"dvb_refined_threshold"
#define DVB_TRACKING_THRESHOLD_KEY			"dvb_tracking_threshold"
#define DETECTION_COARSE_THRESHOLD_KEY		"detection_coarse_threshold"
#define DETECTION_REFINED_THRESHOLD_KEY		"detection_refined_threshold"
#define DETECTION_TRACKING_THRESHOLD_KEY	"detection_tracking_threshold"
#define BEACON_COARSE_THRESHOLD_KEY			"beacon_coarse_threshold"
#define BEACON_REFINED_THRESHOLD_KEY		"beacon_refined_threshold"
#define BEACON_TRACKING_THRESHOLD_KEY		"beacon_tracking_threshold"
#define COARSE_LINGER_FACTOR_KEY			"coarse_linger_factor"
#define REFINED_LINGER_FACTOR_KEY			"refined_linger_factor"
#define TRACKING_LINGER_FACTOR_KEY			"tracking_linger_factor"
#define KP_AZ_KEY							"kp_az"
#define KP_EL_KEY							"kp_el"
#define KP_POL_KEY							"kp_pol"
#define DELTA_AZ_KEY						"delta_az"
#define DELTA_EL_KEY						"delta_el"
#define DELTA_POL_KEY						"delta_pol"

#define WORK_MODE_KEY				"work_mode"
#define OPENAMIP_IP_KEY				"openamip_ip"
#define OPENAMIP_MASK_KEY			"openamip_mask"
#define OPENAMIP_PORT_KEY			"openamip_port"
#define ROLLOFF_KEY 				"rolloff_factor"
#define SERVICER_IP_KEY				"servicer_ip"
#define SERVICER_MASK_KEY			"servicer_mask"
#define SERVICER_PORT_KEY			"servicer_port"
#define TRACKING_MODE_KEY			"tracking_mode"
#define MANUAL_GPS_KEY				"manual_gps"
#define MANUAL_LONGITUDE_KEY		"manual_longitude"
#define MANUAL_LATITUDE_KEY			"manual_latitude"
#define MANUAL_HEIGHT_KEY			"manual_height"

#define SAT_LONGITUDE_KEY			"sat_longitude"
#define SAT_NAME_KEY				"sat_name"
#define RX_FREQ_KEY					"rx_freq"
#define RX_POLARITY_TYPE_KEY		"rx_polarity_type"
#define TX_FREQ_KEY					"tx_freq"
#define TX_POLARITY_TYPE_KEY		"tx_polarity_type"
#define RX_OSC_KEY					"rx_osc"
#define RX_GAIN_KEY					"rx_gain"
#define TX_OSC_KEY					"tx_osc"
#define TX_GAIN_KEY					"tx_gain"
#define SYM_KEY						"beacon_sym"
#define RANG_KEY					"beacon_rang"

#define GYRO_VALID					"valid"
#define GYRO_X_KEY					"gyro_x"
#define GYRO_Y_KEY					"gyro_y"
#define GYRO_Z_KEY					"gyro_z"

#define ANTENNA_NANME_KEY			"antenna_name"
#define ANTENNA_ORB_KEY				"antenna_orb"

#define DEFAULT_TRACKING_PARAMETER			\
{											\
	.beamwidth = 3.5f,						\
											\
	.dvb_coarse_threshold = -2.0f,			\
	.dvb_refined_threshold = 0.0f,			\
	.dvb_tracking_threshold = 0.5f,			\
	.detection_coarse_threshold = -80.0f,	\
	.detection_refined_threshold = -70.0f,	\
	.detection_tracking_threshold = -65.0f,	\
	.beacon_coarse_threshold = -80.0f,		\
	.beacon_refined_threshold = -70.0f,		\
	.beacon_tracking_threshold = -65.0f,	\
											\
	.kp_az = 0.125f,						\
	.kp_el = 0.125f,						\
	.kp_pol = 2.0f,							\
											\
	.coarse_linger_factor = 3.0f,			\
	.refined_linger_factor = 3.0f,			\
	.tracking_linger_factor = 3.5f,			\
											\
	.delta_az = 0.35f,						\
	.delta_el = 0.35f,						\
	.delta_pol = 3.5f,						\
}

#define DEFAULT_WORK_MODE					\
{											\
	.work_mode = 0x00,						\
	.openamip_ip = {"192.168.0.2"},			\
	.openamip_mask = {"255.255.255.0"},		\
	.rolloff_factor = 0.15f,				\
	.openamip_port = 4001,					\
	.servicer_ip = {"192.168.0.23"},		\
	.servicer_mask = {"255.255.255.0"},		\
	.servicer_port = 5500,					\
	.tracking_mode = 0x03,					\
	.manual_gps = 0x00,						\
	.manual_longitude = 104.05f,			\
	.manual_latitude = 30.58f,				\
	.manual_height = 500.0f,				\
}

#define DEFAULT_BEAM_INFO							\
{													\
	.sat_longitude = 134.0f,						\
	.sat_name = {"APSTAR 6D               "},		\
													\
	.rx_freq = 11406.25f,							\
	.rx_polarity_type = 0x00,						\
													\
	.tx_freq = 14221.6f,							\
	.tx_polarity_type = 0x01,						\
													\
	.rx_osc = 9750.0f,								\
	.rx_gain = 0x00,								\
	.tx_osc = 12800.0f,								\
	.tx_gain = 0x00,								\
													\
	.sym = 24750.0f,								\
	.rang = 50,										\
}

#define DEFAULT_IMU_DRIFT		\
{								\
	.valid = 0x00,				\
	.gyro_x = 0.0f,				\
	.gyro_y = 0.0f,				\
	.gyro_z = 0.0f,				\
}

#define DEFAULT_PRODUCT_INFO			\
{										\
	.antenna_name = {"VU5545_BM1"},		\
	.antenna_orb = {"1234567890"},		\
}

typedef struct{
	float	beamwidth;						//天线波束宽度,弧度；盲扫、螺旋扫描时使用

    float   dvb_coarse_threshold;           //DVB粗对准阈值
    float   dvb_refined_threshold;          //DVB精对准阈值
    float   dvb_tracking_threshold;         //DVB动态跟踪阈值
    float   detection_coarse_threshold;     //检波粗对准阈值
    float   detection_refined_threshold;    //检波精对准阈值
    float   detection_tracking_threshold;   //检波动态跟踪阈值
    float   beacon_coarse_threshold;        //信标粗对准阈值
    float   beacon_refined_threshold;       //信标精对准阈值
    float   beacon_tracking_threshold;      //信标动态跟踪阈值

	float	kp_az;							//动态跟踪比例系数
	float	kp_el;							//动态跟踪比例系数
	float	kp_pol;							//动态跟踪比例系数

	float 	coarse_linger_factor;			//粗对准波束驻留时间，ms，以4为倍数向0取整
	float 	refined_linger_factor; 			//精对准束驻留时间，ms，以4为倍数向0取整
	float	tracking_linger_factor;			//动态跟踪波束驻留时间，ms，以4为倍数向0取整

	float	delta_az;						//动态跟踪偏离角度
	float	delta_el;						//动态跟踪偏离角度
	float	delta_pol;						//动态跟踪偏离角度
}tracking_info_t;

typedef struct{
	uint8_t  work_mode;
	char 	 openamip_ip[32];
	char 	 openamip_mask[32];
	uint16_t openamip_port;
	float 	 rolloff_factor;
	char 	 servicer_ip[32];
	char 	 servicer_mask[32];
	uint16_t servicer_port;
	uint8_t	tracking_mode;
	uint8_t	 manual_gps;
	float    manual_longitude;
	float    manual_latitude;
	float    manual_height;
}work_info_t;

typedef struct{
	float	sat_longitude;		//卫星经度,角度。
	char    sat_name[32];		//卫星名称

	float	rx_freq;			//下行频率(MHz)
	uint8_t	rx_polarity_type;	//下行极化参考

	float	tx_freq;			//上行频率(MHz)
	uint8_t	tx_polarity_type;	//上行极化参考

	float	rx_osc;				//下行本振(MHz)
	uint8_t rx_gain;			//下行增益
	float	tx_osc;				//上行本振(MHz)
	uint8_t tx_gain;			//上行增益

	float	sym;				//下行符号速率(Ksps)，DVB或检波模式有效
	uint8_t	rang;				//信标机捕获范围(KHZ)，检波模式或信标模式有效
}beam_info_t;

typedef struct{
	uint8_t valid;
	float gyro_x;
	float gyro_y;
	float gyro_z;
}imu_drift_t;

typedef struct{
	char antenna_name[64];
	char antenna_orb[64];
}product_info_t;

int32_t config_manage_init(const char *path);

int32_t config_manage_get_tracking_info(tracking_info_t *tracking_info);
int32_t config_manage_set_tracking_info(const tracking_info_t *tracking_info);
int32_t config_manage_default_tracking_info(tracking_info_t *tracking_info);

int32_t config_manage_get_work_info(work_info_t *work_info);
int32_t config_manage_set_work_info(const work_info_t *work_info);
int32_t config_manage_default_work_info(work_info_t *work_info);

int32_t config_manage_get_beam_info(beam_info_t *beam_info);
int32_t config_manage_set_beam_info(const beam_info_t *beam_info);
int32_t config_manage_default_beam_info(beam_info_t *beam_info);

int32_t config_manage_get_imu_drift(imu_drift_t *imu_drift);
int32_t config_manage_set_imu_drift(const imu_drift_t *imu_drift);
int32_t config_manage_default_imu_drift(imu_drift_t *imu_drift);

int32_t config_manage_get_product_info(product_info_t *product_info);
int32_t config_manage_set_product_info(const product_info_t *product_info);
int32_t config_manage_default_product_info(product_info_t *product_info);

int32_t config_manage_restore_default(void);

#ifdef __cplusplus
}
#endif

#endif/*__ANTENNA_CONFIG_H_*/
