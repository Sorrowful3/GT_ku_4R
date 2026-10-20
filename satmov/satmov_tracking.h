/*
 * satmov_tracking.h
 *
 *  Created on: 2021年6月15日
 *      Author: Administrator
 */

#ifndef __SATMOV_TRACKING_H_
#define __SATMOV_TRACKING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "satmov_common.h"

#define USING_POLARITY_CORRECT
#define USING_MAIN_LOBE_FIND

typedef struct{
	float pitch;
	float roll;
	float yaw;
}correct_imu_t;
//GPS状态
#define SATMOV_GPS_INVALID  0x01    //GPS无效
#define SATMOV_GPS_LOCATION 0x02    //GPS定位成功
#define SATMOV_GPS_ORIENT   0x04    //GPS定向成功

//天线阵面
#define SATMOV_RX           0x01    //接收天线
#define SATMOV_TX           0x02    //发射天线
#define SATMOV_TR           0x03    //接收和发射天线

//当前工作状态
#define SATMOV_INITIAL_STATE             0x01    //初始状态
#define SATMOV_WAIT_GPS_STATE            0X02    //等待GPS状态
#define SATMOV_READY_STATE               0x04    //就绪状态
#define SATMOV_CORRECT_STATE             0x08    //校准中
#define SATMOV_CORRECT_COMPLETE_STATE    0x10    //校准完成
#define SATMOV_PROGRAM_TRACKING_STATE    0x20    //跟踪状态
#define SATMOV_SUSPEND_STATE             0x40    //停止状态
#define SATMOV_SYSTEM_FAULT_STATE        0X80    //系统错误
#define SATMOV_TMEP_STATE                0X100   //零时状态

//异常编码
#define SATMOV_GPS_FAULT                0x01    //IMU异常
#define SATMOV_IMU_FAULT                0x02    //GPS异常
#define SATMOV_BEACON_FAULT             0x03    //信标机异常
#define SATMOV_ANTENNA_FAULT            0x04    //天线异常
#define SATMOV_STATE_FAULT              0x05    //状态错误
#define SATMOV_CORRECT_FAULT            0x06    //校准出错

typedef struct {
    float yaw;              //通过其他手段得到的航向角，如果没有为-1
    float beamwidth;        //天线波束宽度,弧度；校准时使用
    float threshold_cnr;    //跟踪阈值
    uint32_t scan_count;    //搜索扫描次数
}satmov_currect_info_t;

typedef struct satmov_object{
    //暂存用户设置的状态
    uint32_t                set_state;
    //下一个状态
    uint32_t                next_state;
    //当前状态
    uint32_t                current_state;
    //错误码
    uint32_t                fault_code;

    //惯导数据补偿
    uint32_t                gesture_delay;

    //最佳航向角
    float                   best_yaw;

    //跟踪参数
    satmov_currect_info_t   currect_info;

    //预留扩展功能，也可用于区分不同的
    void                    *private_data;

    void (*currect_complete)(struct satmov_object *satmov, float best_yaw);

    //错误处理
    int32_t (*error_handle)(struct satmov_object *satmov, uint32_t last_state, uint32_t fault_code, uint32_t keep_time);

    //读惯导数据
    int32_t (*read_imu)(struct satmov_object *satmov, satmov_imu_t *imu);
    //读GPS数据
    int32_t (*read_gps)(struct satmov_object *satmov, satmov_gps_t *gps);

    //读信标机数据
    int32_t (*read_cnr)(struct satmov_object *satmov, float *cnr);

    //设置天线指向
    int32_t (*antenna_point)(struct satmov_object *satmov, const satmov_point_t *body_point, uint8_t TR);
    //开天线射频电源
    int32_t (*antenna_on)(struct satmov_object *satmov, uint8_t TR);
    //关天线射频电源
    int32_t (*antenna_off)(struct satmov_object *satmov, uint8_t TR);

    //计算地理指向和极化
    //计算了satmov_point_t中的azimuth、elevation、polarity
    int32_t (*calculate_earth_point_geo)(struct satmov_object *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point);
    //计算了satmov_point_t中的azimuth、elevation、polarity
    int32_t (*calculate_earth_point_leo)(struct satmov_object *satmov, const satmov_gps_t *gps, satmov_point_t *earth_point);
}satmov_object_t;

//初始化对星程序
int32_t satmov_init(satmov_object_t *satmov);
//启动对星算法
int32_t satmov_start(satmov_object_t *satmov);
//启动程序跟踪
int32_t satmov_tracking(satmov_object_t *satmov);
//暂停自动对星
int32_t satmov_suspend(satmov_object_t *satmov);
//恢复自动对星
int32_t satmov_resume(satmov_object_t *satmov);
//获取当前工作状态
uint32_t satmov_get_state(satmov_object_t *satmov);
//启动航向角校准
int32_t satmov_currect_yaw(satmov_object_t *satmov, const satmov_currect_info_t *currect_info);

void get_correct(correct_imu_t *cor);

#ifdef __cplusplus
}
#endif

#endif /* __SATMOV_TRACKING_H_ */
