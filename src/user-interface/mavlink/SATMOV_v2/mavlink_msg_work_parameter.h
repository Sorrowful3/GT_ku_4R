#pragma once
// MESSAGE WORK_PARAMETER PACKING

#define MAVLINK_MSG_ID_WORK_PARAMETER 47

MAVPACKED(
typedef struct __mavlink_work_parameter_t {
 float rolloff_factor; /*<  波束滚降系数+1，OpenAMP使用此参数结合modem发送的带宽计算符号速率*/
 float manual_longitude; /*<  手动GPS经度*/
 float manual_latitude; /*<  手动GPS纬度*/
 float manual_height; /*<  手动GPS高度*/
 uint16_t openamip_port; /*<  OpenAMIP的端口，修改后需要重启APP*/
 uint16_t servicer_port; /*<  服务器的端口，修改后需要重启APP*/
 uint8_t work_mode; /*<  工作模式，0x00固定站，0x01动中通*/
 char openamip_ip[32]; /*<  OpenAMIP的IP地址，修改需要重启APP*/
 char openamip_mask[32]; /*<  OpenAMIP的子网掩码，修改需要重启APP*/
 char servicer_ip[32]; /*<  服务器的IP地址，修改需要重启APP*/
 char servicer_mask[32]; /*<  服务器的子网掩码，修改需要重启APP*/
 uint8_t tracking_mode; /*<  跟踪模式， 0x00 DVB模式，0x01 检波模式，0x02 信标模式，0x03 组合模式*/
 uint8_t manual_gps; /*<  GPS模式，0x00使用GPS模块，0x01使用手动输入*/
 uint8_t issue_mode; /*<  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复*/
}) mavlink_work_parameter_t;

#define MAVLINK_MSG_ID_WORK_PARAMETER_LEN 152
#define MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN 152
#define MAVLINK_MSG_ID_47_LEN 152
#define MAVLINK_MSG_ID_47_MIN_LEN 152

#define MAVLINK_MSG_ID_WORK_PARAMETER_CRC 187
#define MAVLINK_MSG_ID_47_CRC 187

#define MAVLINK_MSG_WORK_PARAMETER_FIELD_OPENAMIP_IP_LEN 32
#define MAVLINK_MSG_WORK_PARAMETER_FIELD_OPENAMIP_MASK_LEN 32
#define MAVLINK_MSG_WORK_PARAMETER_FIELD_SERVICER_IP_LEN 32
#define MAVLINK_MSG_WORK_PARAMETER_FIELD_SERVICER_MASK_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WORK_PARAMETER { \
    47, \
    "WORK_PARAMETER", \
    14, \
    {  { "work_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_work_parameter_t, work_mode) }, \
         { "openamip_ip", NULL, MAVLINK_TYPE_CHAR, 32, 21, offsetof(mavlink_work_parameter_t, openamip_ip) }, \
         { "openamip_mask", NULL, MAVLINK_TYPE_CHAR, 32, 53, offsetof(mavlink_work_parameter_t, openamip_mask) }, \
         { "openamip_port", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_work_parameter_t, openamip_port) }, \
         { "rolloff_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_work_parameter_t, rolloff_factor) }, \
         { "servicer_ip", NULL, MAVLINK_TYPE_CHAR, 32, 85, offsetof(mavlink_work_parameter_t, servicer_ip) }, \
         { "servicer_mask", NULL, MAVLINK_TYPE_CHAR, 32, 117, offsetof(mavlink_work_parameter_t, servicer_mask) }, \
         { "servicer_port", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_work_parameter_t, servicer_port) }, \
         { "tracking_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 149, offsetof(mavlink_work_parameter_t, tracking_mode) }, \
         { "manual_gps", NULL, MAVLINK_TYPE_UINT8_T, 0, 150, offsetof(mavlink_work_parameter_t, manual_gps) }, \
         { "manual_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_work_parameter_t, manual_longitude) }, \
         { "manual_latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_work_parameter_t, manual_latitude) }, \
         { "manual_height", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_work_parameter_t, manual_height) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 151, offsetof(mavlink_work_parameter_t, issue_mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WORK_PARAMETER { \
    "WORK_PARAMETER", \
    14, \
    {  { "work_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_work_parameter_t, work_mode) }, \
         { "openamip_ip", NULL, MAVLINK_TYPE_CHAR, 32, 21, offsetof(mavlink_work_parameter_t, openamip_ip) }, \
         { "openamip_mask", NULL, MAVLINK_TYPE_CHAR, 32, 53, offsetof(mavlink_work_parameter_t, openamip_mask) }, \
         { "openamip_port", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_work_parameter_t, openamip_port) }, \
         { "rolloff_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_work_parameter_t, rolloff_factor) }, \
         { "servicer_ip", NULL, MAVLINK_TYPE_CHAR, 32, 85, offsetof(mavlink_work_parameter_t, servicer_ip) }, \
         { "servicer_mask", NULL, MAVLINK_TYPE_CHAR, 32, 117, offsetof(mavlink_work_parameter_t, servicer_mask) }, \
         { "servicer_port", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_work_parameter_t, servicer_port) }, \
         { "tracking_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 149, offsetof(mavlink_work_parameter_t, tracking_mode) }, \
         { "manual_gps", NULL, MAVLINK_TYPE_UINT8_T, 0, 150, offsetof(mavlink_work_parameter_t, manual_gps) }, \
         { "manual_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_work_parameter_t, manual_longitude) }, \
         { "manual_latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_work_parameter_t, manual_latitude) }, \
         { "manual_height", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_work_parameter_t, manual_height) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 151, offsetof(mavlink_work_parameter_t, issue_mode) }, \
         } \
}
#endif

/**
 * @brief Pack a work_parameter message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param work_mode  工作模式，0x00固定站，0x01动中通
 * @param openamip_ip  OpenAMIP的IP地址，修改需要重启APP
 * @param openamip_mask  OpenAMIP的子网掩码，修改需要重启APP
 * @param openamip_port  OpenAMIP的端口，修改后需要重启APP
 * @param rolloff_factor  波束滚降系数+1，OpenAMP使用此参数结合modem发送的带宽计算符号速率
 * @param servicer_ip  服务器的IP地址，修改需要重启APP
 * @param servicer_mask  服务器的子网掩码，修改需要重启APP
 * @param servicer_port  服务器的端口，修改后需要重启APP
 * @param tracking_mode  跟踪模式， 0x00 DVB模式，0x01 检波模式，0x02 信标模式，0x03 组合模式
 * @param manual_gps  GPS模式，0x00使用GPS模块，0x01使用手动输入
 * @param manual_longitude  手动GPS经度
 * @param manual_latitude  手动GPS纬度
 * @param manual_height  手动GPS高度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_work_parameter_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t work_mode, const char *openamip_ip, const char *openamip_mask, uint16_t openamip_port, float rolloff_factor, const char *servicer_ip, const char *servicer_mask, uint16_t servicer_port, uint8_t tracking_mode, uint8_t manual_gps, float manual_longitude, float manual_latitude, float manual_height, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WORK_PARAMETER_LEN];
    _mav_put_float(buf, 0, rolloff_factor);
    _mav_put_float(buf, 4, manual_longitude);
    _mav_put_float(buf, 8, manual_latitude);
    _mav_put_float(buf, 12, manual_height);
    _mav_put_uint16_t(buf, 16, openamip_port);
    _mav_put_uint16_t(buf, 18, servicer_port);
    _mav_put_uint8_t(buf, 20, work_mode);
    _mav_put_uint8_t(buf, 149, tracking_mode);
    _mav_put_uint8_t(buf, 150, manual_gps);
    _mav_put_uint8_t(buf, 151, issue_mode);
    _mav_put_char_array(buf, 21, openamip_ip, 32);
    _mav_put_char_array(buf, 53, openamip_mask, 32);
    _mav_put_char_array(buf, 85, servicer_ip, 32);
    _mav_put_char_array(buf, 117, servicer_mask, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WORK_PARAMETER_LEN);
#else
    mavlink_work_parameter_t packet;
    packet.rolloff_factor = rolloff_factor;
    packet.manual_longitude = manual_longitude;
    packet.manual_latitude = manual_latitude;
    packet.manual_height = manual_height;
    packet.openamip_port = openamip_port;
    packet.servicer_port = servicer_port;
    packet.work_mode = work_mode;
    packet.tracking_mode = tracking_mode;
    packet.manual_gps = manual_gps;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.openamip_ip, openamip_ip, sizeof(char)*32);
    mav_array_memcpy(packet.openamip_mask, openamip_mask, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_ip, servicer_ip, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_mask, servicer_mask, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WORK_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WORK_PARAMETER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
}

/**
 * @brief Pack a work_parameter message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param work_mode  工作模式，0x00固定站，0x01动中通
 * @param openamip_ip  OpenAMIP的IP地址，修改需要重启APP
 * @param openamip_mask  OpenAMIP的子网掩码，修改需要重启APP
 * @param openamip_port  OpenAMIP的端口，修改后需要重启APP
 * @param rolloff_factor  波束滚降系数+1，OpenAMP使用此参数结合modem发送的带宽计算符号速率
 * @param servicer_ip  服务器的IP地址，修改需要重启APP
 * @param servicer_mask  服务器的子网掩码，修改需要重启APP
 * @param servicer_port  服务器的端口，修改后需要重启APP
 * @param tracking_mode  跟踪模式， 0x00 DVB模式，0x01 检波模式，0x02 信标模式，0x03 组合模式
 * @param manual_gps  GPS模式，0x00使用GPS模块，0x01使用手动输入
 * @param manual_longitude  手动GPS经度
 * @param manual_latitude  手动GPS纬度
 * @param manual_height  手动GPS高度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_work_parameter_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t work_mode,const char *openamip_ip,const char *openamip_mask,uint16_t openamip_port,float rolloff_factor,const char *servicer_ip,const char *servicer_mask,uint16_t servicer_port,uint8_t tracking_mode,uint8_t manual_gps,float manual_longitude,float manual_latitude,float manual_height,uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WORK_PARAMETER_LEN];
    _mav_put_float(buf, 0, rolloff_factor);
    _mav_put_float(buf, 4, manual_longitude);
    _mav_put_float(buf, 8, manual_latitude);
    _mav_put_float(buf, 12, manual_height);
    _mav_put_uint16_t(buf, 16, openamip_port);
    _mav_put_uint16_t(buf, 18, servicer_port);
    _mav_put_uint8_t(buf, 20, work_mode);
    _mav_put_uint8_t(buf, 149, tracking_mode);
    _mav_put_uint8_t(buf, 150, manual_gps);
    _mav_put_uint8_t(buf, 151, issue_mode);
    _mav_put_char_array(buf, 21, openamip_ip, 32);
    _mav_put_char_array(buf, 53, openamip_mask, 32);
    _mav_put_char_array(buf, 85, servicer_ip, 32);
    _mav_put_char_array(buf, 117, servicer_mask, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WORK_PARAMETER_LEN);
#else
    mavlink_work_parameter_t packet;
    packet.rolloff_factor = rolloff_factor;
    packet.manual_longitude = manual_longitude;
    packet.manual_latitude = manual_latitude;
    packet.manual_height = manual_height;
    packet.openamip_port = openamip_port;
    packet.servicer_port = servicer_port;
    packet.work_mode = work_mode;
    packet.tracking_mode = tracking_mode;
    packet.manual_gps = manual_gps;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.openamip_ip, openamip_ip, sizeof(char)*32);
    mav_array_memcpy(packet.openamip_mask, openamip_mask, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_ip, servicer_ip, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_mask, servicer_mask, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WORK_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WORK_PARAMETER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
}

/**
 * @brief Encode a work_parameter struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param work_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_work_parameter_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_work_parameter_t* work_parameter)
{
    return mavlink_msg_work_parameter_pack(system_id, component_id, msg, work_parameter->work_mode, work_parameter->openamip_ip, work_parameter->openamip_mask, work_parameter->openamip_port, work_parameter->rolloff_factor, work_parameter->servicer_ip, work_parameter->servicer_mask, work_parameter->servicer_port, work_parameter->tracking_mode, work_parameter->manual_gps, work_parameter->manual_longitude, work_parameter->manual_latitude, work_parameter->manual_height, work_parameter->issue_mode);
}

/**
 * @brief Encode a work_parameter struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param work_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_work_parameter_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_work_parameter_t* work_parameter)
{
    return mavlink_msg_work_parameter_pack_chan(system_id, component_id, chan, msg, work_parameter->work_mode, work_parameter->openamip_ip, work_parameter->openamip_mask, work_parameter->openamip_port, work_parameter->rolloff_factor, work_parameter->servicer_ip, work_parameter->servicer_mask, work_parameter->servicer_port, work_parameter->tracking_mode, work_parameter->manual_gps, work_parameter->manual_longitude, work_parameter->manual_latitude, work_parameter->manual_height, work_parameter->issue_mode);
}

/**
 * @brief Send a work_parameter message
 * @param chan MAVLink channel to send the message
 *
 * @param work_mode  工作模式，0x00固定站，0x01动中通
 * @param openamip_ip  OpenAMIP的IP地址，修改需要重启APP
 * @param openamip_mask  OpenAMIP的子网掩码，修改需要重启APP
 * @param openamip_port  OpenAMIP的端口，修改后需要重启APP
 * @param rolloff_factor  波束滚降系数+1，OpenAMP使用此参数结合modem发送的带宽计算符号速率
 * @param servicer_ip  服务器的IP地址，修改需要重启APP
 * @param servicer_mask  服务器的子网掩码，修改需要重启APP
 * @param servicer_port  服务器的端口，修改后需要重启APP
 * @param tracking_mode  跟踪模式， 0x00 DVB模式，0x01 检波模式，0x02 信标模式，0x03 组合模式
 * @param manual_gps  GPS模式，0x00使用GPS模块，0x01使用手动输入
 * @param manual_longitude  手动GPS经度
 * @param manual_latitude  手动GPS纬度
 * @param manual_height  手动GPS高度
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_work_parameter_send(mavlink_channel_t chan, uint8_t work_mode, const char *openamip_ip, const char *openamip_mask, uint16_t openamip_port, float rolloff_factor, const char *servicer_ip, const char *servicer_mask, uint16_t servicer_port, uint8_t tracking_mode, uint8_t manual_gps, float manual_longitude, float manual_latitude, float manual_height, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WORK_PARAMETER_LEN];
    _mav_put_float(buf, 0, rolloff_factor);
    _mav_put_float(buf, 4, manual_longitude);
    _mav_put_float(buf, 8, manual_latitude);
    _mav_put_float(buf, 12, manual_height);
    _mav_put_uint16_t(buf, 16, openamip_port);
    _mav_put_uint16_t(buf, 18, servicer_port);
    _mav_put_uint8_t(buf, 20, work_mode);
    _mav_put_uint8_t(buf, 149, tracking_mode);
    _mav_put_uint8_t(buf, 150, manual_gps);
    _mav_put_uint8_t(buf, 151, issue_mode);
    _mav_put_char_array(buf, 21, openamip_ip, 32);
    _mav_put_char_array(buf, 53, openamip_mask, 32);
    _mav_put_char_array(buf, 85, servicer_ip, 32);
    _mav_put_char_array(buf, 117, servicer_mask, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WORK_PARAMETER, buf, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
#else
    mavlink_work_parameter_t packet;
    packet.rolloff_factor = rolloff_factor;
    packet.manual_longitude = manual_longitude;
    packet.manual_latitude = manual_latitude;
    packet.manual_height = manual_height;
    packet.openamip_port = openamip_port;
    packet.servicer_port = servicer_port;
    packet.work_mode = work_mode;
    packet.tracking_mode = tracking_mode;
    packet.manual_gps = manual_gps;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.openamip_ip, openamip_ip, sizeof(char)*32);
    mav_array_memcpy(packet.openamip_mask, openamip_mask, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_ip, servicer_ip, sizeof(char)*32);
    mav_array_memcpy(packet.servicer_mask, servicer_mask, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WORK_PARAMETER, (const char *)&packet, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
#endif
}

/**
 * @brief Send a work_parameter message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_work_parameter_send_struct(mavlink_channel_t chan, const mavlink_work_parameter_t* work_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_work_parameter_send(chan, work_parameter->work_mode, work_parameter->openamip_ip, work_parameter->openamip_mask, work_parameter->openamip_port, work_parameter->rolloff_factor, work_parameter->servicer_ip, work_parameter->servicer_mask, work_parameter->servicer_port, work_parameter->tracking_mode, work_parameter->manual_gps, work_parameter->manual_longitude, work_parameter->manual_latitude, work_parameter->manual_height, work_parameter->issue_mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WORK_PARAMETER, (const char *)work_parameter, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
#endif
}

#if MAVLINK_MSG_ID_WORK_PARAMETER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_work_parameter_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t work_mode, const char *openamip_ip, const char *openamip_mask, uint16_t openamip_port, float rolloff_factor, const char *servicer_ip, const char *servicer_mask, uint16_t servicer_port, uint8_t tracking_mode, uint8_t manual_gps, float manual_longitude, float manual_latitude, float manual_height, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, rolloff_factor);
    _mav_put_float(buf, 4, manual_longitude);
    _mav_put_float(buf, 8, manual_latitude);
    _mav_put_float(buf, 12, manual_height);
    _mav_put_uint16_t(buf, 16, openamip_port);
    _mav_put_uint16_t(buf, 18, servicer_port);
    _mav_put_uint8_t(buf, 20, work_mode);
    _mav_put_uint8_t(buf, 149, tracking_mode);
    _mav_put_uint8_t(buf, 150, manual_gps);
    _mav_put_uint8_t(buf, 151, issue_mode);
    _mav_put_char_array(buf, 21, openamip_ip, 32);
    _mav_put_char_array(buf, 53, openamip_mask, 32);
    _mav_put_char_array(buf, 85, servicer_ip, 32);
    _mav_put_char_array(buf, 117, servicer_mask, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WORK_PARAMETER, buf, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
#else
    mavlink_work_parameter_t *packet = (mavlink_work_parameter_t *)msgbuf;
    packet->rolloff_factor = rolloff_factor;
    packet->manual_longitude = manual_longitude;
    packet->manual_latitude = manual_latitude;
    packet->manual_height = manual_height;
    packet->openamip_port = openamip_port;
    packet->servicer_port = servicer_port;
    packet->work_mode = work_mode;
    packet->tracking_mode = tracking_mode;
    packet->manual_gps = manual_gps;
    packet->issue_mode = issue_mode;
    mav_array_memcpy(packet->openamip_ip, openamip_ip, sizeof(char)*32);
    mav_array_memcpy(packet->openamip_mask, openamip_mask, sizeof(char)*32);
    mav_array_memcpy(packet->servicer_ip, servicer_ip, sizeof(char)*32);
    mav_array_memcpy(packet->servicer_mask, servicer_mask, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WORK_PARAMETER, (const char *)packet, MAVLINK_MSG_ID_WORK_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_LEN, MAVLINK_MSG_ID_WORK_PARAMETER_CRC);
#endif
}
#endif

#endif

// MESSAGE WORK_PARAMETER UNPACKING


/**
 * @brief Get field work_mode from work_parameter message
 *
 * @return  工作模式，0x00固定站，0x01动中通
 */
static inline uint8_t mavlink_msg_work_parameter_get_work_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field openamip_ip from work_parameter message
 *
 * @return  OpenAMIP的IP地址，修改需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_openamip_ip(const mavlink_message_t* msg, char *openamip_ip)
{
    return _MAV_RETURN_char_array(msg, openamip_ip, 32,  21);
}

/**
 * @brief Get field openamip_mask from work_parameter message
 *
 * @return  OpenAMIP的子网掩码，修改需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_openamip_mask(const mavlink_message_t* msg, char *openamip_mask)
{
    return _MAV_RETURN_char_array(msg, openamip_mask, 32,  53);
}

/**
 * @brief Get field openamip_port from work_parameter message
 *
 * @return  OpenAMIP的端口，修改后需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_openamip_port(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field rolloff_factor from work_parameter message
 *
 * @return  波束滚降系数+1，OpenAMP使用此参数结合modem发送的带宽计算符号速率
 */
static inline float mavlink_msg_work_parameter_get_rolloff_factor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field servicer_ip from work_parameter message
 *
 * @return  服务器的IP地址，修改需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_servicer_ip(const mavlink_message_t* msg, char *servicer_ip)
{
    return _MAV_RETURN_char_array(msg, servicer_ip, 32,  85);
}

/**
 * @brief Get field servicer_mask from work_parameter message
 *
 * @return  服务器的子网掩码，修改需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_servicer_mask(const mavlink_message_t* msg, char *servicer_mask)
{
    return _MAV_RETURN_char_array(msg, servicer_mask, 32,  117);
}

/**
 * @brief Get field servicer_port from work_parameter message
 *
 * @return  服务器的端口，修改后需要重启APP
 */
static inline uint16_t mavlink_msg_work_parameter_get_servicer_port(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  18);
}

/**
 * @brief Get field tracking_mode from work_parameter message
 *
 * @return  跟踪模式， 0x00 DVB模式，0x01 检波模式，0x02 信标模式，0x03 组合模式
 */
static inline uint8_t mavlink_msg_work_parameter_get_tracking_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  149);
}

/**
 * @brief Get field manual_gps from work_parameter message
 *
 * @return  GPS模式，0x00使用GPS模块，0x01使用手动输入
 */
static inline uint8_t mavlink_msg_work_parameter_get_manual_gps(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  150);
}

/**
 * @brief Get field manual_longitude from work_parameter message
 *
 * @return  手动GPS经度
 */
static inline float mavlink_msg_work_parameter_get_manual_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field manual_latitude from work_parameter message
 *
 * @return  手动GPS纬度
 */
static inline float mavlink_msg_work_parameter_get_manual_latitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field manual_height from work_parameter message
 *
 * @return  手动GPS高度
 */
static inline float mavlink_msg_work_parameter_get_manual_height(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field issue_mode from work_parameter message
 *
 * @return  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
static inline uint8_t mavlink_msg_work_parameter_get_issue_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  151);
}

/**
 * @brief Decode a work_parameter message into a struct
 *
 * @param msg The message to decode
 * @param work_parameter C-struct to decode the message contents into
 */
static inline void mavlink_msg_work_parameter_decode(const mavlink_message_t* msg, mavlink_work_parameter_t* work_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    work_parameter->rolloff_factor = mavlink_msg_work_parameter_get_rolloff_factor(msg);
    work_parameter->manual_longitude = mavlink_msg_work_parameter_get_manual_longitude(msg);
    work_parameter->manual_latitude = mavlink_msg_work_parameter_get_manual_latitude(msg);
    work_parameter->manual_height = mavlink_msg_work_parameter_get_manual_height(msg);
    work_parameter->openamip_port = mavlink_msg_work_parameter_get_openamip_port(msg);
    work_parameter->servicer_port = mavlink_msg_work_parameter_get_servicer_port(msg);
    work_parameter->work_mode = mavlink_msg_work_parameter_get_work_mode(msg);
    mavlink_msg_work_parameter_get_openamip_ip(msg, work_parameter->openamip_ip);
    mavlink_msg_work_parameter_get_openamip_mask(msg, work_parameter->openamip_mask);
    mavlink_msg_work_parameter_get_servicer_ip(msg, work_parameter->servicer_ip);
    mavlink_msg_work_parameter_get_servicer_mask(msg, work_parameter->servicer_mask);
    work_parameter->tracking_mode = mavlink_msg_work_parameter_get_tracking_mode(msg);
    work_parameter->manual_gps = mavlink_msg_work_parameter_get_manual_gps(msg);
    work_parameter->issue_mode = mavlink_msg_work_parameter_get_issue_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WORK_PARAMETER_LEN? msg->len : MAVLINK_MSG_ID_WORK_PARAMETER_LEN;
        memset(work_parameter, 0, MAVLINK_MSG_ID_WORK_PARAMETER_LEN);
    memcpy(work_parameter, _MAV_PAYLOAD(msg), len);
#endif
}
