#pragma once
// MESSAGE TRACKING_PARAMETER PACKING

#define MAVLINK_MSG_ID_TRACKING_PARAMETER 44

MAVPACKED(
typedef struct __mavlink_tracking_parameter_t {
 float beamwidth; /*<  天线波束宽度(°)*/
 float dvb_coarse_threshold; /*<  DVB粗对准阈值*/
 float dvb_refined_threshold; /*<  DVB精对准阈值*/
 float dvb_tracking_threshold; /*<  DVB动态跟踪阈值*/
 float detection_coarse_threshold; /*<  检波粗对准阈值*/
 float detection_refined_threshold; /*<  检波精对准阈值*/
 float detection_tracking_threshold; /*<  检波动态跟踪阈值*/
 float beacon_coarse_threshold; /*<  信标粗对准阈值*/
 float beacon_refined_threshold; /*<  信标精对准阈值*/
 float beacon_tracking_threshold; /*<  信标动态跟踪阈值*/
 float coarse_linger_factor; /*<  粗对准波束驻留时间系数，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7*/
 float refined_linger_factor; /*<  精对准束驻留时间系数*/
 float tracking_linger_factor; /*<  动态跟踪波束驻留时间系数*/
 float kp_az; /*<  动态跟踪方位角比例系数*/
 float kp_el; /*<  动态跟踪俯仰角比例系数*/
 float kp_pol; /*<  动态跟踪极化角比例系数*/
 float delta_az; /*<  动态跟踪方位角偏离轴心角度*/
 float delta_el; /*<  动态跟踪俯仰角偏离轴心角度*/
 float delta_pol; /*<  动态跟踪极化角偏离轴心角度*/
 uint8_t issue_mode; /*<  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复*/
}) mavlink_tracking_parameter_t;

#define MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN 77
#define MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN 77
#define MAVLINK_MSG_ID_44_LEN 77
#define MAVLINK_MSG_ID_44_MIN_LEN 77

#define MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC 242
#define MAVLINK_MSG_ID_44_CRC 242



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TRACKING_PARAMETER { \
    44, \
    "TRACKING_PARAMETER", \
    20, \
    {  { "beamwidth", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_tracking_parameter_t, beamwidth) }, \
         { "dvb_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_tracking_parameter_t, dvb_coarse_threshold) }, \
         { "dvb_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_tracking_parameter_t, dvb_refined_threshold) }, \
         { "dvb_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_tracking_parameter_t, dvb_tracking_threshold) }, \
         { "detection_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_tracking_parameter_t, detection_coarse_threshold) }, \
         { "detection_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_tracking_parameter_t, detection_refined_threshold) }, \
         { "detection_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_tracking_parameter_t, detection_tracking_threshold) }, \
         { "beacon_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_tracking_parameter_t, beacon_coarse_threshold) }, \
         { "beacon_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_tracking_parameter_t, beacon_refined_threshold) }, \
         { "beacon_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_tracking_parameter_t, beacon_tracking_threshold) }, \
         { "coarse_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_tracking_parameter_t, coarse_linger_factor) }, \
         { "refined_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_tracking_parameter_t, refined_linger_factor) }, \
         { "tracking_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_tracking_parameter_t, tracking_linger_factor) }, \
         { "kp_az", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_tracking_parameter_t, kp_az) }, \
         { "kp_el", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_tracking_parameter_t, kp_el) }, \
         { "kp_pol", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_tracking_parameter_t, kp_pol) }, \
         { "delta_az", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_tracking_parameter_t, delta_az) }, \
         { "delta_el", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_tracking_parameter_t, delta_el) }, \
         { "delta_pol", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_tracking_parameter_t, delta_pol) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 76, offsetof(mavlink_tracking_parameter_t, issue_mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TRACKING_PARAMETER { \
    "TRACKING_PARAMETER", \
    20, \
    {  { "beamwidth", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_tracking_parameter_t, beamwidth) }, \
         { "dvb_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_tracking_parameter_t, dvb_coarse_threshold) }, \
         { "dvb_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_tracking_parameter_t, dvb_refined_threshold) }, \
         { "dvb_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_tracking_parameter_t, dvb_tracking_threshold) }, \
         { "detection_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_tracking_parameter_t, detection_coarse_threshold) }, \
         { "detection_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_tracking_parameter_t, detection_refined_threshold) }, \
         { "detection_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_tracking_parameter_t, detection_tracking_threshold) }, \
         { "beacon_coarse_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_tracking_parameter_t, beacon_coarse_threshold) }, \
         { "beacon_refined_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_tracking_parameter_t, beacon_refined_threshold) }, \
         { "beacon_tracking_threshold", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_tracking_parameter_t, beacon_tracking_threshold) }, \
         { "coarse_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_tracking_parameter_t, coarse_linger_factor) }, \
         { "refined_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_tracking_parameter_t, refined_linger_factor) }, \
         { "tracking_linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_tracking_parameter_t, tracking_linger_factor) }, \
         { "kp_az", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_tracking_parameter_t, kp_az) }, \
         { "kp_el", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_tracking_parameter_t, kp_el) }, \
         { "kp_pol", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_tracking_parameter_t, kp_pol) }, \
         { "delta_az", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_tracking_parameter_t, delta_az) }, \
         { "delta_el", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_tracking_parameter_t, delta_el) }, \
         { "delta_pol", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_tracking_parameter_t, delta_pol) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 76, offsetof(mavlink_tracking_parameter_t, issue_mode) }, \
         } \
}
#endif

/**
 * @brief Pack a tracking_parameter message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param beamwidth  天线波束宽度(°)
 * @param dvb_coarse_threshold  DVB粗对准阈值
 * @param dvb_refined_threshold  DVB精对准阈值
 * @param dvb_tracking_threshold  DVB动态跟踪阈值
 * @param detection_coarse_threshold  检波粗对准阈值
 * @param detection_refined_threshold  检波精对准阈值
 * @param detection_tracking_threshold  检波动态跟踪阈值
 * @param beacon_coarse_threshold  信标粗对准阈值
 * @param beacon_refined_threshold  信标精对准阈值
 * @param beacon_tracking_threshold  信标动态跟踪阈值
 * @param coarse_linger_factor  粗对准波束驻留时间系数，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param refined_linger_factor  精对准束驻留时间系数
 * @param tracking_linger_factor  动态跟踪波束驻留时间系数
 * @param kp_az  动态跟踪方位角比例系数
 * @param kp_el  动态跟踪俯仰角比例系数
 * @param kp_pol  动态跟踪极化角比例系数
 * @param delta_az  动态跟踪方位角偏离轴心角度
 * @param delta_el  动态跟踪俯仰角偏离轴心角度
 * @param delta_pol  动态跟踪极化角偏离轴心角度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tracking_parameter_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float beamwidth, float dvb_coarse_threshold, float dvb_refined_threshold, float dvb_tracking_threshold, float detection_coarse_threshold, float detection_refined_threshold, float detection_tracking_threshold, float beacon_coarse_threshold, float beacon_refined_threshold, float beacon_tracking_threshold, float coarse_linger_factor, float refined_linger_factor, float tracking_linger_factor, float kp_az, float kp_el, float kp_pol, float delta_az, float delta_el, float delta_pol, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN];
    _mav_put_float(buf, 0, beamwidth);
    _mav_put_float(buf, 4, dvb_coarse_threshold);
    _mav_put_float(buf, 8, dvb_refined_threshold);
    _mav_put_float(buf, 12, dvb_tracking_threshold);
    _mav_put_float(buf, 16, detection_coarse_threshold);
    _mav_put_float(buf, 20, detection_refined_threshold);
    _mav_put_float(buf, 24, detection_tracking_threshold);
    _mav_put_float(buf, 28, beacon_coarse_threshold);
    _mav_put_float(buf, 32, beacon_refined_threshold);
    _mav_put_float(buf, 36, beacon_tracking_threshold);
    _mav_put_float(buf, 40, coarse_linger_factor);
    _mav_put_float(buf, 44, refined_linger_factor);
    _mav_put_float(buf, 48, tracking_linger_factor);
    _mav_put_float(buf, 52, kp_az);
    _mav_put_float(buf, 56, kp_el);
    _mav_put_float(buf, 60, kp_pol);
    _mav_put_float(buf, 64, delta_az);
    _mav_put_float(buf, 68, delta_el);
    _mav_put_float(buf, 72, delta_pol);
    _mav_put_uint8_t(buf, 76, issue_mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN);
#else
    mavlink_tracking_parameter_t packet;
    packet.beamwidth = beamwidth;
    packet.dvb_coarse_threshold = dvb_coarse_threshold;
    packet.dvb_refined_threshold = dvb_refined_threshold;
    packet.dvb_tracking_threshold = dvb_tracking_threshold;
    packet.detection_coarse_threshold = detection_coarse_threshold;
    packet.detection_refined_threshold = detection_refined_threshold;
    packet.detection_tracking_threshold = detection_tracking_threshold;
    packet.beacon_coarse_threshold = beacon_coarse_threshold;
    packet.beacon_refined_threshold = beacon_refined_threshold;
    packet.beacon_tracking_threshold = beacon_tracking_threshold;
    packet.coarse_linger_factor = coarse_linger_factor;
    packet.refined_linger_factor = refined_linger_factor;
    packet.tracking_linger_factor = tracking_linger_factor;
    packet.kp_az = kp_az;
    packet.kp_el = kp_el;
    packet.kp_pol = kp_pol;
    packet.delta_az = delta_az;
    packet.delta_el = delta_el;
    packet.delta_pol = delta_pol;
    packet.issue_mode = issue_mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TRACKING_PARAMETER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
}

/**
 * @brief Pack a tracking_parameter message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param beamwidth  天线波束宽度(°)
 * @param dvb_coarse_threshold  DVB粗对准阈值
 * @param dvb_refined_threshold  DVB精对准阈值
 * @param dvb_tracking_threshold  DVB动态跟踪阈值
 * @param detection_coarse_threshold  检波粗对准阈值
 * @param detection_refined_threshold  检波精对准阈值
 * @param detection_tracking_threshold  检波动态跟踪阈值
 * @param beacon_coarse_threshold  信标粗对准阈值
 * @param beacon_refined_threshold  信标精对准阈值
 * @param beacon_tracking_threshold  信标动态跟踪阈值
 * @param coarse_linger_factor  粗对准波束驻留时间系数，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param refined_linger_factor  精对准束驻留时间系数
 * @param tracking_linger_factor  动态跟踪波束驻留时间系数
 * @param kp_az  动态跟踪方位角比例系数
 * @param kp_el  动态跟踪俯仰角比例系数
 * @param kp_pol  动态跟踪极化角比例系数
 * @param delta_az  动态跟踪方位角偏离轴心角度
 * @param delta_el  动态跟踪俯仰角偏离轴心角度
 * @param delta_pol  动态跟踪极化角偏离轴心角度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tracking_parameter_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float beamwidth,float dvb_coarse_threshold,float dvb_refined_threshold,float dvb_tracking_threshold,float detection_coarse_threshold,float detection_refined_threshold,float detection_tracking_threshold,float beacon_coarse_threshold,float beacon_refined_threshold,float beacon_tracking_threshold,float coarse_linger_factor,float refined_linger_factor,float tracking_linger_factor,float kp_az,float kp_el,float kp_pol,float delta_az,float delta_el,float delta_pol,uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN];
    _mav_put_float(buf, 0, beamwidth);
    _mav_put_float(buf, 4, dvb_coarse_threshold);
    _mav_put_float(buf, 8, dvb_refined_threshold);
    _mav_put_float(buf, 12, dvb_tracking_threshold);
    _mav_put_float(buf, 16, detection_coarse_threshold);
    _mav_put_float(buf, 20, detection_refined_threshold);
    _mav_put_float(buf, 24, detection_tracking_threshold);
    _mav_put_float(buf, 28, beacon_coarse_threshold);
    _mav_put_float(buf, 32, beacon_refined_threshold);
    _mav_put_float(buf, 36, beacon_tracking_threshold);
    _mav_put_float(buf, 40, coarse_linger_factor);
    _mav_put_float(buf, 44, refined_linger_factor);
    _mav_put_float(buf, 48, tracking_linger_factor);
    _mav_put_float(buf, 52, kp_az);
    _mav_put_float(buf, 56, kp_el);
    _mav_put_float(buf, 60, kp_pol);
    _mav_put_float(buf, 64, delta_az);
    _mav_put_float(buf, 68, delta_el);
    _mav_put_float(buf, 72, delta_pol);
    _mav_put_uint8_t(buf, 76, issue_mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN);
#else
    mavlink_tracking_parameter_t packet;
    packet.beamwidth = beamwidth;
    packet.dvb_coarse_threshold = dvb_coarse_threshold;
    packet.dvb_refined_threshold = dvb_refined_threshold;
    packet.dvb_tracking_threshold = dvb_tracking_threshold;
    packet.detection_coarse_threshold = detection_coarse_threshold;
    packet.detection_refined_threshold = detection_refined_threshold;
    packet.detection_tracking_threshold = detection_tracking_threshold;
    packet.beacon_coarse_threshold = beacon_coarse_threshold;
    packet.beacon_refined_threshold = beacon_refined_threshold;
    packet.beacon_tracking_threshold = beacon_tracking_threshold;
    packet.coarse_linger_factor = coarse_linger_factor;
    packet.refined_linger_factor = refined_linger_factor;
    packet.tracking_linger_factor = tracking_linger_factor;
    packet.kp_az = kp_az;
    packet.kp_el = kp_el;
    packet.kp_pol = kp_pol;
    packet.delta_az = delta_az;
    packet.delta_el = delta_el;
    packet.delta_pol = delta_pol;
    packet.issue_mode = issue_mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TRACKING_PARAMETER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
}

/**
 * @brief Encode a tracking_parameter struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param tracking_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tracking_parameter_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_tracking_parameter_t* tracking_parameter)
{
    return mavlink_msg_tracking_parameter_pack(system_id, component_id, msg, tracking_parameter->beamwidth, tracking_parameter->dvb_coarse_threshold, tracking_parameter->dvb_refined_threshold, tracking_parameter->dvb_tracking_threshold, tracking_parameter->detection_coarse_threshold, tracking_parameter->detection_refined_threshold, tracking_parameter->detection_tracking_threshold, tracking_parameter->beacon_coarse_threshold, tracking_parameter->beacon_refined_threshold, tracking_parameter->beacon_tracking_threshold, tracking_parameter->coarse_linger_factor, tracking_parameter->refined_linger_factor, tracking_parameter->tracking_linger_factor, tracking_parameter->kp_az, tracking_parameter->kp_el, tracking_parameter->kp_pol, tracking_parameter->delta_az, tracking_parameter->delta_el, tracking_parameter->delta_pol, tracking_parameter->issue_mode);
}

/**
 * @brief Encode a tracking_parameter struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tracking_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tracking_parameter_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_tracking_parameter_t* tracking_parameter)
{
    return mavlink_msg_tracking_parameter_pack_chan(system_id, component_id, chan, msg, tracking_parameter->beamwidth, tracking_parameter->dvb_coarse_threshold, tracking_parameter->dvb_refined_threshold, tracking_parameter->dvb_tracking_threshold, tracking_parameter->detection_coarse_threshold, tracking_parameter->detection_refined_threshold, tracking_parameter->detection_tracking_threshold, tracking_parameter->beacon_coarse_threshold, tracking_parameter->beacon_refined_threshold, tracking_parameter->beacon_tracking_threshold, tracking_parameter->coarse_linger_factor, tracking_parameter->refined_linger_factor, tracking_parameter->tracking_linger_factor, tracking_parameter->kp_az, tracking_parameter->kp_el, tracking_parameter->kp_pol, tracking_parameter->delta_az, tracking_parameter->delta_el, tracking_parameter->delta_pol, tracking_parameter->issue_mode);
}

/**
 * @brief Send a tracking_parameter message
 * @param chan MAVLink channel to send the message
 *
 * @param beamwidth  天线波束宽度(°)
 * @param dvb_coarse_threshold  DVB粗对准阈值
 * @param dvb_refined_threshold  DVB精对准阈值
 * @param dvb_tracking_threshold  DVB动态跟踪阈值
 * @param detection_coarse_threshold  检波粗对准阈值
 * @param detection_refined_threshold  检波精对准阈值
 * @param detection_tracking_threshold  检波动态跟踪阈值
 * @param beacon_coarse_threshold  信标粗对准阈值
 * @param beacon_refined_threshold  信标精对准阈值
 * @param beacon_tracking_threshold  信标动态跟踪阈值
 * @param coarse_linger_factor  粗对准波束驻留时间系数，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param refined_linger_factor  精对准束驻留时间系数
 * @param tracking_linger_factor  动态跟踪波束驻留时间系数
 * @param kp_az  动态跟踪方位角比例系数
 * @param kp_el  动态跟踪俯仰角比例系数
 * @param kp_pol  动态跟踪极化角比例系数
 * @param delta_az  动态跟踪方位角偏离轴心角度
 * @param delta_el  动态跟踪俯仰角偏离轴心角度
 * @param delta_pol  动态跟踪极化角偏离轴心角度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_tracking_parameter_send(mavlink_channel_t chan, float beamwidth, float dvb_coarse_threshold, float dvb_refined_threshold, float dvb_tracking_threshold, float detection_coarse_threshold, float detection_refined_threshold, float detection_tracking_threshold, float beacon_coarse_threshold, float beacon_refined_threshold, float beacon_tracking_threshold, float coarse_linger_factor, float refined_linger_factor, float tracking_linger_factor, float kp_az, float kp_el, float kp_pol, float delta_az, float delta_el, float delta_pol, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN];
    _mav_put_float(buf, 0, beamwidth);
    _mav_put_float(buf, 4, dvb_coarse_threshold);
    _mav_put_float(buf, 8, dvb_refined_threshold);
    _mav_put_float(buf, 12, dvb_tracking_threshold);
    _mav_put_float(buf, 16, detection_coarse_threshold);
    _mav_put_float(buf, 20, detection_refined_threshold);
    _mav_put_float(buf, 24, detection_tracking_threshold);
    _mav_put_float(buf, 28, beacon_coarse_threshold);
    _mav_put_float(buf, 32, beacon_refined_threshold);
    _mav_put_float(buf, 36, beacon_tracking_threshold);
    _mav_put_float(buf, 40, coarse_linger_factor);
    _mav_put_float(buf, 44, refined_linger_factor);
    _mav_put_float(buf, 48, tracking_linger_factor);
    _mav_put_float(buf, 52, kp_az);
    _mav_put_float(buf, 56, kp_el);
    _mav_put_float(buf, 60, kp_pol);
    _mav_put_float(buf, 64, delta_az);
    _mav_put_float(buf, 68, delta_el);
    _mav_put_float(buf, 72, delta_pol);
    _mav_put_uint8_t(buf, 76, issue_mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TRACKING_PARAMETER, buf, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
#else
    mavlink_tracking_parameter_t packet;
    packet.beamwidth = beamwidth;
    packet.dvb_coarse_threshold = dvb_coarse_threshold;
    packet.dvb_refined_threshold = dvb_refined_threshold;
    packet.dvb_tracking_threshold = dvb_tracking_threshold;
    packet.detection_coarse_threshold = detection_coarse_threshold;
    packet.detection_refined_threshold = detection_refined_threshold;
    packet.detection_tracking_threshold = detection_tracking_threshold;
    packet.beacon_coarse_threshold = beacon_coarse_threshold;
    packet.beacon_refined_threshold = beacon_refined_threshold;
    packet.beacon_tracking_threshold = beacon_tracking_threshold;
    packet.coarse_linger_factor = coarse_linger_factor;
    packet.refined_linger_factor = refined_linger_factor;
    packet.tracking_linger_factor = tracking_linger_factor;
    packet.kp_az = kp_az;
    packet.kp_el = kp_el;
    packet.kp_pol = kp_pol;
    packet.delta_az = delta_az;
    packet.delta_el = delta_el;
    packet.delta_pol = delta_pol;
    packet.issue_mode = issue_mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TRACKING_PARAMETER, (const char *)&packet, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
#endif
}

/**
 * @brief Send a tracking_parameter message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_tracking_parameter_send_struct(mavlink_channel_t chan, const mavlink_tracking_parameter_t* tracking_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_tracking_parameter_send(chan, tracking_parameter->beamwidth, tracking_parameter->dvb_coarse_threshold, tracking_parameter->dvb_refined_threshold, tracking_parameter->dvb_tracking_threshold, tracking_parameter->detection_coarse_threshold, tracking_parameter->detection_refined_threshold, tracking_parameter->detection_tracking_threshold, tracking_parameter->beacon_coarse_threshold, tracking_parameter->beacon_refined_threshold, tracking_parameter->beacon_tracking_threshold, tracking_parameter->coarse_linger_factor, tracking_parameter->refined_linger_factor, tracking_parameter->tracking_linger_factor, tracking_parameter->kp_az, tracking_parameter->kp_el, tracking_parameter->kp_pol, tracking_parameter->delta_az, tracking_parameter->delta_el, tracking_parameter->delta_pol, tracking_parameter->issue_mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TRACKING_PARAMETER, (const char *)tracking_parameter, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
#endif
}

#if MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_tracking_parameter_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float beamwidth, float dvb_coarse_threshold, float dvb_refined_threshold, float dvb_tracking_threshold, float detection_coarse_threshold, float detection_refined_threshold, float detection_tracking_threshold, float beacon_coarse_threshold, float beacon_refined_threshold, float beacon_tracking_threshold, float coarse_linger_factor, float refined_linger_factor, float tracking_linger_factor, float kp_az, float kp_el, float kp_pol, float delta_az, float delta_el, float delta_pol, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, beamwidth);
    _mav_put_float(buf, 4, dvb_coarse_threshold);
    _mav_put_float(buf, 8, dvb_refined_threshold);
    _mav_put_float(buf, 12, dvb_tracking_threshold);
    _mav_put_float(buf, 16, detection_coarse_threshold);
    _mav_put_float(buf, 20, detection_refined_threshold);
    _mav_put_float(buf, 24, detection_tracking_threshold);
    _mav_put_float(buf, 28, beacon_coarse_threshold);
    _mav_put_float(buf, 32, beacon_refined_threshold);
    _mav_put_float(buf, 36, beacon_tracking_threshold);
    _mav_put_float(buf, 40, coarse_linger_factor);
    _mav_put_float(buf, 44, refined_linger_factor);
    _mav_put_float(buf, 48, tracking_linger_factor);
    _mav_put_float(buf, 52, kp_az);
    _mav_put_float(buf, 56, kp_el);
    _mav_put_float(buf, 60, kp_pol);
    _mav_put_float(buf, 64, delta_az);
    _mav_put_float(buf, 68, delta_el);
    _mav_put_float(buf, 72, delta_pol);
    _mav_put_uint8_t(buf, 76, issue_mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TRACKING_PARAMETER, buf, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
#else
    mavlink_tracking_parameter_t *packet = (mavlink_tracking_parameter_t *)msgbuf;
    packet->beamwidth = beamwidth;
    packet->dvb_coarse_threshold = dvb_coarse_threshold;
    packet->dvb_refined_threshold = dvb_refined_threshold;
    packet->dvb_tracking_threshold = dvb_tracking_threshold;
    packet->detection_coarse_threshold = detection_coarse_threshold;
    packet->detection_refined_threshold = detection_refined_threshold;
    packet->detection_tracking_threshold = detection_tracking_threshold;
    packet->beacon_coarse_threshold = beacon_coarse_threshold;
    packet->beacon_refined_threshold = beacon_refined_threshold;
    packet->beacon_tracking_threshold = beacon_tracking_threshold;
    packet->coarse_linger_factor = coarse_linger_factor;
    packet->refined_linger_factor = refined_linger_factor;
    packet->tracking_linger_factor = tracking_linger_factor;
    packet->kp_az = kp_az;
    packet->kp_el = kp_el;
    packet->kp_pol = kp_pol;
    packet->delta_az = delta_az;
    packet->delta_el = delta_el;
    packet->delta_pol = delta_pol;
    packet->issue_mode = issue_mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TRACKING_PARAMETER, (const char *)packet, MAVLINK_MSG_ID_TRACKING_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN, MAVLINK_MSG_ID_TRACKING_PARAMETER_CRC);
#endif
}
#endif

#endif

// MESSAGE TRACKING_PARAMETER UNPACKING


/**
 * @brief Get field beamwidth from tracking_parameter message
 *
 * @return  天线波束宽度(°)
 */
static inline float mavlink_msg_tracking_parameter_get_beamwidth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field dvb_coarse_threshold from tracking_parameter message
 *
 * @return  DVB粗对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_dvb_coarse_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field dvb_refined_threshold from tracking_parameter message
 *
 * @return  DVB精对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_dvb_refined_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field dvb_tracking_threshold from tracking_parameter message
 *
 * @return  DVB动态跟踪阈值
 */
static inline float mavlink_msg_tracking_parameter_get_dvb_tracking_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field detection_coarse_threshold from tracking_parameter message
 *
 * @return  检波粗对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_detection_coarse_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field detection_refined_threshold from tracking_parameter message
 *
 * @return  检波精对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_detection_refined_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field detection_tracking_threshold from tracking_parameter message
 *
 * @return  检波动态跟踪阈值
 */
static inline float mavlink_msg_tracking_parameter_get_detection_tracking_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field beacon_coarse_threshold from tracking_parameter message
 *
 * @return  信标粗对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_beacon_coarse_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field beacon_refined_threshold from tracking_parameter message
 *
 * @return  信标精对准阈值
 */
static inline float mavlink_msg_tracking_parameter_get_beacon_refined_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field beacon_tracking_threshold from tracking_parameter message
 *
 * @return  信标动态跟踪阈值
 */
static inline float mavlink_msg_tracking_parameter_get_beacon_tracking_threshold(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field coarse_linger_factor from tracking_parameter message
 *
 * @return  粗对准波束驻留时间系数，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 */
static inline float mavlink_msg_tracking_parameter_get_coarse_linger_factor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Get field refined_linger_factor from tracking_parameter message
 *
 * @return  精对准束驻留时间系数
 */
static inline float mavlink_msg_tracking_parameter_get_refined_linger_factor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  44);
}

/**
 * @brief Get field tracking_linger_factor from tracking_parameter message
 *
 * @return  动态跟踪波束驻留时间系数
 */
static inline float mavlink_msg_tracking_parameter_get_tracking_linger_factor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  48);
}

/**
 * @brief Get field kp_az from tracking_parameter message
 *
 * @return  动态跟踪方位角比例系数
 */
static inline float mavlink_msg_tracking_parameter_get_kp_az(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  52);
}

/**
 * @brief Get field kp_el from tracking_parameter message
 *
 * @return  动态跟踪俯仰角比例系数
 */
static inline float mavlink_msg_tracking_parameter_get_kp_el(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  56);
}

/**
 * @brief Get field kp_pol from tracking_parameter message
 *
 * @return  动态跟踪极化角比例系数
 */
static inline float mavlink_msg_tracking_parameter_get_kp_pol(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  60);
}

/**
 * @brief Get field delta_az from tracking_parameter message
 *
 * @return  动态跟踪方位角偏离轴心角度
 */
static inline float mavlink_msg_tracking_parameter_get_delta_az(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  64);
}

/**
 * @brief Get field delta_el from tracking_parameter message
 *
 * @return  动态跟踪俯仰角偏离轴心角度
 */
static inline float mavlink_msg_tracking_parameter_get_delta_el(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  68);
}

/**
 * @brief Get field delta_pol from tracking_parameter message
 *
 * @return  动态跟踪极化角偏离轴心角度
 */
static inline float mavlink_msg_tracking_parameter_get_delta_pol(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  72);
}

/**
 * @brief Get field issue_mode from tracking_parameter message
 *
 * @return  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
static inline uint8_t mavlink_msg_tracking_parameter_get_issue_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  76);
}

/**
 * @brief Decode a tracking_parameter message into a struct
 *
 * @param msg The message to decode
 * @param tracking_parameter C-struct to decode the message contents into
 */
static inline void mavlink_msg_tracking_parameter_decode(const mavlink_message_t* msg, mavlink_tracking_parameter_t* tracking_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    tracking_parameter->beamwidth = mavlink_msg_tracking_parameter_get_beamwidth(msg);
    tracking_parameter->dvb_coarse_threshold = mavlink_msg_tracking_parameter_get_dvb_coarse_threshold(msg);
    tracking_parameter->dvb_refined_threshold = mavlink_msg_tracking_parameter_get_dvb_refined_threshold(msg);
    tracking_parameter->dvb_tracking_threshold = mavlink_msg_tracking_parameter_get_dvb_tracking_threshold(msg);
    tracking_parameter->detection_coarse_threshold = mavlink_msg_tracking_parameter_get_detection_coarse_threshold(msg);
    tracking_parameter->detection_refined_threshold = mavlink_msg_tracking_parameter_get_detection_refined_threshold(msg);
    tracking_parameter->detection_tracking_threshold = mavlink_msg_tracking_parameter_get_detection_tracking_threshold(msg);
    tracking_parameter->beacon_coarse_threshold = mavlink_msg_tracking_parameter_get_beacon_coarse_threshold(msg);
    tracking_parameter->beacon_refined_threshold = mavlink_msg_tracking_parameter_get_beacon_refined_threshold(msg);
    tracking_parameter->beacon_tracking_threshold = mavlink_msg_tracking_parameter_get_beacon_tracking_threshold(msg);
    tracking_parameter->coarse_linger_factor = mavlink_msg_tracking_parameter_get_coarse_linger_factor(msg);
    tracking_parameter->refined_linger_factor = mavlink_msg_tracking_parameter_get_refined_linger_factor(msg);
    tracking_parameter->tracking_linger_factor = mavlink_msg_tracking_parameter_get_tracking_linger_factor(msg);
    tracking_parameter->kp_az = mavlink_msg_tracking_parameter_get_kp_az(msg);
    tracking_parameter->kp_el = mavlink_msg_tracking_parameter_get_kp_el(msg);
    tracking_parameter->kp_pol = mavlink_msg_tracking_parameter_get_kp_pol(msg);
    tracking_parameter->delta_az = mavlink_msg_tracking_parameter_get_delta_az(msg);
    tracking_parameter->delta_el = mavlink_msg_tracking_parameter_get_delta_el(msg);
    tracking_parameter->delta_pol = mavlink_msg_tracking_parameter_get_delta_pol(msg);
    tracking_parameter->issue_mode = mavlink_msg_tracking_parameter_get_issue_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN? msg->len : MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN;
        memset(tracking_parameter, 0, MAVLINK_MSG_ID_TRACKING_PARAMETER_LEN);
    memcpy(tracking_parameter, _MAV_PAYLOAD(msg), len);
#endif
}
