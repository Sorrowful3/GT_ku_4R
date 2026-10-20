#pragma once
// MESSAGE REPORT_TRANSDUCER_STATE PACKING

#define MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE 104

MAVPACKED(
typedef struct __mavlink_report_transducer_state_t {
 float rx_osc; /*<  接收本振频率(MHz)*/
 float tx_osc; /*<  发射本振频率(MHz)*/
 float if_Value; /*<  中频电平值*/
 float rf_Value; /*<  射频电平值*/
 int16_t temperature; /*<  变频器工作温度*100（°）*/
 int16_t voltage; /*<  变频器工作电压*100（V）*/
 int16_t electricity; /*<  变频器工作电流*100（A）*/
 uint8_t rx_gain; /*<  接收增益*/
 uint8_t tx_gain; /*<  发射增益*/
 uint8_t lock_state; /*<  锁定状态*/
}) mavlink_report_transducer_state_t;

#define MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN 25
#define MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN 25
#define MAVLINK_MSG_ID_104_LEN 25
#define MAVLINK_MSG_ID_104_MIN_LEN 25

#define MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC 86
#define MAVLINK_MSG_ID_104_CRC 86



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_TRANSDUCER_STATE { \
    104, \
    "REPORT_TRANSDUCER_STATE", \
    10, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_transducer_state_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_report_transducer_state_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_transducer_state_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_report_transducer_state_t, tx_gain) }, \
         { "if_Value", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_transducer_state_t, if_Value) }, \
         { "rf_Value", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_transducer_state_t, rf_Value) }, \
         { "lock_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_report_transducer_state_t, lock_state) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_report_transducer_state_t, temperature) }, \
         { "voltage", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_report_transducer_state_t, voltage) }, \
         { "electricity", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_report_transducer_state_t, electricity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_TRANSDUCER_STATE { \
    "REPORT_TRANSDUCER_STATE", \
    10, \
    {  { "rx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_report_transducer_state_t, rx_osc) }, \
         { "rx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_report_transducer_state_t, rx_gain) }, \
         { "tx_osc", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_report_transducer_state_t, tx_osc) }, \
         { "tx_gain", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_report_transducer_state_t, tx_gain) }, \
         { "if_Value", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_report_transducer_state_t, if_Value) }, \
         { "rf_Value", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_report_transducer_state_t, rf_Value) }, \
         { "lock_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_report_transducer_state_t, lock_state) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_report_transducer_state_t, temperature) }, \
         { "voltage", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_report_transducer_state_t, voltage) }, \
         { "electricity", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_report_transducer_state_t, electricity) }, \
         } \
}
#endif

/**
 * @brief Pack a report_transducer_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rx_osc  接收本振频率(MHz)
 * @param rx_gain  接收增益
 * @param tx_osc  发射本振频率(MHz)
 * @param tx_gain  发射增益
 * @param if_Value  中频电平值
 * @param rf_Value  射频电平值
 * @param lock_state  锁定状态
 * @param temperature  变频器工作温度*100（°）
 * @param voltage  变频器工作电压*100（V）
 * @param electricity  变频器工作电流*100（A）
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_transducer_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float if_Value, float rf_Value, uint8_t lock_state, int16_t temperature, int16_t voltage, int16_t electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_float(buf, 8, if_Value);
    _mav_put_float(buf, 12, rf_Value);
    _mav_put_int16_t(buf, 16, temperature);
    _mav_put_int16_t(buf, 18, voltage);
    _mav_put_int16_t(buf, 20, electricity);
    _mav_put_uint8_t(buf, 22, rx_gain);
    _mav_put_uint8_t(buf, 23, tx_gain);
    _mav_put_uint8_t(buf, 24, lock_state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN);
#else
    mavlink_report_transducer_state_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.if_Value = if_Value;
    packet.rf_Value = rf_Value;
    packet.temperature = temperature;
    packet.voltage = voltage;
    packet.electricity = electricity;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.lock_state = lock_state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
}

/**
 * @brief Pack a report_transducer_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx_osc  接收本振频率(MHz)
 * @param rx_gain  接收增益
 * @param tx_osc  发射本振频率(MHz)
 * @param tx_gain  发射增益
 * @param if_Value  中频电平值
 * @param rf_Value  射频电平值
 * @param lock_state  锁定状态
 * @param temperature  变频器工作温度*100（°）
 * @param voltage  变频器工作电压*100（V）
 * @param electricity  变频器工作电流*100（A）
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_transducer_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float rx_osc,uint8_t rx_gain,float tx_osc,uint8_t tx_gain,float if_Value,float rf_Value,uint8_t lock_state,int16_t temperature,int16_t voltage,int16_t electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_float(buf, 8, if_Value);
    _mav_put_float(buf, 12, rf_Value);
    _mav_put_int16_t(buf, 16, temperature);
    _mav_put_int16_t(buf, 18, voltage);
    _mav_put_int16_t(buf, 20, electricity);
    _mav_put_uint8_t(buf, 22, rx_gain);
    _mav_put_uint8_t(buf, 23, tx_gain);
    _mav_put_uint8_t(buf, 24, lock_state);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN);
#else
    mavlink_report_transducer_state_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.if_Value = if_Value;
    packet.rf_Value = rf_Value;
    packet.temperature = temperature;
    packet.voltage = voltage;
    packet.electricity = electricity;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.lock_state = lock_state;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
}

/**
 * @brief Encode a report_transducer_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_transducer_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_transducer_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_transducer_state_t* report_transducer_state)
{
    return mavlink_msg_report_transducer_state_pack(system_id, component_id, msg, report_transducer_state->rx_osc, report_transducer_state->rx_gain, report_transducer_state->tx_osc, report_transducer_state->tx_gain, report_transducer_state->if_Value, report_transducer_state->rf_Value, report_transducer_state->lock_state, report_transducer_state->temperature, report_transducer_state->voltage, report_transducer_state->electricity);
}

/**
 * @brief Encode a report_transducer_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_transducer_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_transducer_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_transducer_state_t* report_transducer_state)
{
    return mavlink_msg_report_transducer_state_pack_chan(system_id, component_id, chan, msg, report_transducer_state->rx_osc, report_transducer_state->rx_gain, report_transducer_state->tx_osc, report_transducer_state->tx_gain, report_transducer_state->if_Value, report_transducer_state->rf_Value, report_transducer_state->lock_state, report_transducer_state->temperature, report_transducer_state->voltage, report_transducer_state->electricity);
}

/**
 * @brief Send a report_transducer_state message
 * @param chan MAVLink channel to send the message
 *
 * @param rx_osc  接收本振频率(MHz)
 * @param rx_gain  接收增益
 * @param tx_osc  发射本振频率(MHz)
 * @param tx_gain  发射增益
 * @param if_Value  中频电平值
 * @param rf_Value  射频电平值
 * @param lock_state  锁定状态
 * @param temperature  变频器工作温度*100（°）
 * @param voltage  变频器工作电压*100（V）
 * @param electricity  变频器工作电流*100（A）
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_transducer_state_send(mavlink_channel_t chan, float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float if_Value, float rf_Value, uint8_t lock_state, int16_t temperature, int16_t voltage, int16_t electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN];
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_float(buf, 8, if_Value);
    _mav_put_float(buf, 12, rf_Value);
    _mav_put_int16_t(buf, 16, temperature);
    _mav_put_int16_t(buf, 18, voltage);
    _mav_put_int16_t(buf, 20, electricity);
    _mav_put_uint8_t(buf, 22, rx_gain);
    _mav_put_uint8_t(buf, 23, tx_gain);
    _mav_put_uint8_t(buf, 24, lock_state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE, buf, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
#else
    mavlink_report_transducer_state_t packet;
    packet.rx_osc = rx_osc;
    packet.tx_osc = tx_osc;
    packet.if_Value = if_Value;
    packet.rf_Value = rf_Value;
    packet.temperature = temperature;
    packet.voltage = voltage;
    packet.electricity = electricity;
    packet.rx_gain = rx_gain;
    packet.tx_gain = tx_gain;
    packet.lock_state = lock_state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE, (const char *)&packet, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
#endif
}

/**
 * @brief Send a report_transducer_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_transducer_state_send_struct(mavlink_channel_t chan, const mavlink_report_transducer_state_t* report_transducer_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_transducer_state_send(chan, report_transducer_state->rx_osc, report_transducer_state->rx_gain, report_transducer_state->tx_osc, report_transducer_state->tx_gain, report_transducer_state->if_Value, report_transducer_state->rf_Value, report_transducer_state->lock_state, report_transducer_state->temperature, report_transducer_state->voltage, report_transducer_state->electricity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE, (const char *)report_transducer_state, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_transducer_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float rx_osc, uint8_t rx_gain, float tx_osc, uint8_t tx_gain, float if_Value, float rf_Value, uint8_t lock_state, int16_t temperature, int16_t voltage, int16_t electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, rx_osc);
    _mav_put_float(buf, 4, tx_osc);
    _mav_put_float(buf, 8, if_Value);
    _mav_put_float(buf, 12, rf_Value);
    _mav_put_int16_t(buf, 16, temperature);
    _mav_put_int16_t(buf, 18, voltage);
    _mav_put_int16_t(buf, 20, electricity);
    _mav_put_uint8_t(buf, 22, rx_gain);
    _mav_put_uint8_t(buf, 23, tx_gain);
    _mav_put_uint8_t(buf, 24, lock_state);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE, buf, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
#else
    mavlink_report_transducer_state_t *packet = (mavlink_report_transducer_state_t *)msgbuf;
    packet->rx_osc = rx_osc;
    packet->tx_osc = tx_osc;
    packet->if_Value = if_Value;
    packet->rf_Value = rf_Value;
    packet->temperature = temperature;
    packet->voltage = voltage;
    packet->electricity = electricity;
    packet->rx_gain = rx_gain;
    packet->tx_gain = tx_gain;
    packet->lock_state = lock_state;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE, (const char *)packet, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_TRANSDUCER_STATE UNPACKING


/**
 * @brief Get field rx_osc from report_transducer_state message
 *
 * @return  接收本振频率(MHz)
 */
static inline float mavlink_msg_report_transducer_state_get_rx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field rx_gain from report_transducer_state message
 *
 * @return  接收增益
 */
static inline uint8_t mavlink_msg_report_transducer_state_get_rx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field tx_osc from report_transducer_state message
 *
 * @return  发射本振频率(MHz)
 */
static inline float mavlink_msg_report_transducer_state_get_tx_osc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field tx_gain from report_transducer_state message
 *
 * @return  发射增益
 */
static inline uint8_t mavlink_msg_report_transducer_state_get_tx_gain(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  23);
}

/**
 * @brief Get field if_Value from report_transducer_state message
 *
 * @return  中频电平值
 */
static inline float mavlink_msg_report_transducer_state_get_if_Value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field rf_Value from report_transducer_state message
 *
 * @return  射频电平值
 */
static inline float mavlink_msg_report_transducer_state_get_rf_Value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field lock_state from report_transducer_state message
 *
 * @return  锁定状态
 */
static inline uint8_t mavlink_msg_report_transducer_state_get_lock_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field temperature from report_transducer_state message
 *
 * @return  变频器工作温度*100（°）
 */
static inline int16_t mavlink_msg_report_transducer_state_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field voltage from report_transducer_state message
 *
 * @return  变频器工作电压*100（V）
 */
static inline int16_t mavlink_msg_report_transducer_state_get_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field electricity from report_transducer_state message
 *
 * @return  变频器工作电流*100（A）
 */
static inline int16_t mavlink_msg_report_transducer_state_get_electricity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Decode a report_transducer_state message into a struct
 *
 * @param msg The message to decode
 * @param report_transducer_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_transducer_state_decode(const mavlink_message_t* msg, mavlink_report_transducer_state_t* report_transducer_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_transducer_state->rx_osc = mavlink_msg_report_transducer_state_get_rx_osc(msg);
    report_transducer_state->tx_osc = mavlink_msg_report_transducer_state_get_tx_osc(msg);
    report_transducer_state->if_Value = mavlink_msg_report_transducer_state_get_if_Value(msg);
    report_transducer_state->rf_Value = mavlink_msg_report_transducer_state_get_rf_Value(msg);
    report_transducer_state->temperature = mavlink_msg_report_transducer_state_get_temperature(msg);
    report_transducer_state->voltage = mavlink_msg_report_transducer_state_get_voltage(msg);
    report_transducer_state->electricity = mavlink_msg_report_transducer_state_get_electricity(msg);
    report_transducer_state->rx_gain = mavlink_msg_report_transducer_state_get_rx_gain(msg);
    report_transducer_state->tx_gain = mavlink_msg_report_transducer_state_get_tx_gain(msg);
    report_transducer_state->lock_state = mavlink_msg_report_transducer_state_get_lock_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN? msg->len : MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN;
        memset(report_transducer_state, 0, MAVLINK_MSG_ID_REPORT_TRANSDUCER_STATE_LEN);
    memcpy(report_transducer_state, _MAV_PAYLOAD(msg), len);
#endif
}
