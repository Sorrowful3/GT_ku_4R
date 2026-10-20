/*
 * satmov_tracking.c
 *
 *  Created on: 2021年6月15日
 *      Author: Administrator
 */

#include "satmov_tracking.h"

#define DEBUG_TAG "satmov_tracking,%d", __LINE__
//#define DEBUG_LEVEL INFO_OUTPUT
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../driver/core/debug.h"




static correct_imu_t correct = {
	.pitch = 0.f,
	.roll = 0.f,
	.yaw = 0.f,
};



//强制切换状态
static void switch_state(satmov_object_t *satmov, uint32_t state)
{
    satmov->next_state = state;
}

//试探性切换状态
static void switch_state_weak(satmov_object_t *satmov, uint32_t state)
{
    if(satmov->next_state == satmov->current_state) {
        satmov->next_state = state;
    }
}

//状态是否改变
static int32_t state_is_change(satmov_object_t *satmov)
{
    //让外部设置的状态生效
    if(satmov->set_state != 0x00) {
        switch_state(satmov, satmov->set_state);
        satmov->set_state = 0x00;
        //临时状态
        satmov->current_state = SATMOV_TMEP_STATE;
    }
    //检查是否需要进行状态切换
    return satmov->next_state != satmov->current_state ? 1 : 0;
}


//读惯导数据
static int32_t read_imu_internal(satmov_object_t *satmov, satmov_imu_t *imu)
{
    int32_t result;

    result = satmov->read_imu(satmov, imu);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_IMU_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("read imu failed");
    }
    return result;
}

//读GPS数据
static int32_t read_gps_internal(satmov_object_t *satmov, satmov_gps_t *gps)
{
    int32_t result;

    result = satmov->read_gps(satmov, gps);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_GPS_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("read gps failed");
    }
    return result;
}

//读信标机数据
static int32_t read_cnr_internal(satmov_object_t *satmov, float *cnr)
{
    int32_t result;

    result = satmov->read_cnr(satmov, cnr);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_BEACON_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("read CNR failed");
    }
    return result;
}

//设置天线指向
static int32_t antenna_point_internal(satmov_object_t *satmov, const satmov_point_t *body_point, uint8_t TR)
{
    int32_t result;

    result = satmov->antenna_point(satmov, body_point, TR);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_ANTENNA_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("set antenna point failed");
    }
    LOG_I("---%.3f，%.3f,%.3f", RADIAN_2_ANGLE(body_point->azimuth),
    							RADIAN_2_ANGLE(body_point->elevation),
    							RADIAN_2_ANGLE(body_point->polarity));
    return result;
}

//开天线射频电源
static int32_t antenna_on_internal(satmov_object_t *satmov, uint8_t TR)
{
    int32_t result;

    result = satmov->antenna_on(satmov, TR);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_ANTENNA_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("on antenna failed");
    }
    return result;
}

//关天线射频电源
static int32_t antenna_off_internal(satmov_object_t *satmov, uint8_t TR)
{
    int32_t result;

    result = satmov->antenna_off(satmov, TR);
    if(result != ERR_OK) {
        satmov->fault_code = SATMOV_ANTENNA_FAULT;
        switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
        LOG_E("off antenna failed");
    }
    return result;
}

//检查指向是否符合天线要求
static int32_t check_point(satmov_object_t *satmov, const satmov_point_t *earth_point, float yaw)
{
    int32_t result;
    satmov_point_t body_point;
    satmov_imu_t imu;

    result = read_imu_internal(satmov, &imu);
    if(result != ERR_OK) {
        return result;
    }
    imu.yaw = yaw;

    //补偿惯导传输延时
    forecast_gesture(&imu, satmov->gesture_delay);
    //将地理指向转换为载体指向
   // result = track_earth_2_body_point(earth_point, &imu, &body_point);
     result = earth_2_body_point(earth_point, &imu, &body_point);

//     LOG_W("earth_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(earth_point->azimuth),
//     					                              RADIAN_2_ANGLE(earth_point->elevation),
//     					                              RADIAN_2_ANGLE(earth_point->polarity));
//     LOG_W("body_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(body_point.azimuth),
//    		                                         RADIAN_2_ANGLE(body_point.elevation),
//													 RADIAN_2_ANGLE(body_point.polarity));
////       LOG_W("--body_point:az=%.3f，el=%.3f,pol=%.3f", body_point.azimuth,
////         		                                       body_point.elevation,
////     												   body_point.polarity);
//     LOG_W("imu:yaw=%2.f,roll=%.3f,pitch=%.3f\n",RADIAN_2_ANGLE(imu.yaw),RADIAN_2_ANGLE(imu.roll),RADIAN_2_ANGLE(imu.pitch));



//    if(result != ERR_OK) {
//        return result;
//    }

    //检查载体指向是否有效
//    if(body_point.elevation > ANGLE_2_RADIAN(70.0)) {
//        return ERR_INVAL;
//    }
    if(body_point.elevation>1.4) {
          return ERR_INVAL;
      }
    return ERR_OK;
}

//设置指向
static int32_t set_point(satmov_object_t *satmov, const satmov_point_t *rx_earth_point, const satmov_point_t *tx_earth_point, uint8_t TR, float yaw, uint32_t repetition)
{
	uint32_t i;
	int32_t result;
	satmov_imu_t imu;
	satmov_point_t body_point;

	for(i=0;i<repetition;i++){
		result = read_imu_internal(satmov,&imu);
		if(result != ERR_OK){
			return result;
		}
		imu.yaw = yaw;

//		LOG_I("--%.3f，%.3f,%.3f", RADIAN_2_ANGLE(rx_earth_point->azimuth),
//							RADIAN_2_ANGLE(rx_earth_point->elevation),
//							RADIAN_2_ANGLE(rx_earth_point->polarity));

		if(TR &SATMOV_RX) {
		//将地理指向转换为载体指向
//		result =track_earth_2_body_point(rx_earth_point,&imu,&body_point);
		result =earth_2_body_point(rx_earth_point,&imu,&body_point);
		if(result !=ERR_OK){
			return result;
		}
		result =earth_2_body_polarity(rx_earth_point,&imu,&body_point);
		if(result !=ERR_OK){
			return result;
		}

//		  LOG_W("earth_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(rx_earth_point->azimuth),
//		     					                              RADIAN_2_ANGLE(rx_earth_point->elevation),
//		     					                              RADIAN_2_ANGLE(rx_earth_point->polarity));
//		  LOG_W("body_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(body_point.azimuth),
//		    		                                         RADIAN_2_ANGLE(body_point.elevation),
//															 RADIAN_2_ANGLE(body_point.polarity));
//		//       LOG_W("--body_point:az=%.3f，el=%.3f,pol=%.3f", body_point.azimuth,
//		//         		                                       body_point.elevation,
//		//     												   body_point.polarity);
//		  LOG_W("imu:yaw=%.3f,roll=%.3f,pitch=%.3f\n",RADIAN_2_ANGLE(imu.yaw),RADIAN_2_ANGLE(imu.roll),RADIAN_2_ANGLE(imu.pitch));
//
//



		//设置天线指向
		result = antenna_point_internal(satmov,&body_point,SATMOV_RX);
		if(result != ERR_OK) {
			return result;
		}
		}

		vTaskDelay(4);


	}
	return ERR_OK;

}

//空闲状态
static int32_t idle(satmov_object_t *satmov)
{
    int32_t result;

    //关闭天线
    result = antenna_off_internal(satmov, SATMOV_TR);
    if(result != ERR_OK) {
        return result;
    }

    //等待退出此状态
    for(; !state_is_change(satmov); ){
        vTaskDelay(20);
    }
    return ERR_OK;
}

//等待GPS就绪
static int32_t wait_gps(satmov_object_t *satmov, uint32_t timeout)
{
	satmov_gps_t gps;
	uint8_t result;
	while(timeout--)
	{
		//读gps信号
		result = satmov->read_gps(satmov,&gps);
		if(result != ERR_OK)
		{
			return result;
		}
		if(gps.state == SATMOV_GPS_LOCATION)
		{
			satmov->set_state = SATMOV_READY_STATE;
			return ERR_OK;;
		}
		vTaskDelay(1);
	}

    return ERR_TIMEOUT;
}

static int32_t azimuth_scan(satmov_object_t *satmov, float start, float stop, float step, float *best_yaw)
{
	int32_t result;
	float scan_yaw;
	float max_cnr;
	float current_cnr;
	satmov_gps_t gps;
	satmov_point_t earth_point;

	if(start< 0.0){
		start = 0.0;
	}
	if(stop >2*PI){
		stop = 2*PI;
//		stop =stop- 2*PI;
	}
	max_cnr =-120.0;

	for(scan_yaw=start;scan_yaw<stop;scan_yaw+=step){
		//LOG_W("scan_yaw=%.3f", scan_yaw*180/PI);
		//检查是否退出当前状态
		if(state_is_change(satmov)){
			return ERR_OK;
		}

		//读取GPS数据
		result =read_gps_internal(satmov,&gps);
		if(result !=ERR_OK) {
			return result;
		}

		if(gps.state !=SATMOV_GPS_INVALID){
			//计算地理指向和极化
			satmov->calculate_earth_point_geo(satmov,&gps,&earth_point);
//			LOG_W("--earth_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(earth_point.azimuth),
//					RADIAN_2_ANGLE(earth_point.elevation),
//					RADIAN_2_ANGLE(earth_point.polarity));
		}
		else {
			//切换到等待GPS状态
			switch_state_weak(satmov,SATMOV_WAIT_GPS_STATE);
			return ERR_INVAL;
		}

		//检查此姿态下载体指向是否有效
		result = check_point(satmov,&earth_point,scan_yaw);
		if(result !=ERR_OK){
			LOG_I("check_point failed.\n");
			vTaskDelay(2);
			continue;
		}

		//设置指向
		result = set_point(satmov,&earth_point,&earth_point,SATMOV_RX,scan_yaw,satmov->currect_info.scan_count);

		if(result !=ERR_OK){
			LOG_I("set_point failed.\n");
			return result;
		}
		//获取圆锥上某一点的信噪比
		result =read_cnr_internal(satmov,&current_cnr);
		//LOG_W("cnr=%.3f", current_cnr);
		if(result != ERR_OK){
			LOG_I("read_cnr_internal failed.\n");
			return result;
		}
		//LOG_I("--cnr=%.3f", current_cnr);
		//记录信号最强的点，以及最佳指向
		if(current_cnr >max_cnr){
//		if(current_cnr >-121.0){
			max_cnr =current_cnr;
			*best_yaw =scan_yaw;
		}
	}
	//if(max_cnr > satmov->currect_info.threshold_cnr){
	if(max_cnr > -50){
		return ERR_OK;
	}


    return ERR_ROUTINE;
}

static int32_t correct_yaw(satmov_object_t *satmov)
{
//	while(1)
//	{
//		vTaskDelay(2000);
//		break;
//	}
//    satmov->set_state = SATMOV_CORRECT_COMPLETE_STATE;

//	correct.pitch = 2.0f;
//	correct.roll = 2.0f;
//	correct.yaw = 2.0f;
//	satmov->set_state = SATMOV_CORRECT_COMPLETE_STATE;
//   return ERR_OK;

	int32_t i;
	int32_t result;
	float best_yaw;
	float constant_step;
	float constant_step2;
	float constant_step4;
	satmov_gps_t gps;
	satmov_point_t earth_point;

	//关闭收天线
	result =antenna_on_internal(satmov,SATMOV_RX);
	//此处为啥不是 off ???????  result =antenna_ff_internal(satmov,SATMOV_RX);
	if(result !=ERR_OK) {
		return result;
	}
	//关闭发射
	result =antenna_off_internal(satmov,SATMOV_TX);
	if(result != ERR_OK) {
		return result;
	}

	//等待多模机数据彻底失锁
	vTaskDelay(200);

	//开接收天线
	result = antenna_on_internal(satmov,SATMOV_RX);
	if(result != ERR_OK) {
		return result;
	}

/////////////////////////////////////////////////////////////////
	constant_step=PI/144.0;
	constant_step2=PI/360.0;
	constant_step4=PI/720.0;
	//校准入口条件是什么？？？为啥小于0？？？，可以尝试无条件校准
	if(satmov->currect_info.yaw < 0.0){

		//进行航向角相略校准
		for(i=0; i<5; i++){
			//result = azimuth_scan(satmov,0.0f,2.0f*PI,satmov->currect_info.beamwidth*PI/450.0f, &best_yaw);
			result = azimuth_scan(satmov,0.0f,2.0f*PI,constant_step, &best_yaw);
//			result = azimuth_scan(satmov,0.0f,2.0f*PI,satmov->currect_info.beamwidth/2.0f, &best_yaw);
			//result = azimuth_scan(satmov,0.0f,2.0f*PI,2.0f, &best_yaw);
		//进行状态切换检查
			if(state_is_change(satmov)){
				return ERR_OK;
			}
			if(result == ERR_OK){
				break;
			}
		}

		//检查校准结果
		if(i >=5){
			LOG_I("i>5");
			satmov->currect_info.yaw = ANGLE_2_RADIAN(-1.0);
			satmov->fault_code = SATMOV_CORRECT_FAULT;
			satmov->next_state = SATMOV_SYSTEM_FAULT_STATE;
			//上报校准结果
			if(satmov->currect_complete){
				LOG_I("currect_complete\n");
				satmov->currect_complete(satmov,ANGLE_2_RADIAN(-1.0f));
			}
				return ERR_ROUTINE;
		}

		LOG_E("azimuth_scan\n");
		//再进行两次精细校准
		//azimuth_scan(satmov,best_yaw-ANGLE_2_RADIAN(5.0), best_yaw+ANGLE_2_RADIAN(5.0), satmov->currect_info.beamwidth/5.0f,&best_yaw);
		azimuth_scan(satmov,best_yaw-ANGLE_2_RADIAN(5.0), best_yaw+ANGLE_2_RADIAN(5.0), constant_step2,&best_yaw);
		//进行状态切换检查
		if(state_is_change(satmov)){
			LOG_E("state_is_change\n");
			return ERR_OK;
		}
		//azimuth_scan(satmov,best_yaw-ANGLE_2_RADIAN(3.0),best_yaw+ANGLE_2_RADIAN(3.0),satmov->currect_info.beamwidth/10.0f,&best_yaw);
		azimuth_scan(satmov,best_yaw-ANGLE_2_RADIAN(3.0),best_yaw+ANGLE_2_RADIAN(3.0),constant_step4,&best_yaw);
		LOG_W("best_yaw=%.3f, i=%d", best_yaw*180/PI, i);

		//进行状态切换检查

		if(state_is_change(satmov)){
			LOG_E("state_is_change\n");
			return ERR_OK;
		}
		//记录校准值
//		best_yaw=best_yaw-1.0f*PI;
//		if(best_yaw<0.0f){
//			best_yaw=best_yaw+2.0f*PI;
//		}

		satmov->best_yaw=best_yaw;
		LOG_I("++best_yaw=%.3f\n", best_yaw);
	}
	else {
	//使用指定的角度
		satmov->best_yaw=ANGLE_2_RADIAN(satmov->currect_info.yaw);
	}

	//读取GPS数据
	result=read_gps_internal(satmov,&gps);
	if(result!=ERR_OK){
		LOG_E("read_gps_internal failed.\n");
		return result;
	}
	if(gps.state != SATMOV_GPS_INVALID){
	//计算地理指向和极化
		satmov->calculate_earth_point_geo(satmov,&gps,&earth_point);
	}
	else {
	//切换到等待GPS状态
		switch_state_weak(satmov,SATMOV_WAIT_GPS_STATE);
		LOG_E("SATMOV_GPS_INVALID\n");
		return ERR_INVAL;
	}
	//设置天线指向
	result = set_point(satmov,&earth_point,&earth_point,SATMOV_RX,satmov->best_yaw,1);
	if(result !=ERR_OK) {
		LOG_E("set_point failed.\n");
		return result;
	}
	//上报校准结果
	if(satmov->currect_complete){
		LOG_E("currect_complete\n");
		satmov->currect_complete(satmov,satmov->best_yaw);
	}

	LOG_E("end.\n");
	//切换到校准完成状态
	satmov->next_state = SATMOV_CORRECT_COMPLETE_STATE;
	return ERR_OK;

}



void get_correct(correct_imu_t *cor)
{
	memcpy(cor,&correct,sizeof(correct_imu_t));
}



//空闲状态
static int32_t correct_yaw_complete(satmov_object_t *satmov)
{
    //等待退出此状态
	LOG_E("correct_yaw complete.....\n");
    for(; !state_is_change(satmov); ){
        vTaskDelay(2000);
    }
    return ERR_OK;
}

//void MUILT_beam_switch_gpio1(u8 channel)
//{
//	ps_gpio_config(0, PS_GPIO_MODE_OUTPUT);
//	ps_gpio_write(0, channel);  			//翻转
//}

static int32_t program_tracking(satmov_object_t *satmov)
{
    int32_t result;
    satmov_gps_t gps;
    satmov_point_t earth_point;
    satmov_point_t body_point;
    satmov_imu_t imu;
    correct_imu_t cor;
    int issue_point_folag=0;
    float cnr;
    float tmp_yaw;
    float current_cnr;
    //开收发天线
    LOG_W("best_yaw=%.3f\n", satmov->best_yaw);
    result = antenna_on_internal(satmov, SATMOV_RX);
    if(result != ERR_OK) {
        return result;
    }

    for(; ; ) {

    	if(state_is_change(satmov)) {
			break;
		}
    	//读gps信号
    	result = satmov->read_gps(satmov,&gps);
		if(result != ERR_OK)
		{
			return result;
		}

		if(gps.state == SATMOV_GPS_LOCATION)
		{
			satmov->calculate_earth_point_leo(satmov,&gps,&earth_point);
		}else{
			switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
			return ERR_INVAL;
		}
		//计算载体指向
		 //将地理指向转换为载体指向
		result = satmov->read_imu(satmov,&imu);
		if(result != ERR_OK)
		{
			switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
			return result;
		}
//		使用校准imu后的值

		//lutao modify 2022
		//imu.yaw =satmov->best_yaw;

//		tmp_yaw=satmov->best_yaw;
//		LOG_W("best_yaw=%.3f\n", tmp_yaw);
//		result = track_earth_2_body_point(&earth_point, &imu, &body_point);
//		result = earth_3_body_point(&earth_point, &imu, &tmp_yaw,&body_point);
		result = earth_2_body_point(&earth_point, &imu,&body_point);
//		result =earth_3_body_polarity(&earth_point,&imu,&tmp_yaw,&body_point);
		result =earth_2_body_polarity(&earth_point,&imu,&body_point);
		if(result !=ERR_OK){
			return result;
		}

//		 LOG_W("earth_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(earth_point.azimuth),
//		     					                              RADIAN_2_ANGLE(earth_point.elevation),
//		     					                              RADIAN_2_ANGLE(earth_point.polarity));

//		 LOG_W("body_point:az=%.3f，el=%.3f,pol=%.3f", RADIAN_2_ANGLE(body_point.azimuth),
//		    		                                         RADIAN_2_ANGLE(body_point.elevation),
//															 RADIAN_2_ANGLE(body_point.polarity));
//		if(result != ERR_OK) {
//			switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
//			return result;
//		}
		if(satmov->antenna_point(satmov,&body_point,SATMOV_RX) != ERR_OK)
		{
			switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
			return result;
		}

//	read_cnr_internal(satmov,&current_cnr);
//	 LOG_W("time=%lu,body_point:az=%.3f,el=%.3f,yaw=%.3f,cnr=%.3f,",
//			 	 	 	 	 	 	 	 	 	 	 gps.second,
//			 	 	 	 	 	 	 	 	 	 	 RADIAN_2_ANGLE(body_point.azimuth),
//			 	 	 	 	 	 	 	 	 	 	 RADIAN_2_ANGLE(body_point.elevation),
//													 RADIAN_2_ANGLE(imu.yaw) ,
//													 current_cnr);


//
//		if(issue_point_folag==1)
//		{
//			MUILT_beam_switch_gpio1(1);
//			issue_point_folag=0;
//		}
//		else
//		{
//			MUILT_beam_switch_gpio1(0);
//			issue_point_folag=1;
//		}

		/*高轨采用，读多模机数据
		result = satmov->read_cnr(satmov,&cnr);
		if(result != ERR_OK)
		{
			switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
			return result;
		}
		*/

        vTaskDelay(4);
    }
    return ERR_OK;
}

//错误处理
void error_handle(satmov_object_t *satmov, uint32_t last_state, uint32_t fault_code, uint32_t keep_time)
{
    uint32_t time;

    //达到保持时间后清除错误，切换到等待GPS状态
    for(time=0; time<keep_time; time+=10) {
        //检查是否退出当前状态
        if(state_is_change(satmov)) {
            return;
        }
        vTaskDelay(10);
    }

    //切换到等待GPS状态
    switch_state_weak(satmov, SATMOV_WAIT_GPS_STATE);
}

void tracking_thread(void *arg)
{
    uint8_t last_state;

    satmov_object_t *satmov = (satmov_object_t *)arg;
    while(1)
    {
    	vTaskDelay(1000);
        //更新用户手动设置的状态
        state_is_change(satmov);

        switch(satmov->next_state) {
        //初始化状态
        case SATMOV_INITIAL_STATE:
            satmov->current_state = SATMOV_INITIAL_STATE;
            idle(satmov);
            break;
        //挂起状态
        case SATMOV_SUSPEND_STATE:
            satmov->current_state = SATMOV_SUSPEND_STATE;
            idle(satmov);
            break;
        //等待GPS状态
        case SATMOV_WAIT_GPS_STATE:
            satmov->current_state = SATMOV_WAIT_GPS_STATE;
            wait_gps(satmov, 5*1000);
            break;
        //设备就绪
        case SATMOV_READY_STATE:
            satmov->current_state = SATMOV_READY_STATE;
            idle(satmov);
            break;
        //航向角校准
        case SATMOV_CORRECT_STATE:
            satmov->current_state = SATMOV_CORRECT_STATE;
            correct_yaw(satmov);
            break;
        //校准完成
        case SATMOV_CORRECT_COMPLETE_STATE:
            satmov->current_state = SATMOV_CORRECT_COMPLETE_STATE;
            correct_yaw_complete(satmov);
            break;
        //程序跟踪
        case SATMOV_PROGRAM_TRACKING_STATE:
            satmov->current_state = SATMOV_PROGRAM_TRACKING_STATE;
            program_tracking(satmov);
            break;
        //错误状态
        case SATMOV_SYSTEM_FAULT_STATE:
            last_state = satmov->current_state;
            satmov->current_state = SATMOV_SYSTEM_FAULT_STATE;
            if(!satmov->error_handle) {
                error_handle(satmov, last_state, satmov->fault_code, 1000);
            }
            else {
                satmov->error_handle(satmov, last_state, satmov->fault_code, 1000);
            }
            //清除错误码
            satmov->fault_code = 0;
            break;
        //未定义状态
        default:
            satmov->fault_code = SATMOV_STATE_FAULT;
            switch_state(satmov, SATMOV_SYSTEM_FAULT_STATE);
            break;
        }
    }
}

//创建线程
int32_t satmov_init(satmov_object_t *satmov)
{
    TaskHandle_t handle;
    portBASE_TYPE result;

    if((!satmov->read_imu)
            || (!satmov->read_gps)
            || (!satmov->read_cnr)
            || (!satmov->antenna_point)
            || (!satmov->antenna_on)
            || (!satmov->antenna_off)
            || (!satmov->calculate_earth_point_geo)
            || (!satmov->calculate_earth_point_leo)) {
        return ERR_INVAL;
    }

    //初始化工作状态
    satmov->set_state = 0x00;
    satmov->next_state = SATMOV_INITIAL_STATE;
    satmov->current_state = SATMOV_INITIAL_STATE;
    satmov->fault_code = 0x00;
    satmov->best_yaw = -1.0f;
    memset(&satmov->currect_info, 0, sizeof(satmov_currect_info_t));

    //创建启动线程
    result = xTaskCreate(tracking_thread,
                         "satmove_thread",
                         1024*10,
                         satmov,
                         configMAX_PRIORITIES-3,
                         &handle);
    if(result != pdPASS) {
        return ERR_NOMEM;
    }
    return ERR_OK;
}

int32_t satmov_start(satmov_object_t *satmov)
{
    if((satmov->current_state == SATMOV_INITIAL_STATE) && (satmov->set_state == 0x00)) {
        satmov->set_state = SATMOV_WAIT_GPS_STATE;
    }
    return ERR_OK;
}

int32_t satmov_tracking(satmov_object_t *satmov)
{
    if((satmov->current_state == SATMOV_CORRECT_COMPLETE_STATE) && (satmov->set_state == 0x00)) {
        satmov->set_state = SATMOV_PROGRAM_TRACKING_STATE;
        LOG_E("satmov->set_state = SATMOV_PROGRAM_TRACKING_STATE\n");
        return ERR_OK;
    }
    else {
        return ERR_BUSY;
    }
}

int32_t satmov_suspend(satmov_object_t *satmov)
{
    if((satmov->current_state != SATMOV_SUSPEND_STATE) && (satmov->set_state == 0x00)) {
        satmov->set_state = SATMOV_SUSPEND_STATE;
    }
    return ERR_OK;
}

int32_t satmov_resume(satmov_object_t *satmov)
{
    if((satmov->current_state == SATMOV_SUSPEND_STATE) && (satmov->set_state == 0x00)) {
        satmov->set_state = SATMOV_WAIT_GPS_STATE;
    }
    return ERR_OK;
}

uint32_t satmov_get_state(satmov_object_t *satmov)
{
    return satmov->current_state;
}

int32_t satmov_currect_yaw(satmov_object_t *satmov, const satmov_currect_info_t *currect_info)
{
    if((satmov->current_state != SATMOV_INITIAL_STATE) &&
            (satmov->current_state != SATMOV_WAIT_GPS_STATE) &&
            (satmov->set_state == 0x00)) {
        satmov->currect_info = *currect_info;
        satmov->set_state = SATMOV_CORRECT_STATE;
        return ERR_OK;
    }
    else {
        return ERR_BUSY;
    }
}

