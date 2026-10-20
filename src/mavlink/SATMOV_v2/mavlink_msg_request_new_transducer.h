#pragma once
// MESSAGE REQUEST_NEW_TRANSDUCER PACKING

#define MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER 78


typedef struct __mavlink_request_new_transducer_t {
 uint8_t mode; /*<  模式，0x00用户请求，0x01对星固件请求*/
} mavlink_request_new_transducer_t;

#define MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN 1
#define MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN 1
#define MAVLINK_MSG_ID_78_LEN 1
#define MAVLINK_MSG_ID_78_MIN_LEN 1

#define MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC 85
#define MAVLINK_MSG_ID_78_CRC 85



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REQUEST_NEW_TRANSDUCER { \
    78, \
    "REQUEST_NEW_TRANSDUCER", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_request_new_transducer_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REQUEST_NEW_TRANSDUCER { \
    "REQUEST_NEW_TRANSDUCER", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_request_new_transducer_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a request_new_transducer message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_request_new_transducer_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN);
#else
    mavlink_request_new_transducer_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
}

/**
 * @brief Pack a request_new_transducer message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_request_new_transducer_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN);
#else
    mavlink_request_new_transducer_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
}

/**
 * @brief Encode a request_new_transducer struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param request_new_transducer C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_request_new_transducer_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_request_new_transducer_t* request_new_transducer)
{
    return mavlink_msg_request_new_transducer_pack(system_id, component_id, msg, request_new_transducer->mode);
}

/**
 * @brief Encode a request_new_transducer struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param request_new_transducer C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_request_new_transducer_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_request_new_transducer_t* request_new_transducer)
{
    return mavlink_msg_request_new_transducer_pack_chan(system_id, component_id, chan, msg, request_new_transducer->mode);
}

/**
 * @brief Send a request_new_transducer message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_request_new_transducer_send(mavlink_channel_t chan, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN];
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER, buf, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
#else
    mavlink_request_new_transducer_t packet;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER, (const char *)&packet, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
#endif
}

/**
 * @brief Send a request_new_transducer message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_request_new_transducer_send_struct(mavlink_channel_t chan, const mavlink_request_new_transducer_t* request_new_transducer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_request_new_transducer_send(chan, request_new_transducer->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER, (const char *)request_new_transducer, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
#endif
}

#if MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_request_new_transducer_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER, buf, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
#else
    mavlink_request_new_transducer_t *packet = (mavlink_request_new_transducer_t *)msgbuf;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER, (const char *)packet, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_MIN_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_CRC);
#endif
}
#endif

#endif

// MESSAGE REQUEST_NEW_TRANSDUCER UNPACKING


/**
 * @brief Get field mode from request_new_transducer message
 *
 * @return  模式，0x00用户请求，0x01对星固件请求
 */
static inline uint8_t mavlink_msg_request_new_transducer_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a request_new_transducer message into a struct
 *
 * @param msg The message to decode
 * @param request_new_transducer C-struct to decode the message contents into
 */
static inline void mavlink_msg_request_new_transducer_decode(const mavlink_message_t* msg, mavlink_request_new_transducer_t* request_new_transducer)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    request_new_transducer->mode = mavlink_msg_request_new_transducer_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN? msg->len : MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN;
        memset(request_new_transducer, 0, MAVLINK_MSG_ID_REQUEST_NEW_TRANSDUCER_LEN);
    memcpy(request_new_transducer, _MAV_PAYLOAD(msg), len);
#endif
}
