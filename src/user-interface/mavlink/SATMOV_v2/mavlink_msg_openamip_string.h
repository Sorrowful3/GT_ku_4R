#pragma once
// MESSAGE OPENAMIP_STRING PACKING

#define MAVLINK_MSG_ID_OPENAMIP_STRING 130

MAVPACKED(
typedef struct __mavlink_openamip_string_t {
 char str[200]; /*<  OPENAMIP×Ö·û´®*/
 uint8_t size; /*<  ×Ö·û´®³¤¶È*/
}) mavlink_openamip_string_t;

#define MAVLINK_MSG_ID_OPENAMIP_STRING_LEN 201
#define MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN 201
#define MAVLINK_MSG_ID_130_LEN 201
#define MAVLINK_MSG_ID_130_MIN_LEN 201

#define MAVLINK_MSG_ID_OPENAMIP_STRING_CRC 165
#define MAVLINK_MSG_ID_130_CRC 165

#define MAVLINK_MSG_OPENAMIP_STRING_FIELD_STR_LEN 200

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_OPENAMIP_STRING { \
    130, \
    "OPENAMIP_STRING", \
    2, \
    {  { "str", NULL, MAVLINK_TYPE_CHAR, 200, 0, offsetof(mavlink_openamip_string_t, str) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 200, offsetof(mavlink_openamip_string_t, size) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_OPENAMIP_STRING { \
    "OPENAMIP_STRING", \
    2, \
    {  { "str", NULL, MAVLINK_TYPE_CHAR, 200, 0, offsetof(mavlink_openamip_string_t, str) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 200, offsetof(mavlink_openamip_string_t, size) }, \
         } \
}
#endif

/**
 * @brief Pack a openamip_string message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param str  OPENAMIP×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_openamip_string_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENAMIP_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN);
#else
    mavlink_openamip_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_OPENAMIP_STRING;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
}

/**
 * @brief Pack a openamip_string message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param str  OPENAMIP×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_openamip_string_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *str,uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENAMIP_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN);
#else
    mavlink_openamip_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_OPENAMIP_STRING;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
}

/**
 * @brief Encode a openamip_string struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param openamip_string C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_openamip_string_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_openamip_string_t* openamip_string)
{
    return mavlink_msg_openamip_string_pack(system_id, component_id, msg, openamip_string->str, openamip_string->size);
}

/**
 * @brief Encode a openamip_string struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param openamip_string C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_openamip_string_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_openamip_string_t* openamip_string)
{
    return mavlink_msg_openamip_string_pack_chan(system_id, component_id, chan, msg, openamip_string->str, openamip_string->size);
}

/**
 * @brief Send a openamip_string message
 * @param chan MAVLink channel to send the message
 *
 * @param str  OPENAMIP×Ö·û´®
 * @param size  ×Ö·û´®³¤¶È
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_openamip_string_send(mavlink_channel_t chan, const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENAMIP_STRING_LEN];
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENAMIP_STRING, buf, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
#else
    mavlink_openamip_string_t packet;
    packet.size = size;
    mav_array_memcpy(packet.str, str, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENAMIP_STRING, (const char *)&packet, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
#endif
}

/**
 * @brief Send a openamip_string message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_openamip_string_send_struct(mavlink_channel_t chan, const mavlink_openamip_string_t* openamip_string)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_openamip_string_send(chan, openamip_string->str, openamip_string->size);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENAMIP_STRING, (const char *)openamip_string, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
#endif
}

#if MAVLINK_MSG_ID_OPENAMIP_STRING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_openamip_string_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *str, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 200, size);
    _mav_put_char_array(buf, 0, str, 200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENAMIP_STRING, buf, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
#else
    mavlink_openamip_string_t *packet = (mavlink_openamip_string_t *)msgbuf;
    packet->size = size;
    mav_array_memcpy(packet->str, str, sizeof(char)*200);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENAMIP_STRING, (const char *)packet, MAVLINK_MSG_ID_OPENAMIP_STRING_MIN_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN, MAVLINK_MSG_ID_OPENAMIP_STRING_CRC);
#endif
}
#endif

#endif

// MESSAGE OPENAMIP_STRING UNPACKING


/**
 * @brief Get field str from openamip_string message
 *
 * @return  OPENAMIP×Ö·û´®
 */
static inline uint16_t mavlink_msg_openamip_string_get_str(const mavlink_message_t* msg, char *str)
{
    return _MAV_RETURN_char_array(msg, str, 200,  0);
}

/**
 * @brief Get field size from openamip_string message
 *
 * @return  ×Ö·û´®³¤¶È
 */
static inline uint8_t mavlink_msg_openamip_string_get_size(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  200);
}

/**
 * @brief Decode a openamip_string message into a struct
 *
 * @param msg The message to decode
 * @param openamip_string C-struct to decode the message contents into
 */
static inline void mavlink_msg_openamip_string_decode(const mavlink_message_t* msg, mavlink_openamip_string_t* openamip_string)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_openamip_string_get_str(msg, openamip_string->str);
    openamip_string->size = mavlink_msg_openamip_string_get_size(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_OPENAMIP_STRING_LEN? msg->len : MAVLINK_MSG_ID_OPENAMIP_STRING_LEN;
        memset(openamip_string, 0, MAVLINK_MSG_ID_OPENAMIP_STRING_LEN);
    memcpy(openamip_string, _MAV_PAYLOAD(msg), len);
#endif
}
