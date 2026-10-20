#pragma once
// MESSAGE SAVE_EARTH_POINT PACKING

#define MAVLINK_MSG_ID_SAVE_EARTH_POINT 74

MAVPACKED(
typedef struct __mavlink_save_earth_point_t {
 float elevation; /*<  地理离轴角(°)*/
 float azimuth; /*<  地理方位角(°)*/
 float polarity; /*<  地理极化角(°)*/
 float rx_freq; /*<  下行频率(MHz)*/
 float rx_osc; /*<  下行本振(MHz)*/
 float tx_freq; /*<  上行频率(MHz)*/
 float tx_osc; /*<  上行本振(MHz)*/
 uint32_t gps_week; /*<  GPS周*/
 uint32_t gps_second; /*<  GPS秒，精确到ms*/
 uint8_t rx_gain; /*<  下行增益*/
 uint8_t rx_polarity_type; /*<  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
 uint8_t tx_gain; /*<  上行增益*/
 uint8_t tx_polarity_type; /*<  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
}) mavlink_save_earth_point_t;

#define MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN 40
#define MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN 40
#define MAVLINK_MSG_ID_74_LEN 40
#define MAVLINK_MSG_ID_74_MIN_LEN 40

#define MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC 144
#define MAVLINK_MSG_ID_74_CRC 144



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SAVE_EARTH_POINT { \
    74, \
    "SAVE_EARTH_POINT", \
    13, \
    {  { "elevation", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_save_earth_point_t, elevation) }, \
         { "azimuth", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_save_earth_point_t, azimuth) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_save_earth_point_t, polarity) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_save_earth_point_t, rx_freq) }, \
         { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_save_earth_point_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_save_earth_point_t, rx_gain) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_save_earth_point_t, rx_polarity_type) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_save_earth_point_t, tx_freq) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_save_earth_point_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 38, offsetof(mavlink_save_earth_point_t, tx_gain) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 39, offsetof(mavlink_save_earth_point_t, tx_polarity_type) }, \
         { "gps_week", NULL, MAVLINK_TYPE_UINT32_T, 0, 28, offsetof(mavlink_save_earth_point_t, gps_week) }, \
         { "gps_second", NULL, MAVLINK_TYPE_UINT32_T, 0, 32, offsetof(mavlink_save_earth_point_t, gps_second) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SAVE_EARTH_POINT { \
    "SAVE_EARTH_POINT", \
    13, \
    {  { "elevation", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_save_earth_point_t, elevation) }, \
         { "azimuth", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_save_earth_point_t, azimuth) }, \
         { "polarity", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_save_earth_point_t, polarity) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_save_earth_point_t, rx_freq) }, \
         { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_save_earth_point_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 36, offsetof(mavlink_save_earth_point_t, rx_gain) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 37, offsetof(mavlink_save_earth_point_t, rx_polarity_type) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_save_earth_point_t, tx_freq) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_save_earth_point_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 38, offsetof(mavlink_save_earth_point_t, tx_gain) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 39, offsetof(mavlink_save_earth_point_t, tx_polarity_type) }, \
         { "gps_week", NULL, MAVLINK_TYPE_UINT32_T, 0, 28, offsetof(mavlink_save_earth_point_t, gps_week) }, \
         { "gps_second", NULL, MAVLINK_TYPE_UINT32_T, 0, 32, offsetof(mavlink_save_earth_point_t, gps_second) }, \
         } \
}
#endif

/**
 * @brief Pack a save_earth_point message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param elevation  地理离轴角(°)
 * @param azimuth  地理方位角(°)
 * @param polarity  地理极化角(°)
 * @param rx_freq  下行频率(MHz)
 * @param rx_osc  下行本振(MHz)
 * @param rx_gain  下行增益
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_osc  上行本振(MHz)
 * @param tx_gain  上行增益
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param gps_week  GPS周
 * @param gps_second  GPS秒，精确到ms
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_save_earth_point_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float elevation, float azimuth, float polarity, float rx_freq, float rx_osc, uint8_t rx_gain, uint8_t rx_polarity_type, float tx_freq, float tx_osc, uint8_t tx_gain, uint8_t tx_polarity_type, uint32_t gps_week, uint32_t gps_second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN];
    _mav_put_float(buf, 0, elevation);
    _mav_put_float(buf, 4, azimuth);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, rx_osc);
    _mav_put_float(buf, 20, tx_freq);
    _mav_put_float(buf, 24, tx_osc);
    _mav_put_uint32_t(buf, 28, gps_week);
    _mav_put_uint32_t(buf, 32, gps_second);
    _mav_put_uint8_t(buf, 36, rx_gain);
    _mav_put_uint8_t(buf, 37, rx_polarity_type);
    _mav_put_uint8_t(buf, 38, tx_gain);
    _mav_put_uint8_t(buf, 39, tx_polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN);
#else
    mavlink_save_earth_point_t packet;
    packet.elevation = elevation;
    packet.azimuth = azimuth;
    packet.polarity = polarity;
    packet.rx_freq = rx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_freq = tx_freq;
    packet.tx_osc = tx_osc;
    packet.gps_week = gps_week;
    packet.gps_second = gps_second;
    packet.rx_gain = rx_gain;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_gain = tx_gain;
    packet.tx_polarity_type = tx_polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SAVE_EARTH_POINT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
}

/**
 * @brief Pack a save_earth_point message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param elevation  地理离轴角(°)
 * @param azimuth  地理方位角(°)
 * @param polarity  地理极化角(°)
 * @param rx_freq  下行频率(MHz)
 * @param rx_osc  下行本振(MHz)
 * @param rx_gain  下行增益
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_osc  上行本振(MHz)
 * @param tx_gain  上行增益
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param gps_week  GPS周
 * @param gps_second  GPS秒，精确到ms
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_save_earth_point_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float elevation,float azimuth,float polarity,float rx_freq,float rx_osc,uint8_t rx_gain,uint8_t rx_polarity_type,float tx_freq,float tx_osc,uint8_t tx_gain,uint8_t tx_polarity_type,uint32_t gps_week,uint32_t gps_second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN];
    _mav_put_float(buf, 0, elevation);
    _mav_put_float(buf, 4, azimuth);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, rx_osc);
    _mav_put_float(buf, 20, tx_freq);
    _mav_put_float(buf, 24, tx_osc);
    _mav_put_uint32_t(buf, 28, gps_week);
    _mav_put_uint32_t(buf, 32, gps_second);
    _mav_put_uint8_t(buf, 36, rx_gain);
    _mav_put_uint8_t(buf, 37, rx_polarity_type);
    _mav_put_uint8_t(buf, 38, tx_gain);
    _mav_put_uint8_t(buf, 39, tx_polarity_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN);
#else
    mavlink_save_earth_point_t packet;
    packet.elevation = elevation;
    packet.azimuth = azimuth;
    packet.polarity = polarity;
    packet.rx_freq = rx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_freq = tx_freq;
    packet.tx_osc = tx_osc;
    packet.gps_week = gps_week;
    packet.gps_second = gps_second;
    packet.rx_gain = rx_gain;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_gain = tx_gain;
    packet.tx_polarity_type = tx_polarity_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SAVE_EARTH_POINT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
}

/**
 * @brief Encode a save_earth_point struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param save_earth_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_save_earth_point_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_save_earth_point_t* save_earth_point)
{
    return mavlink_msg_save_earth_point_pack(system_id, component_id, msg, save_earth_point->elevation, save_earth_point->azimuth, save_earth_point->polarity, save_earth_point->rx_freq, save_earth_point->rx_osc, save_earth_point->rx_gain, save_earth_point->rx_polarity_type, save_earth_point->tx_freq, save_earth_point->tx_osc, save_earth_point->tx_gain, save_earth_point->tx_polarity_type, save_earth_point->gps_week, save_earth_point->gps_second);
}

/**
 * @brief Encode a save_earth_point struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param save_earth_point C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_save_earth_point_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_save_earth_point_t* save_earth_point)
{
    return mavlink_msg_save_earth_point_pack_chan(system_id, component_id, chan, msg, save_earth_point->elevation, save_earth_point->azimuth, save_earth_point->polarity, save_earth_point->rx_freq, save_earth_point->rx_osc, save_earth_point->rx_gain, save_earth_point->rx_polarity_type, save_earth_point->tx_freq, save_earth_point->tx_osc, save_earth_point->tx_gain, save_earth_point->tx_polarity_type, save_earth_point->gps_week, save_earth_point->gps_second);
}

/**
 * @brief Send a save_earth_point message
 * @param chan MAVLink channel to send the message
 *
 * @param elevation  地理离轴角(°)
 * @param azimuth  地理方位角(°)
 * @param polarity  地理极化角(°)
 * @param rx_freq  下行频率(MHz)
 * @param rx_osc  下行本振(MHz)
 * @param rx_gain  下行增益
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_osc  上行本振(MHz)
 * @param tx_gain  上行增益
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param gps_week  GPS周
 * @param gps_second  GPS秒，精确到ms
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_save_earth_point_send(mavlink_channel_t chan, float elevation, float azimuth, float polarity, float rx_freq, float rx_osc, uint8_t rx_gain, uint8_t rx_polarity_type, float tx_freq, float tx_osc, uint8_t tx_gain, uint8_t tx_polarity_type, uint32_t gps_week, uint32_t gps_second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN];
    _mav_put_float(buf, 0, elevation);
    _mav_put_float(buf, 4, azimuth);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, rx_osc);
    _mav_put_float(buf, 20, tx_freq);
    _mav_put_float(buf, 24, tx_osc);
    _mav_put_uint32_t(buf, 28, gps_week);
    _mav_put_uint32_t(buf, 32, gps_second);
    _mav_put_uint8_t(buf, 36, rx_gain);
    _mav_put_uint8_t(buf, 37, rx_polarity_type);
    _mav_put_uint8_t(buf, 38, tx_gain);
    _mav_put_uint8_t(buf, 39, tx_polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT, buf, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
#else
    mavlink_save_earth_point_t packet;
    packet.elevation = elevation;
    packet.azimuth = azimuth;
    packet.polarity = polarity;
    packet.rx_freq = rx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_freq = tx_freq;
    packet.tx_osc = tx_osc;
    packet.gps_week = gps_week;
    packet.gps_second = gps_second;
    packet.rx_gain = rx_gain;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_gain = tx_gain;
    packet.tx_polarity_type = tx_polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT, (const char *)&packet, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
#endif
}

/**
 * @brief Send a save_earth_point message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_save_earth_point_send_struct(mavlink_channel_t chan, const mavlink_save_earth_point_t* save_earth_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_save_earth_point_send(chan, save_earth_point->elevation, save_earth_point->azimuth, save_earth_point->polarity, save_earth_point->rx_freq, save_earth_point->rx_osc, save_earth_point->rx_gain, save_earth_point->rx_polarity_type, save_earth_point->tx_freq, save_earth_point->tx_osc, save_earth_point->tx_gain, save_earth_point->tx_polarity_type, save_earth_point->gps_week, save_earth_point->gps_second);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT, (const char *)save_earth_point, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
#endif
}

#if MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_save_earth_point_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float elevation, float azimuth, float polarity, float rx_freq, float rx_osc, uint8_t rx_gain, uint8_t rx_polarity_type, float tx_freq, float tx_osc, uint8_t tx_gain, uint8_t tx_polarity_type, uint32_t gps_week, uint32_t gps_second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, elevation);
    _mav_put_float(buf, 4, azimuth);
    _mav_put_float(buf, 8, polarity);
    _mav_put_float(buf, 12, rx_freq);
    _mav_put_float(buf, 16, rx_osc);
    _mav_put_float(buf, 20, tx_freq);
    _mav_put_float(buf, 24, tx_osc);
    _mav_put_uint32_t(buf, 28, gps_week);
    _mav_put_uint32_t(buf, 32, gps_second);
    _mav_put_uint8_t(buf, 36, rx_gain);
    _mav_put_uint8_t(buf, 37, rx_polarity_type);
    _mav_put_uint8_t(buf, 38, tx_gain);
    _mav_put_uint8_t(buf, 39, tx_polarity_type);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT, buf, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
#else
    mavlink_save_earth_point_t *packet = (mavlink_save_earth_point_t *)msgbuf;
    packet->elevation = elevation;
    packet->azimuth = azimuth;
    packet->polarity = polarity;
    packet->rx_freq = rx_freq;
    packet->rx_osc = rx_osc;
    packet->tx_freq = tx_freq;
    packet->tx_osc = tx_osc;
    packet->gps_week = gps_week;
    packet->gps_second = gps_second;
    packet->rx_gain = rx_gain;
    packet->rx_polarity_type = rx_polarity_type;
    packet->tx_gain = tx_gain;
    packet->tx_polarity_type = tx_polarity_type;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SAVE_EARTH_POINT, (const char *)packet, MAVLINK_MSG_ID_SAVE_EARTH_POINT_MIN_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN, MAVLINK_MSG_ID_SAVE_EARTH_POINT_CRC);
#endif
}
#endif

#endif

// MESSAGE SAVE_EARTH_POINT UNPACKING


/**
 * @brief Get field elevation from save_earth_point message
 *
 * @return  地理离轴角(°)
 */
static inline float mavlink_msg_save_earth_point_get_elevation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field azimuth from save_earth_point message
 *
 * @return  地理方位角(°)
 */
static inline float mavlink_msg_save_earth_point_get_azimuth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field polarity from save_earth_point message
 *
 * @return  地理极化角(°)
 */
static inline float mavlink_msg_save_earth_point_get_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field rx_freq from save_earth_point message
 *
 * @return  下行频率(MHz)
 */
static inline float mavlink_msg_save_earth_point_get_rx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field rx_osc from save_earth_point message
 *
 * @return  下行本振(MHz)
 */
static inline float mavlink_msg_save_earth_point_get_rx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field rx_gain from save_earth_point message
 *
 * @return  下行增益
 */
static inline uint8_t mavlink_msg_save_earth_point_get_rx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  36);
}

/**
 * @brief Get field rx_polarity_type from save_earth_point message
 *
 * @return  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_save_earth_point_get_rx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  37);
}

/**
 * @brief Get field tx_freq from save_earth_point message
 *
 * @return  上行频率(MHz)
 */
static inline float mavlink_msg_save_earth_point_get_tx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field tx_osc from save_earth_point message
 *
 * @return  上行本振(MHz)
 */
static inline float mavlink_msg_save_earth_point_get_tx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field tx_gain from save_earth_point message
 *
 * @return  上行增益
 */
static inline uint8_t mavlink_msg_save_earth_point_get_tx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  38);
}

/**
 * @brief Get field tx_polarity_type from save_earth_point message
 *
 * @return  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_save_earth_point_get_tx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  39);
}

/**
 * @brief Get field gps_week from save_earth_point message
 *
 * @return  GPS周
 */
static inline uint32_t mavlink_msg_save_earth_point_get_gps_week(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  28);
}

/**
 * @brief Get field gps_second from save_earth_point message
 *
 * @return  GPS秒，精确到ms
 */
static inline uint32_t mavlink_msg_save_earth_point_get_gps_second(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  32);
}

/**
 * @brief Decode a save_earth_point message into a struct
 *
 * @param msg The message to decode
 * @param save_earth_point C-struct to decode the message contents into
 */
static inline void mavlink_msg_save_earth_point_decode(const mavlink_message_t* msg, mavlink_save_earth_point_t* save_earth_point)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    save_earth_point->elevation = mavlink_msg_save_earth_point_get_elevation(msg);
    save_earth_point->azimuth = mavlink_msg_save_earth_point_get_azimuth(msg);
    save_earth_point->polarity = mavlink_msg_save_earth_point_get_polarity(msg);
    save_earth_point->rx_freq = mavlink_msg_save_earth_point_get_rx_freq(msg);
    save_earth_point->rx_osc = mavlink_msg_save_earth_point_get_rx_osc(msg);
    save_earth_point->tx_freq = mavlink_msg_save_earth_point_get_tx_freq(msg);
    save_earth_point->tx_osc = mavlink_msg_save_earth_point_get_tx_osc(msg);
    save_earth_point->gps_week = mavlink_msg_save_earth_point_get_gps_week(msg);
    save_earth_point->gps_second = mavlink_msg_save_earth_point_get_gps_second(msg);
    save_earth_point->rx_gain = mavlink_msg_save_earth_point_get_rx_gain(msg);
    save_earth_point->rx_polarity_type = mavlink_msg_save_earth_point_get_rx_polarity_type(msg);
    save_earth_point->tx_gain = mavlink_msg_save_earth_point_get_tx_gain(msg);
    save_earth_point->tx_polarity_type = mavlink_msg_save_earth_point_get_tx_polarity_type(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN? msg->len : MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN;
        memset(save_earth_point, 0, MAVLINK_MSG_ID_SAVE_EARTH_POINT_LEN);
    memcpy(save_earth_point, _MAV_PAYLOAD(msg), len);
#endif
}
