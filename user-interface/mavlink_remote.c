/*
 * mavlink_parse.c
 *
 *  Created on: 2022年1月23日
 *      Author: Administrator
 */

#include "mavlink_remote.h"

#define DEBUG_TAG "mavlink_remote,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../driver/core/debug.h"

//数据请求通道列表
#define REQUEST_CHANNEL_LIST    {SERIAL_TYPE, AMP_TYPE}

typedef mavlink_imu_drift_t imu_drift_t;
typedef mavlink_switch_antenna_t switch_antenna_t;
typedef mavlink_set_beacon_t set_beacon_t;
typedef mavlink_set_transducer_t set_transducer_t;
typedef mavlink_set_beam_point_t set_beam_point_t;
typedef mavlink_work_parameter_t work_info_t;
typedef mavlink_currect_yaw_t currect_yaw_t;
typedef mavlink_issue_earth_point_t issue_earth_point_t;

typedef struct{
    int32_t            exit_flag;
    mavlink_interval_t interval;
    mavlink_channel_t  chan;
    SemaphoreHandle_t  join;
}remote_config_t;

typedef struct{
    remote_config_t   *remote_config;
    mavlink_message_t message;
}remote_message_t;

typedef struct {
    float rx_phi;              /*<  接收方位角(°)*/
    float rx_theta;            /*<  接收离轴角(°)*/
    float rx_polarity;         /*<  接收极化角(°)*/
    float rx_freq;             /*<  接收射频频率(MHz)*/
    float tx_phi;              /*<  发射方位角(°)*/
    float tx_theta;            /*<  发射离轴角(°)*/
    float tx_polarity;         /*<  发射极化角(°)*/
    float tx_freq;             /*<  发射射频频率(MHz)*/
    uint8_t rx_polarity_type;  /*<  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
    uint8_t tx_polarity_type;  /*<  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
    satmov_point_t earth_point;
}antenna_state_t;

typedef struct{
    float   electricity;    //电流值
    float   vlotage;        //电压值
    float   rx_osc;         //接收本振频率(GHz)
    uint8_t rx_gain;        //接收增益
    float   tx_osc;         //发射本振频率(GHz)
    uint8_t tx_gain;        //发射增益
    float   if_Value;       //中频电平值
    float   rf_Value;       //射频电平值
    float   temperature;     //变频器工作温度（°）
    uint8_t lock_state;     //锁定状态
    uint8_t fault_code;     //变频器故障码
}transducer_state_t;

typedef struct {
    uint32_t fault_code;           /*<  系统故障码*/
    uint32_t tracking;             /*<  跟踪状态*/
    int16_t acu_temperature;       /*<  ACU工作温度*100（°）*/
    int16_t acu_voltage;           /*<  ACU工作电压*100（V）*/
    int16_t acu_electricity;       /*<  ACU工作电流*100（A）*/
    int16_t tx_temperature[8];     /*<  发射天线工作温度*100（°）*/
    int16_t tx_voltage[8];         /*<  发射天线工作电压*100（V）*/
    int16_t tx_electricity[8];     /*<  发射天线工作电流*100（A）*/
    int16_t rx_temperature[8];     /*<  发射天线工作温度*100（°）*/
    int16_t rx_voltage[8];         /*<  发射天线工作电压*100（V）*/
    int16_t rx_electricity[8];     /*<  发射天线工作电流*100（A）*/
    uint8_t rx_rf_state;           /*<  接收射频状态，0x00关闭，0x01打开*/
    uint8_t tx_rf_state;           /*<  发射射频状态，0x00关闭，0x01打开*/
}machine_state_t;

typedef struct {
    uint8_t           valid;    /*<  数据有效标志，0x00无效，0x01有效 */
    float             gyro_x;   /*<  陀螺仪x轴零漂 */
    float             gyro_y;   /*<  陀螺仪y轴零漂 */
    float             gyro_z;   /*<  陀螺仪z轴零漂 */
    SemaphoreHandle_t lock;
    SemaphoreHandle_t sync;     /*< 同步信号量 */
}imu_drift_command_t;


static QueueHandle_t command_queue;
static QueueHandle_t amp_broadcast_queue;
static QueueHandle_t serial_broadcast_queue;

static device_t *beacon_device;
//static device_t *imu_device;
static device_t *noe_m8;
static device_t *transducer_device;

static mavlink_channel_t share_chan;
static SemaphoreHandle_t share_chan_mutex;

static imu_drift_command_t imu_drift_command;

static int32_t transducer_pass_through = 0;
static int32_t beacon_pass_through = 0;
static int32_t antenna_pass_through = 0;

int32_t read_imu(satmov_object_t *satmov, satmov_imu_t *imu);
int32_t read_gps(satmov_object_t *satmov, satmov_gps_t *gps);

static void mavlink_correct_imu(remote_config_t *remote_config)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("correct_imu");

    result = 0x00;
//    //向RAV260发送校准命令
//    if(rav260_aim(imu_device) != ERR_OK) {
//        result = 0xff;
//    }

//    //校准ADIS16465
//    if(adis16465_imu1_calibration_gyro(NULL, 10000) != ERR_OK) {
//        result = 0xff;
//    }

    //回复校准结果
    mavlink_msg_correct_imu_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_imu_drift(const imu_drift_t *imu_drift)
{
    LOG_D("imu_drift, valid=%d, gyro_x=%f, gyro_y=%f, gyro_z=%f",
            imu_drift->valid, imu_drift->gyro_x, imu_drift->gyro_y, imu_drift->gyro_z);

    //缓存零漂数据
    imu_drift_command.valid = imu_drift->valid;
    imu_drift_command.gyro_x = imu_drift->gyro_x;
    imu_drift_command.gyro_y = imu_drift->gyro_y;
    imu_drift_command.gyro_z = imu_drift->gyro_z;
    //释放请求成功信号量
    xSemaphoreGive(imu_drift_command.sync);
}

static void mavlink_reboot(remote_config_t *remote_config, uint8_t mode)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("reboot mode=%d", mode);

    result = 0x00;
    if(mode == 0x04) {
        //重启对星算法
        satmov1_suspend();
        vTaskDelay(80);
        satmov1_resume();
    }
    else {
        //其他参数暂时不支持
        result = 0xff;
    }
    //回复重启结果
    mavlink_msg_restart_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_read_version(remote_config_t *remote_config, uint8_t mode)
{
    uint8_t result;
    uint8_t software_type;
    uint32_t version;
    mavlink_message_t tx_msg;
    char software_version[64];
    char software_data[64];

    LOG_D("read_version mode=%d", mode);

    software_type = 0xFF;
    result = 0x00;
    if(mode == 0x02) {
        //读取多模机版本
        if(beacon_read_version(beacon_device, &version) == ERR_OK) {
            snprintf(software_version, sizeof(software_version), "%ld", version);
            snprintf(software_data, sizeof(software_data), "--");
            software_type = 0x02;
        }
        else {
            result = 0xff;
        }
    }
    else if(mode == 0x03) {
        //变频器版本
        if(transducer_read_version(transducer_device, &version) == ERR_OK) {
            snprintf(software_version, sizeof(software_version), "%ld", version);
            snprintf(software_data, sizeof(software_data), "--");
            software_type = 0x03;
        }
        else {
            result = 0xff;
        }
    }
    else if(mode == 0x04) {
        //惯导版本
        snprintf(software_version, sizeof(software_version), "rav260_v2.0");
        snprintf(software_data, sizeof(software_data), "--");
        software_type = 0x04;
    }
    else if(mode == 0x05) {
        //接收天线阵面版本
        if(antenna_power_state(ANTENNA_RX, 0x00)) {
            if(antenna_read_version(ANTENNA_RX, 0x00, &version) == ERR_OK) {
                snprintf(software_version, sizeof(software_version), "%ld", version);
                snprintf(software_data, sizeof(software_data), "--");
                software_type = 0x05;
            }
            else {
                result = 0xff;
            }
        }
    }
//    else if(mode == 0x06) {
//        //发射天线阵面版本
//        if(antenna_power_state(ANTENNA_TX, 0x00)) {
//            if(antenna_read_version(ANTENNA_TX, 0x00, &version) == ERR_OK) {
//                snprintf(software_version, sizeof(software_version), "%ld", version);
//                snprintf(software_data, sizeof(software_data), "--");
//                software_type = 0x06;
//            }
//            else {
//                result = 0xff;
//            }
//        }
//    }

    else if(mode == 0x07) {
        //固件版本
        snprintf(software_version, sizeof(software_version), FIRWARE_VERSION);
        snprintf(software_data, sizeof(software_data), "%s;%s", __DATE__, __TIME__);
        software_type = 0x07;
    }
    else {
        //不存在此设备
        result = 0xfe;
    }

    if(result != 0x00) {
        //版本读取失败或者无此设备
        snprintf(software_version, sizeof(software_version), "failed");
        snprintf(software_data, sizeof(software_data), "failed");
    }

    mavlink_msg_report_version_pack_chan(0, 0, remote_config->chan, &tx_msg,
            result, PRODUCT_NAME,
            PRODUCT_ORB, software_data,
            software_version, software_type);

    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_switch_antenna(remote_config_t *remote_config, const switch_antenna_t *switch_antenna)
{
    uint8_t result;
    uint8_t TorR;
    mavlink_message_t tx_msg;

    LOG_D("switch_antenna TorR=%d subset=%d mode=%d", switch_antenna->TorR, switch_antenna->subset, switch_antenna->mode);

    if(switch_antenna->TorR == 0x00) {
        TorR = ANTENNA_RX;
    }
    else {
        mavlink_msg_switch_antenna_result_pack_chan(0, 0, remote_config->chan, &tx_msg, 0x00);
        mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        return ;
    }

    result = 0x00;
    if(switch_antenna->mode == 0x0f) {
        //关闭射频
        if(antenna_rf_vcc_off(TorR) != ERR_OK) {
            result = 0Xff;
        }
    }
    else if(switch_antenna->mode == 0x1f) {
        //开启射频，必须先开直流
        if(antenna_power_state(TorR, ANTENNA_SUBSET_ALL)) {
            if(antenna_rf_vcc_on(TorR) != ERR_OK) {
                result = 0Xff;
            }
        }
        else {
            result = 0Xff;
        }
    }
    else if(switch_antenna->mode == 0xf0) {
        //关闭直流，必须先关闭射频
        if(!antenna_rf_vcc_state(TorR)) {
            if(antenna_power_off(TorR, switch_antenna->subset) != ERR_OK) {
                result = 0Xff;
            }
        }
        else {
            result = 0Xff;
        }
    }
    else if(switch_antenna->mode == 0xf1) {
        //开启直流
        if(antenna_power_on(TorR, switch_antenna->subset) != ERR_OK) {
            result = 0Xff;
        }
    }
    else {
        result = 0Xff;
    }

    mavlink_msg_switch_antenna_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_set_report(remote_config_t *remote_config, uint8_t type, uint32_t interval)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("set_report type=%d interval=%ld", type, interval);

    result = 0x00;
    if(type == 0x00) {
        remote_config->interval.beacon_interval = interval;
        remote_config->interval.imu_interval = interval;
        remote_config->interval.antenna_interval = interval;
        remote_config->interval.transducer_interval = interval;
        remote_config->interval.work_interval = interval;
    }
    else if(type == 0x01) {
        remote_config->interval.work_interval = interval;
    }
    else if(type == 0x02) {
        remote_config->interval.transducer_interval = interval;
    }
    else if(type == 0x03) {
        remote_config->interval.antenna_interval = interval;
    }
    else if(type == 0x04) {
        remote_config->interval.imu_interval = interval;
    }
    else if(type == 0x05) {
        remote_config->interval.beacon_interval = interval;
    }
    else {
        result = 0xff;
    }

    mavlink_msg_set_report_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_set_beacon(remote_config_t *remote_config, const set_beacon_t *set_beacon)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("set_beacon freq=%f sym=%f rang=%d mode=%d",
            set_beacon->freq, set_beacon->sym, set_beacon->rang, set_beacon->mode);

    result = 0x00;
    if(set_beacon->mode == 0x00) {
        //配置DVB
        if(beacon_set_DVB(beacon_device, set_beacon->freq*1000.0f, set_beacon->sym) != ERR_OK) {
            result = 0xff;
        }
    }
    else if(set_beacon->mode == 0x01) {
        //配置检波
        if(beacon_set_detection(beacon_device, set_beacon->freq*1000.0f, set_beacon->sym) != ERR_OK) {
            result = 0xff;
        }
    }
    else if(set_beacon->mode == 0x02) {
        //配置信标
        if(beacon_set_freq(beacon_device, set_beacon->freq*1000.0f) != ERR_OK) {
            result = 0xff;
        }
        if(result == 0x00) {
            if(beacon_set_range(beacon_device, set_beacon->rang) != ERR_OK) {
                result = 0xff;
            }
        }
        if(result == 0x00) {
            if(beacon_set_refresh(beacon_device, 4) != ERR_OK) {
                result = 0xff;
            }
        }
    }
    else {
        result = 0xff;
    }

    if(result == 0x00) {
        //设置工作模式
        if(beacon_set_mode(beacon_device, set_beacon->mode) != ERR_OK) {
            result = 0xff;
        }
    }

    mavlink_msg_set_beacon_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_beam_parameter(remote_config_t *remote_config, const mavlink_beam_parameter_t *beam_parameter)
{
	uint8_t result;
	mavlink_message_t tx_msg;
	result = 0x00;

	LOG_D("mavlink_beam_parameter sat_long=%f sat_name=%s rx_freq=%f rx_pol=%d",
			beam_parameter->sat_longitude, beam_parameter->sat_name, beam_parameter->rx_freq, beam_parameter->rx_polarity_type);

	mavlink_msg_set_beam_parameter_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
	mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_set_transducer(remote_config_t *remote_config, const set_transducer_t *set_transducer)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("set_transducer rx_osc=%f rx_gain=%d tx_osc=%f tx_gain=%d",
            set_transducer->rx_osc, set_transducer->rx_gain, set_transducer->tx_osc, set_transducer->tx_gain);

    result = 0x00;
    if(transducer_set_lo(transducer_device, set_transducer->rx_osc/1000.0f) != ERR_OK) {
        result = 0xff;
    }

//    if(transducer_set_gain(transducer_device,set_transducer->rx_gain)!=ERR_OK)
//    {
//    	result=0xff;
//    }
   // result = 0x00;

    mavlink_msg_set_transducer_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_set_beam_point(remote_config_t *remote_config, const set_beam_point_t *beam_point)
{
    uint8_t result;
    antenna_beam_t antenna_beam;
    mavlink_message_t tx_msg;

    LOG_D("set_beam_point TorR=%d theta=%f phi=%f polarity=%f "
            "freq=%f polarity_type=%d",
            beam_point->TorR, beam_point->theta, beam_point->phi, beam_point->polarity,
            beam_point->freq, beam_point->polarity_type);

#if (ANTENNA_TX_TOTAL <= 0)
    if(beam_point->TorR == ANTENNA_TX) {
        mavlink_msg_set_beam_point_result_pack_chan(0, 0, remote_config->chan, &tx_msg, 0x00);
        mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        return;
    }
#endif

    result = 0x00;
    antenna_beam.freq = beam_point->freq/1000.0f;
    antenna_beam.theta = beam_point->theta;
    antenna_beam.phi = beam_point->phi;
    antenna_beam.polarity = beam_point->polarity;
    if(beam_point->polarity_type == 0x00) {
        antenna_beam.polarity_type = ANTENNA_LINEAR_POLARITY_HORIZONTALL;
    }
    else if(beam_point->polarity_type == 0x01) {
        antenna_beam.polarity_type = ANTENNA_LEFT_HAND_CIRCULAR_POLARITY;
    }
    else {
        antenna_beam.polarity_type = ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY;
    }
    antenna_beam.weight = 0x00;
    antenna_beam.nbr = 0x00;
    if(antenna_update_beam(beam_point->TorR, &antenna_beam) != ERR_OK) {
        result = 0xff;
    }

    mavlink_msg_set_beam_point_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_self_check(remote_config_t *remote_config, uint8_t mode)
{
    uint8_t fault_code;
    uint8_t self_check_result;
    int32_t result;
    device_state_t device_state;
    mavlink_message_t tx_msg;
    char buffer[64];
    char describe[200];

    LOG_D("self_check mode=%d", mode);

    self_check_result = 0x00;
    memset(&describe, 0, sizeof(describe));

    //读取设备状态
    memset(&device_state, 0, sizeof(device_state));
    get_device_state(&device_state);
    //执行自检
    if(mode == 0) {
        //自检ACU
        if(device_state.acu_temperature > ACU_UPPER_LIMIT_TEMP) {
            self_check_result = 0xff;
            strncat(describe, "ACU temperature anomaly.\r\n", sizeof(describe)-strlen(describe));
        }
        if(device_state.acu_electricity > ACU_UPPER_LIMIT_ELECTRICITY) {
            self_check_result = 0xff;
            strncat(describe, "ACU Current anomaly.\r\n", sizeof(describe)-strlen(describe));
        }
        if((device_state.acu_voltage < ACU_LOWER_LIMIT_VOLTAGE) || (device_state.acu_voltage > ACU_UPPER_LIMIT_VOLTAGE)) {
            self_check_result = 0xff;
            strncat(describe, "ACU Abnormal voltage.\r\n", sizeof(describe)-strlen(describe));
        }
    }
    else if(mode == 1) {
        //自检变频器
        result = transducer_check(transducer_device, &fault_code);
        if(result != ERR_OK) {
            self_check_result = 0xff;
            if(fault_code & 0x80) {
                strncat(describe, "transducer communication anomaly.\r\n", sizeof(describe)-strlen(describe));
            }
            if(fault_code & 0x01) {
                strncat(describe, "transducer Current anomaly.\r\n", sizeof(describe)-strlen(describe));
            }
            if(fault_code & 0x02) {
                strncat(describe, "transducer voltage anomaly.\r\n", sizeof(describe)-strlen(describe));
            }
            if(fault_code & 0x04) {
                strncat(describe, "transducer temperature anomaly.\r\n", sizeof(describe)-strlen(describe));
            }
        }
    }
    else if(mode == 2) {
        //自检IMU
//        if(rav260_check(imu_device) != ERR_OK) {
//            self_check_result = 0xff;
//            strncat(describe, "IMU self check failed.\r\n", sizeof(describe)-strlen(describe));
//        }
//        if(adis16465_imu1_selfcheck() != ERR_OK) {
//            self_check_result = 0xff;
//            strncat(describe, "ADIS16465 self check failed.\r\n", sizeof(describe)-strlen(describe));
//        }
        if(noe_m8_self_check(noe_m8) != ERR_OK) {
            self_check_result = 0xff;
            strncat(describe, "noe_m8 self check failed.\r\n", sizeof(describe)-strlen(describe));
        }
    }
    else if(mode == 3) {
        //自检多模机
        if(beacon_check(beacon_device) != ERR_OK) {
            self_check_result = 0xff;
            strncat(describe, "Beacon self check failed.\r\n", sizeof(describe)-strlen(describe));
        }
    }
    else if((mode >= 10) && (mode < (10+ANTENNA_RX_TOTAL))) {
        if(antenna_power_state(ANTENNA_RX, mode-10)) {
            if(device_state.rx_temperature[mode-10] > RX_UPPER_LIMIT_TEMP) {
                self_check_result = 0xff;
                snprintf(buffer, sizeof(buffer), "RX antenna %d temperature anomaly.\r\n", mode-10);
                strncat(describe, buffer, sizeof(describe)-strlen(describe));
            }
            if(device_state.rx_electricity[mode-10] > RX_UPPER_LIMIT_ELECTRICITY) {
                self_check_result = 0xff;
                snprintf(buffer, sizeof(buffer), "RX antenna %d Current anomaly.\r\n", mode-10);
                strncat(describe, buffer, sizeof(describe)-strlen(describe));
            }
            if((device_state.rx_voltage[mode-10] < RX_LOWER_LIMIT_VOLTAGE) || (device_state.rx_voltage[mode-10] > RX_UPPER_LIMIT_VOLTAGE)) {
                self_check_result = 0xff;
                snprintf(buffer, sizeof(buffer), "RX antenna %d voltage anomaly.\r\n", mode-10);
                strncat(describe, buffer, sizeof(describe)-strlen(describe));
            }
            if(antenna_selfcheck(ANTENNA_RX, mode-10) != ERR_OK) {
                self_check_result = 0xff;
                snprintf(buffer, sizeof(buffer), "RX antenna %d communication anomaly.\r\n", mode-10);
                strncat(describe, buffer, sizeof(describe)-strlen(describe));
            }
        }
        else {
            self_check_result = 0xff;
            strncat(describe, "RX antenna VCC OFF.\r\n", sizeof(describe)-strlen(describe));
        }
    }
//    else if((mode >= 20) && (mode < (20+ANTENNA_TX_TOTAL))) {
//        if(antenna_power_state(ANTENNA_TX, mode-20)) {
//            if(device_state.tx_temperature[mode-20] > TX_UPPER_LIMIT_TEMP) {
//                self_check_result = 0xff;
//                snprintf(buffer, sizeof(buffer), "TX antenna %d temperature anomaly.\r\n", mode-20);
//                strncat(describe, buffer, sizeof(describe)-strlen(describe));
//            }
//            if(device_state.tx_electricity[mode-20] > TX_UPPER_LIMIT_ELECTRICITY) {
//                self_check_result = 0xff;
//                snprintf(buffer, sizeof(buffer), "TX antenna %d Current anomaly.\r\n", mode-20);
//                strncat(describe, buffer, sizeof(describe)-strlen(describe));
//            }
//            if((device_state.tx_voltage[mode-20] < TX_LOWER_LIMIT_VOLTAGE) || (device_state.rx_voltage[mode-20] > TX_UPPER_LIMIT_VOLTAGE)) {
//                self_check_result = 0xff;
//                snprintf(buffer, sizeof(buffer), "TX antenna %d voltage anomaly.\r\n", mode-20);
//                strncat(describe, buffer, sizeof(describe)-strlen(describe));
//            }
//            if(antenna_selfcheck(ANTENNA_TX, mode-20) != ERR_OK) {
//                self_check_result = 0xff;
//                snprintf(buffer, sizeof(buffer), "TX antenna %d communication anomaly.\r\n", mode-20);
//                strncat(describe, buffer, sizeof(describe)-strlen(describe));
//            }
//        }
//        else {
//            self_check_result = 0xff;
//            strncat(describe, "TX antenna VCC OFF.\r\n", sizeof(describe)-strlen(describe));
//        }
//    }
    else {
        self_check_result = 0xfe;
        strncpy(describe, "Without this equipment.\r\n", sizeof(describe));
    }

    if(self_check_result == 0x00) {
        strncpy(describe, "Self-checking success.\r\n", sizeof(describe));
    }

    mavlink_msg_self_check_result_pack_chan(0, 0, remote_config->chan, &tx_msg,
            self_check_result, "Self-checking success.\r\n");
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_start_tracking(remote_config_t *remote_config)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("start_tracking");

    result = 0x00;
    if(satmov1_tracking() != ERR_OK) {
        result = 0xff;
    }

    mavlink_msg_start_tracking_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_stop_tracking(remote_config_t *remote_config)
{
    uint8_t result;
    mavlink_message_t tx_msg;

    LOG_D("stop_tracking");

    result = 0x00;
    if(satmov1_suspend() != ERR_OK) {
        result = 0xff;
    }

    mavlink_msg_stop_tracking_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_set_monitor(remote_config_t *remote_config, uint8_t mode, uint8_t state)
{
    mavlink_message_t tx_msg;

    //设置监测任务状态
    device_monitor_set(mode, state);

    mavlink_msg_set_monitor_result_pack_chan(0, 0, remote_config->chan, &tx_msg, 0x00);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_work_parameter(remote_config_t *remote_config, const work_info_t *work_info)
{
    mavlink_message_t tx_msg;

    LOG_D("work_parameter work_mode=%d rolloff=%f manual_gps=%d manual_longitude=%f "
            "manual_latitude=%f manual_height=%f issue_mode=%d",
            work_info->work_mode, work_info->rolloff_factor, work_info->manual_gps, work_info->manual_longitude,
            work_info->manual_latitude, work_info->manual_height,  work_info->issue_mode);

    if(work_info->issue_mode == 0x00) {
#ifdef USING_RAV260
        //配置手动GPS参数
        rav260_manual_gps(imu_device, work_info->manual_gps, work_info->manual_longitude, work_info->manual_latitude, work_info->manual_height);
#else
        noe_m8_manual_gps(noe_m8, work_info->manual_gps, work_info->manual_longitude, work_info->manual_latitude, work_info->manual_height);
#endif

        //返回配置结果
        mavlink_msg_set_work_parameter_result_pack_chan(0, 0, remote_config->chan, &tx_msg, 0x00);
        mavlink_chan_write_pack(remote_config->chan, &tx_msg);
    }
}

static void mavlink_antenna_286b_write_command(uint8_t frame_number, uint8_t code, const uint8_t *command)
{
    static uint8_t frame_code = 0;
    static uint8_t buffer[286] = {0};

    if(!antenna_pass_through) {
        return;
    }

    if(frame_number == 1) {
        frame_code = code;
        memcpy(&buffer[0], command, 143);
    }
    else if((frame_number == 2) && (code == frame_code)) {
        memcpy(&buffer[143], command, 143);
#ifdef USING_ANTENNA_V1
        antenna_v1_port_write(buffer, 286);
#endif
#ifdef USING_ANTENNA_V0
        antenna_v0_port_write(buffer, 286);
#endif
        memset(buffer, 0, sizeof(buffer));
        frame_code = 0;
    }
    else {
        ;
    }
}

static void mavlink_antenna_22b_write_command(const uint8_t *command)
{
    mavlink_message_t tx_msg;

    //开透传AA 55 00 16 A0 01
    //关透传AA 55 00 16 A0 00
    if(command[4] == 0xA0) {
        switch(command[5]) {
        case 0x01:
            //开启透传
            antenna_pass_through = 1;
            break;
        case 0x00:
            //关闭透传
            antenna_pass_through = 0;
            break;
        default:
            return;
        }

        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_passthrough_22b_pack_pack_chan(0, 0, share_chan, &tx_msg, command);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);

        return;
    }

    if(!antenna_pass_through) {
        return;
    }

#ifdef USING_ANTENNA_V1
    antenna_v1_port_write(command, 22);
#endif
#ifdef USING_ANTENNA_V0
    antenna_v0_port_write(command, 22);
#endif
}

static void mavlink_beacon_write_command(const uint8_t *command, uint32_t lenght)
{
    mavlink_message_t tx_msg;

    LOG_D("beacon_write_command %x %x %x", command[1], command[2], command[3]);

    //开透传7B F0 'T' 01 7D
    //关透传7B F0 'T' 00 7D
    if((command[1] == 0xF0) && (command[2] == 'T')) {
        switch(command[3]) {
        case 0x01:
            beacon_pass_through = 1;
            break;
        case 0x00:
            beacon_pass_through = 0;
            break;
        default:
            return;
        }

        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_beacon_pack_pack_chan(0, 0, share_chan, &tx_msg, command, 5);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);

        //复位多模机缓存的配置参数
        beacon_reset(beacon_device, 0x00);

        return;
    }

    if(!beacon_pass_through) {
        return;
    }

    beacon_write_command(beacon_device, command, lenght);
}

static void mavlink_transducer_write_command(const uint8_t *command, uint32_t lenght)
{
    mavlink_message_t tx_msg;

    //开透传 AA 55 0C 00 F0 01
    //关透传AA 55 0C 00 F0 00
    if(command[4] == 0xF0) {
        switch(command[5]) {
        case 0x01:
            //开启透传
            transducer_pass_through = 1;
            break;
        case 0x00:
            //开启透传
            transducer_pass_through = 0;
            break;
        default:
            return;
        }

        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_transducer_pack_pack_chan(0, 0, share_chan, &tx_msg, command, 12);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);

        return;
    }

    if(!transducer_pass_through) {
        return;
    }

    transducer_write_command(transducer_device, command, lenght);
}

static void mavlink_currect_yaw(remote_config_t *remote_config, const  currect_yaw_t *currect_yaw)
{
    uint8_t result;
    mavlink_message_t tx_msg;
    satmov_geo_beam_info_t beam_info;
    satmov_currect_info_t currect_info;

    LOG_D("CURRECT_YAW:sat_longitude=%f,freq=%f,"
            "osc=%f,gain=%d,"
            "polarity_type=%d,sym=%f,"
            "rang=%d,mode=%d,"
            "beamwidth=%f,threshold_cnr=%f,"
            "linger_factor=%f,yaw=%f\r\n",
            currect_yaw->sat_longitude, currect_yaw->freq,
            currect_yaw->osc, currect_yaw->gain,
            currect_yaw->polarity_type, currect_yaw->sym,
            currect_yaw->rang, currect_yaw->mode,
            currect_yaw->beamwidth, currect_yaw->threshold_cnr,
            currect_yaw->linger_factor, currect_yaw->yaw);

    beam_info.sat_longitude = currect_yaw->sat_longitude;
    beam_info.rx_freq = currect_yaw->freq;
    beam_info.rx_osc = currect_yaw->osc;
    beam_info.rx_gain = currect_yaw->gain;
    if(currect_yaw->polarity_type == 0x00) {
        beam_info.rx_polarity_type = ANTENNA_LINEAR_POLARITY_VERTICAL;
    }
    else if(currect_yaw->polarity_type == 0x02) {
        beam_info.rx_polarity_type = ANTENNA_LEFT_HAND_CIRCULAR_POLARITY;
    }
    else if(currect_yaw->polarity_type == 0x03) {
        beam_info.rx_polarity_type = ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY;
    }
    else {
        beam_info.rx_polarity_type = ANTENNA_LINEAR_POLARITY_HORIZONTALL;
    }
    beam_info.linger_factor = currect_yaw->linger_factor;
    beam_info.sym = currect_yaw->sym;
    beam_info.beacon_mode = currect_yaw->mode;
    beam_info.beacon_rang = currect_yaw->rang;

    currect_info.yaw = currect_yaw->yaw;
    currect_info.beamwidth = currect_yaw->beamwidth;
    currect_info.threshold_cnr = currect_yaw->threshold_cnr;

    if((currect_yaw->sym >= 20000) || (currect_yaw->mode != 0x00)) {
        currect_info.scan_count = 7;
    }
    else {
        currect_info.scan_count =(uint32_t)((20 - currect_yaw->sym / 1000.0f) * currect_yaw->linger_factor + 7.0f + 0.5f);
    }
   // currect_info.scan_count = 10;
    result = 0x00;
    if(satmov1_currect_yaw(&beam_info, &currect_info) != ERR_OK) {
        result = 0xff;
    }

    mavlink_msg_currect_yaw_result_pack_chan(0, 0, remote_config->chan, &tx_msg, result);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
}

static void mavlink_issue_earth_point(const issue_earth_point_t *issue_earth_point)
{
    stamov_leo_beam_info_t beam_info;
//
//    LOG_W("stamov_leo_beam_info:azimuth=%f,"
//    		"elevation=%f,polarity=%f\n",issue_earth_point->azimuth,issue_earth_point->elevation,issue_earth_point->polarity);

    beam_info.azimuth = issue_earth_point->azimuth;
    beam_info.elevation = issue_earth_point->elevation;;
    beam_info.polarity = issue_earth_point->polarity;
    beam_info.rx_freq = issue_earth_point->rx_freq;
    beam_info.rx_osc = issue_earth_point->rx_osc;
    beam_info.tx_freq = issue_earth_point->tx_freq;
    beam_info.tx_osc = issue_earth_point->tx_osc;
    beam_info.rx_gain = issue_earth_point->rx_gain;
    if(issue_earth_point->rx_polarity_type == 0x00) {
        beam_info.rx_polarity_type = ANTENNA_LINEAR_POLARITY_VERTICAL;
    }
    else if(issue_earth_point->rx_polarity_type == 0x02) {
        beam_info.rx_polarity_type = ANTENNA_LEFT_HAND_CIRCULAR_POLARITY;
    }
    else if(issue_earth_point->rx_polarity_type == 0x03) {
        beam_info.rx_polarity_type = ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY;
    }
    else {
        beam_info.rx_polarity_type = ANTENNA_LINEAR_POLARITY_HORIZONTALL;
    }
    beam_info.tx_gain = issue_earth_point->tx_gain;
    if(issue_earth_point->tx_polarity_type == 0x01) {
        beam_info.tx_polarity_type = ANTENNA_LINEAR_POLARITY_HORIZONTALL;
    }
    else if(issue_earth_point->tx_polarity_type == 0x02) {
        beam_info.tx_polarity_type = ANTENNA_LEFT_HAND_CIRCULAR_POLARITY;
    }
    else if(issue_earth_point->tx_polarity_type == 0x03) {
        beam_info.tx_polarity_type = ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY;
    }
    else {
        beam_info.tx_polarity_type = ANTENNA_LINEAR_POLARITY_VERTICAL;
    }
    satmov1_set_leo_beam_info(&beam_info);
}


//读GPS数据
int32_t read_gps(satmov_object_t *satmov, satmov_gps_t *gps)
{
	int32_t result;


		noe_m8_gps_t temp;
		noe_m8_time_t m8_time;

		result = noe_m8_read_gps(noe_m8, &temp);


		if(result != ERR_OK) {
			return result;
		}

		result = noe_m8_read_time(noe_m8, &m8_time);
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

		gps->week = m8_time.week;
		gps->second = m8_time.second;


//		rav260_gps_t temp;
//		result = rav260_gps(imu_device, &temp);
//		if(result != ERR_OK) {
//			return result;
//		}
//
//		//数据有效标志转换
//		if(temp.valid_flag) {
//			gps->state = SATMOV_GPS_LOCATION;
//		}
//		else {
//			gps->state = SATMOV_GPS_INVALID;
//		}
//		//高度转换为m
//		gps->height = temp.height;
//		//将经纬转换为弧度
//		gps->latitude = ANGLE_2_RADIAN(temp.latitude);
//		gps->longitude = ANGLE_2_RADIAN(temp.longitude);


	return ERR_OK;
}


//读惯导数据
int32_t read_imu(satmov_object_t *satmov, satmov_imu_t *imu)
{
	int32_t result;
	ms6110_t ms;

		noe_m8_read_ms6110(noe_m8, &ms);

		imu->state = ms.loc_flag;

		imu->pitch = ANGLE_2_RADIAN(ms.pitch);
		imu->roll = ANGLE_2_RADIAN(ms.roll);
		imu->yaw = ANGLE_2_RADIAN(ms.azimuth);

		imu->rate_x = 0;//ANGLE_2_RADIAN(temp.gyro.x);
		imu->rate_y = 0;//ANGLE_2_RADIAN(temp.gyro.y);
		imu->rate_z = 0;//ANGLE_2_RADIAN(temp.gyro.z);

	    return ERR_OK;
}


//命令处理线程
static void command_handle_thread(void *arg)
{
    remote_config_t *remote_config;
    BaseType_t result;
    remote_message_t remote_message;
    mavlink_restart_t restart;
    mavlink_imu_drift_t imu_drift;
    mavlink_read_version_t read_version;
    mavlink_switch_antenna_t switch_antenna;
    mavlink_set_report_t set_report;
    mavlink_set_beacon_t set_beacon;
    mavlink_beam_parameter_t beam_parameter;
    mavlink_set_transducer_t set_transducer;
    mavlink_set_beam_point_t set_beam_point;
    mavlink_self_check_t self_check;
    mavlink_set_monitor_t set_monitor;
    mavlink_work_parameter_t work_parameter;
    mavlink_passthrough_286b_pack_t antenna_286b_pack;
    mavlink_passthrough_22b_pack_t antenna_22b_pack;
    mavlink_beacon_pack_t beacon_pack;
    mavlink_transducer_pack_t transducer_pack;
    mavlink_currect_yaw_t currect_yaw;
    mavlink_issue_earth_point_t issue_earth_point;

    while(1)
    {
        //读取上位机发送的msg
        result = xQueueReceive(command_queue, &remote_message, portMAX_DELAY);
        if(result != pdTRUE) {
            continue;
        }

        remote_config = remote_message.remote_config;
        //对msg进行处理
        switch(remote_message.message.msgid) {
        //通用指令
        case MAVLINK_MSG_ID_CORRECT_IMU:
            //启动零漂校准
            mavlink_correct_imu(remote_config);
            break;
        case MAVLINK_MSG_ID_IMU_DRIFT:
            //零漂请求回复
            mavlink_msg_imu_drift_decode(&remote_message.message, &imu_drift);
            mavlink_imu_drift(&imu_drift);
            break;
        case MAVLINK_MSG_ID_RESTART:
            //复位
            mavlink_msg_restart_decode(&remote_message.message, &restart);
            mavlink_reboot(remote_config, restart.mode);
            break;
        case MAVLINK_MSG_ID_READ_VERSION:
            //读版本
            mavlink_msg_read_version_decode(&remote_message.message, &read_version);
            mavlink_read_version(remote_config, read_version.mode);
            break;
        case MAVLINK_MSG_ID_SWITCH_ANTENNA:
            //开关天线
            mavlink_msg_switch_antenna_decode(&remote_message.message, &switch_antenna);
            mavlink_switch_antenna(remote_config, &switch_antenna);
            break;
        case MAVLINK_MSG_ID_SET_REPORT:
            //设置数据上报周期
            mavlink_msg_set_report_decode(&remote_message.message, &set_report);
            mavlink_set_report(remote_config, set_report.type, set_report.interval);
            break;
        case MAVLINK_MSG_ID_SET_BEACON:
            //设置信标机
            mavlink_msg_set_beacon_decode(&remote_message.message, &set_beacon);
            mavlink_set_beacon(remote_config, &set_beacon);
            break;
        case MAVLINK_MSG_ID_BEAM_PARAMETER:
        	LOG_I("----MAVLINK_MSG_ID_BEAM_PARAMETER");
        	mavlink_msg_beam_parameter_decode(&remote_message.message, &beam_parameter);
        	mavlink_beam_parameter(remote_config, &beam_parameter);
        	break;
        case MAVLINK_MSG_ID_SET_TRANSDUCER:
            //设置变频器
            mavlink_msg_set_transducer_decode(&remote_message.message, &set_transducer);
            mavlink_set_transducer(remote_config, &set_transducer);
            break;
        case MAVLINK_MSG_ID_SET_BEAM_POINT:
            //设置波束指向
            mavlink_msg_set_beam_point_decode(&remote_message.message, &set_beam_point);
            mavlink_set_beam_point(remote_config, &set_beam_point);
            break;
        case MAVLINK_MSG_ID_SELF_CHECK:
            //执行自检
            mavlink_msg_self_check_decode(&remote_message.message, &self_check);
            mavlink_self_check(remote_config, self_check.mode);
            break;
        case MAVLINK_MSG_ID_START_TRACKING:
            //启动跟踪
            mavlink_start_tracking(remote_config);
            break;
        case MAVLINK_MSG_ID_STOP_TRACKING:
            //停止跟踪
            mavlink_stop_tracking(remote_config);
            break;
        case MAVLINK_MSG_ID_SET_MONITOR:
            //监测控制
            mavlink_msg_set_monitor_decode(&remote_message.message, &set_monitor);
            mavlink_set_monitor(remote_config, set_monitor.mode, set_monitor.state);
            break;
        case MAVLINK_MSG_ID_WORK_PARAMETER:
            //设置工作模式
            mavlink_msg_work_parameter_decode(&remote_message.message, &work_parameter);
            mavlink_work_parameter(remote_config, &work_parameter);
            break;
            //透传指令
        case MAVLINK_MSG_ID_PASSTHROUGH_286B_PACK:
            //286B命令
            mavlink_msg_passthrough_286b_pack_decode(&remote_message.message, &antenna_286b_pack);
            mavlink_antenna_286b_write_command(antenna_286b_pack.frameCnt, antenna_286b_pack.FCODE, antenna_286b_pack.cmd);
            break;
        case MAVLINK_MSG_ID_PASSTHROUGH_22B_PACK:
            //22B命令
            mavlink_msg_passthrough_22b_pack_decode(&remote_message.message, &antenna_22b_pack);
            mavlink_antenna_22b_write_command(antenna_22b_pack.cmd);
            break;
        case MAVLINK_MSG_ID_BEACON_PACK:
            //多模机透传指令
            mavlink_msg_beacon_pack_decode(&remote_message.message, &beacon_pack);
            mavlink_beacon_write_command(beacon_pack.cmd, beacon_pack.size);
            break;
        case MAVLINK_MSG_ID_TRANSDUCER_PACK:
            //变频器透传命令
            mavlink_msg_transducer_pack_decode(&remote_message.message, &transducer_pack);
            mavlink_transducer_write_command(transducer_pack.cmd, transducer_pack.size);
            break;
            //低轨命令
        case MAVLINK_MSG_ID_CURRECT_YAW:
            mavlink_msg_currect_yaw_decode(&remote_message.message, &currect_yaw);
            mavlink_currect_yaw(remote_config, &currect_yaw);
            break;
        case MAVLINK_MSG_ID_ISSUE_EARTH_POINT:
            mavlink_msg_issue_earth_point_decode(&remote_message.message, &issue_earth_point);
            mavlink_issue_earth_point(&issue_earth_point);
            break;
        default:
            break;
        }
    }
}

static uint32_t imu_state_coalesce(noe_m8_gps_t *gps, uint32_t adis16465_state)
{
    uint32_t state;

    state = 0;
    if(gps->valid_flag) {
        state |= 0x9e;
    }
    else {
        state |= 0x7c;
    }

//    if(adis16465_state & ADIS16465_GYRO_CALIBRATION_STATE) {
//        state |= 0x20;
//    }
//    else {
//        if((adis16465_state & MAG_FAST_GAIN_STATE) && (adis16465_state & ACC_FAST_GAIN_STATE)) {
//            if(gps->valid_flag) {
//                state |= 0x90;
//            }
//            else {
//                state |= 0x70;
//            }
//        }
//        else {
//            state |= 0x10;
//        }
//    }

    return state;
}

void MUILT_beam_switch_gpio1(u8 channel)
{
	ps_gpio_config(0, PS_GPIO_MODE_OUTPUT);
	ps_gpio_write(0, channel);  			//翻转
}

//读取通道数据并解包
static void report_thread(void *arg)
{
    uint8_t polarity_type;
    uint32_t i;
    uint32_t report_tick;
//    float imu_temperature;
    float adis16465_temp;
    remote_config_t *remote_config;
    beacon_data_t beacon_data;
//    rav260_ins_t ins;
    adis16465_attitude_t adis16465_attitude;
    uint32_t adis16465_state;
//    rav260_gps_t gps;
    noe_m8_gps_t gps;
//    rav260_time_t time;
    noe_m8_time_t time;
//    rav260_state_t state;
    antenna_state_t antenna_state;
    transducer_state_t transducer_state;
    device_state_t device_state;
    machine_state_t machine_state;
    mavlink_message_t tx_msg;

	satmov_imu_t sat_imu;
	satmov_gps_t sat_gps;

    report_tick = 0;
    remote_config = (remote_config_t*)arg;
    //上报固件就绪命令，天线名称、天线序列号由Linux端填充
    mavlink_msg_firmware_reday_pack_chan(0, 0, remote_config->chan, &tx_msg,
            "KU2T2R", "12345678", __DATE__, "v2.0", 0x07);
    mavlink_chan_write_pack(remote_config->chan, &tx_msg);
    float issue_point_folag=0;
    while(!remote_config->exit_flag)
    {

    	if((remote_config->interval.beacon_interval > 0) && (report_tick % remote_config->interval.beacon_interval == 0)) {
    		if((issue_point_folag==60))
    		{
    			issue_point_folag=0;
    		}
    		else
    		{
    			issue_point_folag++;
    		}
    	 }
     	//if((remote_config->interval.beacon_interval > 0) && (report_tick % remote_config->interval.beacon_interval == 0)) {
//        		if(issue_point_folag==1)
//        		{
//        			MUILT_beam_switch_gpio1(1);
//        			issue_point_folag=0;
//        		}
//        		else
//        		{
//        			MUILT_beam_switch_gpio1(0);
//        			issue_point_folag=1;
//        		}
//        	        }
        if((remote_config->interval.beacon_interval > 0) && (report_tick % remote_config->interval.beacon_interval == 0)) {
            //读取多模机数据
           beacon_read_data(beacon_device, &beacon_data);
            mavlink_msg_report_beacon_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
                               beacon_data.cnr_power, beacon_data.freq/1000.0f,
                               beacon_data.sym, beacon_data.rang,
                               beacon_data.mode);
//            mavlink_msg_report_beacon_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
//            					issue_point_folag, beacon_data.freq/1000.0f,
//                                        beacon_data.sym, beacon_data.rang,
//                                        beacon_data.mode);

            mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        }
        if((remote_config->interval.imu_interval > 0) && (report_tick % remote_config->interval.imu_interval == 0)) {
            //读取IMU数据
        	read_imu(NULL, &sat_imu);
        	read_gps(NULL, &sat_gps);
//            rav260_ins(imu_device, &ins);
//            adis16465_imu1_get_attitude(&adis16465_attitude);
////            rav260_state(imu_device, &state);
////            adis16465_imu1_get_state(&adis16465_state);
////            rav260_time(imu_device, &time);
//            noe_m8_read_time(noe_m8, &time);
//            rav260_temperature(imu_device, &imu_temperature);
//            adis16465_imu1_get_temp(&adis16465_temp);
            adis16465_temp = 0.0f;
            adis16465_state = 0;
//            sat_imu.state=2;
            mavlink_msg_report_imu_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
            					sat_imu.state, RADIAN_2_ANGLE(sat_imu.pitch),
            					RADIAN_2_ANGLE(sat_imu.roll), RADIAN_2_ANGLE(sat_imu.yaw),
            					sat_imu.rate_x, sat_imu.rate_y,
            					sat_imu.rate_z, RADIAN_2_ANGLE(sat_gps.longitude),
            					RADIAN_2_ANGLE(sat_gps.latitude), sat_gps.height,
            					0, (int16_t)(adis16465_temp*100),
            					sat_gps.week,sat_gps.second);

            mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        }
        if((remote_config->interval.antenna_interval > 0) && (report_tick % remote_config->interval.antenna_interval == 0)) {
            memset(&antenna_state, 0, sizeof(antenna_state));
            //读取天线数据
            antenna_get_point(ANTENNA_RX, &antenna_state.rx_polarity, &antenna_state.rx_phi, &antenna_state.rx_theta);
            antenna_get_polarity(ANTENNA_RX, &polarity_type);
            switch(polarity_type) {
            case ANTENNA_LINEAR_POLARITY_VERTICAL:
            case ANTENNA_LINEAR_POLARITY_VERTICAL_OLAY:
                antenna_state.rx_polarity_type = 0x00;
                break;
            case ANTENNA_LINEAR_POLARITY_HORIZONTALL:
            case ANTENNA_LINEAR_POLARITY_HORIZONTAL_OLAY:
                antenna_state.rx_polarity_type = 0x01;
                break;
            case ANTENNA_LEFT_HAND_CIRCULAR_POLARITY:
                antenna_state.rx_polarity_type = 0x02;
                break;
            case ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY:
                antenna_state.rx_polarity_type = 0x03;
                break;
            default:
                antenna_state.rx_polarity_type = 0x00;
            }
            antenna_get_freq(ANTENNA_RX, &antenna_state.rx_freq);
            antenna_state.rx_freq *= 1000.0f;
//            antenna_get_point(ANTENNA_TX, &antenna_state.tx_polarity, &antenna_state.tx_phi, &antenna_state.tx_theta);
//            antenna_get_polarity(ANTENNA_TX, &polarity_type);
//            switch(polarity_type) {
//            case ANTENNA_LINEAR_POLARITY_VERTICAL:
//            case ANTENNA_LINEAR_POLARITY_VERTICAL_OLAY:
//                antenna_state.tx_polarity_type = 0x00;
//                break;
//            case ANTENNA_LINEAR_POLARITY_HORIZONTALL:
//            case ANTENNA_LINEAR_POLARITY_HORIZONTAL_OLAY:
//                antenna_state.tx_polarity_type = 0x01;
//                break;
//            case ANTENNA_LEFT_HAND_CIRCULAR_POLARITY:
//                antenna_state.tx_polarity_type = 0x02;
//                break;
//            case ANTENNA_RIGHT_HAND_CIRCULAR_POLARITY:
//                antenna_state.tx_polarity_type = 0x03;
//                break;
//            default:
//                antenna_state.tx_polarity_type = 0x00;
//            }
//            antenna_get_freq(ANTENNA_TX, &antenna_state.tx_freq);
//            antenna_state.tx_freq *=1000.0f;
            antenna_state.tx_phi = 0.0f;
            antenna_state.tx_theta = 0.0f;
            antenna_state.tx_polarity = 0.0f;
            antenna_state.tx_freq = 0.0f;
            antenna_state.tx_polarity_type  = 0.0f;
            mavlink_msg_report_antenna_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
                    antenna_state.rx_phi, antenna_state.rx_theta,
                    antenna_state.rx_polarity, antenna_state.rx_freq,
                    antenna_state.rx_polarity_type, antenna_state.tx_phi,
                    antenna_state.tx_theta, antenna_state.tx_polarity,
                    antenna_state.tx_freq, antenna_state.tx_polarity_type,
                    antenna_state.earth_point.azimuth, antenna_state.earth_point.elevation,
                    antenna_state.earth_point.polarity);
            mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        }
        if((remote_config->interval.transducer_interval > 0) && (report_tick % remote_config->interval.transducer_interval == 0)) {
            memset(&transducer_state, 0, sizeof(transducer_state));
            //读取变频器数据
            transducer_get_lo(transducer_device, &transducer_state.rx_osc);
            transducer_state.rx_gain = 0;
            transducer_state.tx_osc = 12.8f;
            transducer_state.tx_gain = 0;
            transducer_get_if_value(transducer_device, &transducer_state.if_Value);
            transducer_get_rf_value(transducer_device, &transducer_state.rf_Value);
            transducer_get_lock_state(transducer_device, &transducer_state.lock_state);
            get_device_state(&device_state);
            transducer_state.temperature = device_state.tr_temperature;
            transducer_state.electricity = device_state.tr_electricity;
            transducer_state.vlotage = device_state.tr_voltage;
            mavlink_msg_report_transducer_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
                    transducer_state.rx_osc*1000.0f, transducer_state.rx_gain,
                    transducer_state.tx_osc*1000.0f, transducer_state.tx_gain,
                    transducer_state.if_Value, transducer_state.rf_Value,
                    transducer_state.lock_state, (int16_t)(transducer_state.temperature*100),
                    (int16_t)(transducer_state.vlotage*100.0f+0.5f), (int16_t)(transducer_state.electricity*100.0f+0.5f));
            mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        }
        if((remote_config->interval.work_interval > 0) && (report_tick % remote_config->interval.work_interval == 0)) {
            memset(&machine_state, 0, sizeof(machine_state));
            //读取整机状态
            get_device_state(&device_state);
            machine_state.fault_code = device_state.global_fault_code;
            machine_state.tracking = satmov1_get_state();
            machine_state.rx_rf_state = antenna_rf_vcc_state(ANTENNA_RX);
//            machine_state.tx_rf_state = antenna_rf_vcc_state(ANTENNA_TX);
            machine_state.tx_rf_state = 0;
            machine_state.acu_temperature = (int16_t)(device_state.acu_temperature * 100.0f + 0.5f);
            machine_state.acu_voltage = (int16_t)(device_state.acu_voltage * 100.0f + 0.5f);
            machine_state.acu_electricity = (int16_t)(device_state.acu_electricity * 100.0f + 0.5f);
            for(i=0; (i<ANTENNA_RX_TOTAL)||(i<ANTENNA_TX_TOTAL); i++) {
                if(i < ANTENNA_TX_TOTAL) {
                    machine_state.tx_temperature[i] = (int16_t)(device_state.tx_temperature[i] * 100.0f + 0.5f);
                    machine_state.tx_voltage[i] = (int16_t)(device_state.tx_voltage[i] * 100.0f + 0.5f);
                    machine_state.tx_electricity[i] = (int16_t)(device_state.tx_electricity[i] * 100.0f + 0.5f);
                }
                if(i < ANTENNA_RX_TOTAL) {
                    machine_state.rx_temperature[i] = (int16_t)(device_state.rx_temperature[i] * 100.0f + 0.5f);
                    machine_state.rx_voltage[i] = (int16_t)(device_state.rx_voltage[i] * 100.0f + 0.5f);
                    machine_state.rx_electricity[i] = (int16_t)(device_state.rx_electricity[i] * 100.0f + 0.5f);
                }
            }
            mavlink_msg_report_machine_state_pack_chan(0, 0, remote_config->chan, &tx_msg,
                    machine_state.fault_code, machine_state.tracking,
                    machine_state.rx_rf_state, machine_state.tx_rf_state,
                    machine_state.acu_temperature, machine_state.acu_voltage,
                    machine_state.acu_electricity, machine_state.tx_temperature,
                    machine_state.tx_voltage, machine_state.tx_electricity,
                    machine_state.rx_temperature, machine_state.rx_voltage,
                    machine_state.rx_electricity);
            mavlink_chan_write_pack(remote_config->chan, &tx_msg);
        }

        vTaskDelay(1);

        report_tick++;
    }

    //释放join信号量
    xSemaphoreGive(remote_config->join);
    //结束线程
    vTaskDelete(NULL);
}

//读取设备数据并上报给通道
static void parse_thread(void *arg)
{
    uint8_t result;
    int32_t i;
    ssize_t lenght;
    remote_config_t *remote_config;
    mavlink_status_t state;
    remote_message_t remote_message;
    portBASE_TYPE xresult;
    TaskHandle_t task;
    uint8_t buffer[128];

    //获取配置参数地址
    remote_config = (remote_config_t*)arg;
    //创建主动上报线程
    xresult = xTaskCreate(report_thread,
                         "report_thread",
                         2048,
                         (void*)remote_config,
                         tskIDLE_PRIORITY + 2,
                         &task );
    if(xresult != pdPASS) {
        mavlink_chan_free(remote_config->chan);
        vSemaphoreDelete(remote_config->join);
        vPortFree(remote_config);
        LOG_E("create task thread failed");
        vTaskDelete(NULL);
    }
    //复位msg和状态
    memset(&state, 0, sizeof(state));
    memset(&remote_message.message, 0, sizeof(remote_message.message));
    while(1)
    {
        //读数据
        lenght = mavlink_chan_read(remote_config->chan, buffer, sizeof(buffer), portMAX_DELAY);
        if(lenght == ERR_DISCONNECT) {
            break;
        }
        if(lenght <= 0) {
            continue;
        }

        for(i=0; i<lenght; i++) {
            //解析一个字节
            result = mavlink_parse_char(remote_config->chan, buffer[i], &remote_message.message, &state);

            //正在接收命令
            if(result == MAVLINK_FRAMING_INCOMPLETE) {
                continue;
            }

            //命令错误
            if(result == MAVLINK_FRAMING_BAD_CRC || result == MAVLINK_FRAMING_BAD_SIGNATURE) {
                LOG_E("Command format error");
            }

            //接收完一帧命令
            if(result == MAVLINK_FRAMING_OK) {
                //发送msg到命令处理线程
                remote_message.remote_config = remote_config;
                xQueueSend(command_queue, &remote_message, 0);
            }

            //复位msg和状态
            memset(&state, 0, sizeof(state));
            memset(&remote_message.message, 0, sizeof(remote_message.message));
        }
    }
    //停止数据上报线程
    remote_config->exit_flag = 1;
    //等待数据上报线程结束
    xSemaphoreTake(remote_config->join, portMAX_DELAY);
    //释放通道
    mavlink_chan_free(remote_config->chan);
    //删除信号量
    vSemaphoreDelete(remote_config->join);
    //释放配置数据内存
    vPortFree(remote_config);
    //结束线程
    vTaskDelete(NULL);
}

//将需要广播的消息队列发送出去
static void amp_broadcast_thread(void *arg)
{
    BaseType_t result;
    mavlink_message_t tx_msg;

    while(1)
    {
        //读取需要广播的msg
        result = xQueueReceive(amp_broadcast_queue, &tx_msg, portMAX_DELAY);
        if(result != pdTRUE) {
            continue;
        }

        //将数据发送给AMP通道
        mavlink_type_chan_write_pack(&tx_msg, AMP_TYPE);

        //将数据包转发到串口广播线程
        if(serial_broadcast_queue) {
            xQueueSend(serial_broadcast_queue, &tx_msg, 0);
        }
    }
}

static void serial_broadcast_thread(void *arg)
{
    BaseType_t result;
    mavlink_message_t tx_msg;

    while(1)
    {
        //读取需要广播的msg
        result = xQueueReceive(serial_broadcast_queue, &tx_msg, portMAX_DELAY);
        if(result != pdTRUE) {
            continue;
        }

        //将数据发送给串口通道
        mavlink_type_chan_write_pack(&tx_msg, SERIAL_TYPE);
    }
}

int32_t mavlink_read_imu_drift(uint8_t *valid, float *gyro_x, float *gyro_y, float *gyro_z)
{
    uint32_t i;
    int32_t result;
    mavlink_message_t tx_msg;
    uint8_t chan_type[] = REQUEST_CHANNEL_LIST;

    if((!valid) || (!gyro_x)|| (!gyro_y)|| (!gyro_z)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    result = ERR_IO;

    //通过公共通道打包命令为msg
    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
    mavlink_msg_request_imu_drift_pack_chan(0, 0, share_chan, &tx_msg, 0x01);
    xSemaphoreGiveRecursive(share_chan_mutex);

    //获取互斥锁
    xSemaphoreTakeRecursive(imu_drift_command.lock, portMAX_DELAY);

    for(i=0; i<sizeof(chan_type)/sizeof(chan_type[0]); i++) {
        //复位信号量
        xSemaphoreTake(imu_drift_command.sync, 0);
        //将数据发送给串口通道
        mavlink_type_chan_write_pack(&tx_msg, chan_type[i]);
        //等待回复
        if(xSemaphoreTake(imu_drift_command.sync, 500) == pdTRUE) {
            *valid = imu_drift_command.valid;
            *gyro_x = imu_drift_command.gyro_x;
            *gyro_y = imu_drift_command.gyro_y;
            *gyro_z = imu_drift_command.gyro_z;
            result = ERR_OK;
            break;
        }
    }

    //释放互斥量
    xSemaphoreGiveRecursive(imu_drift_command.lock);

    return result;
}

int32_t mavlink_write_imu_drift(uint8_t valid, float gyro_x, float gyro_y, float gyro_z)
{
    mavlink_message_t msg;

    //打包命令为msg
    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
    mavlink_msg_imu_drift_pack_chan(0, 0, share_chan, &msg, valid, gyro_x, gyro_y, gyro_z);
    xSemaphoreGiveRecursive(share_chan_mutex);

    //将数据发送给所有通道
    mavlink_all_chan_write_pack(&msg);

    return ERR_OK;
}

void mavlink_print(const char *str)
{
    static mavlink_message_t tx_msg;
    static mavlink_log_string_t log_string;
    uint16_t lenght;
    uint16_t count;

    //检查参数是否合法
    if(!str) {
        return;
    }

    //中断状态不执行此函数
    if(portICCRPR_RUNNING_PRIORITY_REGISTER < 255) {
        return;
    }

    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);

    lenght = strlen(str);
    //通过mavlink广播
    for(count=0; count<lenght; ) {
        //重新格式化字符串
        snprintf(log_string.str, sizeof(log_string.str), "%s", &str[count]);
        log_string.size = strlen(log_string.str);
        //通过公共通道打包命令为msg
        mavlink_msg_log_string_pack_chan(0, 0, share_chan, &tx_msg, log_string.str, log_string.size);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);
        //累计已经发送的数据
        count += log_string.size;
    }

    xSemaphoreGiveRecursive(share_chan_mutex);
}

void mavlink_vprint(const char *fmt, va_list args)
{
    static mavlink_message_t tx_msg;
    static mavlink_log_string_t log_string;
    static char log_buffer[LOG_BUFFER_LENGHT];
    uint16_t lenght;
    uint16_t count;

    //检查参数是否合法
    if(!fmt) {
        return;
    }

    //中断状态不执行此函数
    if(portICCRPR_RUNNING_PRIORITY_REGISTER < 255) {
        return;
    }

    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);

    //格式化字符串
    vsnprintf(log_buffer, sizeof(log_buffer), fmt, args);
    lenght = strlen(log_buffer);
    //通过mavlink广播
    for(count=0; count<lenght; ) {
        //重新格式化字符串
        snprintf(log_string.str, sizeof(log_string.str), "%s", &log_buffer[count]);
        log_string.size = strlen(log_string.str);
        //通过公共通道打包命令为msg
        mavlink_msg_log_string_pack_chan(0, 0, share_chan, &tx_msg, log_string.str, log_string.size);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);
        //累计已经发送的数据
        count += log_string.size;
    }

    xSemaphoreGiveRecursive(share_chan_mutex);
}

void mavlink_beacon_broadcast(const uint8_t *buffer, size_t size)
{
    mavlink_message_t tx_msg;

    if((!beacon_pass_through) || (!buffer) || (size == 0)){
        return;
    }

    //一次最多只能发送MAVLINK_MSG_BEACON_PACK_FIELD_CMD_LEN字节
    if(size > MAVLINK_MSG_BEACON_PACK_FIELD_CMD_LEN) {
        size = MAVLINK_MSG_BEACON_PACK_FIELD_CMD_LEN;
    }

    //打包命令为msg
    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
    mavlink_msg_beacon_pack_pack_chan(0, 0, share_chan, &tx_msg, buffer, size);
    xSemaphoreGiveRecursive(share_chan_mutex);

    //将命令放入广播消息队列
    xQueueSend(amp_broadcast_queue, &tx_msg, 0);
}

void mavlink_transducer_broadcast(const uint8_t *buffer, size_t size)
{
    mavlink_message_t tx_msg;

    if((!transducer_pass_through) || (!buffer) || (size == 0)) {
        return;
    }

    //一次最多只能发送MAVLINK_MSG_TRANSDUCER_PACK_FIELD_CMD_LEN字节
    if(size > MAVLINK_MSG_TRANSDUCER_PACK_FIELD_CMD_LEN) {
        size = MAVLINK_MSG_TRANSDUCER_PACK_FIELD_CMD_LEN;
    }

    //打包命令为msg
    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
    mavlink_msg_transducer_pack_pack_chan(0, 0, share_chan, &tx_msg, buffer, size);
    xSemaphoreGiveRecursive(share_chan_mutex);

    //将命令放入广播消息队列
    xQueueSend(amp_broadcast_queue, &tx_msg, 0);
}

void mavlink_antenna_broadcast(const uint8_t *buffer, size_t size)
{
    mavlink_message_t tx_msg;

    if((!antenna_pass_through) || (!buffer) || (size == 0)) {
        return;
    }

    if(size == MAVLINK_MSG_PASSTHROUGH_22B_PACK_FIELD_CMD_LEN) {
        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_passthrough_22b_pack_pack_chan(0, 0, share_chan, &tx_msg, buffer);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);
    }
    else if(size == MAVLINK_MSG_PASSTHROUGH_286B_PACK_FIELD_CMD_LEN) {
        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_passthrough_286b_pack_pack_chan(0, 0, share_chan, &tx_msg, 1, buffer[4], &buffer[0]);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);

        //打包命令为msg
        xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
        mavlink_msg_passthrough_286b_pack_pack_chan(0, 0, share_chan, &tx_msg, 2, buffer[4], &buffer[143]);
        xSemaphoreGiveRecursive(share_chan_mutex);
        //将命令放入广播消息队列
        xQueueSend(amp_broadcast_queue, &tx_msg, 0);
    }
    else {
        return;
    }
}

void mavlink_currect_yaw_complete(float yaw)
{
    mavlink_message_t tx_msg;

    //打包命令为msg
    xSemaphoreTakeRecursive(share_chan_mutex, portMAX_DELAY);
    mavlink_msg_currect_yaw_complete_pack_chan(0, 0, share_chan, &tx_msg, yaw);
    xSemaphoreGiveRecursive(share_chan_mutex);

    //将命令放入广播消息队列
    xQueueSend(amp_broadcast_queue, &tx_msg, 0);
}

int32_t mavlink_remote_create(mavlink_port_t *chan_ops, uint8_t chan_type, mavlink_interval_t *interval)
{
    int32_t result;
    remote_config_t *remote_config;
    portBASE_TYPE xresult;
    TaskHandle_t task;
    mavlink_channel_t chan;


    if((!chan_ops) || (!interval)){
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //分配通道
    result = mavlink_chan_alloc(&chan, chan_ops, chan_type);
    if(result != ERR_OK) {
        LOG_E("alloc mavlink chan failed");
        return result;
    }

    //初始化配置参数
    remote_config = (remote_config_t*)pvPortMalloc(sizeof(remote_config_t));
    if(!remote_config) {
        mavlink_chan_free(chan);
        LOG_E("malloc failed");
        return ERR_NOMEM;
    }
    remote_config->chan = chan;
    remote_config->interval = *interval;
    remote_config->exit_flag = 0;
    remote_config->join = xSemaphoreCreateBinary();
    if(!remote_config->join) {
        mavlink_chan_free(chan);
        vPortFree(remote_config);
        LOG_E("create semaphore failed");
        return ERR_NOMEM;
    }

    //数据解析线程
    xresult = xTaskCreate(parse_thread,
                         "parse_thread",
                         2048,
                         (void*)remote_config,
                         configMAX_PRIORITIES - 4,
                         &task );
    if(xresult != pdPASS) {
        mavlink_chan_free(chan);
        vSemaphoreDelete(remote_config->join);
        vPortFree(remote_config);
        LOG_E("create task thread failed");
        return ERR_NOMEM;
    }

    return ERR_OK;
}

static mavlink_port_t broadcast_ops = {NULL};
int32_t mavlink_remote_init(void)
{
    int32_t result;
    uint32_t i;
    portBASE_TYPE xresult;
    TaskHandle_t task;

    //打开信标机
    beacon_device = device_open("beacon0", DEVICE_RDWR);
    if(!beacon_device) {
        LOG_E("open beacon failed");
        return ERR_IO;
    }
    //打开惯导设备
//    imu_device = device_open("IMU0", DEVICE_RDWR);
//    if(!imu_device) {
//        LOG_E("open IMU device failed");
//        return ERR_IO;
//    }
    noe_m8 = device_open("noe_m8_0", DEVICE_RDWR);
    if(!noe_m8) {
        LOG_E("open noe_m8 device failed");
        return ERR_IO;
    }
    //打开变频器设备
    transducer_device = device_open("transducer0", DEVICE_RDWR);
    if(!transducer_device) {
        LOG_E("open transducer failed");
        return ERR_IO;
    }

    imu_drift_command.sync = xSemaphoreCreateBinary();
    if(!imu_drift_command.sync) {
        LOG_E("create semaphore failed");
        return ERR_NOMEM;
    }
    imu_drift_command.lock = xSemaphoreCreateRecursiveMutex();
    if(!imu_drift_command.lock) {
        LOG_E("create mutex failed");
        return ERR_NOMEM;
    }

    //分配广播数据打包通道
    result = mavlink_chan_alloc(&share_chan, &broadcast_ops, VIRTUAL_TYPE);
    if(result != ERR_OK) {
        LOG_E("alloc broadcast chan failed");
        return ERR_NOMEM;
    }
    //创建保护广播通道的互斥量
    share_chan_mutex = xSemaphoreCreateRecursiveMutex();
    if(!share_chan_mutex) {
        LOG_E("create mutex failed");
        return ERR_NOMEM;
    }

    //创建广播消息队列
    amp_broadcast_queue = xQueueCreate(64, sizeof(mavlink_message_t));
    if(!amp_broadcast_queue) {
        LOG_E("create amp broadcast queue failed");
        return ERR_NOMEM;
    }
    serial_broadcast_queue = xQueueCreate(64, sizeof(mavlink_message_t));
    if(!serial_broadcast_queue) {
        LOG_E("create serial broadcast queue failed");
        return ERR_NOMEM;
    }

    //创建广播命令MSG的消息队列
    command_queue = xQueueCreate(ERUPT_THREAD_NUMBER*2, sizeof(remote_message_t));
    if(!command_queue) {
        LOG_E("create command queue failed");
        return ERR_NOMEM;
    }

    //创建广播线程
    xresult = xTaskCreate(amp_broadcast_thread,
                         "amp_broadcast_thread",
                         2048,
                         NULL,
                         tskIDLE_PRIORITY + 2,
                         &task );
    if(xresult != pdPASS) {
        LOG_E("create task thread failed");
        return ERR_NOMEM;
    }
    xresult = xTaskCreate(serial_broadcast_thread,
                         "serial_broadcast_thread",
                         2048,
                         NULL,
                         tskIDLE_PRIORITY + 2,
                         &task );
    if(xresult != pdPASS) {
        LOG_E("create task thread failed");
        return ERR_NOMEM;
    }

    //创建命令处理线程
    for(i=0; i<ERUPT_THREAD_NUMBER; i++) {
        xresult = xTaskCreate(command_handle_thread,
                             "command_handle_thread",
                             3072,
                             NULL,
                             configMAX_PRIORITIES - 4,
                             &task );
        if(xresult != pdPASS) {
            LOG_E("create task thread failed");
            return ERR_NOMEM;
        }
    }

    return ERR_OK;
}
