#pragma once
// MESSAGE NEW_TRANSDUCER_RESULT PACKING

#define MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT 79


typedef struct __mavlink_new_transducer_result_t {
 float rx_osc; /*<  接收本振(MHz)*/
 float cal_freq; /*<  校准频率(MHz)*/
 uint8_t state_lock; /*<  锁定状态(0:失锁，1：锁定)*/
} mavlink_new_transducer_result_t;

#define MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN 9
#define MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN 9
#define MAVLINK_MSG_ID_79_LEN 9
#define MAVLINK_MSG_ID_79_MIN_LEN 9

#define MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC 43
#define MAVLINK_MSG_ID_79_CRC 43



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_NEW_TRANSDUCER_RESULT { \
    79, \
    "NEW_TRANSDUCER_RESULT", \
    3, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_new_transducer_result_t, rx_osc) }, \
         { "cal_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_new_transducer_result_t, cal_freq) }, \
         { "state_lock", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_new_transducer_result_t, state_lock) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_NEW_TRANSDUCER_RESULT { \
    "NEW_TRANSDUCER_RESULT", \
    3, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_new_transducer_result_t, rx_osc) }, \
         { "cal_freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_new_transducer_result_t, cal_freq) }, \
         { "state_lock", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_new_transducer_result_t, state_lock) }, \
         } \
}
#endif

/**
 * @brief Pack a new_transducer_result message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rx_osc  接收本振(MHz)
 * @param cal_freq  校准频率(MHz)
 * @param state_lock  锁定状态(0:失锁，1：锁定)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_new_transducer_result_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float rx_osc, float cal_freq, uint8_t state_lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, cal_freq);
    _mav_put_uint8_t(buf, 8, state_lock);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN);
#else
    mavlink_new_transducer_result_t packet;
    packet.rx_osc = rx_osc;
    packet.cal_freq = cal_freq;
    packet.state_lock = state_lock;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
}

/**
 * @brief Pack a new_transducer_result message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx_osc  接收本振(MHz)
 * @param cal_freq  校准频率(MHz)
 * @param state_lock  锁定状态(0:失锁，1：锁定)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_new_transducer_result_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float rx_osc,float cal_freq,uint8_t state_lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, cal_freq);
    _mav_put_uint8_t(buf, 8, state_lock);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN);
#else
    mavlink_new_transducer_result_t packet;
    packet.rx_osc = rx_osc;
    packet.cal_freq = cal_freq;
    packet.state_lock = state_lock;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
}

/**
 * @brief Encode a new_transducer_result struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param new_transducer_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_new_transducer_result_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_new_transducer_result_t* new_transducer_result)
{
    return mavlink_msg_new_transducer_result_pack(system_id, component_id, msg, new_transducer_result->rx_osc, new_transducer_result->cal_freq, new_transducer_result->state_lock);
}

/**
 * @brief Encode a new_transducer_result struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param new_transducer_result C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_new_transducer_result_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_new_transducer_result_t* new_transducer_result)
{
    return mavlink_msg_new_transducer_result_pack_chan(system_id, component_id, chan, msg, new_transducer_result->rx_osc, new_transducer_result->cal_freq, new_transducer_result->state_lock);
}

/**
 * @brief Send a new_transducer_result message
 * @param chan MAVLink channel to send the message
 *
 * @param rx_osc  接收本振(MHz)
 * @param cal_freq  校准频率(MHz)
 * @param state_lock  锁定状态(0:失锁，1：锁定)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_new_transducer_result_send(mavlink_channel_t chan, float rx_osc, float cal_freq, uint8_t state_lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, cal_freq);
    _mav_put_uint8_t(buf, 8, state_lock);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT, buf, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
#else
    mavlink_new_transducer_result_t packet;
    packet.rx_osc = rx_osc;
    packet.cal_freq = cal_freq;
    packet.state_lock = state_lock;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT, (const char *)&packet, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
#endif
}

/**
 * @brief Send a new_transducer_result message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_new_transducer_result_send_struct(mavlink_channel_t chan, const mavlink_new_transducer_result_t* new_transducer_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_new_transducer_result_send(chan, new_transducer_result->rx_osc, new_transducer_result->cal_freq, new_transducer_result->state_lock);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT, (const char *)new_transducer_result, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
#endif
}

#if MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_new_transducer_result_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float rx_osc, float cal_freq, uint8_t state_lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, cal_freq);
    _mav_put_uint8_t(buf, 8, state_lock);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT, buf, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
#else
    mavlink_new_transducer_result_t *packet = (mavlink_new_transducer_result_t *)msgbuf;
    packet->rx_osc = rx_osc;
    packet->cal_freq = cal_freq;
    packet->state_lock = state_lock;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT, (const char *)packet, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_MIN_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_CRC);
#endif
}
#endif

#endif

// MESSAGE NEW_TRANSDUCER_RESULT UNPACKING


/**
 * @brief Get field rx_osc from new_transducer_result message
 *
 * @return  接收本振(MHz)
 */
static inline float mavlink_msg_new_transducer_result_get_rx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field cal_freq from new_transducer_result message
 *
 * @return  校准频率(MHz)
 */
static inline float mavlink_msg_new_transducer_result_get_cal_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field state_lock from new_transducer_result message
 *
 * @return  锁定状态(0:失锁，1：锁定)
 */
static inline uint8_t mavlink_msg_new_transducer_result_get_state_lock(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Decode a new_transducer_result message into a struct
 *
 * @param msg The message to decode
 * @param new_transducer_result C-struct to decode the message contents into
 */
static inline void mavlink_msg_new_transducer_result_decode(const mavlink_message_t* msg, mavlink_new_transducer_result_t* new_transducer_result)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    new_transducer_result->rx_osc = mavlink_msg_new_transducer_result_get_rx_osc(msg);
    new_transducer_result->cal_freq = mavlink_msg_new_transducer_result_get_cal_freq(msg);
    new_transducer_result->state_lock = mavlink_msg_new_transducer_result_get_state_lock(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN? msg->len : MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN;
        memset(new_transducer_result, 0, MAVLINK_MSG_ID_NEW_TRANSDUCER_RESULT_LEN);
    memcpy(new_transducer_result, _MAV_PAYLOAD(msg), len);
#endif
}
