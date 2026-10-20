#pragma once
// MESSAGE FIRMWARE_REDAY PACKING

#define MAVLINK_MSG_ID_FIRMWARE_REDAY 1


typedef struct __mavlink_firmware_reday_t {
 char antenna_name[32]; /*<  天线名称*/
 char antenna_orb[32]; /*<  天线序列号*/
 char firmware_data[32]; /*<  固件发布日期*/
 char firmware_version[32]; /*<  固件版本*/
 uint8_t firmware_type; /*<  固件类型，0x07高轨固件（闭环跟踪），0x08低轨，0x09暗室*/
} mavlink_firmware_reday_t;

#define MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN 129
#define MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN 129
#define MAVLINK_MSG_ID_1_LEN 129
#define MAVLINK_MSG_ID_1_MIN_LEN 129

#define MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC 163
#define MAVLINK_MSG_ID_1_CRC 163

#define MAVLINK_MSG_FIRMWARE_REDAY_FIELD_ANTENNA_NAME_LEN 32
#define MAVLINK_MSG_FIRMWARE_REDAY_FIELD_ANTENNA_ORB_LEN 32
#define MAVLINK_MSG_FIRMWARE_REDAY_FIELD_FIRMWARE_DATA_LEN 32
#define MAVLINK_MSG_FIRMWARE_REDAY_FIELD_FIRMWARE_VERSION_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FIRMWARE_REDAY { \
    1, \
    "FIRMWARE_REDAY", \
    5, \
    {  { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 0, offsetof(mavlink_firmware_reday_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 32, offsetof(mavlink_firmware_reday_t, antenna_orb) }, \
         { "firmware_data", NULL, MAVLINK_TYPE_CHAR, 32, 64, offsetof(mavlink_firmware_reday_t, firmware_data) }, \
         { "firmware_version", NULL, MAVLINK_TYPE_CHAR, 32, 96, offsetof(mavlink_firmware_reday_t, firmware_version) }, \
         { "firmware_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 128, offsetof(mavlink_firmware_reday_t, firmware_type) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FIRMWARE_REDAY { \
    "FIRMWARE_REDAY", \
    5, \
    {  { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 0, offsetof(mavlink_firmware_reday_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 32, offsetof(mavlink_firmware_reday_t, antenna_orb) }, \
         { "firmware_data", NULL, MAVLINK_TYPE_CHAR, 32, 64, offsetof(mavlink_firmware_reday_t, firmware_data) }, \
         { "firmware_version", NULL, MAVLINK_TYPE_CHAR, 32, 96, offsetof(mavlink_firmware_reday_t, firmware_version) }, \
         { "firmware_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 128, offsetof(mavlink_firmware_reday_t, firmware_type) }, \
         } \
}
#endif

/**
 * @brief Pack a firmware_reday message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param firmware_data  固件发布日期
 * @param firmware_version  固件版本
 * @param firmware_type  固件类型，0x07高轨固件（闭环跟踪），0x08低轨，0x09暗室
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_firmware_reday_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *antenna_name, const char *antenna_orb, const char *firmware_data, const char *firmware_version, uint8_t firmware_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN];
    _mav_put_uint8_t(buf, 128, firmware_type);
    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_put_char_array(buf, 64, firmware_data, 32);
    _mav_put_char_array(buf, 96, firmware_version, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN);
#else
    mavlink_firmware_reday_t packet;
    packet.firmware_type = firmware_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_data, firmware_data, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_version, firmware_version, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FIRMWARE_REDAY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
}

/**
 * @brief Pack a firmware_reday message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param firmware_data  固件发布日期
 * @param firmware_version  固件版本
 * @param firmware_type  固件类型，0x07高轨固件（闭环跟踪），0x08低轨，0x09暗室
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_firmware_reday_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *antenna_name,const char *antenna_orb,const char *firmware_data,const char *firmware_version,uint8_t firmware_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN];
    _mav_put_uint8_t(buf, 128, firmware_type);
    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_put_char_array(buf, 64, firmware_data, 32);
    _mav_put_char_array(buf, 96, firmware_version, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN);
#else
    mavlink_firmware_reday_t packet;
    packet.firmware_type = firmware_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_data, firmware_data, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_version, firmware_version, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FIRMWARE_REDAY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
}

/**
 * @brief Encode a firmware_reday struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param firmware_reday C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_firmware_reday_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_firmware_reday_t* firmware_reday)
{
    return mavlink_msg_firmware_reday_pack(system_id, component_id, msg, firmware_reday->antenna_name, firmware_reday->antenna_orb, firmware_reday->firmware_data, firmware_reday->firmware_version, firmware_reday->firmware_type);
}

/**
 * @brief Encode a firmware_reday struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param firmware_reday C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_firmware_reday_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_firmware_reday_t* firmware_reday)
{
    return mavlink_msg_firmware_reday_pack_chan(system_id, component_id, chan, msg, firmware_reday->antenna_name, firmware_reday->antenna_orb, firmware_reday->firmware_data, firmware_reday->firmware_version, firmware_reday->firmware_type);
}

/**
 * @brief Send a firmware_reday message
 * @param chan MAVLink channel to send the message
 *
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @param firmware_data  固件发布日期
 * @param firmware_version  固件版本
 * @param firmware_type  固件类型，0x07高轨固件（闭环跟踪），0x08低轨，0x09暗室
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_firmware_reday_send(mavlink_channel_t chan, const char *antenna_name, const char *antenna_orb, const char *firmware_data, const char *firmware_version, uint8_t firmware_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN];
    _mav_put_uint8_t(buf, 128, firmware_type);
    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_put_char_array(buf, 64, firmware_data, 32);
    _mav_put_char_array(buf, 96, firmware_version, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRMWARE_REDAY, buf, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
#else
    mavlink_firmware_reday_t packet;
    packet.firmware_type = firmware_type;
    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_data, firmware_data, sizeof(char)*32);
    mav_array_memcpy(packet.firmware_version, firmware_version, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRMWARE_REDAY, (const char *)&packet, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
#endif
}

/**
 * @brief Send a firmware_reday message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_firmware_reday_send_struct(mavlink_channel_t chan, const mavlink_firmware_reday_t* firmware_reday)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_firmware_reday_send(chan, firmware_reday->antenna_name, firmware_reday->antenna_orb, firmware_reday->firmware_data, firmware_reday->firmware_version, firmware_reday->firmware_type);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRMWARE_REDAY, (const char *)firmware_reday, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
#endif
}

#if MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_firmware_reday_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *antenna_name, const char *antenna_orb, const char *firmware_data, const char *firmware_version, uint8_t firmware_type)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 128, firmware_type);
    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_put_char_array(buf, 64, firmware_data, 32);
    _mav_put_char_array(buf, 96, firmware_version, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRMWARE_REDAY, buf, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
#else
    mavlink_firmware_reday_t *packet = (mavlink_firmware_reday_t *)msgbuf;
    packet->firmware_type = firmware_type;
    mav_array_memcpy(packet->antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet->antenna_orb, antenna_orb, sizeof(char)*32);
    mav_array_memcpy(packet->firmware_data, firmware_data, sizeof(char)*32);
    mav_array_memcpy(packet->firmware_version, firmware_version, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRMWARE_REDAY, (const char *)packet, MAVLINK_MSG_ID_FIRMWARE_REDAY_MIN_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN, MAVLINK_MSG_ID_FIRMWARE_REDAY_CRC);
#endif
}
#endif

#endif

// MESSAGE FIRMWARE_REDAY UNPACKING


/**
 * @brief Get field antenna_name from firmware_reday message
 *
 * @return  天线名称
 */
static inline uint16_t mavlink_msg_firmware_reday_get_antenna_name(const mavlink_message_t* msg, char *antenna_name)
{
    return _MAV_RETURN_char_array(msg, antenna_name, 32,  0);
}

/**
 * @brief Get field antenna_orb from firmware_reday message
 *
 * @return  天线序列号
 */
static inline uint16_t mavlink_msg_firmware_reday_get_antenna_orb(const mavlink_message_t* msg, char *antenna_orb)
{
    return _MAV_RETURN_char_array(msg, antenna_orb, 32,  32);
}

/**
 * @brief Get field firmware_data from firmware_reday message
 *
 * @return  固件发布日期
 */
static inline uint16_t mavlink_msg_firmware_reday_get_firmware_data(const mavlink_message_t* msg, char *firmware_data)
{
    return _MAV_RETURN_char_array(msg, firmware_data, 32,  64);
}

/**
 * @brief Get field firmware_version from firmware_reday message
 *
 * @return  固件版本
 */
static inline uint16_t mavlink_msg_firmware_reday_get_firmware_version(const mavlink_message_t* msg, char *firmware_version)
{
    return _MAV_RETURN_char_array(msg, firmware_version, 32,  96);
}

/**
 * @brief Get field firmware_type from firmware_reday message
 *
 * @return  固件类型，0x07高轨固件（闭环跟踪），0x08低轨，0x09暗室
 */
static inline uint8_t mavlink_msg_firmware_reday_get_firmware_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  128);
}

/**
 * @brief Decode a firmware_reday message into a struct
 *
 * @param msg The message to decode
 * @param firmware_reday C-struct to decode the message contents into
 */
static inline void mavlink_msg_firmware_reday_decode(const mavlink_message_t* msg, mavlink_firmware_reday_t* firmware_reday)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_firmware_reday_get_antenna_name(msg, firmware_reday->antenna_name);
    mavlink_msg_firmware_reday_get_antenna_orb(msg, firmware_reday->antenna_orb);
    mavlink_msg_firmware_reday_get_firmware_data(msg, firmware_reday->firmware_data);
    mavlink_msg_firmware_reday_get_firmware_version(msg, firmware_reday->firmware_version);
    firmware_reday->firmware_type = mavlink_msg_firmware_reday_get_firmware_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN? msg->len : MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN;
        memset(firmware_reday, 0, MAVLINK_MSG_ID_FIRMWARE_REDAY_LEN);
    memcpy(firmware_reday, _MAV_PAYLOAD(msg), len);
#endif
}
