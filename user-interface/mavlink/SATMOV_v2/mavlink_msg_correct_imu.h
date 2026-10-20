#pragma once
// MESSAGE CORRECT_IMU PACKING

#define MAVLINK_MSG_ID_CORRECT_IMU 2

MAVPACKED(
typedef struct __mavlink_correct_imu_t {
 uint8_t reserved; /*<  Ô¤Áô*/
}) mavlink_correct_imu_t;

#define MAVLINK_MSG_ID_CORRECT_IMU_LEN 1
#define MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN 1
#define MAVLINK_MSG_ID_2_LEN 1
#define MAVLINK_MSG_ID_2_MIN_LEN 1

#define MAVLINK_MSG_ID_CORRECT_IMU_CRC 143
#define MAVLINK_MSG_ID_2_CRC 143



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CORRECT_IMU { \
    2, \
    "CORRECT_IMU", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_correct_imu_t, reserved) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CORRECT_IMU { \
    "CORRECT_IMU", \
    1, \
    {  { "reserved", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_correct_imu_t, reserved) }, \
         } \
}
#endif

/**
 * @brief Pack a correct_imu message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_correct_imu_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CORRECT_IMU_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CORRECT_IMU_LEN);
#else
    mavlink_correct_imu_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CORRECT_IMU_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CORRECT_IMU;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
}

/**
 * @brief Pack a correct_imu message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reserved  Ô¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_correct_imu_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CORRECT_IMU_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CORRECT_IMU_LEN);
#else
    mavlink_correct_imu_t packet;
    packet.reserved = reserved;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CORRECT_IMU_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CORRECT_IMU;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
}

/**
 * @brief Encode a correct_imu struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param correct_imu C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_correct_imu_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_correct_imu_t* correct_imu)
{
    return mavlink_msg_correct_imu_pack(system_id, component_id, msg, correct_imu->reserved);
}

/**
 * @brief Encode a correct_imu struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param correct_imu C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_correct_imu_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_correct_imu_t* correct_imu)
{
    return mavlink_msg_correct_imu_pack_chan(system_id, component_id, chan, msg, correct_imu->reserved);
}

/**
 * @brief Send a correct_imu message
 * @param chan MAVLink channel to send the message
 *
 * @param reserved  Ô¤Áô
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_correct_imu_send(mavlink_channel_t chan, uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CORRECT_IMU_LEN];
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CORRECT_IMU, buf, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
#else
    mavlink_correct_imu_t packet;
    packet.reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CORRECT_IMU, (const char *)&packet, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
#endif
}

/**
 * @brief Send a correct_imu message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_correct_imu_send_struct(mavlink_channel_t chan, const mavlink_correct_imu_t* correct_imu)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_correct_imu_send(chan, correct_imu->reserved);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CORRECT_IMU, (const char *)correct_imu, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
#endif
}

#if MAVLINK_MSG_ID_CORRECT_IMU_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_correct_imu_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reserved)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reserved);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CORRECT_IMU, buf, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
#else
    mavlink_correct_imu_t *packet = (mavlink_correct_imu_t *)msgbuf;
    packet->reserved = reserved;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CORRECT_IMU, (const char *)packet, MAVLINK_MSG_ID_CORRECT_IMU_MIN_LEN, MAVLINK_MSG_ID_CORRECT_IMU_LEN, MAVLINK_MSG_ID_CORRECT_IMU_CRC);
#endif
}
#endif

#endif

// MESSAGE CORRECT_IMU UNPACKING


/**
 * @brief Get field reserved from correct_imu message
 *
 * @return  Ô¤Áô
 */
static inline uint8_t mavlink_msg_correct_imu_get_reserved(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a correct_imu message into a struct
 *
 * @param msg The message to decode
 * @param correct_imu C-struct to decode the message contents into
 */
static inline void mavlink_msg_correct_imu_decode(const mavlink_message_t* msg, mavlink_correct_imu_t* correct_imu)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    correct_imu->reserved = mavlink_msg_correct_imu_get_reserved(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CORRECT_IMU_LEN? msg->len : MAVLINK_MSG_ID_CORRECT_IMU_LEN;
        memset(correct_imu, 0, MAVLINK_MSG_ID_CORRECT_IMU_LEN);
    memcpy(correct_imu, _MAV_PAYLOAD(msg), len);
#endif
}
