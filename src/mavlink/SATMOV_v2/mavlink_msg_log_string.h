#pragma once
// MESSAGE LOG_STRING PACKING

#define MAVLINK_MSG_ID_LOG_STRING 120


typedef struct __mavlink_log_string_t {
 char str[200]; /*<  log×Ö·û´®*/
 uint8_t size; /*<  ×Ö·û´®³¤¶È*/
} mavlink_log_string_t;

#define MAVLINK_MSG_ID_LOG_STRING_LEN 201
#define MAVLINK_MSG_ID_LOG_STRING_MIN_LEN 201
#define MAVLINK_MSG_ID_120_LEN 201
#define MAVLINK_MSG_ID_120_MIN_LEN 201

#define MAVLINK_MSG_ID_LOG_STRING_CRC 196
#define MAVLINK_MSG_ID_120_CRC 196

#define MAVLINK_MSG_LOG_STRING_FIELD_STR_LEN 200

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LOG_STRING { \
    120, \
    "LOG_STRING", \
    2, \
    {  { "str", NULL, MAVLINK_TYPE_CHAR, 200, 0, offsetof(mavlink_log_string_t, str) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 200, offsetof(mavlink_log_string_t, size) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LOG_STRING { \
    "LOG_STRING", \
    2, \
    {  { "str", NULL, MAVLINK_TYPE_CHAR, 200, 0, offsetof(mavlink_log_string_t, str) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 200, offsetof(mavlink_log_string_t, size) }, \
         } \
}
#endif

/**
 * @brief Pack a log_string message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param str  log×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_log_string_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LOG_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LOG_STRING_LEN);
#else
    mavlink_log_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LOG_STRING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LOG_STRING;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
}

/**
 * @brief Pack a log_string message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param str  log×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_log_string_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *str,uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LOG_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LOG_STRING_LEN);
#else
    mavlink_log_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LOG_STRING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LOG_STRING;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
}

/**
 * @brief Encode a log_string struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param log_string C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_log_string_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_log_string_t* log_string)
{
    return mavlink_msg_log_string_pack(system_id, component_id, msg, log_string->str, log_string->size);
}

/**
 * @brief Encode a log_string struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param log_string C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_log_string_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_log_string_t* log_string)
{
    return mavlink_msg_log_string_pack_chan(system_id, component_id, chan, msg, log_string->str, log_string->size);
}

/**
 * @brief Send a log_string message
 * @param chan MAVLink channel to send the message
 *
 * @param str  log×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_log_string_send(mavlink_channel_t chan, const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LOG_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LOG_STRING, buf, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
#else
    mavlink_log_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LOG_STRING, (const char *)&packet, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
#endif
}

/**
 * @brief Send a log_string message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_log_string_send_struct(mavlink_channel_t chan, const mavlink_log_string_t* log_string)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_log_string_send(chan, log_string->str, log_string->size);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LOG_STRING, (const char *)log_string, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
#endif
}

#if MAVLINK_MSG_ID_LOG_STRING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_log_string_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LOG_STRING, buf, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
#else
    mavlink_log_string_t *packet = (mavlink_log_string_t *)msgbuf;
    packet->size = size;
    mav_array_memcpy(packet->str, str, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LOG_STRING, (const char *)packet, MAVLINK_MSG_ID_LOG_STRING_MIN_LEN, MAVLINK_MSG_ID_LOG_STRING_LEN, MAVLINK_MSG_ID_LOG_STRING_CRC);
#endif
}
#endif

#endif

// MESSAGE LOG_STRING UNPACKING


/**
 * @brief Get field str from log_string message
 *
 * @return  log×Ö·û´®
 */
static inline uint16_t mavlink_msg_log_string_get_str(const mavlink_message_t* msg, char *str)
{
    return _MAV_RETURN_char_array(msg, str, 200,  0);
}

/**
 * @brief Get field size from log_string message
 *
 * @return  ×Ö·û´®³¤¶È
 */
static inline uint8_t mavlink_msg_log_string_get_size(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  200);
}

/**
 * @brief Decode a log_string message into a struct
 *
 * @param msg The message to decode
 * @param log_string C-struct to decode the message contents into
 */
static inline void mavlink_msg_log_string_decode(const mavlink_message_t* msg, mavlink_log_string_t* log_string)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_log_string_get_str(msg, log_string->str);
    log_string->size = mavlink_msg_log_string_get_size(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LOG_STRING_LEN? msg->len : MAVLINK_MSG_ID_LOG_STRING_LEN;
        memset(log_string, 0, MAVLINK_MSG_ID_LOG_STRING_LEN);
    memcpy(log_string, _MAV_PAYLOAD(msg), len);
#endif
}
