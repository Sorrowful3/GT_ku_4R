#pragma once
// MESSAGE SWITCH_FIRMWARE PACKING

#define MAVLINK_MSG_ID_SWITCH_FIRMWARE 10

MAVPACKED(
typedef struct __mavlink_switch_firmware_t {
 uint8_t mode; /*<  ¹Ì¼þÀàÐÍ£¬0x01Ñ¡Ôñ¸ß¹ì¹Ì¼þ£¨±Õ»·¸ú×Ù¹Ì¼þ£©£¬0x02Ñ¡ÔñµÍ¹ì¹Ì¼þ£¨³ÌÐò¸ú×Ù¹Ì¼þ£©£¬0x03Ñ¡Ôñ°µÊÒ²âÊÔ¹Ì¼þ*/
}) mavlink_switch_firmware_t;

#define MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN 1
#define MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN 1
#define MAVLINK_MSG_ID_10_LEN 1
#define MAVLINK_MSG_ID_10_MIN_LEN 1

#define MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC 45
#define MAVLINK_MSG_ID_10_CRC 45



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SWITCH_FIRMWARE { \
    10, \
    "SWITCH_FIRMWARE", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_firmware_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SWITCH_FIRMWARE { \
    "SWITCH_FIRMWARE", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_firmware_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a switch_firmware message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  ¹Ì¼þÀàÐÍ£¬0x01Ñ¡Ôñ¸ß¹ì¹Ì¼þ£¨±Õ»·¸ú×Ù¹Ì¼þ£©£¬0x02Ñ¡ÔñµÍ¹ì¹Ì¼þ£¨³ÌÐò¸ú×Ù¹Ì¼þ£©£¬0x03Ñ¡Ôñ°µÊÒ²âÊÔ¹Ì¼þ
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_firmware_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN);
#else
    mavlink_switch_firmware_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_FIRMWARE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
}

/**
 * @brief Pack a switch_firmware message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  ¹Ì¼þÀàÐÍ£¬0x01Ñ¡Ôñ¸ß¹ì¹Ì¼þ£¨±Õ»·¸ú×Ù¹Ì¼þ£©£¬0x02Ñ¡ÔñµÍ¹ì¹Ì¼þ£¨³ÌÐò¸ú×Ù¹Ì¼þ£©£¬0x03Ñ¡Ôñ°µÊÒ²âÊÔ¹Ì¼þ
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_firmware_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN);
#else
    mavlink_switch_firmware_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_FIRMWARE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
}

/**
 * @brief Encode a switch_firmware struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param switch_firmware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_firmware_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_switch_firmware_t* switch_firmware)
{
    return mavlink_msg_switch_firmware_pack(system_id, component_id, msg, switch_firmware->mode);
}

/**
 * @brief Encode a switch_firmware struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param switch_firmware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_firmware_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_switch_firmware_t* switch_firmware)
{
    return mavlink_msg_switch_firmware_pack_chan(system_id, component_id, chan, msg, switch_firmware->mode);
}

/**
 * @brief Send a switch_firmware message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  ¹Ì¼þÀàÐÍ£¬0x01Ñ¡Ôñ¸ß¹ì¹Ì¼þ£¨±Õ»·¸ú×Ù¹Ì¼þ£©£¬0x02Ñ¡ÔñµÍ¹ì¹Ì¼þ£¨³ÌÐò¸ú×Ù¹Ì¼þ£©£¬0x03Ñ¡Ôñ°µÊÒ²âÊÔ¹Ì¼þ
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_switch_firmware_send(mavlink_channel_t chan, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN];
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE, buf, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
#else
    mavlink_switch_firmware_t packet;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE, (const char *)&packet, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
#endif
}

/**
 * @brief Send a switch_firmware message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_switch_firmware_send_struct(mavlink_channel_t chan, const mavlink_switch_firmware_t* switch_firmware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_switch_firmware_send(chan, switch_firmware->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE, (const char *)switch_firmware, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
#endif
}

#if MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_switch_firmware_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE, buf, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
#else
    mavlink_switch_firmware_t *packet = (mavlink_switch_firmware_t *)msgbuf;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_FIRMWARE, (const char *)packet, MAVLINK_MSG_ID_SWITCH_FIRMWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN, MAVLINK_MSG_ID_SWITCH_FIRMWARE_CRC);
#endif
}
#endif

#endif

// MESSAGE SWITCH_FIRMWARE UNPACKING


/**
 * @brief Get field mode from switch_firmware message
 *
 * @return  ¹Ì¼þÀàÐÍ£¬0x01Ñ¡Ôñ¸ß¹ì¹Ì¼þ£¨±Õ»·¸ú×Ù¹Ì¼þ£©£¬0x02Ñ¡ÔñµÍ¹ì¹Ì¼þ£¨³ÌÐò¸ú×Ù¹Ì¼þ£©£¬0x03Ñ¡Ôñ°µÊÒ²âÊÔ¹Ì¼þ
 */
static inline uint8_t mavlink_msg_switch_firmware_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a switch_firmware message into a struct
 *
 * @param msg The message to decode
 * @param switch_firmware C-struct to decode the message contents into
 */
static inline void mavlink_msg_switch_firmware_decode(const mavlink_message_t* msg, mavlink_switch_firmware_t* switch_firmware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    switch_firmware->mode = mavlink_msg_switch_firmware_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN? msg->len : MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN;
        memset(switch_firmware, 0, MAVLINK_MSG_ID_SWITCH_FIRMWARE_LEN);
    memcpy(switch_firmware, _MAV_PAYLOAD(msg), len);
#endif
}
