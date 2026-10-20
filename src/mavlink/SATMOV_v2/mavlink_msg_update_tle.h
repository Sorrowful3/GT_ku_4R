#pragma once
// MESSAGE UPDATE_TLE PACKING

#define MAVLINK_MSG_ID_UPDATE_TLE 49


typedef struct __mavlink_update_tle_t {
 char path[128]; /*<  TLE在ACU中的路径，包括文件名*/
} mavlink_update_tle_t;

#define MAVLINK_MSG_ID_UPDATE_TLE_LEN 128
#define MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN 128
#define MAVLINK_MSG_ID_49_LEN 128
#define MAVLINK_MSG_ID_49_MIN_LEN 128

#define MAVLINK_MSG_ID_UPDATE_TLE_CRC 99
#define MAVLINK_MSG_ID_49_CRC 99

#define MAVLINK_MSG_UPDATE_TLE_FIELD_PATH_LEN 128

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_UPDATE_TLE { \
    49, \
    "UPDATE_TLE", \
    1, \
    {  { "path", NULL, MAVLINK_TYPE_CHAR, 128, 0, offsetof(mavlink_update_tle_t, path) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_UPDATE_TLE { \
    "UPDATE_TLE", \
    1, \
    {  { "path", NULL, MAVLINK_TYPE_CHAR, 128, 0, offsetof(mavlink_update_tle_t, path) }, \
         } \
}
#endif

/**
 * @brief Pack a update_tle message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param path  TLE在ACU中的路径，包括文件名
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_tle_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_LEN];

    _mav_put_char_array(buf, 0, path, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_TLE_LEN);
#else
    mavlink_update_tle_t packet;

    mav_array_memcpy(packet.path, path, sizeof(char)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_TLE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_TLE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
}

/**
 * @brief Pack a update_tle message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param path  TLE在ACU中的路径，包括文件名
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_tle_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_LEN];

    _mav_put_char_array(buf, 0, path, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_TLE_LEN);
#else
    mavlink_update_tle_t packet;

    mav_array_memcpy(packet.path, path, sizeof(char)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_TLE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_TLE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
}

/**
 * @brief Encode a update_tle struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param update_tle C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_tle_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_update_tle_t* update_tle)
{
    return mavlink_msg_update_tle_pack(system_id, component_id, msg, update_tle->path);
}

/**
 * @brief Encode a update_tle struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param update_tle C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_tle_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_update_tle_t* update_tle)
{
    return mavlink_msg_update_tle_pack_chan(system_id, component_id, chan, msg, update_tle->path);
}

/**
 * @brief Send a update_tle message
 * @param chan MAVLink channel to send the message
 *
 * @param path  TLE在ACU中的路径，包括文件名
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_update_tle_send(mavlink_channel_t chan, const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_TLE_LEN];

    _mav_put_char_array(buf, 0, path, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE, buf, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
#else
    mavlink_update_tle_t packet;

    mav_array_memcpy(packet.path, path, sizeof(char)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE, (const char *)&packet, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
#endif
}

/**
 * @brief Send a update_tle message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_update_tle_send_struct(mavlink_channel_t chan, const mavlink_update_tle_t* update_tle)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_update_tle_send(chan, update_tle->path);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE, (const char *)update_tle, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
#endif
}

#if MAVLINK_MSG_ID_UPDATE_TLE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_update_tle_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_char_array(buf, 0, path, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE, buf, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
#else
    mavlink_update_tle_t *packet = (mavlink_update_tle_t *)msgbuf;

    mav_array_memcpy(packet->path, path, sizeof(char)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_TLE, (const char *)packet, MAVLINK_MSG_ID_UPDATE_TLE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_TLE_LEN, MAVLINK_MSG_ID_UPDATE_TLE_CRC);
#endif
}
#endif

#endif

// MESSAGE UPDATE_TLE UNPACKING


/**
 * @brief Get field path from update_tle message
 *
 * @return  TLE在ACU中的路径，包括文件名
 */
static inline uint16_t mavlink_msg_update_tle_get_path(const mavlink_message_t* msg, char *path)
{
    return _MAV_RETURN_char_array(msg, path, 128,  0);
}

/**
 * @brief Decode a update_tle message into a struct
 *
 * @param msg The message to decode
 * @param update_tle C-struct to decode the message contents into
 */
static inline void mavlink_msg_update_tle_decode(const mavlink_message_t* msg, mavlink_update_tle_t* update_tle)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_update_tle_get_path(msg, update_tle->path);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_UPDATE_TLE_LEN? msg->len : MAVLINK_MSG_ID_UPDATE_TLE_LEN;
        memset(update_tle, 0, MAVLINK_MSG_ID_UPDATE_TLE_LEN);
    memcpy(update_tle, _MAV_PAYLOAD(msg), len);
#endif
}
