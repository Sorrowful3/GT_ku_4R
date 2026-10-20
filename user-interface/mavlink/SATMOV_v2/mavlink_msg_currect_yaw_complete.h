#pragma once
// MESSAGE CURRECT_YAW_COMPLETE PACKING

#define MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE 72

MAVPACKED(
typedef struct __mavlink_currect_yaw_complete_t {
 float yaw; /*<  天线真实航向角，-1表示校准出错*/
}) mavlink_currect_yaw_complete_t;

#define MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN 4
#define MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN 4
#define MAVLINK_MSG_ID_72_LEN 4
#define MAVLINK_MSG_ID_72_MIN_LEN 4

#define MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC 247
#define MAVLINK_MSG_ID_72_CRC 247



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CURRECT_YAW_COMPLETE { \
    72, \
    "CURRECT_YAW_COMPLETE", \
    1, \
    {  { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_currect_yaw_complete_t, yaw) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CURRECT_YAW_COMPLETE { \
    "CURRECT_YAW_COMPLETE", \
    1, \
    {  { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_currect_yaw_complete_t, yaw) }, \
         } \
}
#endif

/**
 * @brief Pack a currect_yaw_complete message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param yaw  天线真实航向角，-1表示校准出错
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_currect_yaw_complete_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN];
    _mav_put_float(buf, 0, yaw);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN);
#else
    mavlink_currect_yaw_complete_t packet;
    packet.yaw = yaw;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
}

/**
 * @brief Pack a currect_yaw_complete message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param yaw  天线真实航向角，-1表示校准出错
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_currect_yaw_complete_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN];
    _mav_put_float(buf, 0, yaw);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN);
#else
    mavlink_currect_yaw_complete_t packet;
    packet.yaw = yaw;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
}

/**
 * @brief Encode a currect_yaw_complete struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param currect_yaw_complete C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_currect_yaw_complete_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_currect_yaw_complete_t* currect_yaw_complete)
{
    return mavlink_msg_currect_yaw_complete_pack(system_id, component_id, msg, currect_yaw_complete->yaw);
}

/**
 * @brief Encode a currect_yaw_complete struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param currect_yaw_complete C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_currect_yaw_complete_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_currect_yaw_complete_t* currect_yaw_complete)
{
    return mavlink_msg_currect_yaw_complete_pack_chan(system_id, component_id, chan, msg, currect_yaw_complete->yaw);
}

/**
 * @brief Send a currect_yaw_complete message
 * @param chan MAVLink channel to send the message
 *
 * @param yaw  天线真实航向角，-1表示校准出错
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_currect_yaw_complete_send(mavlink_channel_t chan, float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN];
    _mav_put_float(buf, 0, yaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE, buf, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
#else
    mavlink_currect_yaw_complete_t packet;
    packet.yaw = yaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE, (const char *)&packet, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
#endif
}

/**
 * @brief Send a currect_yaw_complete message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_currect_yaw_complete_send_struct(mavlink_channel_t chan, const mavlink_currect_yaw_complete_t* currect_yaw_complete)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_currect_yaw_complete_send(chan, currect_yaw_complete->yaw);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE, (const char *)currect_yaw_complete, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
#endif
}

#if MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_currect_yaw_complete_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, yaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE, buf, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
#else
    mavlink_currect_yaw_complete_t *packet = (mavlink_currect_yaw_complete_t *)msgbuf;
    packet->yaw = yaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE, (const char *)packet, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_CRC);
#endif
}
#endif

#endif

// MESSAGE CURRECT_YAW_COMPLETE UNPACKING


/**
 * @brief Get field yaw from currect_yaw_complete message
 *
 * @return  天线真实航向角，-1表示校准出错
 */
static inline float mavlink_msg_currect_yaw_complete_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a currect_yaw_complete message into a struct
 *
 * @param msg The message to decode
 * @param currect_yaw_complete C-struct to decode the message contents into
 */
static inline void mavlink_msg_currect_yaw_complete_decode(const mavlink_message_t* msg, mavlink_currect_yaw_complete_t* currect_yaw_complete)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    currect_yaw_complete->yaw = mavlink_msg_currect_yaw_complete_get_yaw(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN? msg->len : MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN;
        memset(currect_yaw_complete, 0, MAVLINK_MSG_ID_CURRECT_YAW_COMPLETE_LEN);
    memcpy(currect_yaw_complete, _MAV_PAYLOAD(msg), len);
#endif
}
