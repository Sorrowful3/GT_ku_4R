#pragma once
// MESSAGE ARM_286B_PACK PACKING

#define MAVLINK_MSG_ID_ARM_286B_PACK 65

MAVPACKED(
typedef struct __mavlink_arm_286b_pack_t {
 uint8_t frame_count; /*<  帧计数，此命令分两次发送，第一次是1，第二次是2*/
 uint8_t code; /*<  两次发送的命令码需要一致*/
 uint8_t cmd[143]; /*<  命令载荷*/
}) mavlink_arm_286b_pack_t;

#define MAVLINK_MSG_ID_ARM_286B_PACK_LEN 145
#define MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN 145
#define MAVLINK_MSG_ID_65_LEN 145
#define MAVLINK_MSG_ID_65_MIN_LEN 145

#define MAVLINK_MSG_ID_ARM_286B_PACK_CRC 63
#define MAVLINK_MSG_ID_65_CRC 63

#define MAVLINK_MSG_ARM_286B_PACK_FIELD_CMD_LEN 143

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ARM_286B_PACK { \
    65, \
    "ARM_286B_PACK", \
    3, \
    {  { "frame_count", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_arm_286b_pack_t, frame_count) }, \
         { "code", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_arm_286b_pack_t, code) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 143, 2, offsetof(mavlink_arm_286b_pack_t, cmd) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ARM_286B_PACK { \
    "ARM_286B_PACK", \
    3, \
    {  { "frame_count", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_arm_286b_pack_t, frame_count) }, \
         { "code", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_arm_286b_pack_t, code) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 143, 2, offsetof(mavlink_arm_286b_pack_t, cmd) }, \
         } \
}
#endif

/**
 * @brief Pack a arm_286b_pack message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param frame_count  帧计数，此命令分两次发送，第一次是1，第二次是2
 * @param code  两次发送的命令码需要一致
 * @param cmd  命令载荷
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_arm_286b_pack_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t frame_count, uint8_t code, const uint8_t *cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ARM_286B_PACK_LEN];
    _mav_put_uint8_t(buf, 0, frame_count);
    _mav_put_uint8_t(buf, 1, code);
    _mav_put_uint8_t_array(buf, 2, cmd, 143);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ARM_286B_PACK_LEN);
#else
    mavlink_arm_286b_pack_t packet;
    packet.frame_count = frame_count;
    packet.code = code;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*143);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ARM_286B_PACK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ARM_286B_PACK;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
}

/**
 * @brief Pack a arm_286b_pack message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param frame_count  帧计数，此命令分两次发送，第一次是1，第二次是2
 * @param code  两次发送的命令码需要一致
 * @param cmd  命令载荷
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_arm_286b_pack_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t frame_count,uint8_t code,const uint8_t *cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ARM_286B_PACK_LEN];
    _mav_put_uint8_t(buf, 0, frame_count);
    _mav_put_uint8_t(buf, 1, code);
    _mav_put_uint8_t_array(buf, 2, cmd, 143);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ARM_286B_PACK_LEN);
#else
    mavlink_arm_286b_pack_t packet;
    packet.frame_count = frame_count;
    packet.code = code;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*143);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ARM_286B_PACK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ARM_286B_PACK;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
}

/**
 * @brief Encode a arm_286b_pack struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param arm_286b_pack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_arm_286b_pack_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_arm_286b_pack_t* arm_286b_pack)
{
    return mavlink_msg_arm_286b_pack_pack(system_id, component_id, msg, arm_286b_pack->frame_count, arm_286b_pack->code, arm_286b_pack->cmd);
}

/**
 * @brief Encode a arm_286b_pack struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param arm_286b_pack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_arm_286b_pack_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_arm_286b_pack_t* arm_286b_pack)
{
    return mavlink_msg_arm_286b_pack_pack_chan(system_id, component_id, chan, msg, arm_286b_pack->frame_count, arm_286b_pack->code, arm_286b_pack->cmd);
}

/**
 * @brief Send a arm_286b_pack message
 * @param chan MAVLink channel to send the message
 *
 * @param frame_count  帧计数，此命令分两次发送，第一次是1，第二次是2
 * @param code  两次发送的命令码需要一致
 * @param cmd  命令载荷
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_arm_286b_pack_send(mavlink_channel_t chan, uint8_t frame_count, uint8_t code, const uint8_t *cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ARM_286B_PACK_LEN];
    _mav_put_uint8_t(buf, 0, frame_count);
    _mav_put_uint8_t(buf, 1, code);
    _mav_put_uint8_t_array(buf, 2, cmd, 143);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ARM_286B_PACK, buf, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
#else
    mavlink_arm_286b_pack_t packet;
    packet.frame_count = frame_count;
    packet.code = code;
    mav_array_memcpy(packet.cmd, cmd, sizeof(uint8_t)*143);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ARM_286B_PACK, (const char *)&packet, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
#endif
}

/**
 * @brief Send a arm_286b_pack message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_arm_286b_pack_send_struct(mavlink_channel_t chan, const mavlink_arm_286b_pack_t* arm_286b_pack)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_arm_286b_pack_send(chan, arm_286b_pack->frame_count, arm_286b_pack->code, arm_286b_pack->cmd);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ARM_286B_PACK, (const char *)arm_286b_pack, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
#endif
}

#if MAVLINK_MSG_ID_ARM_286B_PACK_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_arm_286b_pack_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t frame_count, uint8_t code, const uint8_t *cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, frame_count);
    _mav_put_uint8_t(buf, 1, code);
    _mav_put_uint8_t_array(buf, 2, cmd, 143);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ARM_286B_PACK, buf, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
#else
    mavlink_arm_286b_pack_t *packet = (mavlink_arm_286b_pack_t *)msgbuf;
    packet->frame_count = frame_count;
    packet->code = code;
    mav_array_memcpy(packet->cmd, cmd, sizeof(uint8_t)*143);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ARM_286B_PACK, (const char *)packet, MAVLINK_MSG_ID_ARM_286B_PACK_MIN_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_LEN, MAVLINK_MSG_ID_ARM_286B_PACK_CRC);
#endif
}
#endif

#endif

// MESSAGE ARM_286B_PACK UNPACKING


/**
 * @brief Get field frame_count from arm_286b_pack message
 *
 * @return  帧计数，此命令分两次发送，第一次是1，第二次是2
 */
static inline uint8_t mavlink_msg_arm_286b_pack_get_frame_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field code from arm_286b_pack message
 *
 * @return  两次发送的命令码需要一致
 */
static inline uint8_t mavlink_msg_arm_286b_pack_get_code(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field cmd from arm_286b_pack message
 *
 * @return  命令载荷
 */
static inline uint16_t mavlink_msg_arm_286b_pack_get_cmd(const mavlink_message_t* msg, uint8_t *cmd)
{
    return _MAV_RETURN_uint8_t_array(msg, cmd, 143,  2);
}

/**
 * @brief Decode a arm_286b_pack message into a struct
 *
 * @param msg The message to decode
 * @param arm_286b_pack C-struct to decode the message contents into
 */
static inline void mavlink_msg_arm_286b_pack_decode(const mavlink_message_t* msg, mavlink_arm_286b_pack_t* arm_286b_pack)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    arm_286b_pack->frame_count = mavlink_msg_arm_286b_pack_get_frame_count(msg);
    arm_286b_pack->code = mavlink_msg_arm_286b_pack_get_code(msg);
    mavlink_msg_arm_286b_pack_get_cmd(msg, arm_286b_pack->cmd);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ARM_286B_PACK_LEN? msg->len : MAVLINK_MSG_ID_ARM_286B_PACK_LEN;
        memset(arm_286b_pack, 0, MAVLINK_MSG_ID_ARM_286B_PACK_LEN);
    memcpy(arm_286b_pack, _MAV_PAYLOAD(msg), len);
#endif
}
