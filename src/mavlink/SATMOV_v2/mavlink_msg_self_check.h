#pragma once
// MESSAGE SELF_CHECK PACKING

#define MAVLINK_MSG_ID_SELF_CHECK 24


typedef struct __mavlink_self_check_t {
 uint8_t mode; /*<  自检模式，指定要自检的设备，0ACU，1变频器，2IMU，3信标机，10~19接收天线，20~29发射天线*/
} mavlink_self_check_t;

#define MAVLINK_MSG_ID_SELF_CHECK_LEN 1
#define MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN 1
#define MAVLINK_MSG_ID_24_LEN 1
#define MAVLINK_MSG_ID_24_MIN_LEN 1

#define MAVLINK_MSG_ID_SELF_CHECK_CRC 183
#define MAVLINK_MSG_ID_24_CRC 183



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SELF_CHECK { \
    24, \
    "SELF_CHECK", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_self_check_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SELF_CHECK { \
    "SELF_CHECK", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_self_check_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a self_check message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  自检模式，指定要自检的设备，0ACU，1变频器，2IMU，3信标机，10~19接收天线，20~29发射天线
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_self_check_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SELF_CHECK_LEN);
#else
    mavlink_self_check_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SELF_CHECK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SELF_CHECK;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
}

/**
 * @brief Pack a self_check message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  自检模式，指定要自检的设备，0ACU，1变频器，2IMU，3信标机，10~19接收天线，20~29发射天线
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_self_check_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SELF_CHECK_LEN);
#else
    mavlink_self_check_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SELF_CHECK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SELF_CHECK;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
}

/**
 * @brief Encode a self_check struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param self_check C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_self_check_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_self_check_t* self_check)
{
    return mavlink_msg_self_check_pack(system_id, component_id, msg, self_check->mode);
}

/**
 * @brief Encode a self_check struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param self_check C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_self_check_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_self_check_t* self_check)
{
    return mavlink_msg_self_check_pack_chan(system_id, component_id, chan, msg, self_check->mode);
}

/**
 * @brief Send a self_check message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  自检模式，指定要自检的设备，0ACU，1变频器，2IMU，3信标机，10~19接收天线，20~29发射天线
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_self_check_send(mavlink_channel_t chan, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SELF_CHECK_LEN];
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK, buf, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
#else
    mavlink_self_check_t packet;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK, (const char *)&packet, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
#endif
}

/**
 * @brief Send a self_check message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_self_check_send_struct(mavlink_channel_t chan, const mavlink_self_check_t* self_check)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_self_check_send(chan, self_check->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK, (const char *)self_check, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
#endif
}

#if MAVLINK_MSG_ID_SELF_CHECK_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_self_check_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK, buf, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
#else
    mavlink_self_check_t *packet = (mavlink_self_check_t *)msgbuf;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SELF_CHECK, (const char *)packet, MAVLINK_MSG_ID_SELF_CHECK_MIN_LEN, MAVLINK_MSG_ID_SELF_CHECK_LEN, MAVLINK_MSG_ID_SELF_CHECK_CRC);
#endif
}
#endif

#endif

// MESSAGE SELF_CHECK UNPACKING


/**
 * @brief Get field mode from self_check message
 *
 * @return  自检模式，指定要自检的设备，0ACU，1变频器，2IMU，3信标机，10~19接收天线，20~29发射天线
 */
static inline uint8_t mavlink_msg_self_check_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a self_check message into a struct
 *
 * @param msg The message to decode
 * @param self_check C-struct to decode the message contents into
 */
static inline void mavlink_msg_self_check_decode(const mavlink_message_t* msg, mavlink_self_check_t* self_check)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    self_check->mode = mavlink_msg_self_check_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SELF_CHECK_LEN? msg->len : MAVLINK_MSG_ID_SELF_CHECK_LEN;
        memset(self_check, 0, MAVLINK_MSG_ID_SELF_CHECK_LEN);
    memcpy(self_check, _MAV_PAYLOAD(msg), len);
#endif
}
