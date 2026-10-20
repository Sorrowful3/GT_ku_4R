#pragma once
// MESSAGE REPORT_ANTENNA_STATE PACKING

#define MAVLINK_MSG_ID_REPORT_ANTENNA_STATE 103

MAVPACKED(
typedef struct __mavlink_report_antenna_state_t {
 float rx_phi; /*<  接收方位角(°)*/
 float rx_theta; /*<  接收离轴角(°)*/
 float rx_polarity; /*<  接收极化角(°)*/
 float rx_freq; /*<  接收射频频率(MHz)*/
 float tx_phi; /*<  发射方位角(°)*/
 float tx_theta; /*<  发射离轴角(°)*/
 float tx_polarity; /*<  发射极化角(°)*/
 float tx_freq; /*<  发射射频频率(MHz)*/
 float earth_azimuth; /*<  地理方位角(°)*/
 float earth_elevation; /*<  地理俯仰角(°)*/
 float earth_polarity; /*<  地理极化角(°)*/
 uint8_t rx_polarity_type; /*<  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
 uint8_t tx_polarity_type; /*<  发射极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
}) mavlink_report_antenna_state_t;

#define MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN 46
#define MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN 46
#define MAVLINK_MSG_ID_103_LEN 46
#define MAVLINK_MSG_ID_103_MIN_LEN 46

#define MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC 55
#define MAVLINK_MSG_ID_103_CRC 55



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_ANTENNA_STATE { \
    103, \
    "REPORT_ANTENNA_STATE", \
    13, \
    {  { "rx_phi", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_antenna_state_t, rx_phi) }, \
         { "rx_theta", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_antenna_state_t, rx_theta) }, \
         { "rx_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_antenna_state_t, rx_polarity) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_antenna_state_t, rx_freq) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_report_antenna_state_t, rx_polarity_type) }, \
         { "tx_phi", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_report_antenna_state_t, tx_phi) }, \
         { "tx_theta", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_report_antenna_state_t, tx_theta) }, \
         { "tx_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_report_antenna_state_t, tx_polarity) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_report_antenna_state_t, tx_freq) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_report_antenna_state_t, tx_polarity_type) }, \
         { "earth_azimuth", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_report_antenna_state_t, earth_azimuth) }, \
         { "earth_elevation", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_report_antenna_state_t, earth_elevation) }, \
         { "earth_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_report_antenna_state_t, earth_polarity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_ANTENNA_STATE { \
    "REPORT_ANTENNA_STATE", \
    13, \
    {  { "rx_phi", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_antenna_state_t, rx_phi) }, \
         { "rx_theta", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_antenna_state_t, rx_theta) }, \
         { "rx_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_antenna_state_t, rx_polarity) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_antenna_state_t, rx_freq) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 44, offsetof(mavlink_report_antenna_state_t, rx_polarity_type) }, \
         { "tx_phi", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_report_antenna_state_t, tx_phi) }, \
         { "tx_theta", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_report_antenna_state_t, tx_theta) }, \
         { "tx_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_report_antenna_state_t, tx_polarity) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_report_antenna_state_t, tx_freq) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 45, offsetof(mavlink_report_antenna_state_t, tx_polarity_type) }, \
         { "earth_azimuth", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_report_antenna_state_t, earth_azimuth) }, \
         { "earth_elevation", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_report_antenna_state_t, earth_elevation) }, \
         { "earth_polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_report_antenna_state_t, earth_polarity) }, \
         } \
}
#endif

/**
 * @brief Pack a report_antenna_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rx_phi  接收方位角(°)
 * @param rx_theta  接收离轴角(°)
 * @param rx_polarity  接收极化角(°)
 * @param rx_freq  接收射频频率(MHz)
 * @param rx_polarity_type  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_phi  发射方位角(°)
 * @param tx_theta  发射离轴角(°)
 * @param tx_polarity  发射极化角(°)
 * @param tx_freq  发射射频频率(MHz)
 * @param tx_polarity_type  发射极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param earth_azimuth  地理方位角(°)
 * @param earth_elevation  地理俯仰角(°)
 * @param earth_polarity  地理极化角(°)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_antenna_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float rx_phi, float rx_theta, float rx_polarity, float rx_freq, uint8_t rx_polarity_type, float tx_phi, float tx_theta, float tx_polarity, float tx_freq, uint8_t tx_polarity_type, float earth_azimuth, float earth_elevation, float earth_polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN];
    _mav_put_float(buf, 0, rx_phi);
    _mav_put_float(buf, 4, rx_theta);
    _mav_put_float(buf, 8, rx_polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, tx_phi);
    _mav_put_float(buf, 20, tx_theta);
    _mav_put_float(buf, 24, tx_polarity);
    _mav_put_float(buf, 28, tx_freq);
    _mav_put_float(buf, 32, earth_azimuth);
    _mav_put_float(buf, 36, earth_elevation);
    _mav_put_float(buf, 40, earth_polarity);
    _mav_put_uint8_t(buf, 44, rx_polarity_type);
    _mav_put_uint8_t(buf, 45, tx_polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN);
#else
    mavlink_report_antenna_state_t packet;
    packet.rx_phi = rx_phi;
    packet.rx_theta = rx_theta;
    packet.rx_polarity = rx_polarity;
    packet.rx_freq = rx_freq;
    packet.tx_phi = tx_phi;
    packet.tx_theta = tx_theta;
    packet.tx_polarity = tx_polarity;
    packet.tx_freq = tx_freq;
    packet.earth_azimuth = earth_azimuth;
    packet.earth_elevation = earth_elevation;
    packet.earth_polarity = earth_polarity;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_ANTENNA_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
}

/**
 * @brief Pack a report_antenna_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx_phi  接收方位角(°)
 * @param rx_theta  接收离轴角(°)
 * @param rx_polarity  接收极化角(°)
 * @param rx_freq  接收射频频率(MHz)
 * @param rx_polarity_type  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_phi  发射方位角(°)
 * @param tx_theta  发射离轴角(°)
 * @param tx_polarity  发射极化角(°)
 * @param tx_freq  发射射频频率(MHz)
 * @param tx_polarity_type  发射极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param earth_azimuth  地理方位角(°)
 * @param earth_elevation  地理俯仰角(°)
 * @param earth_polarity  地理极化角(°)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_antenna_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float rx_phi,float rx_theta,float rx_polarity,float rx_freq,uint8_t rx_polarity_type,float tx_phi,float tx_theta,float tx_polarity,float tx_freq,uint8_t tx_polarity_type,float earth_azimuth,float earth_elevation,float earth_polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN];
    _mav_put_float(buf, 0, rx_phi);
    _mav_put_float(buf, 4, rx_theta);
    _mav_put_float(buf, 8, rx_polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, tx_phi);
    _mav_put_float(buf, 20, tx_theta);
    _mav_put_float(buf, 24, tx_polarity);
    _mav_put_float(buf, 28, tx_freq);
    _mav_put_float(buf, 32, earth_azimuth);
    _mav_put_float(buf, 36, earth_elevation);
    _mav_put_float(buf, 40, earth_polarity);
    _mav_put_uint8_t(buf, 44, rx_polarity_type);
    _mav_put_uint8_t(buf, 45, tx_polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN);
#else
    mavlink_report_antenna_state_t packet;
    packet.rx_phi = rx_phi;
    packet.rx_theta = rx_theta;
    packet.rx_polarity = rx_polarity;
    packet.rx_freq = rx_freq;
    packet.tx_phi = tx_phi;
    packet.tx_theta = tx_theta;
    packet.tx_polarity = tx_polarity;
    packet.tx_freq = tx_freq;
    packet.earth_azimuth = earth_azimuth;
    packet.earth_elevation = earth_elevation;
    packet.earth_polarity = earth_polarity;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_ANTENNA_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
}

/**
 * @brief Encode a report_antenna_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_antenna_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_antenna_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_antenna_state_t* report_antenna_state)
{
    return mavlink_msg_report_antenna_state_pack(system_id, component_id, msg, report_antenna_state->rx_phi, report_antenna_state->rx_theta, report_antenna_state->rx_polarity, report_antenna_state->rx_freq, report_antenna_state->rx_polarity_type, report_antenna_state->tx_phi, report_antenna_state->tx_theta, report_antenna_state->tx_polarity, report_antenna_state->tx_freq, report_antenna_state->tx_polarity_type, report_antenna_state->earth_azimuth, report_antenna_state->earth_elevation, report_antenna_state->earth_polarity);
}

/**
 * @brief Encode a report_antenna_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_antenna_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_antenna_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_antenna_state_t* report_antenna_state)
{
    return mavlink_msg_report_antenna_state_pack_chan(system_id, component_id, chan, msg, report_antenna_state->rx_phi, report_antenna_state->rx_theta, report_antenna_state->rx_polarity, report_antenna_state->rx_freq, report_antenna_state->rx_polarity_type, report_antenna_state->tx_phi, report_antenna_state->tx_theta, report_antenna_state->tx_polarity, report_antenna_state->tx_freq, report_antenna_state->tx_polarity_type, report_antenna_state->earth_azimuth, report_antenna_state->earth_elevation, report_antenna_state->earth_polarity);
}

/**
 * @brief Send a report_antenna_state message
 * @param chan MAVLink channel to send the message
 *
 * @param rx_phi  接收方位角(°)
 * @param rx_theta  接收离轴角(°)
 * @param rx_polarity  接收极化角(°)
 * @param rx_freq  接收射频频率(MHz)
 * @param rx_polarity_type  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_phi  发射方位角(°)
 * @param tx_theta  发射离轴角(°)
 * @param tx_polarity  发射极化角(°)
 * @param tx_freq  发射射频频率(MHz)
 * @param tx_polarity_type  发射极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param earth_azimuth  地理方位角(°)
 * @param earth_elevation  地理俯仰角(°)
 * @param earth_polarity  地理极化角(°)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_antenna_state_send(mavlink_channel_t chan, float rx_phi, float rx_theta, float rx_polarity, float rx_freq, uint8_t rx_polarity_type, float tx_phi, float tx_theta, float tx_polarity, float tx_freq, uint8_t tx_polarity_type, float earth_azimuth, float earth_elevation, float earth_polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN];
    _mav_put_float(buf, 0, rx_phi);
    _mav_put_float(buf, 4, rx_theta);
    _mav_put_float(buf, 8, rx_polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, tx_phi);
    _mav_put_float(buf, 20, tx_theta);
    _mav_put_float(buf, 24, tx_polarity);
    _mav_put_float(buf, 28, tx_freq);
    _mav_put_float(buf, 32, earth_azimuth);
    _mav_put_float(buf, 36, earth_elevation);
    _mav_put_float(buf, 40, earth_polarity);
    _mav_put_uint8_t(buf, 44, rx_polarity_type);
    _mav_put_uint8_t(buf, 45, tx_polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE, buf, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
#else
    mavlink_report_antenna_state_t packet;
    packet.rx_phi = rx_phi;
    packet.rx_theta = rx_theta;
    packet.rx_polarity = rx_polarity;
    packet.rx_freq = rx_freq;
    packet.tx_phi = tx_phi;
    packet.tx_theta = tx_theta;
    packet.tx_polarity = tx_polarity;
    packet.tx_freq = tx_freq;
    packet.earth_azimuth = earth_azimuth;
    packet.earth_elevation = earth_elevation;
    packet.earth_polarity = earth_polarity;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE, (const char *)&packet, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
#endif
}

/**
 * @brief Send a report_antenna_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_antenna_state_send_struct(mavlink_channel_t chan, const mavlink_report_antenna_state_t* report_antenna_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_antenna_state_send(chan, report_antenna_state->rx_phi, report_antenna_state->rx_theta, report_antenna_state->rx_polarity, report_antenna_state->rx_freq, report_antenna_state->rx_polarity_type, report_antenna_state->tx_phi, report_antenna_state->tx_theta, report_antenna_state->tx_polarity, report_antenna_state->tx_freq, report_antenna_state->tx_polarity_type, report_antenna_state->earth_azimuth, report_antenna_state->earth_elevation, report_antenna_state->earth_polarity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE, (const char *)report_antenna_state, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_antenna_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float rx_phi, float rx_theta, float rx_polarity, float rx_freq, uint8_t rx_polarity_type, float tx_phi, float tx_theta, float tx_polarity, float tx_freq, uint8_t tx_polarity_type, float earth_azimuth, float earth_elevation, float earth_polarity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, rx_phi);
    _mav_put_float(buf, 4, rx_theta);
    _mav_put_float(buf, 8, rx_polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, tx_phi);
    _mav_put_float(buf, 20, tx_theta);
    _mav_put_float(buf, 24, tx_polarity);
    _mav_put_float(buf, 28, tx_freq);
    _mav_put_float(buf, 32, earth_azimuth);
    _mav_put_float(buf, 36, earth_elevation);
    _mav_put_float(buf, 40, earth_polarity);
    _mav_put_uint8_t(buf, 44, rx_polarity_type);
    _mav_put_uint8_t(buf, 45, tx_polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE, buf, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
#else
    mavlink_report_antenna_state_t *packet = (mavlink_report_antenna_state_t *)msgbuf;
    packet->rx_phi = rx_phi;
    packet->rx_theta = rx_theta;
    packet->rx_polarity = rx_polarity;
    packet->rx_freq = rx_freq;
    packet->tx_phi = tx_phi;
    packet->tx_theta = tx_theta;
    packet->tx_polarity = tx_polarity;
    packet->tx_freq = tx_freq;
    packet->earth_azimuth = earth_azimuth;
    packet->earth_elevation = earth_elevation;
    packet->earth_polarity = earth_polarity;
    packet->rx_polarity_type = rx_polarity_type;
    packet->tx_polarity_type = tx_polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE, (const char *)packet, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_ANTENNA_STATE UNPACKING


/**
 * @brief Get field rx_phi from report_antenna_state message
 *
 * @return  接收方位角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_rx_phi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field rx_theta from report_antenna_state message
 *
 * @return  接收离轴角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_rx_theta(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field rx_polarity from report_antenna_state message
 *
 * @return  接收极化角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_rx_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field rx_freq from report_antenna_state message
 *
 * @return  接收射频频率(MHz)
 */
static inline float mavlink_msg_report_antenna_state_get_rx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field rx_polarity_type from report_antenna_state message
 *
 * @return  接收极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_report_antenna_state_get_rx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  44);
}

/**
 * @brief Get field tx_phi from report_antenna_state message
 *
 * @return  发射方位角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_tx_phi(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field tx_theta from report_antenna_state message
 *
 * @return  发射离轴角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_tx_theta(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field tx_polarity from report_antenna_state message
 *
 * @return  发射极化角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_tx_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field tx_freq from report_antenna_state message
 *
 * @return  发射射频频率(MHz)
 */
static inline float mavlink_msg_report_antenna_state_get_tx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field tx_polarity_type from report_antenna_state message
 *
 * @return  发射极化类型，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_report_antenna_state_get_tx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  45);
}

/**
 * @brief Get field earth_azimuth from report_antenna_state message
 *
 * @return  地理方位角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_earth_azimuth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field earth_elevation from report_antenna_state message
 *
 * @return  地理俯仰角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_earth_elevation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field earth_polarity from report_antenna_state message
 *
 * @return  地理极化角(°)
 */
static inline float mavlink_msg_report_antenna_state_get_earth_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Decode a report_antenna_state message into a struct
 *
 * @param msg The message to decode
 * @param report_antenna_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_antenna_state_decode(const mavlink_message_t* msg, mavlink_report_antenna_state_t* report_antenna_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_antenna_state->rx_phi = mavlink_msg_report_antenna_state_get_rx_phi(msg);
    report_antenna_state->rx_theta = mavlink_msg_report_antenna_state_get_rx_theta(msg);
    report_antenna_state->rx_polarity = mavlink_msg_report_antenna_state_get_rx_polarity(msg);
    report_antenna_state->rx_freq = mavlink_msg_report_antenna_state_get_rx_freq(msg);
    report_antenna_state->tx_phi = mavlink_msg_report_antenna_state_get_tx_phi(msg);
    report_antenna_state->tx_theta = mavlink_msg_report_antenna_state_get_tx_theta(msg);
    report_antenna_state->tx_polarity = mavlink_msg_report_antenna_state_get_tx_polarity(msg);
    report_antenna_state->tx_freq = mavlink_msg_report_antenna_state_get_tx_freq(msg);
    report_antenna_state->earth_azimuth = mavlink_msg_report_antenna_state_get_earth_azimuth(msg);
    report_antenna_state->earth_elevation = mavlink_msg_report_antenna_state_get_earth_elevation(msg);
    report_antenna_state->earth_polarity = mavlink_msg_report_antenna_state_get_earth_polarity(msg);
    report_antenna_state->rx_polarity_type = mavlink_msg_report_antenna_state_get_rx_polarity_type(msg);
    report_antenna_state->tx_polarity_type = mavlink_msg_report_antenna_state_get_tx_polarity_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN? msg->len : MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN;
        memset(report_antenna_state, 0, MAVLINK_MSG_ID_REPORT_ANTENNA_STATE_LEN);
    memcpy(report_antenna_state, _MAV_PAYLOAD(msg), len);
#endif
}
