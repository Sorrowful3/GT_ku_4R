#pragma once
// MESSAGE REQUEST_IMU_DRIFT PACKING

#define MAVLINK_MSG_ID_REQUEST_IMU_DRIFT 4

MAVPACKED(
typedef struct __mavlink_request_imu_drift_t {
 uint8_t mode; /*<  模式，0x00用户请求，0x01对星固件请求*/
}) mavlink_request_imu_drift_t;

#define MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN 1
#define MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN 1
#define MAVLINK_MSG_ID_4_LEN 1
#define MAVLINK_MSG_ID_4_MIN_LEN 1

#define MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC 24
#define MAVLINK_MSG_ID_4_CRC 24



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REQUEST_IMU_DRIFT { \
    4, \
    "REQUEST_IMU_DRIFT", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_request_imu_drift_t, mode) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REQUEST_IMU_DRIFT { \
    "REQUEST_IMU_DRIFT", \
    1, \
    {  { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_request_imu_drift_t, mode) }, \
         } \
}
#endif

/**
 * @brief Pack a request_imu_drift message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_request_imu_drift_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN);
#else
    mavlink_request_imu_drift_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REQUEST_IMU_DRIFT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
}

/**
 * @brief Pack a request_imu_drift message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_request_imu_drift_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN];
    _mav_put_uint8_t(buf, 0, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN);
#else
    mavlink_request_imu_drift_t packet;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REQUEST_IMU_DRIFT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
}

/**
 * @brief Encode a request_imu_drift struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param request_imu_drift C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_request_imu_drift_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_request_imu_drift_t* request_imu_drift)
{
    return mavlink_msg_request_imu_drift_pack(system_id, component_id, msg, request_imu_drift->mode);
}

/**
 * @brief Encode a request_imu_drift struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param request_imu_drift C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_request_imu_drift_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_request_imu_drift_t* request_imu_drift)
{
    return mavlink_msg_request_imu_drift_pack_chan(system_id, component_id, chan, msg, request_imu_drift->mode);
}

/**
 * @brief Send a request_imu_drift message
 * @param chan MAVLink channel to send the message
 *
 * @param mode  模式，0x00用户请求，0x01对星固件请求
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_request_imu_drift_send(mavlink_channel_t chan, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN];
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT, buf, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
#else
    mavlink_request_imu_drift_t packet;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT, (const char *)&packet, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
#endif
}

/**
 * @brief Send a request_imu_drift message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_request_imu_drift_send_struct(mavlink_channel_t chan, const mavlink_request_imu_drift_t* request_imu_drift)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_request_imu_drift_send(chan, request_imu_drift->mode);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT, (const char *)request_imu_drift, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
#endif
}

#if MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_request_imu_drift_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT, buf, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
#else
    mavlink_request_imu_drift_t *packet = (mavlink_request_imu_drift_t *)msgbuf;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT, (const char *)packet, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_CRC);
#endif
}
#endif

#endif

// MESSAGE REQUEST_IMU_DRIFT UNPACKING


/**
 * @brief Get field mode from request_imu_drift message
 *
 * @return  模式，0x00用户请求，0x01对星固件请求
 */
static inline uint8_t mavlink_msg_request_imu_drift_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a request_imu_drift message into a struct
 *
 * @param msg The message to decode
 * @param request_imu_drift C-struct to decode the message contents into
 */
static inline void mavlink_msg_request_imu_drift_decode(const mavlink_message_t* msg, mavlink_request_imu_drift_t* request_imu_drift)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    request_imu_drift->mode = mavlink_msg_request_imu_drift_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN? msg->len : MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN;
        memset(request_imu_drift, 0, MAVLINK_MSG_ID_REQUEST_IMU_DRIFT_LEN);
    memcpy(request_imu_drift, _MAV_PAYLOAD(msg), len);
#endif
}
