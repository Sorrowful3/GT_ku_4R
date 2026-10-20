#pragma once
// MESSAGE SWITCH_ANTENNA PACKING

#define MAVLINK_MSG_ID_SWITCH_ANTENNA 14

MAVPACKED(
typedef struct __mavlink_switch_antenna_t {
 uint8_t TorR; /*<  收发，0x00收，0xFF发*/
 uint8_t subset; /*<  子阵号，开关射频供电时此值无意义*/
 uint8_t mode; /*<  模式，0xF0关直流供电，0xF1开直流供电，0x0F关射频供电，0x1F开射频供电*/
}) mavlink_switch_antenna_t;

#define MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN 3
#define MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN 3
#define MAVLINK_MSG_ID_14_LEN 3
#define MAVLINK_MSG_ID_14_MIN_LEN 3

#define MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC 108
#define MAVLINK_MSG_ID_14_CRC 108



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SWITCH_ANTENNA { \
    14, \
    "SWITCH_ANTENNA", \
    3, \
    {  { "TorR", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_antenna_t, TorR) }, \
         { "subset", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_switch_antenna_t, subset) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_switch_antenna_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SWITCH_ANTENNA { \
    "SWITCH_ANTENNA", \
    3, \
    {  { "TorR", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_antenna_t, TorR) }, \
         { "subset", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_switch_antenna_t, subset) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_switch_antenna_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a switch_antenna message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param TorR  收发，0x00收，0xFF发
 * @param subset  子阵号，开关射频供电时此值无意义
 * @param mode  模式，0xF0关直流供电，0xF1开直流供电，0x0F关射频供电，0x1F开射频供电
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_antenna_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t TorR, uint8_t subset, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN];
    _mav_put_uint8_t(buf, 0, TorR);
    _mav_put_uint8_t(buf, 1, subset);
    _mav_put_uint8_t(buf, 2, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN);
#else
    mavlink_switch_antenna_t packet;
    packet.TorR = TorR;
    packet.subset = subset;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_ANTENNA;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
}

/**
 * @brief Pack a switch_antenna message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param TorR  收发，0x00收，0xFF发
 * @param subset  子阵号，开关射频供电时此值无意义
 * @param mode  模式，0xF0关直流供电，0xF1开直流供电，0x0F关射频供电，0x1F开射频供电
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_antenna_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t TorR,uint8_t subset,uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN];
    _mav_put_uint8_t(buf, 0, TorR);
    _mav_put_uint8_t(buf, 1, subset);
    _mav_put_uint8_t(buf, 2, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN);
#else
    mavlink_switch_antenna_t packet;
    packet.TorR = TorR;
    packet.subset = subset;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_ANTENNA;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
}

/**
 * @brief Encode a switch_antenna struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param switch_antenna C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_antenna_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_switch_antenna_t* switch_antenna)
{
    return mavlink_msg_switch_antenna_pack(system_id, component_id, msg, switch_antenna->TorR, switch_antenna->subset, switch_antenna->mode);
}

/**
 * @brief Encode a switch_antenna struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param switch_antenna C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_antenna_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_switch_antenna_t* switch_antenna)
{
    return mavlink_msg_switch_antenna_pack_chan(system_id, component_id, chan, msg, switch_antenna->TorR, switch_antenna->subset, switch_antenna->mode);
}

/**
 * @brief Send a switch_antenna message
 * @param chan MAVLink channel to send the message
 *
 * @param TorR  收发，0x00收，0xFF发
 * @param subset  子阵号，开关射频供电时此值无意义
 * @param mode  模式，0xF0关直流供电，0xF1开直流供电，0x0F关射频供电，0x1F开射频供电
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_switch_antenna_send(mavlink_channel_t chan, uint8_t TorR, uint8_t subset, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN];
    _mav_put_uint8_t(buf, 0, TorR);
    _mav_put_uint8_t(buf, 1, subset);
    _mav_put_uint8_t(buf, 2, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_ANTENNA, buf, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
#else
    mavlink_switch_antenna_t packet;
    packet.TorR = TorR;
    packet.subset = subset;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_ANTENNA, (const char *)&packet, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
#endif
}

/**
 * @brief Send a switch_antenna message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_switch_antenna_send_struct(mavlink_channel_t chan, const mavlink_switch_antenna_t* switch_antenna)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_switch_antenna_send(chan, switch_antenna->TorR, switch_antenna->subset, switch_antenna->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_ANTENNA, (const char *)switch_antenna, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
#endif
}

#if MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_switch_antenna_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t TorR, uint8_t subset, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, TorR);
    _mav_put_uint8_t(buf, 1, subset);
    _mav_put_uint8_t(buf, 2, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_ANTENNA, buf, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
#else
    mavlink_switch_antenna_t *packet = (mavlink_switch_antenna_t *)msgbuf;
    packet->TorR = TorR;
    packet->subset = subset;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_ANTENNA, (const char *)packet, MAVLINK_MSG_ID_SWITCH_ANTENNA_MIN_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN, MAVLINK_MSG_ID_SWITCH_ANTENNA_CRC);
#endif
}
#endif

#endif

// MESSAGE SWITCH_ANTENNA UNPACKING


/**
 * @brief Get field TorR from switch_antenna message
 *
 * @return  收发，0x00收，0xFF发
 */
static inline uint8_t mavlink_msg_switch_antenna_get_TorR(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field subset from switch_antenna message
 *
 * @return  子阵号，开关射频供电时此值无意义
 */
static inline uint8_t mavlink_msg_switch_antenna_get_subset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field mode from switch_antenna message
 *
 * @return  模式，0xF0关直流供电，0xF1开直流供电，0x0F关射频供电，0x1F开射频供电
 */
static inline uint8_t mavlink_msg_switch_antenna_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Decode a switch_antenna message into a struct
 *
 * @param msg The message to decode
 * @param switch_antenna C-struct to decode the message contents into
 */
static inline void mavlink_msg_switch_antenna_decode(const mavlink_message_t* msg, mavlink_switch_antenna_t* switch_antenna)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    switch_antenna->TorR = mavlink_msg_switch_antenna_get_TorR(msg);
    switch_antenna->subset = mavlink_msg_switch_antenna_get_subset(msg);
    switch_antenna->mode = mavlink_msg_switch_antenna_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN? msg->len : MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN;
        memset(switch_antenna, 0, MAVLINK_MSG_ID_SWITCH_ANTENNA_LEN);
    memcpy(switch_antenna, _MAV_PAYLOAD(msg), len);
#endif
}
