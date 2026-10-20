#pragma once
// MESSAGE REPORT_BEACON_STATE PACKING

#define MAVLINK_MSG_ID_REPORT_BEACON_STATE 101

MAVPACKED(
typedef struct __mavlink_report_beacon_state_t {
 float cnr_power; /*<  信噪比或接收电平能量*/
 float freq; /*<  中频频率或信标频率(MHz)*/
 float sym; /*<  符号速率(ksps)，DVB或检波模式有效*/
 uint8_t rang; /*<  捕获范围(KHZ)，检波模式或信标模式有效*/
 uint8_t mode; /*<  多模机工作模式， 0x00 DVB，0x01 检波，0x02 信标*/
}) mavlink_report_beacon_state_t;

#define MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN 14
#define MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN 14
#define MAVLINK_MSG_ID_101_LEN 14
#define MAVLINK_MSG_ID_101_MIN_LEN 14

#define MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC 82
#define MAVLINK_MSG_ID_101_CRC 82



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_BEACON_STATE { \
    101, \
    "REPORT_BEACON_STATE", \
    5, \
    {  { "cnr_power", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_beacon_state_t, cnr_power) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_beacon_state_t, freq) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_beacon_state_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_report_beacon_state_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_report_beacon_state_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_BEACON_STATE { \
    "REPORT_BEACON_STATE", \
    5, \
    {  { "cnr_power", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_beacon_state_t, cnr_power) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_beacon_state_t, freq) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_beacon_state_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_report_beacon_state_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_report_beacon_state_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a report_beacon_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cnr_power  信噪比或接收电平能量
 * @param freq  中频频率或信标频率(MHz)
 * @param sym  符号速率(ksps)，DVB或检波模式有效
 * @param rang  捕获范围(KHZ)，检波模式或信标模式有效
 * @param mode  多模机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_beacon_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float cnr_power, float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN];
    _mav_put_float(buf, 0, cnr_power);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, sym);
    _mav_put_uint8_t(buf, 12, rang);
    _mav_put_uint8_t(buf, 13, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN);
#else
    mavlink_report_beacon_state_t packet;
    packet.cnr_power = cnr_power;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_BEACON_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
}

/**
 * @brief Pack a report_beacon_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cnr_power  信噪比或接收电平能量
 * @param freq  中频频率或信标频率(MHz)
 * @param sym  符号速率(ksps)，DVB或检波模式有效
 * @param rang  捕获范围(KHZ)，检波模式或信标模式有效
 * @param mode  多模机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_beacon_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float cnr_power,float freq,float sym,uint8_t rang,uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN];
    _mav_put_float(buf, 0, cnr_power);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, sym);
    _mav_put_uint8_t(buf, 12, rang);
    _mav_put_uint8_t(buf, 13, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN);
#else
    mavlink_report_beacon_state_t packet;
    packet.cnr_power = cnr_power;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_BEACON_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
}

/**
 * @brief Encode a report_beacon_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_beacon_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_beacon_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_beacon_state_t* report_beacon_state)
{
    return mavlink_msg_report_beacon_state_pack(system_id, component_id, msg, report_beacon_state->cnr_power, report_beacon_state->freq, report_beacon_state->sym, report_beacon_state->rang, report_beacon_state->mode);
}

/**
 * @brief Encode a report_beacon_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_beacon_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_beacon_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_beacon_state_t* report_beacon_state)
{
    return mavlink_msg_report_beacon_state_pack_chan(system_id, component_id, chan, msg, report_beacon_state->cnr_power, report_beacon_state->freq, report_beacon_state->sym, report_beacon_state->rang, report_beacon_state->mode);
}

/**
 * @brief Send a report_beacon_state message
 * @param chan MAVLink channel to send the message
 *
 * @param cnr_power  信噪比或接收电平能量
 * @param freq  中频频率或信标频率(MHz)
 * @param sym  符号速率(ksps)，DVB或检波模式有效
 * @param rang  捕获范围(KHZ)，检波模式或信标模式有效
 * @param mode  多模机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_beacon_state_send(mavlink_channel_t chan, float cnr_power, float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN];
    _mav_put_float(buf, 0, cnr_power);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, sym);
    _mav_put_uint8_t(buf, 12, rang);
    _mav_put_uint8_t(buf, 13, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE, buf, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
#else
    mavlink_report_beacon_state_t packet;
    packet.cnr_power = cnr_power;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE, (const char *)&packet, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
#endif
}

/**
 * @brief Send a report_beacon_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_beacon_state_send_struct(mavlink_channel_t chan, const mavlink_report_beacon_state_t* report_beacon_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_beacon_state_send(chan, report_beacon_state->cnr_power, report_beacon_state->freq, report_beacon_state->sym, report_beacon_state->rang, report_beacon_state->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE, (const char *)report_beacon_state, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_beacon_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float cnr_power, float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, cnr_power);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, sym);
    _mav_put_uint8_t(buf, 12, rang);
    _mav_put_uint8_t(buf, 13, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE, buf, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
#else
    mavlink_report_beacon_state_t *packet = (mavlink_report_beacon_state_t *)msgbuf;
    packet->cnr_power = cnr_power;
    packet->freq = freq;
    packet->sym = sym;
    packet->rang = rang;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_BEACON_STATE, (const char *)packet, MAVLINK_MSG_ID_REPORT_BEACON_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN, MAVLINK_MSG_ID_REPORT_BEACON_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_BEACON_STATE UNPACKING


/**
 * @brief Get field cnr_power from report_beacon_state message
 *
 * @return  信噪比或接收电平能量
 */
static inline float mavlink_msg_report_beacon_state_get_cnr_power(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field freq from report_beacon_state message
 *
 * @return  中频频率或信标频率(MHz)
 */
static inline float mavlink_msg_report_beacon_state_get_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field sym from report_beacon_state message
 *
 * @return  符号速率(ksps)，DVB或检波模式有效
 */
static inline float mavlink_msg_report_beacon_state_get_sym(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field rang from report_beacon_state message
 *
 * @return  捕获范围(KHZ)，检波模式或信标模式有效
 */
static inline uint8_t mavlink_msg_report_beacon_state_get_rang(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field mode from report_beacon_state message
 *
 * @return  多模机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 */
static inline uint8_t mavlink_msg_report_beacon_state_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Decode a report_beacon_state message into a struct
 *
 * @param msg The message to decode
 * @param report_beacon_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_beacon_state_decode(const mavlink_message_t* msg, mavlink_report_beacon_state_t* report_beacon_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_beacon_state->cnr_power = mavlink_msg_report_beacon_state_get_cnr_power(msg);
    report_beacon_state->freq = mavlink_msg_report_beacon_state_get_freq(msg);
    report_beacon_state->sym = mavlink_msg_report_beacon_state_get_sym(msg);
    report_beacon_state->rang = mavlink_msg_report_beacon_state_get_rang(msg);
    report_beacon_state->mode = mavlink_msg_report_beacon_state_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN? msg->len : MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN;
        memset(report_beacon_state, 0, MAVLINK_MSG_ID_REPORT_BEACON_STATE_LEN);
    memcpy(report_beacon_state, _MAV_PAYLOAD(msg), len);
#endif
}
