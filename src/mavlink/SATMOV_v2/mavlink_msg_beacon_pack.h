#pragma once
// MESSAGE BEACON_PACK PACKING

#define MAVLINK_MSG_ID_BEACON_PACK 62


typedef struct __mavlink_beacon_pack_t {
 uint8_t cmd[22]; /*<  命令数据*/
 uint8_t size; /*<  命令长度，多模机指令可能是变长的*/
} mavlink_beacon_pack_t;

#define MAVLINK_MSG_ID_BEACON_PACK_LEN 23
#define MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN 23
#define MAVLINK_MSG_ID_62_LEN 23
#define MAVLINK_MSG_ID_62_MIN_LEN 23

#define MAVLINK_MSG_ID_BEACON_PACK_CRC 105
#define MAVLINK_MSG_ID_62_CRC 105

#define MAVLINK_MSG_BEACON_PACK_FIELD_CMD_LEN 22

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_BEACON_PACK { \
    62, \
    "BEACON_PACK", \
    2, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 22, 0, offsetof(mavlink_beacon_pack_t, cmd) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_beacon_pack_t, size) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_BEACON_PACK { \
    "BEACON_PACK", \
    2, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 22, 0, offsetof(mavlink_beacon_pack_t, cmd) }, \
         { "size", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_beacon_pack_t, size) }, \
         } \
}
#endif

/**
 * @brief Pack a beacon_pack message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cmd  命令数据
 * @param size  命令长度，多模机指令可能是变长的
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_beacon_pack_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const uint8_t *cmd, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEACON_PACK_LEN];
    _mav_put_uint8_t(buf, 22, size);
    _mav_put_uint8_t_array(buf, 0, cmd, 22);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BEACON_PACK_LEN);
#else
    mavlink_beacon_pack_t packet;
    packet.size = size;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*22);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BEACON_PACK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BEACON_PACK;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
}

/**
 * @brief Pack a beacon_pack message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmd  命令数据
 * @param size  命令长度，多模机指令可能是变长的
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_beacon_pack_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const uint8_t *cmd,uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEACON_PACK_LEN];
    _mav_put_uint8_t(buf, 22, size);
    _mav_put_uint8_t_array(buf, 0, cmd, 22);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_BEACON_PACK_LEN);
#else
    mavlink_beacon_pack_t packet;
    packet.size = size;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*22);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_BEACON_PACK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_BEACON_PACK;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
}

/**
 * @brief Encode a beacon_pack struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param beacon_pack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_beacon_pack_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_beacon_pack_t* beacon_pack)
{
    return mavlink_msg_beacon_pack_pack(system_id, component_id, msg, beacon_pack->cmd, beacon_pack->size);
}

/**
 * @brief Encode a beacon_pack struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param beacon_pack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_beacon_pack_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_beacon_pack_t* beacon_pack)
{
    return mavlink_msg_beacon_pack_pack_chan(system_id, component_id, chan, msg, beacon_pack->cmd, beacon_pack->size);
}

/**
 * @brief Send a beacon_pack message
 * @param chan MAVLink channel to send the message
 *
 * @param cmd  命令数据
 * @param size  命令长度，多模机指令可能是变长的
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_beacon_pack_send(mavlink_channel_t chan, const uint8_t *cmd, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_BEACON_PACK_LEN];
    _mav_put_uint8_t(buf, 22, size);
    _mav_put_uint8_t_array(buf, 0, cmd, 22);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEACON_PACK, buf, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
#else
    mavlink_beacon_pack_t packet;
    packet.size = size;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*22);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEACON_PACK, (const char *)&packet, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
#endif
}

/**
 * @brief Send a beacon_pack message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_beacon_pack_send_struct(mavlink_channel_t chan, const mavlink_beacon_pack_t* beacon_pack)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_beacon_pack_send(chan, beacon_pack->cmd, beacon_pack->size);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEACON_PACK, (const char *)beacon_pack, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
#endif
}

#if MAVLINK_MSG_ID_BEACON_PACK_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_beacon_pack_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const uint8_t *cmd, uint8_t size)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 22, size);
    _mav_put_uint8_t_array(buf, 0, cmd, 22);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEACON_PACK, buf, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
#else
    mavlink_beacon_pack_t *packet = (mavlink_beacon_pack_t *)msgbuf;
    packet->size = size;
    mav_array_memcpy(packet->cmd, cmd, sizeof(uint8_t)*22);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_BEACON_PACK, (const char *)packet, MAVLINK_MSG_ID_BEACON_PACK_MIN_LEN, MAVLINK_MSG_ID_BEACON_PACK_LEN, MAVLINK_MSG_ID_BEACON_PACK_CRC);
#endif
}
#endif

#endif

// MESSAGE BEACON_PACK UNPACKING


/**
 * @brief Get field cmd from beacon_pack message
 *
 * @return  命令数据
 */
static inline uint16_t mavlink_msg_beacon_pack_get_cmd(const mavlink_message_t* msg, uint8_t *cmd)
{
    return _MAV_RETURN_uint8_t_array(msg, cmd, 22,  0);
}

/**
 * @brief Get field size from beacon_pack message
 *
 * @return  命令长度，多模机指令可能是变长的
 */
static inline uint8_t mavlink_msg_beacon_pack_get_size(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Decode a beacon_pack message into a struct
 *
 * @param msg The message to decode
 * @param beacon_pack C-struct to decode the message contents into
 */
static inline void mavlink_msg_beacon_pack_decode(const mavlink_message_t* msg, mavlink_beacon_pack_t* beacon_pack)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_beacon_pack_get_cmd(msg, beacon_pack->cmd);
    beacon_pack->size = mavlink_msg_beacon_pack_get_size(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_BEACON_PACK_LEN? msg->len : MAVLINK_MSG_ID_BEACON_PACK_LEN;
        memset(beacon_pack, 0, MAVLINK_MSG_ID_BEACON_PACK_LEN);
    memcpy(beacon_pack, _MAV_PAYLOAD(msg), len);
#endif
}
