#pragma once
// MESSAGE CLEAR_EARTH_POINT PACKING

#define MAVLINK_MSG_ID_CLEAR_EARTH_POINT 76

MAVPACKED(
typedef struct __mavlink_clear_earth_point_t {
 uint8_t reserved; /*<  Ô¤Áô*/
}) mavlink_clear_earth_point_t;

#define MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN 1
#define MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN 1
#define MAVLINK_MSG_ID_76_LEN 1
#define MAVLINK_MSG_ID_76_MIN_LEN 1

#define MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC 217
#define MAVLINK_MSG_ID_76_CRC 217



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CLEAR_EARTH_POINT { \
    76, \
    "CLEAR_EARTH_POINT", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_clear_earth_point_t, reserved) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CLEAR_EARTH_POINT { \
    "CLEAR_EARTH_POINT", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_clear_earth_point_t, reserved) }, \
         } \
}
#endif

/**
 * @brief Pack a clear_earth_point message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_clear_earth_point_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN);
#else
    mavlink_clear_earth_point_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CLEAR_EARTH_POINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
}

/**
 * @brief Pack a clear_earth_point message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_clear_earth_point_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN);
#else
    mavlink_clear_earth_point_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CLEAR_EARTH_POINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
}

/**
 * @brief Encode a clear_earth_point struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param clear_earth_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_clear_earth_point_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_clear_earth_point_t* clear_earth_point)
{
    return mavlink_msg_clear_earth_point_pack(system_id, component_id, msg, clear_earth_point->reserved);
}

/**
 * @brief Encode a clear_earth_point struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param clear_earth_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_clear_earth_point_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_clear_earth_point_t* clear_earth_point)
{
    return mavlink_msg_clear_earth_point_pack_chan(system_id, component_id, chan, msg, clear_earth_point->reserved);
}

/**
 * @brief Send a clear_earth_point message
 * @param chan MAVLink channel to send the message
 *
 * @param reserved  Ô¤Áô
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_clear_earth_point_send(mavlink_channel_t chan, uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT, buf, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
#else
    mavlink_clear_earth_point_t packet;
    packet.reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT, (const char *)&packet, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
#endif
}

/**
 * @brief Send a clear_earth_point message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_clear_earth_point_send_struct(mavlink_channel_t chan, const mavlink_clear_earth_point_t* clear_earth_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_clear_earth_point_send(chan, clear_earth_point->reserved);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT, (const char *)clear_earth_point, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_clear_earth_point_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT, buf, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
#else
    mavlink_clear_earth_point_t *packet = (mavlink_clear_earth_point_t *)msgbuf;
    packet->reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CLEAR_EARTH_POINT, (const char *)packet, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_CRC);
#endif
}
#endif

#endif

// MESSAGE CLEAR_EARTH_POINT UNPACKING


/**
 * @brief Get field reserved from clear_earth_point message
 *
 * @return  Ô¤Áô
 */
static inline uint8_t mavlink_msg_clear_earth_point_get_reserved(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a clear_earth_point message into a struct
 *
 * @param msg The message to decode
 * @param clear_earth_point C-struct to decode the message contents into
 */
static inline void mavlink_msg_clear_earth_point_decode(const mavlink_message_t* msg, mavlink_clear_earth_point_t* clear_earth_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    clear_earth_point->reserved = mavlink_msg_clear_earth_point_get_reserved(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN? msg->len : MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN;
        memset(clear_earth_point, 0, MAVLINK_MSG_ID_CLEAR_EARTH_POINT_LEN);
    memcpy(clear_earth_point, _MAV_PAYLOAD(msg), len);
#endif
}
