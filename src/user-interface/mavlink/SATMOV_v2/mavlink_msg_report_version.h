#pragma once
// MESSAGE REPORT_VERSION PACKING

#define MAVLINK_MSG_ID_REPORT_VERSION 13

MAVPACKED(
typedef struct __mavlink_report_version_t {
 uint8_t result; /*<  0x00读取成功，0xfe无此设备或此设备不支持版本查询，0xff读取失败*/
 char antenna_name[32]; /*<  天线名称*/
 char antenna_orb[32]; /*<  天线序列号*/
 char software_data[32]; /*<  软件发布日期*/
 char software_version[32]; /*<  软件版本*/
 uint8_t software_type; /*<  软件类型，0x01APP，0x02多模机，0x03变频器，0x04IMU，0x05接收天线，0x06发射天线，0x07高轨固件（闭环跟踪），0x08低轨固件（开环跟踪），0x09暗室测试*/
}) mavlink_report_version_t;

#define MAVLINK_MSG_ID_REPORT_VERSION_LEN 130
#define MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN 130
#define MAVLINK_MSG_ID_13_LEN 130
#define MAVLINK_MSG_ID_13_MIN_LEN 130

#define MAVLINK_MSG_ID_REPORT_VERSION_CRC 78
#define MAVLINK_MSG_ID_13_CRC 78

#define MAVLINK_MSG_REPORT_VERSION_FIELD_ANTENNA_NAME_LEN 32
#define MAVLINK_MSG_REPORT_VERSION_FIELD_ANTENNA_ORB_LEN 32
#define MAVLINK_MSG_REPORT_VERSION_FIELD_SOFTWARE_DATA_LEN 32
#define MAVLINK_MSG_REPORT_VERSION_FIELD_SOFTWARE_VERSION_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_VERSION { \
    13, \
    "REPORT_VERSION", \
    6, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_report_version_t, result) }, \
         { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 1, offsetof(mavlink_report_version_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 33, offsetof(mavlink_report_version_t, antenna_orb) }, \
         { "software_data", NULL, MAVLINK_TYPE_CHAR, 32, 65, offsetof(mavlink_report_version_t, software_data) }, \
         { "software_version", NULL, MAVLINK_TYPE_CHAR, 32, 97, offsetof(mavlink_report_version_t, software_version) }, \
         { "software_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 129, offsetof(mavlink_report_version_t, software_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_VERSION { \
    "REPORT_VERSION", \
    6, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_report_version_t, result) }, \
         { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 1, offsetof(mavlink_report_version_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 33, offsetof(mavlink_report_version_t, antenna_orb) }, \
         { "software_data", NULL, MAVLINK_TYPE_CHAR, 32, 65, offsetof(mavlink_report_version_t, software_data) }, \
         { "software_version", NULL, MAVLINK_TYPE_CHAR, 32, 97, offsetof(mavlink_report_version_t, software_version) }, \
         { "software_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 129, offsetof(mavlink_report_version_t, software_type) }, \
         } \
}
#endif

/**
 * @brief Pack a report_version message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param result  0x00读取成功，0xfe无此设备或此设备不支持版本查询，0xff读取失败
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param software_data  软件发布日期
 * @param software_version  软件版本
 * @param software_type  软件类型，0x01APP，0x02多模机，0x03变频器，0x04IMU，0x05接收天线，0x06发射天线，0x07高轨固件（闭环跟踪），0x08低轨固件（开环跟踪），0x09暗室测试
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_version_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t result, const char *antenna_name, const char *antenna_orb, const char *software_data, const char *software_version, uint8_t software_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_VERSION_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_uint8_t(buf, 129, software_type);
    _mav_put_char_array(buf, 1, antenna_name, 32);
    _mav_put_char_array(buf, 33, antenna_orb, 32);
    _mav_put_char_array(buf, 65, software_data, 32);
    _mav_put_char_array(buf, 97, software_version, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_VERSION_LEN);
#else
    mavlink_report_version_t packet;
    packet.result = result;
    packet.software_type = software_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.software_data, software_data, sizeof(char)*32);
    mav_array_memcpy(packet.software_version, software_version, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_VERSION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_VERSION;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
}

/**
 * @brief Pack a report_version message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param result  0x00读取成功，0xfe无此设备或此设备不支持版本查询，0xff读取失败
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param software_data  软件发布日期
 * @param software_version  软件版本
 * @param software_type  软件类型，0x01APP，0x02多模机，0x03变频器，0x04IMU，0x05接收天线，0x06发射天线，0x07高轨固件（闭环跟踪），0x08低轨固件（开环跟踪），0x09暗室测试
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_version_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t result,const char *antenna_name,const char *antenna_orb,const char *software_data,const char *software_version,uint8_t software_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_VERSION_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_uint8_t(buf, 129, software_type);
    _mav_put_char_array(buf, 1, antenna_name, 32);
    _mav_put_char_array(buf, 33, antenna_orb, 32);
    _mav_put_char_array(buf, 65, software_data, 32);
    _mav_put_char_array(buf, 97, software_version, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_VERSION_LEN);
#else
    mavlink_report_version_t packet;
    packet.result = result;
    packet.software_type = software_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.software_data, software_data, sizeof(char)*32);
    mav_array_memcpy(packet.software_version, software_version, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_VERSION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_VERSION;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
}

/**
 * @brief Encode a report_version struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_version C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_version_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_version_t* report_version)
{
    return mavlink_msg_report_version_pack(system_id, component_id, msg, report_version->result, report_version->antenna_name, report_version->antenna_orb, report_version->software_data, report_version->software_version, report_version->software_type);
}

/**
 * @brief Encode a report_version struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_version C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_version_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_version_t* report_version)
{
    return mavlink_msg_report_version_pack_chan(system_id, component_id, chan, msg, report_version->result, report_version->antenna_name, report_version->antenna_orb, report_version->software_data, report_version->software_version, report_version->software_type);
}

/**
 * @brief Send a report_version message
 * @param chan MAVLink channel to send the message
 *
 * @param result  0x00读取成功，0xfe无此设备或此设备不支持版本查询，0xff读取失败
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param software_data  软件发布日期
 * @param software_version  软件版本
 * @param software_type  软件类型，0x01APP，0x02多模机，0x03变频器，0x04IMU，0x05接收天线，0x06发射天线，0x07高轨固件（闭环跟踪），0x08低轨固件（开环跟踪），0x09暗室测试
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_version_send(mavlink_channel_t chan, uint8_t result, const char *antenna_name, const char *antenna_orb, const char *software_data, const char *software_version, uint8_t software_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_VERSION_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_uint8_t(buf, 129, software_type);
    _mav_put_char_array(buf, 1, antenna_name, 32);
    _mav_put_char_array(buf, 33, antenna_orb, 32);
    _mav_put_char_array(buf, 65, software_data, 32);
    _mav_put_char_array(buf, 97, software_version, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_VERSION, buf, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
#else
    mavlink_report_version_t packet;
    packet.result = result;
    packet.software_type = software_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.software_data, software_data, sizeof(char)*32);
    mav_array_memcpy(packet.software_version, software_version, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_VERSION, (const char *)&packet, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
#endif
}

/**
 * @brief Send a report_version message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_version_send_struct(mavlink_channel_t chan, const mavlink_report_version_t* report_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_version_send(chan, report_version->result, report_version->antenna_name, report_version->antenna_orb, report_version->software_data, report_version->software_version, report_version->software_type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_VERSION, (const char *)report_version, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_VERSION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_version_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t result, const char *antenna_name, const char *antenna_orb, const char *software_data, const char *software_version, uint8_t software_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_uint8_t(buf, 129, software_type);
    _mav_put_char_array(buf, 1, antenna_name, 32);
    _mav_put_char_array(buf, 33, antenna_orb, 32);
    _mav_put_char_array(buf, 65, software_data, 32);
    _mav_put_char_array(buf, 97, software_version, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_VERSION, buf, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
#else
    mavlink_report_version_t *packet = (mavlink_report_version_t *)msgbuf;
    packet->result = result;
    packet->software_type = software_type;
    mav_array_memcpy(packet->antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet->antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet->software_data, software_data, sizeof(char)*32);
    mav_array_memcpy(packet->software_version, software_version, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_VERSION, (const char *)packet, MAVLINK_MSG_ID_REPORT_VERSION_MIN_LEN, MAVLINK_MSG_ID_REPORT_VERSION_LEN, MAVLINK_MSG_ID_REPORT_VERSION_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_VERSION UNPACKING


/**
 * @brief Get field result from report_version message
 *
 * @return  0x00读取成功，0xfe无此设备或此设备不支持版本查询，0xff读取失败
 */
static inline uint8_t mavlink_msg_report_version_get_result(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field antenna_name from report_version message
 *
 * @return  天线名称
 */
static inline uint16_t mavlink_msg_report_version_get_antenna_name(const mavlink_message_t* msg, char *antenna_name)
{
    return _MAV_RETURN_char_array(msg, antenna_name, 32,  1);
}

/**
 * @brief Get field antenna_orb from report_version message
 *
 * @return  天线序列号
 */
static inline uint16_t mavlink_msg_report_version_get_antenna_orb(const mavlink_message_t* msg, char *antenna_orb)
{
    return _MAV_RETURN_char_array(msg, antenna_orb, 32,  33);
}

/**
 * @brief Get field software_data from report_version message
 *
 * @return  软件发布日期
 */
static inline uint16_t mavlink_msg_report_version_get_software_data(const mavlink_message_t* msg, char *software_data)
{
    return _MAV_RETURN_char_array(msg, software_data, 32,  65);
}

/**
 * @brief Get field software_version from report_version message
 *
 * @return  软件版本
 */
static inline uint16_t mavlink_msg_report_version_get_software_version(const mavlink_message_t* msg, char *software_version)
{
    return _MAV_RETURN_char_array(msg, software_version, 32,  97);
}

/**
 * @brief Get field software_type from report_version message
 *
 * @return  软件类型，0x01APP，0x02多模机，0x03变频器，0x04IMU，0x05接收天线，0x06发射天线，0x07高轨固件（闭环跟踪），0x08低轨固件（开环跟踪），0x09暗室测试
 */
static inline uint8_t mavlink_msg_report_version_get_software_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  129);
}

/**
 * @brief Decode a report_version message into a struct
 *
 * @param msg The message to decode
 * @param report_version C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_version_decode(const mavlink_message_t* msg, mavlink_report_version_t* report_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_version->result = mavlink_msg_report_version_get_result(msg);
    mavlink_msg_report_version_get_antenna_name(msg, report_version->antenna_name);
    mavlink_msg_report_version_get_antenna_orb(msg, report_version->antenna_orb);
    mavlink_msg_report_version_get_software_data(msg, report_version->software_data);
    mavlink_msg_report_version_get_software_version(msg, report_version->software_version);
    report_version->software_type = mavlink_msg_report_version_get_software_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_VERSION_LEN? msg->len : MAVLINK_MSG_ID_REPORT_VERSION_LEN;
        memset(report_version, 0, MAVLINK_MSG_ID_REPORT_VERSION_LEN);
    memcpy(report_version, _MAV_PAYLOAD(msg), len);
#endif
}
