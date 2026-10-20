#pragma once
// MESSAGE ISSUE_EARTH_POINT_LOCAL PACKING

#define MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL 51

MAVPACKED(
typedef struct __mavlink_issue_earth_point_local_t {
 float pitch; /*<  俯仰角(°)*/
 float phi; /*<  方位角(°)*/
 float polarity; /*<  极化角(°)*/
 uint8_t valid; /*<  有效标志，0x00无效， 0x01有效*/
}) mavlink_issue_earth_point_local_t;

#define MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN 13
#define MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN 13
#define MAVLINK_MSG_ID_51_LEN 13
#define MAVLINK_MSG_ID_51_MIN_LEN 13

#define MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC 91
#define MAVLINK_MSG_ID_51_CRC 91



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ISSUE_EARTH_POINT_LOCAL { \
    51, \
    "ISSUE_EARTH_POINT_LOCAL", \
    4, \
    {  { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_issue_earth_point_local_t, valid) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_issue_earth_point_local_t, pitch) }, \
         { "phi", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_issue_earth_point_local_t, phi) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_issue_earth_point_local_t, polarity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ISSUE_EARTH_POINT_LOCAL { \
    "ISSUE_EARTH_POINT_LOCAL", \
    4, \
    {  { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_issue_earth_point_local_t, valid) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_issue_earth_point_local_t, pitch) }, \
         { "phi", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_issue_earth_point_local_t, phi) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_issue_earth_point_local_t, polarity) }, \
         } \
}
#endif

/**
 * @brief Pack a issue_earth_point_local message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param valid  有效标志，0x00无效， 0x01有效
 * @param pitch  俯仰角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_issue_earth_point_local_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t valid, float pitch, float phi, float polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_uint8_t(buf, 12, valid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN);
#else
    mavlink_issue_earth_point_local_t packet;
    packet.pitch = pitch;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.valid = valid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
}

/**
 * @brief Pack a issue_earth_point_local message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param valid  有效标志，0x00无效， 0x01有效
 * @param pitch  俯仰角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_issue_earth_point_local_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t valid,float pitch,float phi,float polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_uint8_t(buf, 12, valid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN);
#else
    mavlink_issue_earth_point_local_t packet;
    packet.pitch = pitch;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.valid = valid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
}

/**
 * @brief Encode a issue_earth_point_local struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param issue_earth_point_local C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_issue_earth_point_local_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_issue_earth_point_local_t* issue_earth_point_local)
{
    return mavlink_msg_issue_earth_point_local_pack(system_id, component_id, msg, issue_earth_point_local->valid, issue_earth_point_local->pitch, issue_earth_point_local->phi, issue_earth_point_local->polarity);
}

/**
 * @brief Encode a issue_earth_point_local struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param issue_earth_point_local C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_issue_earth_point_local_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_issue_earth_point_local_t* issue_earth_point_local)
{
    return mavlink_msg_issue_earth_point_local_pack_chan(system_id, component_id, chan, msg, issue_earth_point_local->valid, issue_earth_point_local->pitch, issue_earth_point_local->phi, issue_earth_point_local->polarity);
}

/**
 * @brief Send a issue_earth_point_local message
 * @param chan MAVLink channel to send the message
 *
 * @param valid  有效标志，0x00无效， 0x01有效
 * @param pitch  俯仰角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_issue_earth_point_local_send(mavlink_channel_t chan, uint8_t valid, float pitch, float phi, float polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_uint8_t(buf, 12, valid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL, buf, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
#else
    mavlink_issue_earth_point_local_t packet;
    packet.pitch = pitch;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.valid = valid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL, (const char *)&packet, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
#endif
}

/**
 * @brief Send a issue_earth_point_local message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_issue_earth_point_local_send_struct(mavlink_channel_t chan, const mavlink_issue_earth_point_local_t* issue_earth_point_local)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_issue_earth_point_local_send(chan, issue_earth_point_local->valid, issue_earth_point_local->pitch, issue_earth_point_local->phi, issue_earth_point_local->polarity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL, (const char *)issue_earth_point_local, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
#endif
}

#if MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_issue_earth_point_local_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t valid, float pitch, float phi, float polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_uint8_t(buf, 12, valid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL, buf, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
#else
    mavlink_issue_earth_point_local_t *packet = (mavlink_issue_earth_point_local_t *)msgbuf;
    packet->pitch = pitch;
    packet->phi = phi;
    packet->polarity = polarity;
    packet->valid = valid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL, (const char *)packet, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_MIN_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_CRC);
#endif
}
#endif

#endif

// MESSAGE ISSUE_EARTH_POINT_LOCAL UNPACKING


/**
 * @brief Get field valid from issue_earth_point_local message
 *
 * @return  有效标志，0x00无效， 0x01有效
 */
static inline uint8_t mavlink_msg_issue_earth_point_local_get_valid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field pitch from issue_earth_point_local message
 *
 * @return  俯仰角(°)
 */
static inline float mavlink_msg_issue_earth_point_local_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field phi from issue_earth_point_local message
 *
 * @return  方位角(°)
 */
static inline float mavlink_msg_issue_earth_point_local_get_phi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field polarity from issue_earth_point_local message
 *
 * @return  极化角(°)
 */
static inline float mavlink_msg_issue_earth_point_local_get_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a issue_earth_point_local message into a struct
 *
 * @param msg The message to decode
 * @param issue_earth_point_local C-struct to decode the message contents into
 */
static inline void mavlink_msg_issue_earth_point_local_decode(const mavlink_message_t* msg, mavlink_issue_earth_point_local_t* issue_earth_point_local)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    issue_earth_point_local->pitch = mavlink_msg_issue_earth_point_local_get_pitch(msg);
    issue_earth_point_local->phi = mavlink_msg_issue_earth_point_local_get_phi(msg);
    issue_earth_point_local->polarity = mavlink_msg_issue_earth_point_local_get_polarity(msg);
    issue_earth_point_local->valid = mavlink_msg_issue_earth_point_local_get_valid(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN? msg->len : MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN;
        memset(issue_earth_point_local, 0, MAVLINK_MSG_ID_ISSUE_EARTH_POINT_LOCAL_LEN);
    memcpy(issue_earth_point_local, _MAV_PAYLOAD(msg), len);
#endif
}
