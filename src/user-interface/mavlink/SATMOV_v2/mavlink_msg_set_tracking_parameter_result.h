#pragma once
// MESSAGE SET_TRACKING_PARAMETER_RESULT PACKING

#define MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT 45

MAVPACKED(
typedef struct __mavlink_set_tracking_parameter_result_t {
 uint8_t result; /*<  配置结果，0x00成功，0xff失败*/
}) mavlink_set_tracking_parameter_result_t;

#define MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN 1
#define MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN 1
#define MAVLINK_MSG_ID_45_LEN 1
#define MAVLINK_MSG_ID_45_MIN_LEN 1

#define MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC 174
#define MAVLINK_MSG_ID_45_CRC 174



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_TRACKING_PARAMETER_RESULT { \
    45, \
    "SET_TRACKING_PARAMETER_RESULT", \
    1, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_set_tracking_parameter_result_t, result) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_TRACKING_PARAMETER_RESULT { \
    "SET_TRACKING_PARAMETER_RESULT", \
    1, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_set_tracking_parameter_result_t, result) }, \
         } \
}
#endif

/**
 * @brief Pack a set_tracking_parameter_result message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param result  配置结果，0x00成功，0xff失败
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_tracking_parameter_result_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN);
#else
    mavlink_set_tracking_parameter_result_t packet;
    packet.result = result;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
}

/**
 * @brief Pack a set_tracking_parameter_result message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param result  配置结果，0x00成功，0xff失败
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_tracking_parameter_result_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN);
#else
    mavlink_set_tracking_parameter_result_t packet;
    packet.result = result;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
}

/**
 * @brief Encode a set_tracking_parameter_result struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_tracking_parameter_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_tracking_parameter_result_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_tracking_parameter_result_t* set_tracking_parameter_result)
{
    return mavlink_msg_set_tracking_parameter_result_pack(system_id, component_id, msg, set_tracking_parameter_result->result);
}

/**
 * @brief Encode a set_tracking_parameter_result struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_tracking_parameter_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_tracking_parameter_result_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_tracking_parameter_result_t* set_tracking_parameter_result)
{
    return mavlink_msg_set_tracking_parameter_result_pack_chan(system_id, component_id, chan, msg, set_tracking_parameter_result->result);
}

/**
 * @brief Send a set_tracking_parameter_result message
 * @param chan MAVLink channel to send the message
 *
 * @param result  配置结果，0x00成功，0xff失败
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_tracking_parameter_result_send(mavlink_channel_t chan, uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT, buf, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
#else
    mavlink_set_tracking_parameter_result_t packet;
    packet.result = result;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT, (const char *)&packet, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
#endif
}

/**
 * @brief Send a set_tracking_parameter_result message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_tracking_parameter_result_send_struct(mavlink_channel_t chan, const mavlink_set_tracking_parameter_result_t* set_tracking_parameter_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_tracking_parameter_result_send(chan, set_tracking_parameter_result->result);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT, (const char *)set_tracking_parameter_result, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_tracking_parameter_result_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, result);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT, buf, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
#else
    mavlink_set_tracking_parameter_result_t *packet = (mavlink_set_tracking_parameter_result_t *)msgbuf;
    packet->result = result;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT, (const char *)packet, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_MIN_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_TRACKING_PARAMETER_RESULT UNPACKING


/**
 * @brief Get field result from set_tracking_parameter_result message
 *
 * @return  配置结果，0x00成功，0xff失败
 */
static inline uint8_t mavlink_msg_set_tracking_parameter_result_get_result(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a set_tracking_parameter_result message into a struct
 *
 * @param msg The message to decode
 * @param set_tracking_parameter_result C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_tracking_parameter_result_decode(const mavlink_message_t* msg, mavlink_set_tracking_parameter_result_t* set_tracking_parameter_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_tracking_parameter_result->result = mavlink_msg_set_tracking_parameter_result_get_result(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN? msg->len : MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN;
        memset(set_tracking_parameter_result, 0, MAVLINK_MSG_ID_SET_TRACKING_PARAMETER_RESULT_LEN);
    memcpy(set_tracking_parameter_result, _MAV_PAYLOAD(msg), len);
#endif
}
