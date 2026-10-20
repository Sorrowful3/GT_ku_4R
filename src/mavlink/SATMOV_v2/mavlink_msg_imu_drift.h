#pragma once
// MESSAGE IMU_DRIFT PACKING

#define MAVLINK_MSG_ID_IMU_DRIFT 5


typedef struct __mavlink_imu_drift_t {
 float gyro_x; /*<  ÍÓÂÝÒÇxÖáÁãÆ¯*/
 float gyro_y; /*<  ÍÓÂÝÒÇyÖáÁãÆ¯*/
 float gyro_z; /*<  ÍÓÂÝÒÇzÖáÁãÆ¯*/
 uint8_t valid; /*<  Êý¾ÝÓÐÐ§±êÖ¾£¬0x00ÎÞÐ§£¬0x01ÓÐÐ§*/
} mavlink_imu_drift_t;

#define MAVLINK_MSG_ID_IMU_DRIFT_LEN 13
#define MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN 13
#define MAVLINK_MSG_ID_5_LEN 13
#define MAVLINK_MSG_ID_5_MIN_LEN 13

#define MAVLINK_MSG_ID_IMU_DRIFT_CRC 140
#define MAVLINK_MSG_ID_5_CRC 140



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IMU_DRIFT { \
    5, \
    "IMU_DRIFT", \
    4, \
    {  { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_imu_drift_t, valid) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_imu_drift_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_imu_drift_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_imu_drift_t, gyro_z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IMU_DRIFT { \
    "IMU_DRIFT", \
    4, \
    {  { "valid", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_imu_drift_t, valid) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_imu_drift_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_imu_drift_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_imu_drift_t, gyro_z) }, \
         } \
}
#endif

/**
 * @brief Pack a imu_drift message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param valid  Êý¾ÝÓÐÐ§±êÖ¾£¬0x00ÎÞÐ§£¬0x01ÓÐÐ§
 * @param gyro_x  ÍÓÂÝÒÇxÖáÁãÆ¯
 * @param gyro_y  ÍÓÂÝÒÇyÖáÁãÆ¯
 * @param gyro_z  ÍÓÂÝÒÇzÖáÁãÆ¯
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_drift_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t valid, float gyro_x, float gyro_y, float gyro_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_DRIFT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_uint8_t(buf, 12, valid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_DRIFT_LEN);
#else
    mavlink_imu_drift_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.valid = valid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_DRIFT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_DRIFT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
}

/**
 * @brief Pack a imu_drift message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param valid  Êý¾ÝÓÐÐ§±êÖ¾£¬0x00ÎÞÐ§£¬0x01ÓÐÐ§
 * @param gyro_x  ÍÓÂÝÒÇxÖáÁãÆ¯
 * @param gyro_y  ÍÓÂÝÒÇyÖáÁãÆ¯
 * @param gyro_z  ÍÓÂÝÒÇzÖáÁãÆ¯
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_drift_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t valid,float gyro_x,float gyro_y,float gyro_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_DRIFT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_uint8_t(buf, 12, valid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_DRIFT_LEN);
#else
    mavlink_imu_drift_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.valid = valid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_DRIFT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_DRIFT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
}

/**
 * @brief Encode a imu_drift struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param imu_drift C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_drift_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_imu_drift_t* imu_drift)
{
    return mavlink_msg_imu_drift_pack(system_id, component_id, msg, imu_drift->valid, imu_drift->gyro_x, imu_drift->gyro_y, imu_drift->gyro_z);
}

/**
 * @brief Encode a imu_drift struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param imu_drift C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_drift_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_imu_drift_t* imu_drift)
{
    return mavlink_msg_imu_drift_pack_chan(system_id, component_id, chan, msg, imu_drift->valid, imu_drift->gyro_x, imu_drift->gyro_y, imu_drift->gyro_z);
}

/**
 * @brief Send a imu_drift message
 * @param chan MAVLink channel to send the message
 *
 * @param valid  Êý¾ÝÓÐÐ§±êÖ¾£¬0x00ÎÞÐ§£¬0x01ÓÐÐ§
 * @param gyro_x  ÍÓÂÝÒÇxÖáÁãÆ¯
 * @param gyro_y  ÍÓÂÝÒÇyÖáÁãÆ¯
 * @param gyro_z  ÍÓÂÝÒÇzÖáÁãÆ¯
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_imu_drift_send(mavlink_channel_t chan, uint8_t valid, float gyro_x, float gyro_y, float gyro_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_DRIFT_LEN];
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_uint8_t(buf, 12, valid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_DRIFT, buf, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
#else
    mavlink_imu_drift_t packet;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.valid = valid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_DRIFT, (const char *)&packet, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
#endif
}

/**
 * @brief Send a imu_drift message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_imu_drift_send_struct(mavlink_channel_t chan, const mavlink_imu_drift_t* imu_drift)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_drift_send(chan, imu_drift->valid, imu_drift->gyro_x, imu_drift->gyro_y, imu_drift->gyro_z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_DRIFT, (const char *)imu_drift, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
#endif
}

#if MAVLINK_MSG_ID_IMU_DRIFT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_imu_drift_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t valid, float gyro_x, float gyro_y, float gyro_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, gyro_x);
    _mav_put_float(buf, 4, gyro_y);
    _mav_put_float(buf, 8, gyro_z);
    _mav_put_uint8_t(buf, 12, valid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_DRIFT, buf, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
#else
    mavlink_imu_drift_t *packet = (mavlink_imu_drift_t *)msgbuf;
    packet->gyro_x = gyro_x;
    packet->gyro_y = gyro_y;
    packet->gyro_z = gyro_z;
    packet->valid = valid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_DRIFT, (const char *)packet, MAVLINK_MSG_ID_IMU_DRIFT_MIN_LEN, MAVLINK_MSG_ID_IMU_DRIFT_LEN, MAVLINK_MSG_ID_IMU_DRIFT_CRC);
#endif
}
#endif

#endif

// MESSAGE IMU_DRIFT UNPACKING


/**
 * @brief Get field valid from imu_drift message
 *
 * @return  Êý¾ÝÓÐÐ§±êÖ¾£¬0x00ÎÞÐ§£¬0x01ÓÐÐ§
 */
static inline uint8_t mavlink_msg_imu_drift_get_valid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field gyro_x from imu_drift message
 *
 * @return  ÍÓÂÝÒÇxÖáÁãÆ¯
 */
static inline float mavlink_msg_imu_drift_get_gyro_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field gyro_y from imu_drift message
 *
 * @return  ÍÓÂÝÒÇyÖáÁãÆ¯
 */
static inline float mavlink_msg_imu_drift_get_gyro_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field gyro_z from imu_drift message
 *
 * @return  ÍÓÂÝÒÇzÖáÁãÆ¯
 */
static inline float mavlink_msg_imu_drift_get_gyro_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a imu_drift message into a struct
 *
 * @param msg The message to decode
 * @param imu_drift C-struct to decode the message contents into
 */
static inline void mavlink_msg_imu_drift_decode(const mavlink_message_t* msg, mavlink_imu_drift_t* imu_drift)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    imu_drift->gyro_x = mavlink_msg_imu_drift_get_gyro_x(msg);
    imu_drift->gyro_y = mavlink_msg_imu_drift_get_gyro_y(msg);
    imu_drift->gyro_z = mavlink_msg_imu_drift_get_gyro_z(msg);
    imu_drift->valid = mavlink_msg_imu_drift_get_valid(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IMU_DRIFT_LEN? msg->len : MAVLINK_MSG_ID_IMU_DRIFT_LEN;
        memset(imu_drift, 0, MAVLINK_MSG_ID_IMU_DRIFT_LEN);
    memcpy(imu_drift, _MAV_PAYLOAD(msg), len);
#endif
}
