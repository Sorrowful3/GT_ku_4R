#pragma once
// MESSAGE SET_BEACON PACKING

#define MAVLINK_MSG_ID_SET_BEACON 18

MAVPACKED(
typedef struct __mavlink_set_beacon_t {
 float freq; /*<  中频频率(MHz)*/
 float sym; /*<  符号速率(Ksps)*/
 uint8_t rang; /*<  信标捕获范围(kHZ)*/
 uint8_t mode; /*<  工作模式，0x00 DVB，0x01 检波，0x02 信标*/
}) mavlink_set_beacon_t;

#define MAVLINK_MSG_ID_SET_BEACON_LEN 10
#define MAVLINK_MSG_ID_SET_BEACON_MIN_LEN 10
#define MAVLINK_MSG_ID_18_LEN 10
#define MAVLINK_MSG_ID_18_MIN_LEN 10

#define MAVLINK_MSG_ID_SET_BEACON_CRC 30
#define MAVLINK_MSG_ID_18_CRC 30



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_BEACON { \
    18, \
    "SET_BEACON", \
    4, \
    {  { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_beacon_t, freq) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_beacon_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_set_beacon_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_set_beacon_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_BEACON { \
    "SET_BEACON", \
    4, \
    {  { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_beacon_t, freq) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_beacon_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_set_beacon_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_set_beacon_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a set_beacon message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param freq  中频频率(MHz)
 * @param sym  符号速率(Ksps)
 * @param rang  信标捕获范围(kHZ)
 * @param mode  工作模式，0x00 DVB，0x01 检波，0x02 信标
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_beacon_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEACON_LEN];
    _mav_put_float(buf, 0, freq);
    _mav_put_float(buf, 4, sym);
    _mav_put_uint8_t(buf, 8, rang);
    _mav_put_uint8_t(buf, 9, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_BEACON_LEN);
#else
    mavlink_set_beacon_t packet;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_BEACON_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_BEACON;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
}

/**
 * @brief Pack a set_beacon message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param freq  中频频率(MHz)
 * @param sym  符号速率(Ksps)
 * @param rang  信标捕获范围(kHZ)
 * @param mode  工作模式，0x00 DVB，0x01 检波，0x02 信标
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_beacon_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float freq,float sym,uint8_t rang,uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEACON_LEN];
    _mav_put_float(buf, 0, freq);
    _mav_put_float(buf, 4, sym);
    _mav_put_uint8_t(buf, 8, rang);
    _mav_put_uint8_t(buf, 9, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_BEACON_LEN);
#else
    mavlink_set_beacon_t packet;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_BEACON_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_BEACON;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
}

/**
 * @brief Encode a set_beacon struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_beacon C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_beacon_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_beacon_t* set_beacon)
{
    return mavlink_msg_set_beacon_pack(system_id, component_id, msg, set_beacon->freq, set_beacon->sym, set_beacon->rang, set_beacon->mode);
}

/**
 * @brief Encode a set_beacon struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_beacon C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_beacon_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_beacon_t* set_beacon)
{
    return mavlink_msg_set_beacon_pack_chan(system_id, component_id, chan, msg, set_beacon->freq, set_beacon->sym, set_beacon->rang, set_beacon->mode);
}

/**
 * @brief Send a set_beacon message
 * @param chan MAVLink channel to send the message
 *
 * @param freq  中频频率(MHz)
 * @param sym  符号速率(Ksps)
 * @param rang  信标捕获范围(kHZ)
 * @param mode  工作模式，0x00 DVB，0x01 检波，0x02 信标
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_beacon_send(mavlink_channel_t chan, float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_BEACON_LEN];
    _mav_put_float(buf, 0, freq);
    _mav_put_float(buf, 4, sym);
    _mav_put_uint8_t(buf, 8, rang);
    _mav_put_uint8_t(buf, 9, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEACON, buf, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
#else
    mavlink_set_beacon_t packet;
    packet.freq = freq;
    packet.sym = sym;
    packet.rang = rang;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEACON, (const char *)&packet, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
#endif
}

/**
 * @brief Send a set_beacon message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_beacon_send_struct(mavlink_channel_t chan, const mavlink_set_beacon_t* set_beacon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_beacon_send(chan, set_beacon->freq, set_beacon->sym, set_beacon->rang, set_beacon->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEACON, (const char *)set_beacon, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_BEACON_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_beacon_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float freq, float sym, uint8_t rang, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, freq);
    _mav_put_float(buf, 4, sym);
    _mav_put_uint8_t(buf, 8, rang);
    _mav_put_uint8_t(buf, 9, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEACON, buf, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
#else
    mavlink_set_beacon_t *packet = (mavlink_set_beacon_t *)msgbuf;
    packet->freq = freq;
    packet->sym = sym;
    packet->rang = rang;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_BEACON, (const char *)packet, MAVLINK_MSG_ID_SET_BEACON_MIN_LEN, MAVLINK_MSG_ID_SET_BEACON_LEN, MAVLINK_MSG_ID_SET_BEACON_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_BEACON UNPACKING


/**
 * @brief Get field freq from set_beacon message
 *
 * @return  中频频率(MHz)
 */
static inline float mavlink_msg_set_beacon_get_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field sym from set_beacon message
 *
 * @return  符号速率(Ksps)
 */
static inline float mavlink_msg_set_beacon_get_sym(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field rang from set_beacon message
 *
 * @return  信标捕获范围(kHZ)
 */
static inline uint8_t mavlink_msg_set_beacon_get_rang(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field mode from set_beacon message
 *
 * @return  工作模式，0x00 DVB，0x01 检波，0x02 信标
 */
static inline uint8_t mavlink_msg_set_beacon_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Decode a set_beacon message into a struct
 *
 * @param msg The message to decode
 * @param set_beacon C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_beacon_decode(const mavlink_message_t* msg, mavlink_set_beacon_t* set_beacon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_beacon->freq = mavlink_msg_set_beacon_get_freq(msg);
    set_beacon->sym = mavlink_msg_set_beacon_get_sym(msg);
    set_beacon->rang = mavlink_msg_set_beacon_get_rang(msg);
    set_beacon->mode = mavlink_msg_set_beacon_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_BEACON_LEN? msg->len : MAVLINK_MSG_ID_SET_BEACON_LEN;
        memset(set_beacon, 0, MAVLINK_MSG_ID_SET_BEACON_LEN);
    memcpy(set_beacon, _MAV_PAYLOAD(msg), len);
#endif
}
