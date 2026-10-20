#pragma once
// MESSAGE NEW_LOG_FILE PACKING

#define MAVLINK_MSG_ID_NEW_LOG_FILE 34

MAVPACKED(
typedef struct __mavlink_new_log_file_t {
 uint8_t reserved; /*<  Ô¤Áô*/
}) mavlink_new_log_file_t;

#define MAVLINK_MSG_ID_NEW_LOG_FILE_LEN 1
#define MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN 1
#define MAVLINK_MSG_ID_34_LEN 1
#define MAVLINK_MSG_ID_34_MIN_LEN 1

#define MAVLINK_MSG_ID_NEW_LOG_FILE_CRC 234
#define MAVLINK_MSG_ID_34_CRC 234



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_NEW_LOG_FILE { \
    34, \
    "NEW_LOG_FILE", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_new_log_file_t, reserved) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_NEW_LOG_FILE { \
    "NEW_LOG_FILE", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_new_log_file_t, reserved) }, \
         } \
}
#endif

/**
 * @brief Pack a new_log_file message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_new_log_file_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_LOG_FILE_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN);
#else
    mavlink_new_log_file_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_NEW_LOG_FILE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
}

/**
 * @brief Pack a new_log_file message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_new_log_file_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_LOG_FILE_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN);
#else
    mavlink_new_log_file_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_NEW_LOG_FILE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
}

/**
 * @brief Encode a new_log_file struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param new_log_file C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_new_log_file_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_new_log_file_t* new_log_file)
{
    return mavlink_msg_new_log_file_pack(system_id, component_id, msg, new_log_file->reserved);
}

/**
 * @brief Encode a new_log_file struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param new_log_file C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_new_log_file_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_new_log_file_t* new_log_file)
{
    return mavlink_msg_new_log_file_pack_chan(system_id, component_id, chan, msg, new_log_file->reserved);
}

/**
 * @brief Send a new_log_file message
 * @param chan MAVLink channel to send the message
 *
 * @param reserved  Ô¤Áô
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_new_log_file_send(mavlink_channel_t chan, uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_NEW_LOG_FILE_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_LOG_FILE, buf, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
#else
    mavlink_new_log_file_t packet;
    packet.reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_LOG_FILE, (const char *)&packet, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
#endif
}

/**
 * @brief Send a new_log_file message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_new_log_file_send_struct(mavlink_channel_t chan, const mavlink_new_log_file_t* new_log_file)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_new_log_file_send(chan, new_log_file->reserved);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_LOG_FILE, (const char *)new_log_file, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
#endif
}

#if MAVLINK_MSG_ID_NEW_LOG_FILE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_new_log_file_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_LOG_FILE, buf, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
#else
    mavlink_new_log_file_t *packet = (mavlink_new_log_file_t *)msgbuf;
    packet->reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NEW_LOG_FILE, (const char *)packet, MAVLINK_MSG_ID_NEW_LOG_FILE_MIN_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN, MAVLINK_MSG_ID_NEW_LOG_FILE_CRC);
#endif
}
#endif

#endif

// MESSAGE NEW_LOG_FILE UNPACKING


/**
 * @brief Get field reserved from new_log_file message
 *
 * @return  Ô¤Áô
 */
static inline uint8_t mavlink_msg_new_log_file_get_reserved(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a new_log_file message into a struct
 *
 * @param msg The message to decode
 * @param new_log_file C-struct to decode the message contents into
 */
static inline void mavlink_msg_new_log_file_decode(const mavlink_message_t* msg, mavlink_new_log_file_t* new_log_file)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    new_log_file->reserved = mavlink_msg_new_log_file_get_reserved(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_NEW_LOG_FILE_LEN? msg->len : MAVLINK_MSG_ID_NEW_LOG_FILE_LEN;
        memset(new_log_file, 0, MAVLINK_MSG_ID_NEW_LOG_FILE_LEN);
    memcpy(new_log_file, _MAV_PAYLOAD(msg), len);
#endif
}
