/*
 * device_monitor.c
 *
 *  Created on: 2021年4月28日
 *      Author: Administrator
 */

#include "device_monitor.h"
#include "../satmov/satmov_port.h"
#include "../tr_arhs/adis16465_port.h"

#define DEBUG_TAG "device_monitor,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../driver/core/debug.h"

#define LFP_SAMPLING_PERIOD 100
#define LFP_SAMPLING_FREQ   (1000/LFP_SAMPLING_PERIOD)

static device_state_t global_device_state;

static biquad_filter_t acu_temperature_filter;
static biquad_filter_t acu_electricity_filter;
static biquad_filter_t acu_voltage_filter;

static biquad_filter_t tx_temperature_filter[ANTENNA_TX_TOTAL];
static biquad_filter_t tx_electricity_filter[ANTENNA_TX_TOTAL];
static biquad_filter_t tx_voltage_filter[ANTENNA_TX_TOTAL];

static biquad_filter_t rx_temperature_filter[ANTENNA_RX_TOTAL];
static biquad_filter_t rx_electricity_filter[ANTENNA_RX_TOTAL];
static biquad_filter_t rx_voltage_filter[ANTENNA_RX_TOTAL];

//static device_t *imu;
//#define DEBUG
static device_t *transducer;
static device_t *beacon;
static device_t *tmp112_0;
static device_t *ina22x_0;
#ifdef USING_INA22x_1
static device_t *ina22x_1;
#endif

static uint32_t monitor_mode = 0;

static void monitor_thread(void *arg)
{
    uint8_t i;
    uint8_t j;
    uint8_t state;
    float temp;
    int32_t result;
    ssize_t lenght0;
#ifdef USING_INA22x_1
    ssize_t lenght1;
#endif
 //   inax_data_t ina0;
#ifdef USING_INA22x_1
    inax_data_t ina1;
#endif
    antenna_temperature_t antenna_temperature;
    antenna_power_t antenna_power;
    device_state_t device_state;

    while(1)
    {
        vTaskDelay(LFP_SAMPLING_PERIOD);

        memset(&device_state, 0, sizeof(device_state));
        device_state.max_temperature = -99.0f;

        //接收天线自检

        if(!(monitor_mode & 0x01)) {
           for(i=0; i<ANTENNA_RX_TOTAL; i++) {
        	// for(i=0; i<1; i++) {
                //检查直流供电状态
                if(!antenna_power_state(ANTENNA_RX, i)) {
                    continue;
                }

                //接收天线温度
                result = antenna_read_temperature(ANTENNA_RX, i, &antenna_temperature);
                if(result == ERR_OK) {
                    //取最高温度作为阵面温度
                    for(j=0, temp=-99.0f; j<3; j++) {
                        if(temp < antenna_temperature.board_temperature[j]) {
                            temp = antenna_temperature.board_temperature[j];
                        }
                    }
                    device_state.rx_temperature[i] = biquad_filter_apply(&rx_temperature_filter[i], temp);
#ifdef DEBUG
                    LOG_E("rx_temperature[%d]=%.2f\n",i,device_state.rx_temperature[i]);
#endif
                    //                    //记录设备最高温度
//                    if(device_state.max_temperature < device_state.rx_temperature[i]) {
//                        device_state.max_temperature = device_state.rx_temperature[i];
//                    }

                }
                else {
                    //温度读取失败则默认温升1°没200ms
                    device_state.rx_temperature[i] = global_device_state.rx_temperature[i] + 1.0f;
                }

                //接收天线电流、电压
                result = antenna_read_power(ANTENNA_RX, i, &antenna_power);
                if(result == ERR_OK) {
                    device_state.rx_electricity[i] = biquad_filter_apply(&rx_electricity_filter[i], antenna_power.total_electricity);
                    device_state.rx_voltage[i] = biquad_filter_apply(&rx_voltage_filter[i], antenna_power.total_voltage);
#ifdef DEBUG
                    LOG_E("rx_electricity[%d]=%.2f\n",i,device_state.rx_electricity[i]);
                    LOG_E("rx_voltage[%d]=%.2f\n",i,device_state.rx_voltage[i]);
#endif
                }

                //接收天线自检
                if(device_state.rx_temperature[i] > RX_UPPER_LIMIT_TEMP) {
                    device_state.global_fault_code |= RX_TEMP_ANOMALY;
                }
                if(device_state.rx_electricity[i] > RX_UPPER_LIMIT_ELECTRICITY) {
                    device_state.global_fault_code |= RX_ELECTRICITY_ANOMALY;
                }
                if((device_state.rx_voltage[i] < RX_LOWER_LIMIT_VOLTAGE) || (device_state.rx_voltage[i] > RX_UPPER_LIMIT_VOLTAGE)) {
                    device_state.global_fault_code |= RX_VOLTAGE_ANOMALY;
                }
                result = antenna_selfcheck(ANTENNA_RX, i);
                if(result != ERR_OK) {
                    device_state.global_fault_code |= RX_CTRL_ANOMALY;
                }
            }
        }
        else {
            for(i=0; i<ANTENNA_RX_TOTAL; i++) {
                device_state.rx_temperature[i] = 70.0f;
            }
        }

        //发射天线自检
        if(!(monitor_mode & 0x02)) {
            for(i=0; i<ANTENNA_TX_TOTAL; i++) {
                //检查直流供电状态
                if(!antenna_power_state(ANTENNA_TX, i)) {
                    continue;
                }

                //发射天线温度
                result = antenna_read_temperature(ANTENNA_TX, i, &antenna_temperature);
                if(result == ERR_OK) {
                    //取最高温度作为阵面温度
                    for(j=0, temp=-99.0f; j<3; j++) {
                        if(temp < antenna_temperature.board_temperature[j]) {
                            temp = antenna_temperature.board_temperature[j];
                        }
                    }
                    device_state.tx_temperature[i] = biquad_filter_apply(&tx_temperature_filter[i], temp);
                    //记录设备最高温度
                    if(device_state.max_temperature < device_state.tx_temperature[i]) {
                        device_state.max_temperature = device_state.tx_temperature[i];
                    }
                }
                else {
                    //温度读取失败则默认温升1°没200ms
                    device_state.tx_temperature[i] = global_device_state.tx_temperature[i]+1.0f;
                }

                //发射天线电流、电压
                result = antenna_read_power(ANTENNA_TX, i, &antenna_power);
                if(result == ERR_OK) {
                    device_state.tx_electricity[i] = biquad_filter_apply(&tx_electricity_filter[i], antenna_power.total_electricity);
                    device_state.tx_voltage[i] = biquad_filter_apply(&tx_voltage_filter[i], antenna_power.total_voltage);
                }

                //发射天线自检
                if(device_state.tx_temperature[i] > TX_UPPER_LIMIT_TEMP) {
                    device_state.global_fault_code |= TX_TEMP_ANOMALY;
                }
                if(device_state.tx_electricity[i] > TX_UPPER_LIMIT_ELECTRICITY) {
                    device_state.global_fault_code |= TX_ELECTRICITY_ANOMALY;
                }
                if((device_state.tx_voltage[i] < TX_LOWER_LIMIT_VOLTAGE) || (device_state.tx_voltage[i] > TX_UPPER_LIMIT_VOLTAGE)) {
                    device_state.global_fault_code |= TX_VOLTAGE_ANOMALY;
                }
                result = antenna_selfcheck(ANTENNA_TX, i);
                if(result != ERR_OK) {
                    device_state.global_fault_code |= TX_CTRL_ANOMALY;
                }
            }
        }
        else {
            for(i=0; i<ANTENNA_TX_TOTAL; i++) {
                device_state.tx_temperature[i] = 70.0f;
            }
        }

        if(!(monitor_mode & 0x04)) {
            //ACU温度  ******
//            lenght0 = device_read(tmp112_0, &temp, sizeof(temp));
//            if(lenght0 == sizeof(temp)) {
//                device_state.acu_temperature = biquad_filter_apply(&acu_temperature_filter, temp);
//#ifdef DEBUG
//                LOG_E("acu_temperature=%.2f\n",device_state.acu_temperature);
//#endif
////                //记录设备最高温度
////                if(device_state.max_temperature < device_state.acu_temperature) {
////                    device_state.max_temperature = device_state.acu_temperature;
////                }
//            }
//            else {
//                device_state.global_fault_code |= ACU_CTRL_ANOMALY;
//            }
            //ACU电流、电压
           // lenght0 = device_read(ina22x_0, &ina0, sizeof(ina0));
    #ifdef USING_INA22x_1
            lenght1 = device_read(ina22x_1, &ina1, sizeof(ina1));
            if((lenght0 == sizeof(ina0)) && (lenght1 == sizeof(ina1))) {
                device_state.acu_electricity = biquad_filter_apply(&acu_electricity_filter, ina0.electricity+ina1.electricity);
                device_state.acu_voltage = biquad_filter_apply(&acu_voltage_filter, (ina0.voltage+ina1.voltage)/2.0f);
            }
            else {
                device_state.global_fault_code |= ACU_CTRL_ANOMALY;
            }
    #else
////            if(lenght0 == sizeof(ina0)) {
//            if(lenght0 == sizeof(char)) {
////                device_state.acu_electricity = biquad_filter_apply(&acu_electricity_filter, ina0.electricity);
////                device_state.acu_voltage = biquad_filter_apply(&acu_voltage_filter, ina0.voltage);
//#ifdef DEBUG
//                LOG_E("acu_electricity=%.2f,acu_voltage=%.2f\n",
//                		device_state.acu_electricity,device_state.acu_voltage);
//#endif
//            }
//            else {
//                device_state.global_fault_code |= ACU_CTRL_ANOMALY;
//            }
    #endif
            //ACU自检
            if(device_state.acu_temperature > ACU_UPPER_LIMIT_TEMP) {
                device_state.global_fault_code |= ACU_TEMP_ANOMALY;
            }
            if(device_state.acu_electricity > ACU_UPPER_LIMIT_ELECTRICITY) {
                device_state.global_fault_code |= ACU_ELECTRICITY_ANOMALY;
            }
            if((device_state.acu_voltage < ACU_LOWER_LIMIT_VOLTAGE) || (device_state.acu_voltage > ACU_UPPER_LIMIT_VOLTAGE)) {
                device_state.global_fault_code |= ACU_VOLTAGE_ANOMALY;
            }

            //惯导自检
//            result = rav260_check(imu);
//            if(result != ERR_OK) {
//                device_state.global_fault_code |= IMU_CTRL_ANOMALY;
//            }

            //信标机自检
            result = beacon_check(beacon);
            if(result != ERR_OK) {
                device_state.global_fault_code |= BEAM_CTRL_ANOMALY;
            }
#ifdef DEBUG
            uint32_t version;
			uint8_t mode;
            beacon_read_version(beacon,&version);

            beacon_set_mode(beacon,0x01);
            beacon_get_mode(beacon,&mode);
            LOG_E("version=%ld，mode=%d,\n",version,mode);
#endif
            //变频器自检
//            transducer_read_state(transducer, &device_state.tr_temperature, &device_state.tr_electricity, &device_state.tr_voltage);
//            result = transducer_check(transducer, &state);
//            if(result != ERR_OK) {
//                device_state.global_fault_code |= TRANSDUCER_CTRL_ANOMALY;
//            }
//            if(state != 0) {
//                device_state.global_fault_code |= TRANSDUCER_STATE_ANOMALY;
//            }
#ifdef DEBUG
            float lo;
            //transducer_set_lo(transducer,9.75);
            transducer_get_lo(transducer,&lo);
            LOG_E("lo=%.2f\n",lo);
#endif
        }
#ifdef DEBUG /*imu*/
        adis16465_attitude_t data;
        adis16465_imu1_get_attitude(&data);
        LOG_E("roll=%.2f,yaw=%.2f,pitch=%.2f\n",
        		data.euler_angle.roll,data.euler_angle.yaw,data.euler_angle.pitch);
        LOG_E("x=%.2f,y=%.2f,z=%.2f\n",
                		data.gyro.x,data.gyro.y,data.gyro.z);
#endif
#ifdef DEBUG /*gps*/
        read_gps_temp();
        sleep(1);
#endif

        taskENTER_CRITICAL();
        global_device_state = device_state;
        taskEXIT_CRITICAL();
    }
}

int32_t get_device_state(device_state_t *device_state)
{
    if(!device_state) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *device_state = global_device_state;
    taskEXIT_CRITICAL();

    return ERR_OK;
}

int32_t device_monitor_set(uint8_t mode, uint8_t state)
{
    if(state) {
        monitor_mode &= ~(0x01 << mode);
    }
    else {
        monitor_mode |= 0x01 << mode;
    }

    return ERR_OK;
}

int32_t device_monitor_init(void)
{
    uint32_t i;
    int32_t result;
    TaskHandle_t handle;
    portBASE_TYPE xresult;

    //打开设备
//    imu = device_open("IMU0", DEVICE_RDWR);
//    if(!imu) {
//        LOG_E("open IMU0 failed");
//        return ERR_ROUTINE;
//    }
    transducer = device_open("transducer0", DEVICE_RDWR);
    if(!transducer) {
        LOG_E("open transducer0 failed");
        return ERR_ROUTINE;
    }
    beacon = device_open("beacon0", DEVICE_RDWR);
    if(!beacon) {
        LOG_E("open beacon0 failed");
        return ERR_ROUTINE;
    }
//    tmp112_0 = device_open("tmp112_0", DEVICE_RDWR);
//    if(!tmp112_0) {
//        LOG_E("open tmp112_0 failed");
//        return ERR_ROUTINE;
//    }
//    ina22x_0 = device_open(INAx0_NAME, DEVICE_RDWR);
//    if(!ina22x_0) {
//        LOG_E("open "INAx0_NAME" failed");
//        return ERR_ROUTINE;
//    }
#ifdef USING_INA22x_1
    ina22x_1 = device_open(INAx1_NAME, DEVICE_RDWR);
    if(!ina22x_1) {
        LOG_E("open "INAx1_NAME" failed");
        return ERR_ROUTINE;
    }
#endif

    //初始化数据滤波器
    result = biquad_filter_init_LPF(&acu_temperature_filter, LFP_SAMPLING_FREQ, 1.0f);
    if(result != ERR_OK) {
        LOG_E("initialize filter failed");
        return result;
    }
    result = biquad_filter_init_LPF(&acu_electricity_filter, LFP_SAMPLING_FREQ, 1.0f);
    if(result != ERR_OK) {
        LOG_E("initialize filter failed");
        return result;
    }
    result = biquad_filter_init_LPF(&acu_voltage_filter, LFP_SAMPLING_FREQ, 1.0f);
    if(result != ERR_OK) {
        LOG_E("initialize filter failed");
        return result;
    }
    for(i=0; i<ANTENNA_TX_TOTAL; i++) {
        result = biquad_filter_init_LPF(&tx_temperature_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
        result = biquad_filter_init_LPF(&tx_electricity_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
        result = biquad_filter_init_LPF(&tx_voltage_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
    }
    for(i=0; i<ANTENNA_RX_TOTAL; i++) {
        result = biquad_filter_init_LPF(&rx_temperature_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
        result = biquad_filter_init_LPF(&rx_electricity_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
        result = biquad_filter_init_LPF(&rx_voltage_filter[i], LFP_SAMPLING_FREQ, 1.0f);
        if(result != ERR_OK) {
            LOG_E("initialize filter failed");
            return result;
        }
    }

    //创建监测线程
    xresult = xTaskCreate(monitor_thread,
                         "monitor_thread",
                         3072,
                         NULL,
                         tskIDLE_PRIORITY + 2,
                         &handle );
    if(xresult != pdPASS) {
        LOG_E("create task failed\r\n");
        return ERR_NOMEM;
    }

    return ERR_OK;
}
