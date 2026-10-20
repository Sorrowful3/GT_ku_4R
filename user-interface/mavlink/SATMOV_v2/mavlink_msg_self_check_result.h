#pragma once
// MESSAGE SELF_CHECK_RESULT PACKING

#define MAVLINK_MSG_ID_SELF_CHECK_RESULT 25

MAVPACKED(
typedef struct __mavlink_self_check_result_t {
 uint8_t result; /*<  自检结果，0x00成功，0xfe无此设备，0xff失败*/
 char describe[200]; /*<  自检描述*/
}) mavlink_self_check_result_t;

#define MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN 201
#define MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN 201
#define MAVLINK_MSG_ID_25_LEN 201
#define MAVLINK_MSG_ID_25_MIN_LEN 201

#define MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC 240
#define MAVLINK_MSG_ID_25_CRC 240

#define MAVLINK_MSG_SELF_CHECK_RESULT_FIELD_DESCRIBE_LEN 200

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SELF_CHECK_RESULT { \
    25, \
    "SELF_CHECK_RESULT", \
    2, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_self_check_result_t, result) }, \
         { "describe", NULL, MAVLINK_TYPE_CHAR, 200, 1, offsetof(mavlink_self_check_result_t, describe) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SELF_CHECK_RESULT { \
    "SELF_CHECK_RESULT", \
    2, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_self_check_result_t, result) }, \
         { "describe", NULL, MAVLINK_TYPE_CHAR, 200, 1, offsetof(mavlink_self_check_result_t, describe) }, \
         } \
}
#endif

/**
 * @brief Pack a self_check_result message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param result  自检结果，0x00成功，0xfe无此设备，0xff失败
 * @param describe  自检描述
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_self_check_result_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t result, const char *describe)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_char_array(buf, 1, describe, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN);
#else
    mavlink_self_check_result_t packet;
    packet.result = result;
    mav_array_memcpy(packet.describe, describe, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SELF_CHECK_RESULT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
}

/**
 * @brief Pack a self_check_result message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param result  自检结果，0x00成功，0xfe无此设备，0xff失败
 * @param describe  自检描述
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_self_check_result_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t result,const char *describe)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_char_array(buf, 1, describe, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN);
#else
    mavlink_self_check_result_t packet;
    packet.result = result;
    mav_array_memcpy(packet.describe, describe, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SELF_CHECK_RESULT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
}

/**
 * @brief Encode a self_check_result struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param self_check_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_self_check_result_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_self_check_result_t* self_check_result)
{
    return mavlink_msg_self_check_result_pack(system_id, component_id, msg, self_check_result->result, self_check_result->describe);
}

/**
 * @brief Encode a self_check_result struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param self_check_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_self_check_result_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_self_check_result_t* self_check_result)
{
    return mavlink_msg_self_check_result_pack_chan(system_id, component_id, chan, msg, self_check_result->result, self_check_result->describe);
}

/**
 * @brief Send a self_check_result message
 * @param chan MAVLink channel to send the message
 *
 * @param result  自检结果，0x00成功，0xfe无此设备，0xff失败
 * @param describe  自检描述
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_self_check_result_send(mavlink_channel_t chan, uint8_t result, const char *describe)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_char_array(buf, 1, describe, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT, buf, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
#else
    mavlink_self_check_result_t packet;
    packet.result = result;
    mav_array_memcpy(packet.describe, describe, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT, (const char *)&packet, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
#endif
}

/**
 * @brief Send a self_check_result message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_self_check_result_send_struct(mavlink_channel_t chan, const mavlink_self_check_result_t* self_check_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_self_check_result_send(chan, self_check_result->result, self_check_result->describe);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT, (const char *)self_check_result, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_self_check_result_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t result, const char *describe)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, result);
    _mav_put_char_array(buf, 1, describe, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT, buf, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
#else
    mavlink_self_check_result_t *packet = (mavlink_self_check_result_t *)msgbuf;
    packet->result = result;
    mav_array_memcpy(packet->describe, describe, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK_RESULT, (const char *)packet, MAVLINK_MSG_ID_SELF_CHECK_RESULT_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN, MAVLINK_MSG_ID_SELF_CHECK_RESULT_CRC);
#endif
}
#endif

#endif

// MESSAGE SELF_CHECK_RESULT UNPACKING


/**
 * @brief Get field result from self_check_result message
 *
 * @return  自检结果，0x00成功，0xfe无此设备，0xff失败
 */
static inline uint8_t mavlink_msg_self_check_result_get_result(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field describe from self_check_result message
 *
 * @return  自检描述
 */
static inline uint16_t mavlink_msg_self_check_result_get_describe(const mavlink_message_t* msg, char *describe)
{
    return _MAV_RETURN_char_array(msg, describe, 200,  1);
}

/**
 * @brief Decode a self_check_result message into a struct
 *
 * @param msg The message to decode
 * @param self_check_result C-struct to decode the message contents into
 */
static inline void mavlink_msg_self_check_result_decode(const mavlink_message_t* msg, mavlink_self_check_result_t* self_check_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    self_check_result->result = mavlink_msg_self_check_result_get_result(msg);
    mavlink_msg_self_check_result_get_describe(msg, self_check_result->describe);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN? msg->len : MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN;
        memset(self_check_result, 0, MAVLINK_MSG_ID_SELF_CHECK_RESULT_LEN);
    memcpy(self_check_result, _MAV_PAYLOAD(msg), len);
#endif
}
