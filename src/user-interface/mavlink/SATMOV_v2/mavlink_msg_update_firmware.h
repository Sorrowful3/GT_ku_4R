#pragma once
// MESSAGE UPDATE_FIRMWARE PACKING

#define MAVLINK_MSG_ID_UPDATE_FIRMWARE 8

MAVPACKED(
typedef struct __mavlink_update_firmware_t {
 uint8_t mode; /*<  升级模式，0x00升级APP，0x01升级高轨固件（闭环跟踪固件），0x02升级低轨固件（程序跟踪固件），0x03升级暗室测试固件*/
 char path[128]; /*<  软件或固件在ACU中的路径，包括文件名*/
}) mavlink_update_firmware_t;

#define MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN 129
#define MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN 129
#define MAVLINK_MSG_ID_8_LEN 129
#define MAVLINK_MSG_ID_8_MIN_LEN 129

#define MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC 68
#define MAVLINK_MSG_ID_8_CRC 68

#define MAVLINK_MSG_UPDATE_FIRMWARE_FIELD_PATH_LEN 128

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_UPDATE_FIRMWARE { \
    8, \
    "UPDATE_FIRMWARE", \
    2, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_update_firmware_t, mode) }, \
         { "path", NULL, MAVLINK_TYPE_CHAR, 128, 1, offsetof(mavlink_update_firmware_t, path) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_UPDATE_FIRMWARE { \
    "UPDATE_FIRMWARE", \
    2, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_update_firmware_t, mode) }, \
         { "path", NULL, MAVLINK_TYPE_CHAR, 128, 1, offsetof(mavlink_update_firmware_t, path) }, \
         } \
}
#endif

/**
 * @brief Pack a update_firmware message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  升级模式，0x00升级APP，0x01升级高轨固件（闭环跟踪固件），0x02升级低轨固件（程序跟踪固件），0x03升级暗室测试固件
 * @param path  软件或固件在ACU中的路径，包括文件名
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_firmware_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode, const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_char_array(buf, 1, path, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN);
#else
    mavlink_update_firmware_t packet;
    packet.mode = mode;
    mav_array_memcpy(packet.path, path, sizeof(char)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_FIRMWARE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
}

/**
 * @brief Pack a update_firmware message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  升级模式，0x00升级APP，0x01升级高轨固件（闭环跟踪固件），0x02升级低轨固件（程序跟踪固件），0x03升级暗室测试固件
 * @param path  软件或固件在ACU中的路径，包括文件名
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_update_firmware_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode,const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_char_array(buf, 1, path, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN);
#else
    mavlink_update_firmware_t packet;
    packet.mode = mode;
    mav_array_memcpy(packet.path, path, sizeof(char)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_UPDATE_FIRMWARE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
}

/**
 * @brief Encode a update_firmware struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param update_firmware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_firmware_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_update_firmware_t* update_firmware)
{
    return mavlink_msg_update_firmware_pack(system_id, component_id, msg, update_firmware->mode, update_firmware->path);
}

/**
 * @brief Encode a update_firmware struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param update_firmware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_update_firmware_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_update_firmware_t* update_firmware)
{
    return mavlink_msg_update_firmware_pack_chan(system_id, component_id, chan, msg, update_firmware->mode, update_firmware->path);
}

/**
 * @brief Send a update_firmware message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  升级模式，0x00升级APP，0x01升级高轨固件（闭环跟踪固件），0x02升级低轨固件（程序跟踪固件），0x03升级暗室测试固件
 * @param path  软件或固件在ACU中的路径，包括文件名
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_update_firmware_send(mavlink_channel_t chan, uint8_t mode, const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_char_array(buf, 1, path, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE, buf, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
#else
    mavlink_update_firmware_t packet;
    packet.mode = mode;
    mav_array_memcpy(packet.path, path, sizeof(char)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE, (const char *)&packet, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
#endif
}

/**
 * @brief Send a update_firmware message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_update_firmware_send_struct(mavlink_channel_t chan, const mavlink_update_firmware_t* update_firmware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_update_firmware_send(chan, update_firmware->mode, update_firmware->path);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE, (const char *)update_firmware, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
#endif
}

#if MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_update_firmware_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode, const char *path)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);
    _mav_put_char_array(buf, 1, path, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE, buf, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
#else
    mavlink_update_firmware_t *packet = (mavlink_update_firmware_t *)msgbuf;
    packet->mode = mode;
    mav_array_memcpy(packet->path, path, sizeof(char)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_UPDATE_FIRMWARE, (const char *)packet, MAVLINK_MSG_ID_UPDATE_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN, MAVLINK_MSG_ID_UPDATE_FIRMWARE_CRC);
#endif
}
#endif

#endif

// MESSAGE UPDATE_FIRMWARE UNPACKING


/**
 * @brief Get field mode from update_firmware message
 *
 * @return  升级模式，0x00升级APP，0x01升级高轨固件（闭环跟踪固件），0x02升级低轨固件（程序跟踪固件），0x03升级暗室测试固件
 */
static inline uint8_t mavlink_msg_update_firmware_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field path from update_firmware message
 *
 * @return  软件或固件在ACU中的路径，包括文件名
 */
static inline uint16_t mavlink_msg_update_firmware_get_path(const mavlink_message_t* msg, char *path)
{
    return _MAV_RETURN_char_array(msg, path, 128,  1);
}

/**
 * @brief Decode a update_firmware message into a struct
 *
 * @param msg The message to decode
 * @param update_firmware C-struct to decode the message contents into
 */
static inline void mavlink_msg_update_firmware_decode(const mavlink_message_t* msg, mavlink_update_firmware_t* update_firmware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    update_firmware->mode = mavlink_msg_update_firmware_get_mode(msg);
    mavlink_msg_update_firmware_get_path(msg, update_firmware->path);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN? msg->len : MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN;
        memset(update_firmware, 0, MAVLINK_MSG_ID_UPDATE_FIRMWARE_LEN);
    memcpy(update_firmware, _MAV_PAYLOAD(msg), len);
#endif
}
