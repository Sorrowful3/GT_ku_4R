#pragma once
// MESSAGE BEAM_PARAMETER PACKING

#define MAVLINK_MSG_ID_BEAM_PARAMETER 41

MAVPACKED(
typedef struct __mavlink_beam_parameter_t {
 float sat_longitude; /*<  卫星经度(°)*/
 float rx_freq; /*<  下行频率(MHz)*/
 float tx_freq; /*<  上行频率(MHz)*/
 float rx_osc; /*<  下行变频器本振(MHz)*/
 float tx_osc; /*<  上行变频器本振(MHz)*/
 float sym; /*<  下行符号速率(Ksps)*/
 char sat_name[32]; /*<  卫星名称，为低轨闭环跟踪预留*/
 uint8_t rx_polarity_type; /*<  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
 uint8_t tx_polarity_type; /*<  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
 uint8_t rx_gain; /*<  下行变频器增益(dB)*/
 uint8_t tx_gain; /*<  上行变频器增益(dB)*/
 uint8_t rang; /*<  信标捕获范围(KHZ)*/
 uint8_t issue_mode; /*<  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复*/
}) mavlink_beam_parameter_t;

#define MAVLINK_MSG_ID_BEAM_PARAMETER_LEN 62
#define MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN 62
#define MAVLINK_MSG_ID_41_LEN 62
#define MAVLINK_MSG_ID_41_MIN_LEN 62

#define MAVLINK_MSG_ID_BEAM_PARAMETER_CRC 108
#define MAVLINK_MSG_ID_41_CRC 108

#define MAVLINK_MSG_BEAM_PARAMETER_FIELD_SAT_NAME_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_BEAM_PARAMETER { \
    41, \
    "BEAM_PARAMETER", \
    13, \
    {  { "sat_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_beam_parameter_t, sat_longitude) }, \
         { "sat_name", NULL, MAVLINK_TYPE_CHAR, 32, 24, offsetof(mavlink_beam_parameter_t, sat_name) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_beam_parameter_t, rx_freq) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 56, offsetof(mavlink_beam_parameter_t, rx_polarity_type) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_beam_parameter_t, tx_freq) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 57, offsetof(mavlink_beam_parameter_t, tx_polarity_type) }, \
         { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_beam_parameter_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 58, offsetof(mavlink_beam_parameter_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_beam_parameter_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 59, offsetof(mavlink_beam_parameter_t, tx_gain) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_beam_parameter_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_beam_parameter_t, rang) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_beam_parameter_t, issue_mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_BEAM_PARAMETER { \
    "BEAM_PARAMETER", \
    13, \
    {  { "sat_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_beam_parameter_t, sat_longitude) }, \
         { "sat_name", NULL, MAVLINK_TYPE_CHAR, 32, 24, offsetof(mavlink_beam_parameter_t, sat_name) }, \
         { "rx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_beam_parameter_t, rx_freq) }, \
         { "rx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 56, offsetof(mavlink_beam_parameter_t, rx_polarity_type) }, \
         { "tx_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_beam_parameter_t, tx_freq) }, \
         { "tx_polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 57, offsetof(mavlink_beam_parameter_t, tx_polarity_type) }, \
         { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_beam_parameter_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 58, offsetof(mavlink_beam_parameter_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_beam_parameter_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 59, offsetof(mavlink_beam_parameter_t, tx_gain) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_beam_parameter_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 60, offsetof(mavlink_beam_parameter_t, rang) }, \
         { "issue_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 61, offsetof(mavlink_beam_parameter_t, issue_mode) }, \
         } \
}
#endif

/**
 * @brief Pack a beam_parameter message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param sat_longitude  卫星经度(°)
 * @param sat_name  卫星名称，为低轨闭环跟踪预留
 * @param rx_freq  下行频率(MHz)
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param rx_osc  下行变频器本振(MHz)
 * @param rx_gain  下行变频器增益(dB)
 * @param tx_osc  上行变频器本振(MHz)
 * @param tx_gain  上行变频器增益(dB)
 * @param sym  下行符号速率(Ksps)
 * @param rang  信标捕获范围(KHZ)
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_beam_parameter_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float sat_longitude, const char *sat_name, float rx_freq, uint8_t rx_polarity_type, float tx_freq, uint8_t tx_polarity_type, float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float sym, uint8_t rang, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEAM_PARAMETER_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, rx_freq);
    _mav_put_float(buf, 8, tx_freq);
    _mav_put_float(buf, 12, rx_osc);
    _mav_put_float(buf, 16, tx_osc);
    _mav_put_float(buf, 20, sym);
    _mav_put_uint8_t(buf, 56, rx_polarity_type);
    _mav_put_uint8_t(buf, 57, tx_polarity_type);
    _mav_put_uint8_t(buf, 58, rx_gain);
    _mav_put_uint8_t(buf, 59, tx_gain);
    _mav_put_uint8_t(buf, 60, rang);
    _mav_put_uint8_t(buf, 61, issue_mode);
    _mav_put_char_array(buf, 24, sat_name, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN);
#else
    mavlink_beam_parameter_t packet;
    packet.sat_longitude = sat_longitude;
    packet.rx_freq = rx_freq;
    packet.tx_freq = tx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.sym = sym;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.rang = rang;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.sat_name, sat_name, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BEAM_PARAMETER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
}

/**
 * @brief Pack a beam_parameter message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sat_longitude  卫星经度(°)
 * @param sat_name  卫星名称，为低轨闭环跟踪预留
 * @param rx_freq  下行频率(MHz)
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param rx_osc  下行变频器本振(MHz)
 * @param rx_gain  下行变频器增益(dB)
 * @param tx_osc  上行变频器本振(MHz)
 * @param tx_gain  上行变频器增益(dB)
 * @param sym  下行符号速率(Ksps)
 * @param rang  信标捕获范围(KHZ)
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_beam_parameter_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float sat_longitude,const char *sat_name,float rx_freq,uint8_t rx_polarity_type,float tx_freq,uint8_t tx_polarity_type,float rx_osc,uint8_t rx_gain,float tx_osc,uint8_t tx_gain,float sym,uint8_t rang,uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEAM_PARAMETER_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, rx_freq);
    _mav_put_float(buf, 8, tx_freq);
    _mav_put_float(buf, 12, rx_osc);
    _mav_put_float(buf, 16, tx_osc);
    _mav_put_float(buf, 20, sym);
    _mav_put_uint8_t(buf, 56, rx_polarity_type);
    _mav_put_uint8_t(buf, 57, tx_polarity_type);
    _mav_put_uint8_t(buf, 58, rx_gain);
    _mav_put_uint8_t(buf, 59, tx_gain);
    _mav_put_uint8_t(buf, 60, rang);
    _mav_put_uint8_t(buf, 61, issue_mode);
    _mav_put_char_array(buf, 24, sat_name, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN);
#else
    mavlink_beam_parameter_t packet;
    packet.sat_longitude = sat_longitude;
    packet.rx_freq = rx_freq;
    packet.tx_freq = tx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.sym = sym;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.rang = rang;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.sat_name, sat_name, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BEAM_PARAMETER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
}

/**
 * @brief Encode a beam_parameter struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param beam_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_beam_parameter_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_beam_parameter_t* beam_parameter)
{
    return mavlink_msg_beam_parameter_pack(system_id, component_id, msg, beam_parameter->sat_longitude, beam_parameter->sat_name, beam_parameter->rx_freq, beam_parameter->rx_polarity_type, beam_parameter->tx_freq, beam_parameter->tx_polarity_type, beam_parameter->rx_osc, beam_parameter->rx_gain, beam_parameter->tx_osc, beam_parameter->tx_gain, beam_parameter->sym, beam_parameter->rang, beam_parameter->issue_mode);
}

/**
 * @brief Encode a beam_parameter struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param beam_parameter C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_beam_parameter_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_beam_parameter_t* beam_parameter)
{
    return mavlink_msg_beam_parameter_pack_chan(system_id, component_id, chan, msg, beam_parameter->sat_longitude, beam_parameter->sat_name, beam_parameter->rx_freq, beam_parameter->rx_polarity_type, beam_parameter->tx_freq, beam_parameter->tx_polarity_type, beam_parameter->rx_osc, beam_parameter->rx_gain, beam_parameter->tx_osc, beam_parameter->tx_gain, beam_parameter->sym, beam_parameter->rang, beam_parameter->issue_mode);
}

/**
 * @brief Send a beam_parameter message
 * @param chan MAVLink channel to send the message
 *
 * @param sat_longitude  卫星经度(°)
 * @param sat_name  卫星名称，为低轨闭环跟踪预留
 * @param rx_freq  下行频率(MHz)
 * @param rx_polarity_type  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param tx_freq  上行频率(MHz)
 * @param tx_polarity_type  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param rx_osc  下行变频器本振(MHz)
 * @param rx_gain  下行变频器增益(dB)
 * @param tx_osc  上行变频器本振(MHz)
 * @param tx_gain  上行变频器增益(dB)
 * @param sym  下行符号速率(Ksps)
 * @param rang  信标捕获范围(KHZ)
 * @param issue_mode  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_beam_parameter_send(mavlink_channel_t chan, float sat_longitude, const char *sat_name, float rx_freq, uint8_t rx_polarity_type, float tx_freq, uint8_t tx_polarity_type, float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float sym, uint8_t rang, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEAM_PARAMETER_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, rx_freq);
    _mav_put_float(buf, 8, tx_freq);
    _mav_put_float(buf, 12, rx_osc);
    _mav_put_float(buf, 16, tx_osc);
    _mav_put_float(buf, 20, sym);
    _mav_put_uint8_t(buf, 56, rx_polarity_type);
    _mav_put_uint8_t(buf, 57, tx_polarity_type);
    _mav_put_uint8_t(buf, 58, rx_gain);
    _mav_put_uint8_t(buf, 59, tx_gain);
    _mav_put_uint8_t(buf, 60, rang);
    _mav_put_uint8_t(buf, 61, issue_mode);
    _mav_put_char_array(buf, 24, sat_name, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEAM_PARAMETER, buf, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
#else
    mavlink_beam_parameter_t packet;
    packet.sat_longitude = sat_longitude;
    packet.rx_freq = rx_freq;
    packet.tx_freq = tx_freq;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.sym = sym;
    packet.rx_polarity_type = rx_polarity_type;
    packet.tx_polarity_type = tx_polarity_type;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.rang = rang;
    packet.issue_mode = issue_mode;
    mav_array_memcpy(packet.sat_name, sat_name, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEAM_PARAMETER, (const char *)&packet, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
#endif
}

/**
 * @brief Send a beam_parameter message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_beam_parameter_send_struct(mavlink_channel_t chan, const mavlink_beam_parameter_t* beam_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_beam_parameter_send(chan, beam_parameter->sat_longitude, beam_parameter->sat_name, beam_parameter->rx_freq, beam_parameter->rx_polarity_type, beam_parameter->tx_freq, beam_parameter->tx_polarity_type, beam_parameter->rx_osc, beam_parameter->rx_gain, beam_parameter->tx_osc, beam_parameter->tx_gain, beam_parameter->sym, beam_parameter->rang, beam_parameter->issue_mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEAM_PARAMETER, (const char *)beam_parameter, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
#endif
}

#if MAVLINK_MSG_ID_BEAM_PARAMETER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_beam_parameter_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float sat_longitude, const char *sat_name, float rx_freq, uint8_t rx_polarity_type, float tx_freq, uint8_t tx_polarity_type, float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float sym, uint8_t rang, uint8_t issue_mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, rx_freq);
    _mav_put_float(buf, 8, tx_freq);
    _mav_put_float(buf, 12, rx_osc);
    _mav_put_float(buf, 16, tx_osc);
    _mav_put_float(buf, 20, sym);
    _mav_put_uint8_t(buf, 56, rx_polarity_type);
    _mav_put_uint8_t(buf, 57, tx_polarity_type);
    _mav_put_uint8_t(buf, 58, rx_gain);
    _mav_put_uint8_t(buf, 59, tx_gain);
    _mav_put_uint8_t(buf, 60, rang);
    _mav_put_uint8_t(buf, 61, issue_mode);
    _mav_put_char_array(buf, 24, sat_name, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEAM_PARAMETER, buf, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
#else
    mavlink_beam_parameter_t *packet = (mavlink_beam_parameter_t *)msgbuf;
    packet->sat_longitude = sat_longitude;
    packet->rx_freq = rx_freq;
    packet->tx_freq = tx_freq;
    packet->rx_osc = rx_osc;
    packet->tx_osc = tx_osc;
    packet->sym = sym;
    packet->rx_polarity_type = rx_polarity_type;
    packet->tx_polarity_type = tx_polarity_type;
    packet->rx_gain = rx_gain;
    packet->tx_gain = tx_gain;
    packet->rang = rang;
    packet->issue_mode = issue_mode;
    mav_array_memcpy(packet->sat_name, sat_name, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEAM_PARAMETER, (const char *)packet, MAVLINK_MSG_ID_BEAM_PARAMETER_MIN_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN, MAVLINK_MSG_ID_BEAM_PARAMETER_CRC);
#endif
}
#endif

#endif

// MESSAGE BEAM_PARAMETER UNPACKING


/**
 * @brief Get field sat_longitude from beam_parameter message
 *
 * @return  卫星经度(°)
 */
static inline float mavlink_msg_beam_parameter_get_sat_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field sat_name from beam_parameter message
 *
 * @return  卫星名称，为低轨闭环跟踪预留
 */
static inline uint16_t mavlink_msg_beam_parameter_get_sat_name(const mavlink_message_t* msg, char *sat_name)
{
    return _MAV_RETURN_char_array(msg, sat_name, 32,  24);
}

/**
 * @brief Get field rx_freq from beam_parameter message
 *
 * @return  下行频率(MHz)
 */
static inline float mavlink_msg_beam_parameter_get_rx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field rx_polarity_type from beam_parameter message
 *
 * @return  下行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_beam_parameter_get_rx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  56);
}

/**
 * @brief Get field tx_freq from beam_parameter message
 *
 * @return  上行频率(MHz)
 */
static inline float mavlink_msg_beam_parameter_get_tx_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field tx_polarity_type from beam_parameter message
 *
 * @return  上行极化参考，0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_beam_parameter_get_tx_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  57);
}

/**
 * @brief Get field rx_osc from beam_parameter message
 *
 * @return  下行变频器本振(MHz)
 */
static inline float mavlink_msg_beam_parameter_get_rx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field rx_gain from beam_parameter message
 *
 * @return  下行变频器增益(dB)
 */
static inline uint8_t mavlink_msg_beam_parameter_get_rx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  58);
}

/**
 * @brief Get field tx_osc from beam_parameter message
 *
 * @return  上行变频器本振(MHz)
 */
static inline float mavlink_msg_beam_parameter_get_tx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field tx_gain from beam_parameter message
 *
 * @return  上行变频器增益(dB)
 */
static inline uint8_t mavlink_msg_beam_parameter_get_tx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  59);
}

/**
 * @brief Get field sym from beam_parameter message
 *
 * @return  下行符号速率(Ksps)
 */
static inline float mavlink_msg_beam_parameter_get_sym(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field rang from beam_parameter message
 *
 * @return  信标捕获范围(KHZ)
 */
static inline uint8_t mavlink_msg_beam_parameter_get_rang(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  60);
}

/**
 * @brief Get field issue_mode from beam_parameter message
 *
 * @return  指令下发模式模式， 0x00 用户主动发送，0x01 收到请求后回复
 */
static inline uint8_t mavlink_msg_beam_parameter_get_issue_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  61);
}

/**
 * @brief Decode a beam_parameter message into a struct
 *
 * @param msg The message to decode
 * @param beam_parameter C-struct to decode the message contents into
 */
static inline void mavlink_msg_beam_parameter_decode(const mavlink_message_t* msg, mavlink_beam_parameter_t* beam_parameter)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    beam_parameter->sat_longitude = mavlink_msg_beam_parameter_get_sat_longitude(msg);
    beam_parameter->rx_freq = mavlink_msg_beam_parameter_get_rx_freq(msg);
    beam_parameter->tx_freq = mavlink_msg_beam_parameter_get_tx_freq(msg);
    beam_parameter->rx_osc = mavlink_msg_beam_parameter_get_rx_osc(msg);
    beam_parameter->tx_osc = mavlink_msg_beam_parameter_get_tx_osc(msg);
    beam_parameter->sym = mavlink_msg_beam_parameter_get_sym(msg);
    mavlink_msg_beam_parameter_get_sat_name(msg, beam_parameter->sat_name);
    beam_parameter->rx_polarity_type = mavlink_msg_beam_parameter_get_rx_polarity_type(msg);
    beam_parameter->tx_polarity_type = mavlink_msg_beam_parameter_get_tx_polarity_type(msg);
    beam_parameter->rx_gain = mavlink_msg_beam_parameter_get_rx_gain(msg);
    beam_parameter->tx_gain = mavlink_msg_beam_parameter_get_tx_gain(msg);
    beam_parameter->rang = mavlink_msg_beam_parameter_get_rang(msg);
    beam_parameter->issue_mode = mavlink_msg_beam_parameter_get_issue_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_BEAM_PARAMETER_LEN? msg->len : MAVLINK_MSG_ID_BEAM_PARAMETER_LEN;
        memset(beam_parameter, 0, MAVLINK_MSG_ID_BEAM_PARAMETER_LEN);
    memcpy(beam_parameter, _MAV_PAYLOAD(msg), len);
#endif
}
