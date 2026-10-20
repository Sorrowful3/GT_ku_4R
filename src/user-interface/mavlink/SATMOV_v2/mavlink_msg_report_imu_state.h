#pragma once
// MESSAGE REPORT_IMU_STATE PACKING

#define MAVLINK_MSG_ID_REPORT_IMU_STATE 102

MAVPACKED(
typedef struct __mavlink_report_imu_state_t {
 float pitch; /*<  俯仰角(°)*/
 float roll; /*<  横滚角(°)*/
 float yaw; /*<  方位角(°)*/
 float x; /*<  x角速度(°/s)*/
 float y; /*<  y角速度(°/s)*/
 float z; /*<  z角速度(°/s)*/
 float longitude; /*<  经度(°)*/
 float latitude; /*<  纬度(°)*/
 float height; /*<  高度(m)*/
 float speed; /*<  速度(m/s)*/
 uint32_t week; /*<  GPS时间*/
 uint32_t second; /*<  GPS时间*/
 int16_t temperature; /*<  工作温度*100（°）*/
 uint8_t state; /*<  惯导状态*/
}) mavlink_report_imu_state_t;

#define MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN 51
#define MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN 51
#define MAVLINK_MSG_ID_102_LEN 51
#define MAVLINK_MSG_ID_102_MIN_LEN 51

#define MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC 250
#define MAVLINK_MSG_ID_102_CRC 250



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_IMU_STATE { \
    102, \
    "REPORT_IMU_STATE", \
    14, \
    {  { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_report_imu_state_t, state) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_imu_state_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_imu_state_t, roll) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_imu_state_t, yaw) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_imu_state_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_report_imu_state_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_report_imu_state_t, z) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_report_imu_state_t, longitude) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_report_imu_state_t, latitude) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_report_imu_state_t, height) }, \
         { "speed", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_report_imu_state_t, speed) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 48, offsetof(mavlink_report_imu_state_t, temperature) }, \
         { "week", NULL, MAVLINK_TYPE_UINT32_T, 0, 40, offsetof(mavlink_report_imu_state_t, week) }, \
         { "second", NULL, MAVLINK_TYPE_UINT32_T, 0, 44, offsetof(mavlink_report_imu_state_t, second) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_IMU_STATE { \
    "REPORT_IMU_STATE", \
    14, \
    {  { "state", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_report_imu_state_t, state) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_imu_state_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_imu_state_t, roll) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_imu_state_t, yaw) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_imu_state_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_report_imu_state_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_report_imu_state_t, z) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_report_imu_state_t, longitude) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_report_imu_state_t, latitude) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_report_imu_state_t, height) }, \
         { "speed", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_report_imu_state_t, speed) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 48, offsetof(mavlink_report_imu_state_t, temperature) }, \
         { "week", NULL, MAVLINK_TYPE_UINT32_T, 0, 40, offsetof(mavlink_report_imu_state_t, week) }, \
         { "second", NULL, MAVLINK_TYPE_UINT32_T, 0, 44, offsetof(mavlink_report_imu_state_t, second) }, \
         } \
}
#endif

/**
 * @brief Pack a report_imu_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param state  惯导状态
 * @param pitch  俯仰角(°)
 * @param roll  横滚角(°)
 * @param yaw  方位角(°)
 * @param x  x角速度(°/s)
 * @param y  y角速度(°/s)
 * @param z  z角速度(°/s)
 * @param longitude  经度(°)
 * @param latitude  纬度(°)
 * @param height  高度(m)
 * @param speed  速度(m/s)
 * @param temperature  工作温度*100（°）
 * @param week  GPS时间
 * @param second  GPS时间
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_imu_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t state, float pitch, float roll, float yaw, float x, float y, float z, float longitude, float latitude, float height, float speed, int16_t temperature, uint32_t week, uint32_t second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, roll);
    _mav_put_float(buf, 8, yaw);
    _mav_put_float(buf, 12, x);
    _mav_put_float(buf, 16, y);
    _mav_put_float(buf, 20, z);
    _mav_put_float(buf, 24, longitude);
    _mav_put_float(buf, 28, latitude);
    _mav_put_float(buf, 32, height);
    _mav_put_float(buf, 36, speed);
    _mav_put_uint32_t(buf, 40, week);
    _mav_put_uint32_t(buf, 44, second);
    _mav_put_int16_t(buf, 48, temperature);
    _mav_put_uint8_t(buf, 50, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN);
#else
    mavlink_report_imu_state_t packet;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.yaw = yaw;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.height = height;
    packet.speed = speed;
    packet.week = week;
    packet.second = second;
    packet.temperature = temperature;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_IMU_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
}

/**
 * @brief Pack a report_imu_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param state  惯导状态
 * @param pitch  俯仰角(°)
 * @param roll  横滚角(°)
 * @param yaw  方位角(°)
 * @param x  x角速度(°/s)
 * @param y  y角速度(°/s)
 * @param z  z角速度(°/s)
 * @param longitude  经度(°)
 * @param latitude  纬度(°)
 * @param height  高度(m)
 * @param speed  速度(m/s)
 * @param temperature  工作温度*100（°）
 * @param week  GPS时间
 * @param second  GPS时间
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_imu_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t state,float pitch,float roll,float yaw,float x,float y,float z,float longitude,float latitude,float height,float speed,int16_t temperature,uint32_t week,uint32_t second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, roll);
    _mav_put_float(buf, 8, yaw);
    _mav_put_float(buf, 12, x);
    _mav_put_float(buf, 16, y);
    _mav_put_float(buf, 20, z);
    _mav_put_float(buf, 24, longitude);
    _mav_put_float(buf, 28, latitude);
    _mav_put_float(buf, 32, height);
    _mav_put_float(buf, 36, speed);
    _mav_put_uint32_t(buf, 40, week);
    _mav_put_uint32_t(buf, 44, second);
    _mav_put_int16_t(buf, 48, temperature);
    _mav_put_uint8_t(buf, 50, state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN);
#else
    mavlink_report_imu_state_t packet;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.yaw = yaw;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.height = height;
    packet.speed = speed;
    packet.week = week;
    packet.second = second;
    packet.temperature = temperature;
    packet.state = state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_IMU_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
}

/**
 * @brief Encode a report_imu_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_imu_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_imu_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_imu_state_t* report_imu_state)
{
    return mavlink_msg_report_imu_state_pack(system_id, component_id, msg, report_imu_state->state, report_imu_state->pitch, report_imu_state->roll, report_imu_state->yaw, report_imu_state->x, report_imu_state->y, report_imu_state->z, report_imu_state->longitude, report_imu_state->latitude, report_imu_state->height, report_imu_state->speed, report_imu_state->temperature, report_imu_state->week, report_imu_state->second);
}

/**
 * @brief Encode a report_imu_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_imu_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_imu_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_imu_state_t* report_imu_state)
{
    return mavlink_msg_report_imu_state_pack_chan(system_id, component_id, chan, msg, report_imu_state->state, report_imu_state->pitch, report_imu_state->roll, report_imu_state->yaw, report_imu_state->x, report_imu_state->y, report_imu_state->z, report_imu_state->longitude, report_imu_state->latitude, report_imu_state->height, report_imu_state->speed, report_imu_state->temperature, report_imu_state->week, report_imu_state->second);
}

/**
 * @brief Send a report_imu_state message
 * @param chan MAVLink channel to send the message
 *
 * @param state  惯导状态
 * @param pitch  俯仰角(°)
 * @param roll  横滚角(°)
 * @param yaw  方位角(°)
 * @param x  x角速度(°/s)
 * @param y  y角速度(°/s)
 * @param z  z角速度(°/s)
 * @param longitude  经度(°)
 * @param latitude  纬度(°)
 * @param height  高度(m)
 * @param speed  速度(m/s)
 * @param temperature  工作温度*100（°）
 * @param week  GPS时间
 * @param second  GPS时间
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_imu_state_send(mavlink_channel_t chan, uint8_t state, float pitch, float roll, float yaw, float x, float y, float z, float longitude, float latitude, float height, float speed, int16_t temperature, uint32_t week, uint32_t second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN];
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, roll);
    _mav_put_float(buf, 8, yaw);
    _mav_put_float(buf, 12, x);
    _mav_put_float(buf, 16, y);
    _mav_put_float(buf, 20, z);
    _mav_put_float(buf, 24, longitude);
    _mav_put_float(buf, 28, latitude);
    _mav_put_float(buf, 32, height);
    _mav_put_float(buf, 36, speed);
    _mav_put_uint32_t(buf, 40, week);
    _mav_put_uint32_t(buf, 44, second);
    _mav_put_int16_t(buf, 48, temperature);
    _mav_put_uint8_t(buf, 50, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_IMU_STATE, buf, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
#else
    mavlink_report_imu_state_t packet;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.yaw = yaw;
    packet.x = x;
    packet.y = y;
    packet.z = z;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.height = height;
    packet.speed = speed;
    packet.week = week;
    packet.second = second;
    packet.temperature = temperature;
    packet.state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_IMU_STATE, (const char *)&packet, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
#endif
}

/**
 * @brief Send a report_imu_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_imu_state_send_struct(mavlink_channel_t chan, const mavlink_report_imu_state_t* report_imu_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_imu_state_send(chan, report_imu_state->state, report_imu_state->pitch, report_imu_state->roll, report_imu_state->yaw, report_imu_state->x, report_imu_state->y, report_imu_state->z, report_imu_state->longitude, report_imu_state->latitude, report_imu_state->height, report_imu_state->speed, report_imu_state->temperature, report_imu_state->week, report_imu_state->second);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_IMU_STATE, (const char *)report_imu_state, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_imu_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t state, float pitch, float roll, float yaw, float x, float y, float z, float longitude, float latitude, float height, float speed, int16_t temperature, uint32_t week, uint32_t second)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, pitch);
    _mav_put_float(buf, 4, roll);
    _mav_put_float(buf, 8, yaw);
    _mav_put_float(buf, 12, x);
    _mav_put_float(buf, 16, y);
    _mav_put_float(buf, 20, z);
    _mav_put_float(buf, 24, longitude);
    _mav_put_float(buf, 28, latitude);
    _mav_put_float(buf, 32, height);
    _mav_put_float(buf, 36, speed);
    _mav_put_uint32_t(buf, 40, week);
    _mav_put_uint32_t(buf, 44, second);
    _mav_put_int16_t(buf, 48, temperature);
    _mav_put_uint8_t(buf, 50, state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_IMU_STATE, buf, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
#else
    mavlink_report_imu_state_t *packet = (mavlink_report_imu_state_t *)msgbuf;
    packet->pitch = pitch;
    packet->roll = roll;
    packet->yaw = yaw;
    packet->x = x;
    packet->y = y;
    packet->z = z;
    packet->longitude = longitude;
    packet->latitude = latitude;
    packet->height = height;
    packet->speed = speed;
    packet->week = week;
    packet->second = second;
    packet->temperature = temperature;
    packet->state = state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_IMU_STATE, (const char *)packet, MAVLINK_MSG_ID_REPORT_IMU_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN, MAVLINK_MSG_ID_REPORT_IMU_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_IMU_STATE UNPACKING


/**
 * @brief Get field state from report_imu_state message
 *
 * @return  惯导状态
 */
static inline uint8_t mavlink_msg_report_imu_state_get_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  50);
}

/**
 * @brief Get field pitch from report_imu_state message
 *
 * @return  俯仰角(°)
 */
static inline float mavlink_msg_report_imu_state_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field roll from report_imu_state message
 *
 * @return  横滚角(°)
 */
static inline float mavlink_msg_report_imu_state_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field yaw from report_imu_state message
 *
 * @return  方位角(°)
 */
static inline float mavlink_msg_report_imu_state_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field x from report_imu_state message
 *
 * @return  x角速度(°/s)
 */
static inline float mavlink_msg_report_imu_state_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field y from report_imu_state message
 *
 * @return  y角速度(°/s)
 */
static inline float mavlink_msg_report_imu_state_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field z from report_imu_state message
 *
 * @return  z角速度(°/s)
 */
static inline float mavlink_msg_report_imu_state_get_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field longitude from report_imu_state message
 *
 * @return  经度(°)
 */
static inline float mavlink_msg_report_imu_state_get_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field latitude from report_imu_state message
 *
 * @return  纬度(°)
 */
static inline float mavlink_msg_report_imu_state_get_latitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field height from report_imu_state message
 *
 * @return  高度(m)
 */
static inline float mavlink_msg_report_imu_state_get_height(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field speed from report_imu_state message
 *
 * @return  速度(m/s)
 */
static inline float mavlink_msg_report_imu_state_get_speed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field temperature from report_imu_state message
 *
 * @return  工作温度*100（°）
 */
static inline int16_t mavlink_msg_report_imu_state_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  48);
}

/**
 * @brief Get field week from report_imu_state message
 *
 * @return  GPS时间
 */
static inline uint32_t mavlink_msg_report_imu_state_get_week(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  40);
}

/**
 * @brief Get field second from report_imu_state message
 *
 * @return  GPS时间
 */
static inline uint32_t mavlink_msg_report_imu_state_get_second(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  44);
}

/**
 * @brief Decode a report_imu_state message into a struct
 *
 * @param msg The message to decode
 * @param report_imu_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_imu_state_decode(const mavlink_message_t* msg, mavlink_report_imu_state_t* report_imu_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_imu_state->pitch = mavlink_msg_report_imu_state_get_pitch(msg);
    report_imu_state->roll = mavlink_msg_report_imu_state_get_roll(msg);
    report_imu_state->yaw = mavlink_msg_report_imu_state_get_yaw(msg);
    report_imu_state->x = mavlink_msg_report_imu_state_get_x(msg);
    report_imu_state->y = mavlink_msg_report_imu_state_get_y(msg);
    report_imu_state->z = mavlink_msg_report_imu_state_get_z(msg);
    report_imu_state->longitude = mavlink_msg_report_imu_state_get_longitude(msg);
    report_imu_state->latitude = mavlink_msg_report_imu_state_get_latitude(msg);
    report_imu_state->height = mavlink_msg_report_imu_state_get_height(msg);
    report_imu_state->speed = mavlink_msg_report_imu_state_get_speed(msg);
    report_imu_state->week = mavlink_msg_report_imu_state_get_week(msg);
    report_imu_state->second = mavlink_msg_report_imu_state_get_second(msg);
    report_imu_state->temperature = mavlink_msg_report_imu_state_get_temperature(msg);
    report_imu_state->state = mavlink_msg_report_imu_state_get_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN? msg->len : MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN;
        memset(report_imu_state, 0, MAVLINK_MSG_ID_REPORT_IMU_STATE_LEN);
    memcpy(report_imu_state, _MAV_PAYLOAD(msg), len);
#endif
}
