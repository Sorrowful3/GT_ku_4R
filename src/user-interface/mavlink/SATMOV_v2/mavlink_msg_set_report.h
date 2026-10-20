#pragma once
// MESSAGE SET_REPORT PACKING

#define MAVLINK_MSG_ID_SET_REPORT 16

MAVPACKED(
typedef struct __mavlink_set_report_t {
 uint32_t interval; /*<  数据上报间隔时间，ms，0ms不上报*/
 uint8_t type; /*<  此次设置所作用的数据类型，0x00所有数据，0x01系统工作状态，0x02变频器工作状态，0x03天线指向，0x04IMU，0x05信标机 0x06卫星信息*/
}) mavlink_set_report_t;

#define MAVLINK_MSG_ID_SET_REPORT_LEN 5
#define MAVLINK_MSG_ID_SET_REPORT_MIN_LEN 5
#define MAVLINK_MSG_ID_16_LEN 5
#define MAVLINK_MSG_ID_16_MIN_LEN 5

#define MAVLINK_MSG_ID_SET_REPORT_CRC 128
#define MAVLINK_MSG_ID_16_CRC 128



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_REPORT { \
    16, \
    "SET_REPORT", \
    2, \
    {  { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_set_report_t, type) }, \
         { "interval", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_set_report_t, interval) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_REPORT { \
    "SET_REPORT", \
    2, \
    {  { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_set_report_t, type) }, \
         { "interval", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_set_report_t, interval) }, \
         } \
}
#endif

/**
 * @brief Pack a set_report message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param type  此次设置所作用的数据类型，0x00所有数据，0x01系统工作状态，0x02变频器工作状态，0x03天线指向，0x04IMU，0x05信标机 0x06卫星信息
 * @param interval  数据上报间隔时间，ms，0ms不上报
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_report_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t type, uint32_t interval)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_REPORT_LEN];
    _mav_put_uint32_t(buf, 0, interval);
    _mav_put_uint8_t(buf, 4, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_REPORT_LEN);
#else
    mavlink_set_report_t packet;
    packet.interval = interval;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_REPORT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_REPORT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
}

/**
 * @brief Pack a set_report message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param type  此次设置所作用的数据类型，0x00所有数据，0x01系统工作状态，0x02变频器工作状态，0x03天线指向，0x04IMU，0x05信标机 0x06卫星信息
 * @param interval  数据上报间隔时间，ms，0ms不上报
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_report_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t type,uint32_t interval)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_REPORT_LEN];
    _mav_put_uint32_t(buf, 0, interval);
    _mav_put_uint8_t(buf, 4, type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_REPORT_LEN);
#else
    mavlink_set_report_t packet;
    packet.interval = interval;
    packet.type = type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_REPORT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_REPORT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
}

/**
 * @brief Encode a set_report struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_report C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_report_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_report_t* set_report)
{
    return mavlink_msg_set_report_pack(system_id, component_id, msg, set_report->type, set_report->interval);
}

/**
 * @brief Encode a set_report struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_report C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_report_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_report_t* set_report)
{
    return mavlink_msg_set_report_pack_chan(system_id, component_id, chan, msg, set_report->type, set_report->interval);
}

/**
 * @brief Send a set_report message
 * @param chan MAVLink channel to send the message
 *
 * @param type  此次设置所作用的数据类型，0x00所有数据，0x01系统工作状态，0x02变频器工作状态，0x03天线指向，0x04IMU，0x05信标机 0x06卫星信息
 * @param interval  数据上报间隔时间，ms，0ms不上报
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_report_send(mavlink_channel_t chan, uint8_t type, uint32_t interval)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_REPORT_LEN];
    _mav_put_uint32_t(buf, 0, interval);
    _mav_put_uint8_t(buf, 4, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_REPORT, buf, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
#else
    mavlink_set_report_t packet;
    packet.interval = interval;
    packet.type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_REPORT, (const char *)&packet, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
#endif
}

/**
 * @brief Send a set_report message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_report_send_struct(mavlink_channel_t chan, const mavlink_set_report_t* set_report)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_report_send(chan, set_report->type, set_report->interval);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_REPORT, (const char *)set_report, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_REPORT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_report_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t type, uint32_t interval)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, interval);
    _mav_put_uint8_t(buf, 4, type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_REPORT, buf, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
#else
    mavlink_set_report_t *packet = (mavlink_set_report_t *)msgbuf;
    packet->interval = interval;
    packet->type = type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_REPORT, (const char *)packet, MAVLINK_MSG_ID_SET_REPORT_MIN_LEN, MAVLINK_MSG_ID_SET_REPORT_LEN, MAVLINK_MSG_ID_SET_REPORT_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_REPORT UNPACKING


/**
 * @brief Get field type from set_report message
 *
 * @return  此次设置所作用的数据类型，0x00所有数据，0x01系统工作状态，0x02变频器工作状态，0x03天线指向，0x04IMU，0x05信标机 0x06卫星信息
 */
static inline uint8_t mavlink_msg_set_report_get_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field interval from set_report message
 *
 * @return  数据上报间隔时间，ms，0ms不上报
 */
static inline uint32_t mavlink_msg_set_report_get_interval(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Decode a set_report message into a struct
 *
 * @param msg The message to decode
 * @param set_report C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_report_decode(const mavlink_message_t* msg, mavlink_set_report_t* set_report)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_report->interval = mavlink_msg_set_report_get_interval(msg);
    set_report->type = mavlink_msg_set_report_get_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_REPORT_LEN? msg->len : MAVLINK_MSG_ID_SET_REPORT_LEN;
        memset(set_report, 0, MAVLINK_MSG_ID_SET_REPORT_LEN);
    memcpy(set_report, _MAV_PAYLOAD(msg), len);
#endif
}
