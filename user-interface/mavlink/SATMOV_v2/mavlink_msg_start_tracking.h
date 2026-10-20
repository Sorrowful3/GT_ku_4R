#pragma once
// MESSAGE START_TRACKING PACKING

#define MAVLINK_MSG_ID_START_TRACKING 26

MAVPACKED(
typedef struct __mavlink_start_tracking_t {
 uint8_t reserved; /*<  Ô¤Áô*/
}) mavlink_start_tracking_t;

#define MAVLINK_MSG_ID_START_TRACKING_LEN 1
#define MAVLINK_MSG_ID_START_TRACKING_MIN_LEN 1
#define MAVLINK_MSG_ID_26_LEN 1
#define MAVLINK_MSG_ID_26_MIN_LEN 1

#define MAVLINK_MSG_ID_START_TRACKING_CRC 39
#define MAVLINK_MSG_ID_26_CRC 39



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_START_TRACKING { \
    26, \
    "START_TRACKING", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_start_tracking_t, reserved) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_START_TRACKING { \
    "START_TRACKING", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_start_tracking_t, reserved) }, \
         } \
}
#endif

/**
 * @brief Pack a start_tracking message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_start_tracking_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_START_TRACKING_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_START_TRACKING_LEN);
#else
    mavlink_start_tracking_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_START_TRACKING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_START_TRACKING;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
}

/**
 * @brief Pack a start_tracking message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_start_tracking_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_START_TRACKING_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_START_TRACKING_LEN);
#else
    mavlink_start_tracking_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_START_TRACKING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_START_TRACKING;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
}

/**
 * @brief Encode a start_tracking struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param start_tracking C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_start_tracking_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_start_tracking_t* start_tracking)
{
    return mavlink_msg_start_tracking_pack(system_id, component_id, msg, start_tracking->reserved);
}

/**
 * @brief Encode a start_tracking struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param start_tracking C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_start_tracking_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_start_tracking_t* start_tracking)
{
    return mavlink_msg_start_tracking_pack_chan(system_id, component_id, chan, msg, start_tracking->reserved);
}

/**
 * @brief Send a start_tracking message
 * @param chan MAVLink channel to send the message
 *
 * @param reserved  Ô¤Áô
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_start_tracking_send(mavlink_channel_t chan, uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_START_TRACKING_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_START_TRACKING, buf, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
#else
    mavlink_start_tracking_t packet;
    packet.reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_START_TRACKING, (const char *)&packet, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
#endif
}

/**
 * @brief Send a start_tracking message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_start_tracking_send_struct(mavlink_channel_t chan, const mavlink_start_tracking_t* start_tracking)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_start_tracking_send(chan, start_tracking->reserved);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_START_TRACKING, (const char *)start_tracking, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
#endif
}

#if MAVLINK_MSG_ID_START_TRACKING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_start_tracking_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_START_TRACKING, buf, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
#else
    mavlink_start_tracking_t *packet = (mavlink_start_tracking_t *)msgbuf;
    packet->reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_START_TRACKING, (const char *)packet, MAVLINK_MSG_ID_START_TRACKING_MIN_LEN, MAVLINK_MSG_ID_START_TRACKING_LEN, MAVLINK_MSG_ID_START_TRACKING_CRC);
#endif
}
#endif

#endif

// MESSAGE START_TRACKING UNPACKING


/**
 * @brief Get field reserved from start_tracking message
 *
 * @return  Ô¤Áô
 */
static inline uint8_t mavlink_msg_start_tracking_get_reserved(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a start_tracking message into a struct
 *
 * @param msg The message to decode
 * @param start_tracking C-struct to decode the message contents into
 */
static inline void mavlink_msg_start_tracking_decode(const mavlink_message_t* msg, mavlink_start_tracking_t* start_tracking)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    start_tracking->reserved = mavlink_msg_start_tracking_get_reserved(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_START_TRACKING_LEN? msg->len : MAVLINK_MSG_ID_START_TRACKING_LEN;
        memset(start_tracking, 0, MAVLINK_MSG_ID_START_TRACKING_LEN);
    memcpy(start_tracking, _MAV_PAYLOAD(msg), len);
#endif
}
