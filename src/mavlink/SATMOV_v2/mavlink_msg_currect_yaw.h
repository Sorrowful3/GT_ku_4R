#pragma once
// MESSAGE CURRECT_YAW PACKING

#define MAVLINK_MSG_ID_CURRECT_YAW 70


typedef struct __mavlink_currect_yaw_t {
 float sat_longitude; /*<  高轨卫星经度(°)*/
 float freq; /*<  高轨卫星下行频率(MHz)*/
 float osc; /*<  高轨卫星下行变频器本振(MHz)*/
 float sym; /*<  高轨卫星下行下行符号速率(ksps)*/
 float beamwidth; /*<  天线波束宽度(°)*/
 float threshold_cnr; /*<  跟踪阈值*/
 float linger_factor; /*<  波束驻留时间系数，DVB有效，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7*/
 float yaw; /*<  通过其他手段得到的航向角，如果没有为-1*/
 uint8_t gain; /*<  高轨卫星下行变频器增益*/
 uint8_t polarity_type; /*<  高轨卫星极化参考,0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋*/
 uint8_t rang; /*<  信标机捕获范围(KHZ)，信标模式有效*/
 uint8_t mode; /*<  信标机工作模式， 0x00 DVB，0x01 检波，0x02 信标*/
} mavlink_currect_yaw_t;

#define MAVLINK_MSG_ID_CURRECT_YAW_LEN 36
#define MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN 36
#define MAVLINK_MSG_ID_70_LEN 36
#define MAVLINK_MSG_ID_70_MIN_LEN 36

#define MAVLINK_MSG_ID_CURRECT_YAW_CRC 222
#define MAVLINK_MSG_ID_70_CRC 222



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CURRECT_YAW { \
    70, \
    "CURRECT_YAW", \
    12, \
    {  { "sat_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_currect_yaw_t, sat_longitude) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_currect_yaw_t, freq) }, \
         { "osc", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_currect_yaw_t, osc) }, \
         { "gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_currect_yaw_t, gain) }, \
         { "polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_currect_yaw_t, polarity_type) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_currect_yaw_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_currect_yaw_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 35, offsetof(mavlink_currect_yaw_t, mode) }, \
         { "beamwidth", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_currect_yaw_t, beamwidth) }, \
         { "threshold_cnr", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_currect_yaw_t, threshold_cnr) }, \
         { "linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_currect_yaw_t, linger_factor) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_currect_yaw_t, yaw) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CURRECT_YAW { \
    "CURRECT_YAW", \
    12, \
    {  { "sat_longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_currect_yaw_t, sat_longitude) }, \
         { "freq", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_currect_yaw_t, freq) }, \
         { "osc", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_currect_yaw_t, osc) }, \
         { "gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_currect_yaw_t, gain) }, \
         { "polarity_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 33, offsetof(mavlink_currect_yaw_t, polarity_type) }, \
         { "sym", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_currect_yaw_t, sym) }, \
         { "rang", NULL, MAVLINK_TYPE_UINT8_T, 0, 34, offsetof(mavlink_currect_yaw_t, rang) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 35, offsetof(mavlink_currect_yaw_t, mode) }, \
         { "beamwidth", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_currect_yaw_t, beamwidth) }, \
         { "threshold_cnr", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_currect_yaw_t, threshold_cnr) }, \
         { "linger_factor", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_currect_yaw_t, linger_factor) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_currect_yaw_t, yaw) }, \
         } \
}
#endif

/**
 * @brief Pack a currect_yaw message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param sat_longitude  高轨卫星经度(°)
 * @param freq  高轨卫星下行频率(MHz)
 * @param osc  高轨卫星下行变频器本振(MHz)
 * @param gain  高轨卫星下行变频器增益
 * @param polarity_type  高轨卫星极化参考,0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param sym  高轨卫星下行下行符号速率(ksps)
 * @param rang  信标机捕获范围(KHZ)，信标模式有效
 * @param mode  信标机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 * @param beamwidth  天线波束宽度(°)
 * @param threshold_cnr  跟踪阈值
 * @param linger_factor  波束驻留时间系数，DVB有效，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param yaw  通过其他手段得到的航向角，如果没有为-1
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_currect_yaw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float sat_longitude, float freq, float osc, uint8_t gain, uint8_t polarity_type, float sym, uint8_t rang, uint8_t mode, float beamwidth, float threshold_cnr, float linger_factor, float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, osc);
    _mav_put_float(buf, 12, sym);
    _mav_put_float(buf, 16, beamwidth);
    _mav_put_float(buf, 20, threshold_cnr);
    _mav_put_float(buf, 24, linger_factor);
    _mav_put_float(buf, 28, yaw);
    _mav_put_uint8_t(buf, 32, gain);
    _mav_put_uint8_t(buf, 33, polarity_type);
    _mav_put_uint8_t(buf, 34, rang);
    _mav_put_uint8_t(buf, 35, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CURRECT_YAW_LEN);
#else
    mavlink_currect_yaw_t packet;
    packet.sat_longitude = sat_longitude;
    packet.freq = freq;
    packet.osc = osc;
    packet.sym = sym;
    packet.beamwidth = beamwidth;
    packet.threshold_cnr = threshold_cnr;
    packet.linger_factor = linger_factor;
    packet.yaw = yaw;
    packet.gain = gain;
    packet.polarity_type = polarity_type;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CURRECT_YAW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CURRECT_YAW;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
}

/**
 * @brief Pack a currect_yaw message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sat_longitude  高轨卫星经度(°)
 * @param freq  高轨卫星下行频率(MHz)
 * @param osc  高轨卫星下行变频器本振(MHz)
 * @param gain  高轨卫星下行变频器增益
 * @param polarity_type  高轨卫星极化参考,0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param sym  高轨卫星下行下行符号速率(ksps)
 * @param rang  信标机捕获范围(KHZ)，信标模式有效
 * @param mode  信标机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 * @param beamwidth  天线波束宽度(°)
 * @param threshold_cnr  跟踪阈值
 * @param linger_factor  波束驻留时间系数，DVB有效，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param yaw  通过其他手段得到的航向角，如果没有为-1
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_currect_yaw_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float sat_longitude,float freq,float osc,uint8_t gain,uint8_t polarity_type,float sym,uint8_t rang,uint8_t mode,float beamwidth,float threshold_cnr,float linger_factor,float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, osc);
    _mav_put_float(buf, 12, sym);
    _mav_put_float(buf, 16, beamwidth);
    _mav_put_float(buf, 20, threshold_cnr);
    _mav_put_float(buf, 24, linger_factor);
    _mav_put_float(buf, 28, yaw);
    _mav_put_uint8_t(buf, 32, gain);
    _mav_put_uint8_t(buf, 33, polarity_type);
    _mav_put_uint8_t(buf, 34, rang);
    _mav_put_uint8_t(buf, 35, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CURRECT_YAW_LEN);
#else
    mavlink_currect_yaw_t packet;
    packet.sat_longitude = sat_longitude;
    packet.freq = freq;
    packet.osc = osc;
    packet.sym = sym;
    packet.beamwidth = beamwidth;
    packet.threshold_cnr = threshold_cnr;
    packet.linger_factor = linger_factor;
    packet.yaw = yaw;
    packet.gain = gain;
    packet.polarity_type = polarity_type;
    packet.rang = rang;
    packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CURRECT_YAW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CURRECT_YAW;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
}

/**
 * @brief Encode a currect_yaw struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param currect_yaw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_currect_yaw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_currect_yaw_t* currect_yaw)
{
    return mavlink_msg_currect_yaw_pack(system_id, component_id, msg, currect_yaw->sat_longitude, currect_yaw->freq, currect_yaw->osc, currect_yaw->gain, currect_yaw->polarity_type, currect_yaw->sym, currect_yaw->rang, currect_yaw->mode, currect_yaw->beamwidth, currect_yaw->threshold_cnr, currect_yaw->linger_factor, currect_yaw->yaw);
}

/**
 * @brief Encode a currect_yaw struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param currect_yaw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_currect_yaw_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_currect_yaw_t* currect_yaw)
{
    return mavlink_msg_currect_yaw_pack_chan(system_id, component_id, chan, msg, currect_yaw->sat_longitude, currect_yaw->freq, currect_yaw->osc, currect_yaw->gain, currect_yaw->polarity_type, currect_yaw->sym, currect_yaw->rang, currect_yaw->mode, currect_yaw->beamwidth, currect_yaw->threshold_cnr, currect_yaw->linger_factor, currect_yaw->yaw);
}

/**
 * @brief Send a currect_yaw message
 * @param chan MAVLink channel to send the message
 *
 * @param sat_longitude  高轨卫星经度(°)
 * @param freq  高轨卫星下行频率(MHz)
 * @param osc  高轨卫星下行变频器本振(MHz)
 * @param gain  高轨卫星下行变频器增益
 * @param polarity_type  高轨卫星极化参考,0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 * @param sym  高轨卫星下行下行符号速率(ksps)
 * @param rang  信标机捕获范围(KHZ)，信标模式有效
 * @param mode  信标机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 * @param beamwidth  天线波束宽度(°)
 * @param threshold_cnr  跟踪阈值
 * @param linger_factor  波束驻留时间系数，DVB有效，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 * @param yaw  通过其他手段得到的航向角，如果没有为-1
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_currect_yaw_send(mavlink_channel_t chan, float sat_longitude, float freq, float osc, uint8_t gain, uint8_t polarity_type, float sym, uint8_t rang, uint8_t mode, float beamwidth, float threshold_cnr, float linger_factor, float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CURRECT_YAW_LEN];
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, osc);
    _mav_put_float(buf, 12, sym);
    _mav_put_float(buf, 16, beamwidth);
    _mav_put_float(buf, 20, threshold_cnr);
    _mav_put_float(buf, 24, linger_factor);
    _mav_put_float(buf, 28, yaw);
    _mav_put_uint8_t(buf, 32, gain);
    _mav_put_uint8_t(buf, 33, polarity_type);
    _mav_put_uint8_t(buf, 34, rang);
    _mav_put_uint8_t(buf, 35, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW, buf, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
#else
    mavlink_currect_yaw_t packet;
    packet.sat_longitude = sat_longitude;
    packet.freq = freq;
    packet.osc = osc;
    packet.sym = sym;
    packet.beamwidth = beamwidth;
    packet.threshold_cnr = threshold_cnr;
    packet.linger_factor = linger_factor;
    packet.yaw = yaw;
    packet.gain = gain;
    packet.polarity_type = polarity_type;
    packet.rang = rang;
    packet.mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW, (const char *)&packet, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
#endif
}

/**
 * @brief Send a currect_yaw message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_currect_yaw_send_struct(mavlink_channel_t chan, const mavlink_currect_yaw_t* currect_yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_currect_yaw_send(chan, currect_yaw->sat_longitude, currect_yaw->freq, currect_yaw->osc, currect_yaw->gain, currect_yaw->polarity_type, currect_yaw->sym, currect_yaw->rang, currect_yaw->mode, currect_yaw->beamwidth, currect_yaw->threshold_cnr, currect_yaw->linger_factor, currect_yaw->yaw);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW, (const char *)currect_yaw, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
#endif
}

#if MAVLINK_MSG_ID_CURRECT_YAW_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_currect_yaw_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float sat_longitude, float freq, float osc, uint8_t gain, uint8_t polarity_type, float sym, uint8_t rang, uint8_t mode, float beamwidth, float threshold_cnr, float linger_factor, float yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, sat_longitude);
    _mav_put_float(buf, 4, freq);
    _mav_put_float(buf, 8, osc);
    _mav_put_float(buf, 12, sym);
    _mav_put_float(buf, 16, beamwidth);
    _mav_put_float(buf, 20, threshold_cnr);
    _mav_put_float(buf, 24, linger_factor);
    _mav_put_float(buf, 28, yaw);
    _mav_put_uint8_t(buf, 32, gain);
    _mav_put_uint8_t(buf, 33, polarity_type);
    _mav_put_uint8_t(buf, 34, rang);
    _mav_put_uint8_t(buf, 35, mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW, buf, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
#else
    mavlink_currect_yaw_t *packet = (mavlink_currect_yaw_t *)msgbuf;
    packet->sat_longitude = sat_longitude;
    packet->freq = freq;
    packet->osc = osc;
    packet->sym = sym;
    packet->beamwidth = beamwidth;
    packet->threshold_cnr = threshold_cnr;
    packet->linger_factor = linger_factor;
    packet->yaw = yaw;
    packet->gain = gain;
    packet->polarity_type = polarity_type;
    packet->rang = rang;
    packet->mode = mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CURRECT_YAW, (const char *)packet, MAVLINK_MSG_ID_CURRECT_YAW_MIN_LEN, MAVLINK_MSG_ID_CURRECT_YAW_LEN, MAVLINK_MSG_ID_CURRECT_YAW_CRC);
#endif
}
#endif

#endif

// MESSAGE CURRECT_YAW UNPACKING


/**
 * @brief Get field sat_longitude from currect_yaw message
 *
 * @return  高轨卫星经度(°)
 */
static inline float mavlink_msg_currect_yaw_get_sat_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field freq from currect_yaw message
 *
 * @return  高轨卫星下行频率(MHz)
 */
static inline float mavlink_msg_currect_yaw_get_freq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field osc from currect_yaw message
 *
 * @return  高轨卫星下行变频器本振(MHz)
 */
static inline float mavlink_msg_currect_yaw_get_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field gain from currect_yaw message
 *
 * @return  高轨卫星下行变频器增益
 */
static inline uint8_t mavlink_msg_currect_yaw_get_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  32);
}

/**
 * @brief Get field polarity_type from currect_yaw message
 *
 * @return  高轨卫星极化参考,0x00垂直参考，0x01水平参考，0x02左旋，0x03右旋
 */
static inline uint8_t mavlink_msg_currect_yaw_get_polarity_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  33);
}

/**
 * @brief Get field sym from currect_yaw message
 *
 * @return  高轨卫星下行下行符号速率(ksps)
 */
static inline float mavlink_msg_currect_yaw_get_sym(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field rang from currect_yaw message
 *
 * @return  信标机捕获范围(KHZ)，信标模式有效
 */
static inline uint8_t mavlink_msg_currect_yaw_get_rang(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  34);
}

/**
 * @brief Get field mode from currect_yaw message
 *
 * @return  信标机工作模式， 0x00 DVB，0x01 检波，0x02 信标
 */
static inline uint8_t mavlink_msg_currect_yaw_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  35);
}

/**
 * @brief Get field beamwidth from currect_yaw message
 *
 * @return  天线波束宽度(°)
 */
static inline float mavlink_msg_currect_yaw_get_beamwidth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field threshold_cnr from currect_yaw message
 *
 * @return  跟踪阈值
 */
static inline float mavlink_msg_currect_yaw_get_threshold_cnr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field linger_factor from currect_yaw message
 *
 * @return  波束驻留时间系数，DVB有效，驻留时间=(20-波束符号速率(Msps))*驻留时间系数+7
 */
static inline float mavlink_msg_currect_yaw_get_linger_factor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yaw from currect_yaw message
 *
 * @return  通过其他手段得到的航向角，如果没有为-1
 */
static inline float mavlink_msg_currect_yaw_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Decode a currect_yaw message into a struct
 *
 * @param msg The message to decode
 * @param currect_yaw C-struct to decode the message contents into
 */
static inline void mavlink_msg_currect_yaw_decode(const mavlink_message_t* msg, mavlink_currect_yaw_t* currect_yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    currect_yaw->sat_longitude = mavlink_msg_currect_yaw_get_sat_longitude(msg);
    currect_yaw->freq = mavlink_msg_currect_yaw_get_freq(msg);
    currect_yaw->osc = mavlink_msg_currect_yaw_get_osc(msg);
    currect_yaw->sym = mavlink_msg_currect_yaw_get_sym(msg);
    currect_yaw->beamwidth = mavlink_msg_currect_yaw_get_beamwidth(msg);
    currect_yaw->threshold_cnr = mavlink_msg_currect_yaw_get_threshold_cnr(msg);
    currect_yaw->linger_factor = mavlink_msg_currect_yaw_get_linger_factor(msg);
    currect_yaw->yaw = mavlink_msg_currect_yaw_get_yaw(msg);
    currect_yaw->gain = mavlink_msg_currect_yaw_get_gain(msg);
    currect_yaw->polarity_type = mavlink_msg_currect_yaw_get_polarity_type(msg);
    currect_yaw->rang = mavlink_msg_currect_yaw_get_rang(msg);
    currect_yaw->mode = mavlink_msg_currect_yaw_get_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CURRECT_YAW_LEN? msg->len : MAVLINK_MSG_ID_CURRECT_YAW_LEN;
        memset(currect_yaw, 0, MAVLINK_MSG_ID_CURRECT_YAW_LEN);
    memcpy(currect_yaw, _MAV_PAYLOAD(msg), len);
#endif
}
