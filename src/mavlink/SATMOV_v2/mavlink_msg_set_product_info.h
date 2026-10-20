#pragma once
// MESSAGE SET_PRODUCT_INFO PACKING

#define MAVLINK_MSG_ID_SET_PRODUCT_INFO 32


typedef struct __mavlink_set_product_info_t {
 char antenna_name[32]; /*<  天线名称*/
 char antenna_orb[32]; /*<  天线序列号*/
} mavlink_set_product_info_t;

#define MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN 64
#define MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN 64
#define MAVLINK_MSG_ID_32_LEN 64
#define MAVLINK_MSG_ID_32_MIN_LEN 64

#define MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC 235
#define MAVLINK_MSG_ID_32_CRC 235

#define MAVLINK_MSG_SET_PRODUCT_INFO_FIELD_ANTENNA_NAME_LEN 32
#define MAVLINK_MSG_SET_PRODUCT_INFO_FIELD_ANTENNA_ORB_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_PRODUCT_INFO { \
    32, \
    "SET_PRODUCT_INFO", \
    2, \
    {  { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 0, offsetof(mavlink_set_product_info_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 32, offsetof(mavlink_set_product_info_t, antenna_orb) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_PRODUCT_INFO { \
    "SET_PRODUCT_INFO", \
    2, \
    {  { "antenna_name", NULL, MAVLINK_TYPE_CHAR, 32, 0, offsetof(mavlink_set_product_info_t, antenna_name) }, \
         { "antenna_orb", NULL, MAVLINK_TYPE_CHAR, 32, 32, offsetof(mavlink_set_product_info_t, antenna_orb) }, \
         } \
}
#endif

/**
 * @brief Pack a set_product_info message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_product_info_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *antenna_name, const char *antenna_orb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN];

    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN);
#else
    mavlink_set_product_info_t packet;

    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_PRODUCT_INFO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
}

/**
 * @brief Pack a set_product_info message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_product_info_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *antenna_name,const char *antenna_orb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN];

    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN);
#else
    mavlink_set_product_info_t packet;

    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_PRODUCT_INFO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
}

/**
 * @brief Encode a set_product_info struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_product_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_product_info_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_product_info_t* set_product_info)
{
    return mavlink_msg_set_product_info_pack(system_id, component_id, msg, set_product_info->antenna_name, set_product_info->antenna_orb);
}

/**
 * @brief Encode a set_product_info struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_product_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_product_info_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_product_info_t* set_product_info)
{
    return mavlink_msg_set_product_info_pack_chan(system_id, component_id, chan, msg, set_product_info->antenna_name, set_product_info->antenna_orb);
}

/**
 * @brief Send a set_product_info message
 * @param chan MAVLink channel to send the message
 *
 * @param antenna_name  天线名称
 * @param antenna_orb  天线序列号
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_product_info_send(mavlink_channel_t chan, const char *antenna_name, const char *antenna_orb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN];

    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO, buf, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
#else
    mavlink_set_product_info_t packet;

    mav_array_memcpy(packet.antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet.antenna_orb, antenna_orb, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO, (const char *)&packet, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
#endif
}

/**
 * @brief Send a set_product_info message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_product_info_send_struct(mavlink_channel_t chan, const mavlink_set_product_info_t* set_product_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_product_info_send(chan, set_product_info->antenna_name, set_product_info->antenna_orb);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO, (const char *)set_product_info, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_product_info_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *antenna_name, const char *antenna_orb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_char_array(buf, 0, antenna_name, 32);
    _mav_put_char_array(buf, 32, antenna_orb, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO, buf, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
#else
    mavlink_set_product_info_t *packet = (mavlink_set_product_info_t *)msgbuf;

    mav_array_memcpy(packet->antenna_name, antenna_name, sizeof(char)*32);
    mav_array_memcpy(packet->antenna_orb, antenna_orb, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_PRODUCT_INFO, (const char *)packet, MAVLINK_MSG_ID_SET_PRODUCT_INFO_MIN_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN, MAVLINK_MSG_ID_SET_PRODUCT_INFO_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_PRODUCT_INFO UNPACKING


/**
 * @brief Get field antenna_name from set_product_info message
 *
 * @return  天线名称
 */
static inline uint16_t mavlink_msg_set_product_info_get_antenna_name(const mavlink_message_t* msg, char *antenna_name)
{
    return _MAV_RETURN_char_array(msg, antenna_name, 32,  0);
}

/**
 * @brief Get field antenna_orb from set_product_info message
 *
 * @return  天线序列号
 */
static inline uint16_t mavlink_msg_set_product_info_get_antenna_orb(const mavlink_message_t* msg, char *antenna_orb)
{
    return _MAV_RETURN_char_array(msg, antenna_orb, 32,  32);
}

/**
 * @brief Decode a set_product_info message into a struct
 *
 * @param msg The message to decode
 * @param set_product_info C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_product_info_decode(const mavlink_message_t* msg, mavlink_set_product_info_t* set_product_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_product_info_get_antenna_name(msg, set_product_info->antenna_name);
    mavlink_msg_set_product_info_get_antenna_orb(msg, set_product_info->antenna_orb);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN? msg->len : MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN;
        memset(set_product_info, 0, MAVLINK_MSG_ID_SET_PRODUCT_INFO_LEN);
    memcpy(set_product_info, _MAV_PAYLOAD(msg), len);
#endif
}
