#pragma once
// MESSAGE SET_BEAM_POINT PACKING

#define MAVLINK_MSG_ID_SET_BEAM_POINT 22


typedef struct __mavlink_set_beam_point_t {
 float theta; /*<  离轴角(°)*/
 float phi; /*<  方位角(°)*/
 float polarity; /*<  极化角(°)*/
 float freq; /*<  频率(MHz)*/
 uint8_t TorR; /*<  收发选择，0x00收，0xFF发*/
 uint8_t polarity_type; /*<  极化类型，0x00线极化，0x01左旋，0x02右旋*/
} mavlink_set_beam_point_t;

#define MAVLINK_MSG_ID_SET_BEAM_POINT_LEN 18
#define MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN 18
#define MAVLINK_MSG_ID_22_LEN 18
#define MAVLINK_MSG_ID_22_MIN_LEN 18

#define MAVLINK_MSG_ID_SET_BEAM_POINT_CRC 127
#define MAVLINK_MSG_ID_22_CRC 127



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_BEAM_POINT { \
    22, \
    "SET_BEAM_POINT", \
    6, \
    {  { "TorR", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_set_beam_point_t, TorR) }, \
         { "polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_set_beam_point_t, polarity_type) }, \
         { "theta", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_beam_point_t, theta) }, \
         { "phi", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_beam_point_t, phi) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_set_beam_point_t, polarity) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_set_beam_point_t, freq) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_BEAM_POINT { \
    "SET_BEAM_POINT", \
    6, \
    {  { "TorR", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_set_beam_point_t, TorR) }, \
         { "polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_set_beam_point_t, polarity_type) }, \
         { "theta", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_beam_point_t, theta) }, \
         { "phi", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_beam_point_t, phi) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_set_beam_point_t, polarity) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_set_beam_point_t, freq) }, \
         } \
}
#endif

/**
 * @brief Pack a set_beam_point message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param TorR  收发选择，0x00收，0xFF发
 * @param polarity_type  极化类型，0x00线极化，0x01左旋，0x02右旋
 * @param theta  离轴角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 * @param freq  频率(MHz)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_beam_point_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t TorR, uint8_t polarity_type, float theta, float phi, float polarity, float freq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEAM_POINT_LEN];
    _mav_put_float(buf, 0, theta);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, freq);
    _mav_put_uint8_t(buf, 16, TorR);
    _mav_put_uint8_t(buf, 17, polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN);
#else
    mavlink_set_beam_point_t packet;
    packet.theta = theta;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.freq = freq;
    packet.TorR = TorR;
    packet.polarity_type = polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_BEAM_POINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
}

/**
 * @brief Pack a set_beam_point message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param TorR  收发选择，0x00收，0xFF发
 * @param polarity_type  极化类型，0x00线极化，0x01左旋，0x02右旋
 * @param theta  离轴角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 * @param freq  频率(MHz)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_beam_point_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t TorR,uint8_t polarity_type,float theta,float phi,float polarity,float freq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEAM_POINT_LEN];
    _mav_put_float(buf, 0, theta);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, freq);
    _mav_put_uint8_t(buf, 16, TorR);
    _mav_put_uint8_t(buf, 17, polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN);
#else
    mavlink_set_beam_point_t packet;
    packet.theta = theta;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.freq = freq;
    packet.TorR = TorR;
    packet.polarity_type = polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_BEAM_POINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
}

/**
 * @brief Encode a set_beam_point struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_beam_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_beam_point_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_beam_point_t* set_beam_point)
{
    return mavlink_msg_set_beam_point_pack(system_id, component_id, msg, set_beam_point->TorR, set_beam_point->polarity_type, set_beam_point->theta, set_beam_point->phi, set_beam_point->polarity, set_beam_point->freq);
}

/**
 * @brief Encode a set_beam_point struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_beam_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_beam_point_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_beam_point_t* set_beam_point)
{
    return mavlink_msg_set_beam_point_pack_chan(system_id, component_id, chan, msg, set_beam_point->TorR, set_beam_point->polarity_type, set_beam_point->theta, set_beam_point->phi, set_beam_point->polarity, set_beam_point->freq);
}

/**
 * @brief Send a set_beam_point message
 * @param chan MAVLink channel to send the message
 *
 * @param TorR  收发选择，0x00收，0xFF发
 * @param polarity_type  极化类型，0x00线极化，0x01左旋，0x02右旋
 * @param theta  离轴角(°)
 * @param phi  方位角(°)
 * @param polarity  极化角(°)
 * @param freq  频率(MHz)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_beam_point_send(mavlink_channel_t chan, uint8_t TorR, uint8_t polarity_type, float theta, float phi, float polarity, float freq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEAM_POINT_LEN];
    _mav_put_float(buf, 0, theta);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, freq);
    _mav_put_uint8_t(buf, 16, TorR);
    _mav_put_uint8_t(buf, 17, polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEAM_POINT, buf, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
#else
    mavlink_set_beam_point_t packet;
    packet.theta = theta;
    packet.phi = phi;
    packet.polarity = polarity;
    packet.freq = freq;
    packet.TorR = TorR;
    packet.polarity_type = polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEAM_POINT, (const char *)&packet, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
#endif
}

/**
 * @brief Send a set_beam_point message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_beam_point_send_struct(mavlink_channel_t chan, const mavlink_set_beam_point_t* set_beam_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_beam_point_send(chan, set_beam_point->TorR, set_beam_point->polarity_type, set_beam_point->theta, set_beam_point->phi, set_beam_point->polarity, set_beam_point->freq);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEAM_POINT, (const char *)set_beam_point, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_BEAM_POINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_beam_point_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t TorR, uint8_t polarity_type, float theta, float phi, float polarity, float freq)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, theta);
    _mav_put_float(buf, 4, phi);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, freq);
    _mav_put_uint8_t(buf, 16, TorR);
    _mav_put_uint8_t(buf, 17, polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEAM_POINT, buf, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
#else
    mavlink_set_beam_point_t *packet = (mavlink_set_beam_point_t *)msgbuf;
    packet->theta = theta;
    packet->phi = phi;
    packet->polarity = polarity;
    packet->freq = freq;
    packet->TorR = TorR;
    packet->polarity_type = polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEAM_POINT, (const char *)packet, MAVLINK_MSG_ID_SET_BEAM_POINT_MIN_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN, MAVLINK_MSG_ID_SET_BEAM_POINT_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_BEAM_POINT UNPACKING


/**
 * @brief Get field TorR from set_beam_point message
 *
 * @return  收发选择，0x00收，0xFF发
 */
static inline uint8_t mavlink_msg_set_beam_point_get_TorR(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field polarity_type from set_beam_point message
 *
 * @return  极化类型，0x00线极化，0x01左旋，0x02右旋
 */
static inline uint8_t mavlink_msg_set_beam_point_get_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field theta from set_beam_point message
 *
 * @return  离轴角(°)
 */
static inline float mavlink_msg_set_beam_point_get_theta(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field phi from set_beam_point message
 *
 * @return  方位角(°)
 */
static inline float mavlink_msg_set_beam_point_get_phi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field polarity from set_beam_point message
 *
 * @return  极化角(°)
 */
static inline float mavlink_msg_set_beam_point_get_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field freq from set_beam_point message
 *
 * @return  频率(MHz)
 */
static inline float mavlink_msg_set_beam_point_get_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a set_beam_point message into a struct
 *
 * @param msg The message to decode
 * @param set_beam_point C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_beam_point_decode(const mavlink_message_t* msg, mavlink_set_beam_point_t* set_beam_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_beam_point->theta = mavlink_msg_set_beam_point_get_theta(msg);
    set_beam_point->phi = mavlink_msg_set_beam_point_get_phi(msg);
    set_beam_point->polarity = mavlink_msg_set_beam_point_get_polarity(msg);
    set_beam_point->freq = mavlink_msg_set_beam_point_get_freq(msg);
    set_beam_point->TorR = mavlink_msg_set_beam_point_get_TorR(msg);
    set_beam_point->polarity_type = mavlink_msg_set_beam_point_get_polarity_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_BEAM_POINT_LEN? msg->len : MAVLINK_MSG_ID_SET_BEAM_POINT_LEN;
        memset(set_beam_point, 0, MAVLINK_MSG_ID_SET_BEAM_POINT_LEN);
    memcpy(set_beam_point, _MAV_PAYLOAD(msg), len);
#endif
}
