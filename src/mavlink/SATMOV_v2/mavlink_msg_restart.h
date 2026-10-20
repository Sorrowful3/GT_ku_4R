#pragma once
// MESSAGE RESTART PACKING

#define MAVLINK_MSG_ID_RESTART 6


typedef struct __mavlink_restart_t {
 uint8_t mode; /*<  复位模式，0x00复位固件，0x01APP应用，0x03系统复位，0x04对星参数复位*/
} mavlink_restart_t;

#define MAVLINK_MSG_ID_RESTART_LEN 1
#define MAVLINK_MSG_ID_RESTART_MIN_LEN 1
#define MAVLINK_MSG_ID_6_LEN 1
#define MAVLINK_MSG_ID_6_MIN_LEN 1

#define MAVLINK_MSG_ID_RESTART_CRC 206
#define MAVLINK_MSG_ID_6_CRC 206



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_RESTART { \
    6, \
    "RESTART", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_restart_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_RESTART { \
    "RESTART", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_restart_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a restart message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  复位模式，0x00复位固件，0x01APP应用，0x03系统复位，0x04对星参数复位
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_restart_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RESTART_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RESTART_LEN);
#else
    mavlink_restart_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RESTART_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RESTART;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
}

/**
 * @brief Pack a restart message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  复位模式，0x00复位固件，0x01APP应用，0x03系统复位，0x04对星参数复位
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_restart_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RESTART_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RESTART_LEN);
#else
    mavlink_restart_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RESTART_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RESTART;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
}

/**
 * @brief Encode a restart struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param restart C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_restart_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_restart_t* restart)
{
    return mavlink_msg_restart_pack(system_id, component_id, msg, restart->mode);
}

/**
 * @brief Encode a restart struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param restart C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_restart_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_restart_t* restart)
{
    return mavlink_msg_restart_pack_chan(system_id, component_id, chan, msg, restart->mode);
}

/**
 * @brief Send a restart message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  复位模式，0x00复位固件，0x01APP应用，0x03系统复位，0x04对星参数复位
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_restart_send(mavlink_channel_t chan, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RESTART_LEN];
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RESTART, buf, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
#else
    mavlink_restart_t packet;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RESTART, (const char *)&packet, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
#endif
}

/**
 * @brief Send a restart message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_restart_send_struct(mavlink_channel_t chan, const mavlink_restart_t* restart)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_restart_send(chan, restart->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RESTART, (const char *)restart, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
#endif
}

#if MAVLINK_MSG_ID_RESTART_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_restart_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RESTART, buf, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
#else
    mavlink_restart_t *packet = (mavlink_restart_t *)msgbuf;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_RESTART, (const char *)packet, MAVLINK_MSG_ID_RESTART_MIN_LEN, MAVLINK_MSG_ID_RESTART_LEN, MAVLINK_MSG_ID_RESTART_CRC);
#endif
}
#endif

#endif

// MESSAGE RESTART UNPACKING


/**
 * @brief Get field mode from restart message
 *
 * @return  复位模式，0x00复位固件，0x01APP应用，0x03系统复位，0x04对星参数复位
 */
static inline uint8_t mavlink_msg_restart_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a restart message into a struct
 *
 * @param msg The message to decode
 * @param restart C-struct to decode the message contents into
 */
static inline void mavlink_msg_restart_decode(const mavlink_message_t* msg, mavlink_restart_t* restart)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    restart->mode = mavlink_msg_restart_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_RESTART_LEN? msg->len : MAVLINK_MSG_ID_RESTART_LEN;
        memset(restart, 0, MAVLINK_MSG_ID_RESTART_LEN);
    memcpy(restart, _MAV_PAYLOAD(msg), len);
#endif
}
