/*
 * satmov_port.c
 *
 *  Created on: 2021年6月17日
 *      Author: Administrator
 */

#include "satmov_port.h"

#define DEBUG_TAG "satmov_port,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../driver/core/debug.h"

static int32_t read_imu(satmov_object_t *satmov, satmov_imu_t *imu);
static int32_t read_gps(satmov_object_t *satmov, satmov_gps_t *gps);
static int32_t read_cnr(satmov_object_t *satmov, float *cnr);
static int32_t antenna_point(satmov_object_t *satmov, const satmov_point_t *body_point, uint8_t TR);
static int32_t antenna_on(satmov_object_t *satmov, uint8_t TR);
static int32_t antenna_off(satmov_object_t *satmov, uint8_t TR);
static int32_t calculate_earth_point_geo(satmov_object_t *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point);
static int32_t calculate_earth_point_leo(satmov_object_t *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point);
static void currect_complete(struct satmov_object *satmov, float best_yaw);

//static device_t *imu_device = NULL;
static device_t *noe_m8 = NULL;
static device_t *transducer_device = NULL;
static device_t *beacon_device = NULL;
static float geo_sat_longitude = 0.0f;
static stamov_leo_beam_info_t leo_beam_info = {
    .azimuth = ANGLE_2_RADIAN(0.0f),
    .elevation = ANGLE_2_RADIAN(-90.0f),
    .polarity = ANGLE_2_RADIAN(0.0f),
};
static satmov1_currect_complete currect_complete_callbk = NULL;

static satmov_object_t satmov = {
        .set_state = 0,
        .next_state = SATMOV_INITIAL_STATE,
        .current_state = SATMOV_INITIAL_STATE,
        .fault_code = 0,
        .gesture_delay = 15,
        .best_yaw = 0.0f,
        .currect_info = {0.0f, 0.0f, 0.0f, 7},
        .private_data = NULL,
        .currect_complete = currect_complete,
        .error_handle = NULL,
        .read_imu = read_imu,
        .read_gps = read_gps,
        .read_cnr = read_cnr,
        .antenna_point = antenna_point,
        .antenna_on = antenna_on,
        .antenna_off = antenna_off,
        .calculate_earth_point_geo = calculate_earth_point_geo,
        .calculate_earth_point_leo = calculate_earth_point_leo,
};

//读惯导数据
static int32_t read_imu(satmov_object_t *satmov, satmov_imu_t *imu)
{
    int32_t result;
//    rav260_ins_t temp;
//
//    result = rav260_ins(imu_device, &temp);
//    if(result != ERR_OK) {
//        return result;
//    }
//
//    //姿态转换未弧度
//    imu->pitch = ANGLE_2_RADIAN(temp.pitch);
//    imu->roll = ANGLE_2_RADIAN(temp.roll);
//    imu->yaw = ANGLE_2_RADIAN(temp.yaw);
//    //角速度转换弧度/s
//    imu->rate_x = ANGLE_2_RADIAN(temp.x_speed);
//    imu->rate_y = ANGLE_2_RADIAN(temp.y_speed);
//    imu->rate_z = ANGLE_2_RADIAN(temp.z_speed);

    adis16465_attitude_t temp;

	ms6110_t ms;
//    result = adis16465_imu1_get_attitude(&temp);
//    if(result != ERR_OK) {
//        return result;
//    }

	noe_m8_read_ms6110(noe_m8, &ms);
	imu->state = ms.loc_flag;
	imu->pitch = ANGLE_2_RADIAN(ms.pitch);
	imu->roll = ANGLE_2_RADIAN(ms.roll);
	imu->yaw = ANGLE_2_RADIAN(ms.azimuth);
	imu->rate_x = 0;//ANGLE_2_RADIAN(temp.gyro.x);
			imu->rate_y = 0;//ANGLE_2_RADIAN(temp.gyro.y);
			imu->rate_z = 0;//ANGLE_2_RADIAN(temp.gyro.z);

//    //姿态转换未弧度
//    imu->pitch = ANGLE_2_RADIAN(temp.euler_angle.pitch);
//    imu->roll = ANGLE_2_RADIAN(temp.euler_angle.roll);
//    imu->yaw = ANGLE_2_RADIAN(temp.euler_angle.yaw);
//    //角速度转换弧度/s
//    imu->rate_x = ANGLE_2_RADIAN(temp.gyro.x);
//    imu->rate_y = ANGLE_2_RADIAN(temp.gyro.y);
//    imu->rate_z = ANGLE_2_RADIAN(temp.gyro.z);
    return ERR_OK;
}

//读GPS数据
static int32_t read_gps(satmov_object_t *satmov, satmov_gps_t *gps)
{
    int32_t result;
//    rav260_gps_t temp;
//
//    result = rav260_gps(imu_device, &temp);
//    if(result != ERR_OK) {
//        return result;
//    }
//
//    //数据有效标志转换
//    if(temp.valid_flag) {
//        gps->state = SATMOV_GPS_LOCATION;
//    }
//    else {
//        gps->state = SATMOV_GPS_INVALID;
//    }
//    //高度转换为m
//    gps->height = temp.height;
//    //将经纬转换为弧度
//    gps->latitude = ANGLE_2_RADIAN(temp.latitude);
//    gps->longitude = ANGLE_2_RADIAN(temp.longitude);

    noe_m8_gps_t temp;
    noe_m8_time_t noe_m8_time;

    result = noe_m8_read_gps(noe_m8, &temp);
    if(result != ERR_OK) {
        return result;
    }

    result = noe_m8_read_time(noe_m8,&noe_m8_time);
    if(result != ERR_OK) {
          return result;
     }
    //数据有效标志转换

    if(temp.valid_flag) {
        gps->state = SATMOV_GPS_LOCATION;
    }
    else {
        gps->state = SATMOV_GPS_INVALID;
    }
    //高度转换为m
    gps->height = temp.height;
    //将经纬转换为弧度
    gps->latitude = ANGLE_2_RADIAN(temp.latitude);
    gps->longitude = ANGLE_2_RADIAN(temp.longitude);

    gps->second=noe_m8_time.second;

    return ERR_OK;
}

//读信标机数据
static int32_t read_cnr(satmov_object_t *satmov, float *cnr)
{
    int32_t result;

    beacon_data_t temp;

    result = beacon_read_data(beacon_device, &temp);
    if(result != ERR_OK) {
        return result;
    }

    //提取信噪比
    *cnr = temp.filter_cnr;
    return ERR_OK;
}

//设置天线指向
static int32_t antenna_point(satmov_object_t *satmov, const satmov_point_t *body_point, uint8_t TR)
{
    int32_t result;

    float polarity;
    float phi;
    float theta;


    polarity = RADIAN_2_ANGLE(body_point->polarity);
    phi = RADIAN_2_ANGLE(body_point->azimuth);
    theta = RADIAN_2_ANGLE(body_point->elevation);
    if(TR & SATMOV_RX) {
        if(antenna_rf_vcc_state(ANTENNA_RX)) {
            result = antenna_set_point(ANTENNA_RX, polarity, phi, theta);
            if(result != ERR_OK) {
                return result;
            }
        }
    }
    if(TR & SATMOV_TX) {
    }

    return ERR_OK;
}

//开天线射频电源
static int32_t antenna_on(satmov_object_t *satmov, uint8_t TR)
{
    int32_t result;

    if(TR & SATMOV_RX) {
        result = antenna_rf_vcc_on(ANTENNA_RX);
        if(result != ERR_OK) {
            return result;
        }
    }
    if(TR & SATMOV_TX) {
    }
    return ERR_OK;
}

//关天线射频电源
static int32_t antenna_off(satmov_object_t *satmov, uint8_t TR)
{
    int32_t result;

    if(TR & SATMOV_RX) {
        result = antenna_rf_vcc_off(ANTENNA_RX);
        if(result != ERR_OK) {
            return result;
        }
    }
    if(TR & SATMOV_TX) {
    }
    return ERR_OK;
}

static int32_t calculate_earth_point_geo(satmov_object_t *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point)
{
float longitude_dx;
float cos_longitude_dx;
float sin_longitude_dx;
float tan_longitude_dx;
float sin_latitude;
float cos_latitude;
float tan_latitude;
float tmp;
tmp=(6378.0f+(gps->height/1000.0f))/ (6378.0f+35785.965f);
longitude_dx=gps->longitude-geo_sat_longitude;
cos_longitude_dx=cosf(longitude_dx);
tan_longitude_dx=tanf(longitude_dx);
sin_latitude =sinf(gps->latitude);
cos_latitude =cosf(gps->latitude);

earth_point->azimuth=atan2f(tan_longitude_dx, sin_latitude)+PI;
earth_point->elevation=-atanf((cos_longitude_dx*cos_latitude-tmp)/
                       sqrt(1 - cos_longitude_dx*cos_longitude_dx*cos_latitude*cos_latitude));

earth_point->elevation=1.570796-abs(earth_point->elevation);
longitude_dx=gps->longitude-geo_sat_longitude;
sin_longitude_dx=sinf(longitude_dx);
tan_latitude=tanf(gps->latitude);
earth_point->polarity=PI/2.0f-atanf(-sin_longitude_dx/ tan_latitude);
return ERR_OK;
}


static int32_t calculate_earth_point_leo(satmov_object_t *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point)
{
//	earth_point->azimuth = RADIAN_2_ANGLE(leo_beam_info.azimuth);
//	earth_point->elevation = RADIAN_2_ANGLE(leo_beam_info.elevation);
//	earth_point->polarity= RADIAN_2_ANGLE(leo_beam_info.polarity);
	earth_point->azimuth = (leo_beam_info.azimuth);
	earth_point->elevation = 1.570796-(leo_beam_info.elevation);
	earth_point->polarity= (leo_beam_info.polarity);

//	LOG_I("leo_beam_info %.3f,%.3f,%.3f", RADIAN_2_ANGLE(earth_point->azimuth),
//										RADIAN_2_ANGLE(earth_point->elevation),
//											RADIAN_2_ANGLE(earth_point->polarity));

    return ERR_OK;
}

static void currect_complete(struct satmov_object *satmov, float best_yaw)
{
    if(currect_complete_callbk) {
        currect_complete_callbk(RADIAN_2_ANGLE(best_yaw));
    }
}

static int32_t config_geo_beam(const satmov_geo_beam_info_t *beam_info)
{
    int32_t result;

    //设置接收频率
    result = antenna_set_freq(ANTENNA_RX, beam_info->rx_freq/1000.0f);
    if(result != ERR_OK) {
        return result;
    }
    //设置接收极化
    result = antenna_set_polarity(ANTENNA_RX, beam_info->rx_polarity_type);
    if(result != ERR_OK) {
        return result;
    }

    //配置变频器
    result = transducer_set_lo(transducer_device, beam_info->rx_osc/1000.0f);
    if(result != ERR_OK) {
        return result;
    }

    //关闭电压映射
    beacon_set_voltage_map(beacon_device, 0x00);
    //关闭馈电
    beacon_set_feed(beacon_device, 0x00);
    //关闭单音
    beacon_set_single_tone(beacon_device, 0x00);

    //配置工作模式
    result = beacon_set_mode(beacon_device, beam_info->beacon_mode);
    if(result != ERR_OK) {
        return result;
    }

    if(beam_info->beacon_mode == 0x00) {
        //配置DVB
        result = beacon_set_DVB(beacon_device, (beam_info->rx_freq-beam_info->rx_osc)*1000.0f, beam_info->sym);
        if(result != ERR_OK) {
            return result;
        }
    }
    else if(beam_info->beacon_mode == 0x01) {
        //配置检波，大于40M时不使用检波
        result = beacon_set_detection(beacon_device, (beam_info->rx_freq-beam_info->rx_osc)*1000.0f, beam_info->sym);
        if(result != ERR_OK) {
            return result;
        }
    }
    else {
        return ERR_INVAL;
    }

    //配置卫星经度
    geo_sat_longitude = ANGLE_2_RADIAN(beam_info->sat_longitude);
    return ERR_OK;
}

//初始化对星程序
int32_t satmov1_init(void)
{
    noe_m8 = device_open("noe_m8_0", DEVICE_RDWR);
    if(!noe_m8) {
        return ERR_IO;
    }
    transducer_device = device_open("transducer0", DEVICE_RDWR);
    if(!transducer_device) {
        return ERR_IO;
    }
    beacon_device = device_open("beacon0", DEVICE_RDWR);
    if(!beacon_device) {
        return ERR_IO;
    }
    return satmov_init(&satmov);
}

//启动自动对星
int32_t satmov1_start(void)
{
    return satmov_start(&satmov);
}

int32_t satmov1_tracking(void)
{
    return satmov_tracking(&satmov);
}

//暂停自动对星
int32_t satmov1_suspend(void)
{
    return satmov_suspend(&satmov);
}

//恢复自动对星
int32_t satmov1_resume(void)
{
    return satmov_resume(&satmov);
}



//int32_t satmov1_currect_yaw(const satmov_geo_beam_info_t *beam_info, const satmov_currect_info_t *currect_info)
//{
//    int32_t result;
//    satmov_currect_info_t temp;
//
//    return satmov_currect_yaw(&satmov, currect_info);
//}

int32_t satmov1_currect_yaw(const satmov_geo_beam_info_t *beam_info, const satmov_currect_info_t *currect_info)
{
    int32_t result;
    // satmov_currect_info_t temp;

    result = config_geo_beam(beam_info);
    if (result) {
        LOG_E("config_geo_beam failed.");
    }
    LOG_I("safaf\n");
    return satmov_currect_yaw(&satmov, currect_info);
}


//获取当前工作状态
uint32_t satmov1_get_state(void)
{
    return satmov_get_state(&satmov);
}

int32_t satmov1_set_currect_callbk(satmov1_currect_complete callbk)
{
    currect_complete_callbk = callbk;
    return ERR_OK;
}

//设置地理指向
int32_t satmov1_set_leo_beam_info(stamov_leo_beam_info_t *beam_info)
{
	uint8_t result;
	leo_beam_info.azimuth = ANGLE_2_RADIAN(beam_info->azimuth);
	leo_beam_info.elevation = ANGLE_2_RADIAN(beam_info->elevation);
	leo_beam_info.polarity = ANGLE_2_RADIAN(beam_info->polarity);

	//设置接收频率
	if(leo_beam_info.rx_freq != beam_info->rx_freq)
	{
		result = antenna_set_freq(ANTENNA_RX, beam_info->rx_freq/1000.0f);
		if(result != ERR_OK) {
			return result;
		}
		leo_beam_info.rx_freq = beam_info->rx_freq;
	}

	   // LOG_E("stamov_leo_beam_info:azimuth=%f,"
//	    		"elevation=%f,polarity=%f,lo_rx=%f\n",leo_beam_info.azimuth,leo_beam_info.elevation,leo_beam_info.polarity,beam_info->rx_osc);

	//设置接收本振
	if(leo_beam_info.rx_osc != beam_info->rx_osc)
	{
		//配置变频器
		result = transducer_set_lo(transducer_device, beam_info->rx_osc/1000.0f);
		if(result != ERR_OK) {
			return result;
		}
		leo_beam_info.rx_osc = beam_info->rx_osc;
	}

	//设置接收增益
	if(leo_beam_info.rx_gain != beam_info->rx_gain)
	{
		leo_beam_info.rx_gain = beam_info->rx_gain;
	}

	if(leo_beam_info.rx_polarity_type != beam_info->rx_polarity_type)
	{
		//设置接收极化
		result = antenna_set_polarity(ANTENNA_RX, beam_info->rx_polarity_type);
		if(result != ERR_OK) {
			return result;
		}
		leo_beam_info.rx_polarity_type = beam_info->rx_polarity_type;
	}

	//配置发射
	if(leo_beam_info.tx_freq != beam_info->tx_freq)
	{
		leo_beam_info.tx_freq = beam_info->tx_freq;
	}
	if(leo_beam_info.tx_osc != beam_info->tx_osc)
	{
		leo_beam_info.tx_osc = beam_info->tx_osc;
	}
	if(leo_beam_info.tx_gain != beam_info->tx_gain)
	{
		leo_beam_info.tx_gain = beam_info->tx_gain;
	}

	if(leo_beam_info.tx_polarity_type != beam_info->tx_polarity_type)
	{
		leo_beam_info.tx_polarity_type = beam_info->tx_polarity_type;
	}

    return ERR_OK;
}

//获取地理指向
int32_t satmov1_get_earth_point(satmov_point_t *earth_point)
{
    return ERR_OK;
}
