#pragma once
// MESSAGE SET_TRANSDUCER PACKING

#define MAVLINK_MSG_ID_SET_TRANSDUCER 20

MAVPACKED(
typedef struct __mavlink_set_transducer_t {
 float rx_osc; /*<  接收本振(MHz)*/
 float tx_osc; /*<  发射本振(MHz)*/
 uint8_t rx_gain; /*<  接收增益(dB)*/
 uint8_t tx_gain; /*<  发射增益(dB)*/
}) mavlink_set_transducer_t;

#define MAVLINK_MSG_ID_SET_TRANSDUCER_LEN 10
#define MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN 10
#define MAVLINK_MSG_ID_20_LEN 10
#define MAVLINK_MSG_ID_20_MIN_LEN 10

#define MAVLINK_MSG_ID_SET_TRANSDUCER_CRC 189
#define MAVLINK_MSG_ID_20_CRC 189



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SET_TRANSDUCER { \
    20, \
    "SET_TRANSDUCER", \
    4, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_transducer_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_set_transducer_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_transducer_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_set_transducer_t, tx_gain) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SET_TRANSDUCER { \
    "SET_TRANSDUCER", \
    4, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_set_transducer_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_set_transducer_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_set_transducer_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_set_transducer_t, tx_gain) }, \
         } \
}
#endif

/**
 * @brief Pack a set_transducer message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rx_osc  接收本振(MHz)
 * @param rx_gain  接收增益(dB)
 * @param tx_osc  发射本振(MHz)
 * @param tx_gain  发射增益(dB)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_transducer_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRANSDUCER_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_uint8_t(buf, 8, rx_gain);
    _mav_put_uint8_t(buf, 9, tx_gain);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN);
#else
    mavlink_set_transducer_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_TRANSDUCER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
}

/**
 * @brief Pack a set_transducer message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx_osc  接收本振(MHz)
 * @param rx_gain  接收增益(dB)
 * @param tx_osc  发射本振(MHz)
 * @param tx_gain  发射增益(dB)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_set_transducer_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float rx_osc,uint8_t rx_gain,float tx_osc,uint8_t tx_gain)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRANSDUCER_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_uint8_t(buf, 8, rx_gain);
    _mav_put_uint8_t(buf, 9, tx_gain);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN);
#else
    mavlink_set_transducer_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SET_TRANSDUCER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
}

/**
 * @brief Encode a set_transducer struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param set_transducer C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_transducer_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_set_transducer_t* set_transducer)
{
    return mavlink_msg_set_transducer_pack(system_id, component_id, msg, set_transducer->rx_osc, set_transducer->rx_gain, set_transducer->tx_osc, set_transducer->tx_gain);
}

/**
 * @brief Encode a set_transducer struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param set_transducer C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_set_transducer_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_set_transducer_t* set_transducer)
{
    return mavlink_msg_set_transducer_pack_chan(system_id, component_id, chan, msg, set_transducer->rx_osc, set_transducer->rx_gain, set_transducer->tx_osc, set_transducer->tx_gain);
}

/**
 * @brief Send a set_transducer message
 * @param chan MAVLink channel to send the message
 *
 * @param rx_osc  接收本振(MHz)
 * @param rx_gain  接收增益(dB)
 * @param tx_osc  发射本振(MHz)
 * @param tx_gain  发射增益(dB)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_set_transducer_send(mavlink_channel_t chan, float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SET_TRANSDUCER_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_uint8_t(buf, 8, rx_gain);
    _mav_put_uint8_t(buf, 9, tx_gain);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRANSDUCER, buf, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
#else
    mavlink_set_transducer_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRANSDUCER, (const char *)&packet, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
#endif
}

/**
 * @brief Send a set_transducer message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_set_transducer_send_struct(mavlink_channel_t chan, const mavlink_set_transducer_t* set_transducer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_set_transducer_send(chan, set_transducer->rx_osc, set_transducer->rx_gain, set_transducer->tx_osc, set_transducer->tx_gain);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRANSDUCER, (const char *)set_transducer, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
#endif
}

#if MAVLINK_MSG_ID_SET_TRANSDUCER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_set_transducer_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_uint8_t(buf, 8, rx_gain);
    _mav_put_uint8_t(buf, 9, tx_gain);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRANSDUCER, buf, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
#else
    mavlink_set_transducer_t *packet = (mavlink_set_transducer_t *)msgbuf;
    packet->rx_osc = rx_osc;
    packet->tx_osc = tx_osc;
    packet->rx_gain = rx_gain;
    packet->tx_gain = tx_gain;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SET_TRANSDUCER, (const char *)packet, MAVLINK_MSG_ID_SET_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN, MAVLINK_MSG_ID_SET_TRANSDUCER_CRC);
#endif
}
#endif

#endif

// MESSAGE SET_TRANSDUCER UNPACKING


/**
 * @brief Get field rx_osc from set_transducer message
 *
 * @return  接收本振(MHz)
 */
static inline float mavlink_msg_set_transducer_get_rx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field rx_gain from set_transducer message
 *
 * @return  接收增益(dB)
 */
static inline uint8_t mavlink_msg_set_transducer_get_rx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field tx_osc from set_transducer message
 *
 * @return  发射本振(MHz)
 */
static inline float mavlink_msg_set_transducer_get_tx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field tx_gain from set_transducer message
 *
 * @return  发射增益(dB)
 */
static inline uint8_t mavlink_msg_set_transducer_get_tx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Decode a set_transducer message into a struct
 *
 * @param msg The message to decode
 * @param set_transducer C-struct to decode the message contents into
 */
static inline void mavlink_msg_set_transducer_decode(const mavlink_message_t* msg, mavlink_set_transducer_t* set_transducer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    set_transducer->rx_osc = mavlink_msg_set_transducer_get_rx_osc(msg);
    set_transducer->tx_osc = mavlink_msg_set_transducer_get_tx_osc(msg);
    set_transducer->rx_gain = mavlink_msg_set_transducer_get_rx_gain(msg);
    set_transducer->tx_gain = mavlink_msg_set_transducer_get_tx_gain(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SET_TRANSDUCER_LEN? msg->len : MAVLINK_MSG_ID_SET_TRANSDUCER_LEN;
        memset(set_transducer, 0, MAVLINK_MSG_ID_SET_TRANSDUCER_LEN);
    memcpy(set_transducer, _MAV_PAYLOAD(msg), len);
#endif
}
