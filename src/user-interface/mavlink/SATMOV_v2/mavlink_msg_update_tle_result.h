#pragma once
// MESSAGE UPDATE_TLE_RESULT PACKING

#define MAVLINK_MSG_ID_UPDATE_TLE_RESULT 50

MAVPACKED(
typedef struct __mavlink_update_tle_result_t {
 uint8_t result; /*<  0x00更新成功，0xff更新失败*/
}) mavlink_update_tle_result_t;

#define MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN 1
#define MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN 1
#define MAVLINK_MSG_ID_50_LEN 1
#define MAVLINK_MSG_ID_50_MIN_LEN 1

#define MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC 167
#define MAVLINK_MSG_ID_50_CRC 167



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_UPDATE_TLE_RESULT { \
    50, \
    "UPDATE_TLE_RESULT", \
    1, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_update_tle_result_t, result) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_UPDATE_TLE_RESULT { \
    "UPDATE_TLE_RESULT", \
    1, \
    {  { "result", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_update_tle_result_t, result) }, \
         } \
}
#endif

/**
 * @brief Pack a update_tle_result message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param result  0x00更新成功，0xff更新失败
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_tle_result_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN);
#else
    mavlink_update_tle_result_t packet;
    packet.result = result;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_TLE_RESULT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
}

/**
 * @brief Pack a update_tle_result message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param result  0x00更新成功，0xff更新失败
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_tle_result_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN);
#else
    mavlink_update_tle_result_t packet;
    packet.result = result;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_TLE_RESULT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
}

/**
 * @brief Encode a update_tle_result struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param update_tle_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_tle_result_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_update_tle_result_t* update_tle_result)
{
    return mavlink_msg_update_tle_result_pack(system_id, component_id, msg, update_tle_result->result);
}

/**
 * @brief Encode a update_tle_result struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param update_tle_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_tle_result_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_update_tle_result_t* update_tle_result)
{
    return mavlink_msg_update_tle_result_pack_chan(system_id, component_id, chan, msg, update_tle_result->result);
}

/**
 * @brief Send a update_tle_result message
 * @param chan MAVLink channel to send the message
 *
 * @param result  0x00更新成功，0xff更新失败
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_update_tle_result_send(mavlink_channel_t chan, uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN];
    _mav_put_uint8_t(buf, 0, result);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT, buf, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
#else
    mavlink_update_tle_result_t packet;
    packet.result = result;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT, (const char *)&packet, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
#endif
}

/**
 * @brief Send a update_tle_result message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_update_tle_result_send_struct(mavlink_channel_t chan, const mavlink_update_tle_result_t* update_tle_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_update_tle_result_send(chan, update_tle_result->result);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT, (const char *)update_tle_result, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
#endif
}

#if MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_update_tle_result_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, result);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT, buf, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
#else
    mavlink_update_tle_result_t *packet = (mavlink_update_tle_result_t *)msgbuf;
    packet->result = result;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE_RESULT, (const char *)packet, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_CRC);
#endif
}
#endif

#endif

// MESSAGE UPDATE_TLE_RESULT UNPACKING


/**
 * @brief Get field result from update_tle_result message
 *
 * @return  0x00更新成功，0xff更新失败
 */
static inline uint8_t mavlink_msg_update_tle_result_get_result(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a update_tle_result message into a struct
 *
 * @param msg The message to decode
 * @param update_tle_result C-struct to decode the message contents into
 */
static inline void mavlink_msg_update_tle_result_decode(const mavlink_message_t* msg, mavlink_update_tle_result_t* update_tle_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    update_tle_result->result = mavlink_msg_update_tle_result_get_result(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN? msg->len : MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN;
        memset(update_tle_result, 0, MAVLINK_MSG_ID_UPDATE_TLE_RESULT_LEN);
    memcpy(update_tle_result, _MAV_PAYLOAD(msg), len);
#endif
}
