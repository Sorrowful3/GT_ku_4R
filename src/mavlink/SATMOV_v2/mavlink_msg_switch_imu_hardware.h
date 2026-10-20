#pragma once
// MESSAGE SWITCH_IMU_HARDWARE PACKING

#define MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE 81


typedef struct __mavlink_switch_imu_hardware_t {
 uint8_t hardware; /*<  IMU设备号(0x00:6110惯导，0x01:授时定位定向模块)*/
} mavlink_switch_imu_hardware_t;

#define MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN 1
#define MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN 1
#define MAVLINK_MSG_ID_81_LEN 1
#define MAVLINK_MSG_ID_81_MIN_LEN 1

#define MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC 233
#define MAVLINK_MSG_ID_81_CRC 233



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SWITCH_IMU_HARDWARE { \
    81, \
    "SWITCH_IMU_HARDWARE", \
    1, \
    {  { "hardware", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_imu_hardware_t, hardware) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SWITCH_IMU_HARDWARE { \
    "SWITCH_IMU_HARDWARE", \
    1, \
    {  { "hardware", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_switch_imu_hardware_t, hardware) }, \
         } \
}
#endif

/**
 * @brief Pack a switch_imu_hardware message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param hardware  IMU设备号(0x00:6110惯导，0x01:授时定位定向模块)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_imu_hardware_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN];
    _mav_put_uint8_t(buf, 0, hardware);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN);
#else
    mavlink_switch_imu_hardware_t packet;
    packet.hardware = hardware;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
}

/**
 * @brief Pack a switch_imu_hardware message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hardware  IMU设备号(0x00:6110惯导，0x01:授时定位定向模块)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_switch_imu_hardware_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN];
    _mav_put_uint8_t(buf, 0, hardware);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN);
#else
    mavlink_switch_imu_hardware_t packet;
    packet.hardware = hardware;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
}

/**
 * @brief Encode a switch_imu_hardware struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param switch_imu_hardware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_imu_hardware_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_switch_imu_hardware_t* switch_imu_hardware)
{
    return mavlink_msg_switch_imu_hardware_pack(system_id, component_id, msg, switch_imu_hardware->hardware);
}

/**
 * @brief Encode a switch_imu_hardware struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param switch_imu_hardware C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_switch_imu_hardware_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_switch_imu_hardware_t* switch_imu_hardware)
{
    return mavlink_msg_switch_imu_hardware_pack_chan(system_id, component_id, chan, msg, switch_imu_hardware->hardware);
}

/**
 * @brief Send a switch_imu_hardware message
 * @param chan MAVLink channel to send the message
 *
 * @param hardware  IMU设备号(0x00:6110惯导，0x01:授时定位定向模块)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_switch_imu_hardware_send(mavlink_channel_t chan, uint8_t hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN];
    _mav_put_uint8_t(buf, 0, hardware);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE, buf, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
#else
    mavlink_switch_imu_hardware_t packet;
    packet.hardware = hardware;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE, (const char *)&packet, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
#endif
}

/**
 * @brief Send a switch_imu_hardware message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_switch_imu_hardware_send_struct(mavlink_channel_t chan, const mavlink_switch_imu_hardware_t* switch_imu_hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_switch_imu_hardware_send(chan, switch_imu_hardware->hardware);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE, (const char *)switch_imu_hardware, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
#endif
}

#if MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_switch_imu_hardware_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, hardware);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE, buf, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
#else
    mavlink_switch_imu_hardware_t *packet = (mavlink_switch_imu_hardware_t *)msgbuf;
    packet->hardware = hardware;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE, (const char *)packet, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_MIN_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_CRC);
#endif
}
#endif

#endif

// MESSAGE SWITCH_IMU_HARDWARE UNPACKING


/**
 * @brief Get field hardware from switch_imu_hardware message
 *
 * @return  IMU设备号(0x00:6110惯导，0x01:授时定位定向模块)
 */
static inline uint8_t mavlink_msg_switch_imu_hardware_get_hardware(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a switch_imu_hardware message into a struct
 *
 * @param msg The message to decode
 * @param switch_imu_hardware C-struct to decode the message contents into
 */
static inline void mavlink_msg_switch_imu_hardware_decode(const mavlink_message_t* msg, mavlink_switch_imu_hardware_t* switch_imu_hardware)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    switch_imu_hardware->hardware = mavlink_msg_switch_imu_hardware_get_hardware(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN? msg->len : MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN;
        memset(switch_imu_hardware, 0, MAVLINK_MSG_ID_SWITCH_IMU_HARDWARE_LEN);
    memcpy(switch_imu_hardware, _MAV_PAYLOAD(msg), len);
#endif
}
