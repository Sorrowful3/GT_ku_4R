#pragma once
// MESSAGE SET_MONITOR PACKING

#define MAVLINK_MSG_ID_SET_MONITOR 30


typedef struct __mavlink_set_monitor_t {
 uint8_t mode; /*<  模式，0x00 接收天线，0x01 发射天线，0x02 其他设备*/
 uint8_t state; /*<  状态，0x00 关闭，0x01 开启*/
} mavlink_set_monitor_t;

#define MAVLINK_MSG_ID_SET_MONITOR_LEN 2
#define MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN 2
#define MAVLINK_MSG_ID_30_LEN 2
#define MAVLINK_MSG_ID_30_MIN_LEN 2

#define MAVLINK_MSG_ID_SET_MONITOR_CRC 33
#define MAVLINK_MSG_ID_30_CRC 33



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_MONITOR { \
    30, \
    "SET_MONITOR", \
    2, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_set_monitor_t, mode) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_set_monitor_t, state) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_MONITOR { \
    "SET_MONITOR", \
    2, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_set_monitor_t, mode) }, \
         { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_set_monitor_t, state) }, \
         } \
}
#endif

/**
 * @brief Pack a set_monitor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  模式，0x00 接收天线，0x01 发射天线，0x02 其他设备
 * @param state  状态，0x00 关闭，0x01 开启
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_monitor_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_MONITOR_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_uint8_t(buf, 1, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_MONITOR_LEN);
#else
    mavlink_set_monitor_t packet;
    packet.mode = mode;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_MONITOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_MONITOR;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
}

/**
 * @brief Pack a set_monitor message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  模式，0x00 接收天线，0x01 发射天线，0x02 其他设备
 * @param state  状态，0x00 关闭，0x01 开启
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_monitor_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode,uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_MONITOR_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_uint8_t(buf, 1, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_MONITOR_LEN);
#else
    mavlink_set_monitor_t packet;
    packet.mode = mode;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_MONITOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_MONITOR;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
}

/**
 * @brief Encode a set_monitor struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_monitor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_monitor_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_monitor_t* set_monitor)
{
    return mavlink_msg_set_monitor_pack(system_id, component_id, msg, set_monitor->mode, set_monitor->state);
}

/**
 * @brief Encode a set_monitor struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_monitor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_monitor_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_monitor_t* set_monitor)
{
    return mavlink_msg_set_monitor_pack_chan(system_id, component_id, chan, msg, set_monitor->mode, set_monitor->state);
}

/**
 * @brief Send a set_monitor message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  模式，0x00 接收天线，0x01 发射天线，0x02 其他设备
 * @param state  状态，0x00 关闭，0x01 开启
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_monitor_send(mavlink_channel_t chan, uint8_t mode, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_MONITOR_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_uint8_t(buf, 1, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_MONITOR, buf, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
#else
    mavlink_set_monitor_t packet;
    packet.mode = mode;
    packet.state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_MONITOR, (const char *)&packet, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
#endif
}

/**
 * @brief Send a set_monitor message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_monitor_send_struct(mavlink_channel_t chan, const mavlink_set_monitor_t* set_monitor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_monitor_send(chan, set_monitor->mode, set_monitor->state);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_MONITOR, (const char *)set_monitor, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_MONITOR_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_monitor_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode, uint8_t state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_uint8_t(buf, 1, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_MONITOR, buf, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
#else
    mavlink_set_monitor_t *packet = (mavlink_set_monitor_t *)msgbuf;
    packet->mode = mode;
    packet->state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_MONITOR, (const char *)packet, MAVLINK_MSG_ID_SET_MONITOR_MIN_LEN, MAVLINK_MSG_ID_SET_MONITOR_LEN, MAVLINK_MSG_ID_SET_MONITOR_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_MONITOR UNPACKING


/**
 * @brief Get field mode from set_monitor message
 *
 * @return  模式，0x00 接收天线，0x01 发射天线，0x02 其他设备
 */
static inline uint8_t mavlink_msg_set_monitor_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field state from set_monitor message
 *
 * @return  状态，0x00 关闭，0x01 开启
 */
static inline uint8_t mavlink_msg_set_monitor_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a set_monitor message into a struct
 *
 * @param msg The message to decode
 * @param set_monitor C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_monitor_decode(const mavlink_message_t* msg, mavlink_set_monitor_t* set_monitor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_monitor->mode = mavlink_msg_set_monitor_get_mode(msg);
    set_monitor->state = mavlink_msg_set_monitor_get_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_MONITOR_LEN? msg->len : MAVLINK_MSG_ID_SET_MONITOR_LEN;
        memset(set_monitor, 0, MAVLINK_MSG_ID_SET_MONITOR_LEN);
    memcpy(set_monitor, _MAV_PAYLOAD(msg), len);
#endif
}
