#pragma once
// MESSAGE REPORT_SATELLITE_INFO PACKING

#define MAVLINK_MSG_ID_REPORT_SATELLITE_INFO 106

MAVPACKED(
typedef struct __mavlink_report_satellite_info_t {
 float longitude; /*<  ÎÀÐÇ¾­¶È(¡ã)*/
 float latitude; /*<  ÎÀÐÇÎ³¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô*/
 float altitude; /*<  ÎÀÐÇ¸ß¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô*/
 char name[32]; /*<  ÎÀÐÇÃû³Æ£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô*/
}) mavlink_report_satellite_info_t;

#define MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN 44
#define MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN 44
#define MAVLINK_MSG_ID_106_LEN 44
#define MAVLINK_MSG_ID_106_MIN_LEN 44

#define MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC 47
#define MAVLINK_MSG_ID_106_CRC 47

#define MAVLINK_MSG_REPORT_SATELLITE_INFO_FIELD_NAME_LEN 32

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_SATELLITE_INFO { \
    106, \
    "REPORT_SATELLITE_INFO", \
    4, \
    {  { "name", NULL, MAVLINK_TYPE_CHAR, 32, 12, offsetof(mavlink_report_satellite_info_t, name) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_satellite_info_t, longitude) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_satellite_info_t, latitude) }, \
         { "altitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_satellite_info_t, altitude) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_SATELLITE_INFO { \
    "REPORT_SATELLITE_INFO", \
    4, \
    {  { "name", NULL, MAVLINK_TYPE_CHAR, 32, 12, offsetof(mavlink_report_satellite_info_t, name) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_satellite_info_t, longitude) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_satellite_info_t, latitude) }, \
         { "altitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_satellite_info_t, altitude) }, \
         } \
}
#endif

/**
 * @brief Pack a report_satellite_info message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param name  ÎÀÐÇÃû³Æ£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param longitude  ÎÀÐÇ¾­¶È(¡ã)
 * @param latitude  ÎÀÐÇÎ³¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param altitude  ÎÀÐÇ¸ß¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_satellite_info_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *name, float longitude, float latitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN];
    _mav_put_float(buf, 0, longitude);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, altitude);
    _mav_put_char_array(buf, 12, name, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN);
#else
    mavlink_report_satellite_info_t packet;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.altitude = altitude;
    mav_array_memcpy(packet.name, name, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_SATELLITE_INFO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
}

/**
 * @brief Pack a report_satellite_info message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param name  ÎÀÐÇÃû³Æ£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param longitude  ÎÀÐÇ¾­¶È(¡ã)
 * @param latitude  ÎÀÐÇÎ³¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param altitude  ÎÀÐÇ¸ß¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_satellite_info_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *name,float longitude,float latitude,float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN];
    _mav_put_float(buf, 0, longitude);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, altitude);
    _mav_put_char_array(buf, 12, name, 32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN);
#else
    mavlink_report_satellite_info_t packet;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.altitude = altitude;
    mav_array_memcpy(packet.name, name, sizeof(char)*32);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_SATELLITE_INFO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
}

/**
 * @brief Encode a report_satellite_info struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_satellite_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_satellite_info_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_satellite_info_t* report_satellite_info)
{
    return mavlink_msg_report_satellite_info_pack(system_id, component_id, msg, report_satellite_info->name, report_satellite_info->longitude, report_satellite_info->latitude, report_satellite_info->altitude);
}

/**
 * @brief Encode a report_satellite_info struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_satellite_info C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_satellite_info_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_satellite_info_t* report_satellite_info)
{
    return mavlink_msg_report_satellite_info_pack_chan(system_id, component_id, chan, msg, report_satellite_info->name, report_satellite_info->longitude, report_satellite_info->latitude, report_satellite_info->altitude);
}

/**
 * @brief Send a report_satellite_info message
 * @param chan MAVLink channel to send the message
 *
 * @param name  ÎÀÐÇÃû³Æ£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param longitude  ÎÀÐÇ¾­¶È(¡ã)
 * @param latitude  ÎÀÐÇÎ³¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 * @param altitude  ÎÀÐÇ¸ß¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_satellite_info_send(mavlink_channel_t chan, const char *name, float longitude, float latitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN];
    _mav_put_float(buf, 0, longitude);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, altitude);
    _mav_put_char_array(buf, 12, name, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO, buf, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
#else
    mavlink_report_satellite_info_t packet;
    packet.longitude = longitude;
    packet.latitude = latitude;
    packet.altitude = altitude;
    mav_array_memcpy(packet.name, name, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO, (const char *)&packet, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
#endif
}

/**
 * @brief Send a report_satellite_info message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_satellite_info_send_struct(mavlink_channel_t chan, const mavlink_report_satellite_info_t* report_satellite_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_satellite_info_send(chan, report_satellite_info->name, report_satellite_info->longitude, report_satellite_info->latitude, report_satellite_info->altitude);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO, (const char *)report_satellite_info, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_satellite_info_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *name, float longitude, float latitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, longitude);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, altitude);
    _mav_put_char_array(buf, 12, name, 32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO, buf, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
#else
    mavlink_report_satellite_info_t *packet = (mavlink_report_satellite_info_t *)msgbuf;
    packet->longitude = longitude;
    packet->latitude = latitude;
    packet->altitude = altitude;
    mav_array_memcpy(packet->name, name, sizeof(char)*32);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO, (const char *)packet, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_MIN_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_SATELLITE_INFO UNPACKING


/**
 * @brief Get field name from report_satellite_info message
 *
 * @return  ÎÀÐÇÃû³Æ£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 */
static inline uint16_t mavlink_msg_report_satellite_info_get_name(const mavlink_message_t* msg, char *name)
{
    return _MAV_RETURN_char_array(msg, name, 32,  12);
}

/**
 * @brief Get field longitude from report_satellite_info message
 *
 * @return  ÎÀÐÇ¾­¶È(¡ã)
 */
static inline float mavlink_msg_report_satellite_info_get_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field latitude from report_satellite_info message
 *
 * @return  ÎÀÐÇÎ³¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 */
static inline float mavlink_msg_report_satellite_info_get_latitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field altitude from report_satellite_info message
 *
 * @return  ÎÀÐÇ¸ß¶È(¡ã)£¬ÎªµÍ¹ì±Õ»·¸ú×ÙÔ¤Áô
 */
static inline float mavlink_msg_report_satellite_info_get_altitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a report_satellite_info message into a struct
 *
 * @param msg The message to decode
 * @param report_satellite_info C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_satellite_info_decode(const mavlink_message_t* msg, mavlink_report_satellite_info_t* report_satellite_info)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_satellite_info->longitude = mavlink_msg_report_satellite_info_get_longitude(msg);
    report_satellite_info->latitude = mavlink_msg_report_satellite_info_get_latitude(msg);
    report_satellite_info->altitude = mavlink_msg_report_satellite_info_get_altitude(msg);
    mavlink_msg_report_satellite_info_get_name(msg, report_satellite_info->name);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN? msg->len : MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN;
        memset(report_satellite_info, 0, MAVLINK_MSG_ID_REPORT_SATELLITE_INFO_LEN);
    memcpy(report_satellite_info, _MAV_PAYLOAD(msg), len);
#endif
}
