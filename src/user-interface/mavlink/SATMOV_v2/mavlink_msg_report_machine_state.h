#pragma once
// MESSAGE REPORT_MACHINE_STATE PACKING

#define MAVLINK_MSG_ID_REPORT_MACHINE_STATE 105

MAVPACKED(
typedef struct __mavlink_report_machine_state_t {
 uint32_t fault_code; /*<  系统故障码*/
 uint32_t tracking; /*<  跟踪状态*/
 int16_t acu_temperature; /*<  ACU工作温度*100（°）*/
 int16_t acu_voltage; /*<  ACU工作电压*100（V）*/
 int16_t acu_electricity; /*<  ACU工作电流*100（A）*/
 int16_t tx_temperature[8]; /*<  发射天线工作温度*100（°）*/
 int16_t tx_voltage[8]; /*<  发射天线工作电压*100（V）*/
 int16_t tx_electricity[8]; /*<  发射天线工作电流*100（A）*/
 int16_t rx_temperature[8]; /*<  接收天线工作温度*100（°）*/
 int16_t rx_voltage[8]; /*<  接收天线工作电压*100（V）*/
 int16_t rx_electricity[8]; /*<  接收天线工作电流*100（A）*/
 uint8_t rx_rf_state; /*<  接收射频状态，0x00关闭，0x01打开*/
 uint8_t tx_rf_state; /*<  发射射频状态，0x00关闭，0x01打开*/
}) mavlink_report_machine_state_t;

#define MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN 112
#define MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN 112
#define MAVLINK_MSG_ID_105_LEN 112
#define MAVLINK_MSG_ID_105_MIN_LEN 112

#define MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC 196
#define MAVLINK_MSG_ID_105_CRC 196

#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_TX_TEMPERATURE_LEN 8
#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_TX_VOLTAGE_LEN 8
#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_TX_ELECTRICITY_LEN 8
#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_RX_TEMPERATURE_LEN 8
#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_RX_VOLTAGE_LEN 8
#define MAVLINK_MSG_REPORT_MACHINE_STATE_FIELD_RX_ELECTRICITY_LEN 8

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_REPORT_MACHINE_STATE { \
    105, \
    "REPORT_MACHINE_STATE", \
    13, \
    {  { "fault_code", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_report_machine_state_t, fault_code) }, \
         { "tracking", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_report_machine_state_t, tracking) }, \
         { "rx_rf_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 110, offsetof(mavlink_report_machine_state_t, rx_rf_state) }, \
         { "tx_rf_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 111, offsetof(mavlink_report_machine_state_t, tx_rf_state) }, \
         { "acu_temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_report_machine_state_t, acu_temperature) }, \
         { "acu_voltage", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_report_machine_state_t, acu_voltage) }, \
         { "acu_electricity", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_report_machine_state_t, acu_electricity) }, \
         { "tx_temperature", NULL, MAVLINK_TYPE_INT16_T, 8, 14, offsetof(mavlink_report_machine_state_t, tx_temperature) }, \
         { "tx_voltage", NULL, MAVLINK_TYPE_INT16_T, 8, 30, offsetof(mavlink_report_machine_state_t, tx_voltage) }, \
         { "tx_electricity", NULL, MAVLINK_TYPE_INT16_T, 8, 46, offsetof(mavlink_report_machine_state_t, tx_electricity) }, \
         { "rx_temperature", NULL, MAVLINK_TYPE_INT16_T, 8, 62, offsetof(mavlink_report_machine_state_t, rx_temperature) }, \
         { "rx_voltage", NULL, MAVLINK_TYPE_INT16_T, 8, 78, offsetof(mavlink_report_machine_state_t, rx_voltage) }, \
         { "rx_electricity", NULL, MAVLINK_TYPE_INT16_T, 8, 94, offsetof(mavlink_report_machine_state_t, rx_electricity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_REPORT_MACHINE_STATE { \
    "REPORT_MACHINE_STATE", \
    13, \
    {  { "fault_code", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_report_machine_state_t, fault_code) }, \
         { "tracking", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_report_machine_state_t, tracking) }, \
         { "rx_rf_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 110, offsetof(mavlink_report_machine_state_t, rx_rf_state) }, \
         { "tx_rf_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 111, offsetof(mavlink_report_machine_state_t, tx_rf_state) }, \
         { "acu_temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_report_machine_state_t, acu_temperature) }, \
         { "acu_voltage", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_report_machine_state_t, acu_voltage) }, \
         { "acu_electricity", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_report_machine_state_t, acu_electricity) }, \
         { "tx_temperature", NULL, MAVLINK_TYPE_INT16_T, 8, 14, offsetof(mavlink_report_machine_state_t, tx_temperature) }, \
         { "tx_voltage", NULL, MAVLINK_TYPE_INT16_T, 8, 30, offsetof(mavlink_report_machine_state_t, tx_voltage) }, \
         { "tx_electricity", NULL, MAVLINK_TYPE_INT16_T, 8, 46, offsetof(mavlink_report_machine_state_t, tx_electricity) }, \
         { "rx_temperature", NULL, MAVLINK_TYPE_INT16_T, 8, 62, offsetof(mavlink_report_machine_state_t, rx_temperature) }, \
         { "rx_voltage", NULL, MAVLINK_TYPE_INT16_T, 8, 78, offsetof(mavlink_report_machine_state_t, rx_voltage) }, \
         { "rx_electricity", NULL, MAVLINK_TYPE_INT16_T, 8, 94, offsetof(mavlink_report_machine_state_t, rx_electricity) }, \
         } \
}
#endif

/**
 * @brief Pack a report_machine_state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param fault_code  系统故障码
 * @param tracking  跟踪状态
 * @param rx_rf_state  接收射频状态，0x00关闭，0x01打开
 * @param tx_rf_state  发射射频状态，0x00关闭，0x01打开
 * @param acu_temperature  ACU工作温度*100（°）
 * @param acu_voltage  ACU工作电压*100（V）
 * @param acu_electricity  ACU工作电流*100（A）
 * @param tx_temperature  发射天线工作温度*100（°）
 * @param tx_voltage  发射天线工作电压*100（V）
 * @param tx_electricity  发射天线工作电流*100（A）
 * @param rx_temperature  接收天线工作温度*100（°）
 * @param rx_voltage  接收天线工作电压*100（V）
 * @param rx_electricity  接收天线工作电流*100（A）
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_machine_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t fault_code, uint32_t tracking, uint8_t rx_rf_state, uint8_t tx_rf_state, int16_t acu_temperature, int16_t acu_voltage, int16_t acu_electricity, const int16_t *tx_temperature, const int16_t *tx_voltage, const int16_t *tx_electricity, const int16_t *rx_temperature, const int16_t *rx_voltage, const int16_t *rx_electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN];
    _mav_put_uint32_t(buf, 0, fault_code);
    _mav_put_uint32_t(buf, 4, tracking);
    _mav_put_int16_t(buf, 8, acu_temperature);
    _mav_put_int16_t(buf, 10, acu_voltage);
    _mav_put_int16_t(buf, 12, acu_electricity);
    _mav_put_uint8_t(buf, 110, rx_rf_state);
    _mav_put_uint8_t(buf, 111, tx_rf_state);
    _mav_put_int16_t_array(buf, 14, tx_temperature, 8);
    _mav_put_int16_t_array(buf, 30, tx_voltage, 8);
    _mav_put_int16_t_array(buf, 46, tx_electricity, 8);
    _mav_put_int16_t_array(buf, 62, rx_temperature, 8);
    _mav_put_int16_t_array(buf, 78, rx_voltage, 8);
    _mav_put_int16_t_array(buf, 94, rx_electricity, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN);
#else
    mavlink_report_machine_state_t packet;
    packet.fault_code = fault_code;
    packet.tracking = tracking;
    packet.acu_temperature = acu_temperature;
    packet.acu_voltage = acu_voltage;
    packet.acu_electricity = acu_electricity;
    packet.rx_rf_state = rx_rf_state;
    packet.tx_rf_state = tx_rf_state;
    mav_array_memcpy(packet.tx_temperature, tx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_voltage, tx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_electricity, tx_electricity, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_temperature, rx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_voltage, rx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_electricity, rx_electricity, sizeof(int16_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_MACHINE_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
}

/**
 * @brief Pack a report_machine_state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fault_code  系统故障码
 * @param tracking  跟踪状态
 * @param rx_rf_state  接收射频状态，0x00关闭，0x01打开
 * @param tx_rf_state  发射射频状态，0x00关闭，0x01打开
 * @param acu_temperature  ACU工作温度*100（°）
 * @param acu_voltage  ACU工作电压*100（V）
 * @param acu_electricity  ACU工作电流*100（A）
 * @param tx_temperature  发射天线工作温度*100（°）
 * @param tx_voltage  发射天线工作电压*100（V）
 * @param tx_electricity  发射天线工作电流*100（A）
 * @param rx_temperature  接收天线工作温度*100（°）
 * @param rx_voltage  接收天线工作电压*100（V）
 * @param rx_electricity  接收天线工作电流*100（A）
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_report_machine_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t fault_code,uint32_t tracking,uint8_t rx_rf_state,uint8_t tx_rf_state,int16_t acu_temperature,int16_t acu_voltage,int16_t acu_electricity,const int16_t *tx_temperature,const int16_t *tx_voltage,const int16_t *tx_electricity,const int16_t *rx_temperature,const int16_t *rx_voltage,const int16_t *rx_electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN];
    _mav_put_uint32_t(buf, 0, fault_code);
    _mav_put_uint32_t(buf, 4, tracking);
    _mav_put_int16_t(buf, 8, acu_temperature);
    _mav_put_int16_t(buf, 10, acu_voltage);
    _mav_put_int16_t(buf, 12, acu_electricity);
    _mav_put_uint8_t(buf, 110, rx_rf_state);
    _mav_put_uint8_t(buf, 111, tx_rf_state);
    _mav_put_int16_t_array(buf, 14, tx_temperature, 8);
    _mav_put_int16_t_array(buf, 30, tx_voltage, 8);
    _mav_put_int16_t_array(buf, 46, tx_electricity, 8);
    _mav_put_int16_t_array(buf, 62, rx_temperature, 8);
    _mav_put_int16_t_array(buf, 78, rx_voltage, 8);
    _mav_put_int16_t_array(buf, 94, rx_electricity, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN);
#else
    mavlink_report_machine_state_t packet;
    packet.fault_code = fault_code;
    packet.tracking = tracking;
    packet.acu_temperature = acu_temperature;
    packet.acu_voltage = acu_voltage;
    packet.acu_electricity = acu_electricity;
    packet.rx_rf_state = rx_rf_state;
    packet.tx_rf_state = tx_rf_state;
    mav_array_memcpy(packet.tx_temperature, tx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_voltage, tx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_electricity, tx_electricity, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_temperature, rx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_voltage, rx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_electricity, rx_electricity, sizeof(int16_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_REPORT_MACHINE_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
}

/**
 * @brief Encode a report_machine_state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param report_machine_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_machine_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_report_machine_state_t* report_machine_state)
{
    return mavlink_msg_report_machine_state_pack(system_id, component_id, msg, report_machine_state->fault_code, report_machine_state->tracking, report_machine_state->rx_rf_state, report_machine_state->tx_rf_state, report_machine_state->acu_temperature, report_machine_state->acu_voltage, report_machine_state->acu_electricity, report_machine_state->tx_temperature, report_machine_state->tx_voltage, report_machine_state->tx_electricity, report_machine_state->rx_temperature, report_machine_state->rx_voltage, report_machine_state->rx_electricity);
}

/**
 * @brief Encode a report_machine_state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param report_machine_state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_report_machine_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_report_machine_state_t* report_machine_state)
{
    return mavlink_msg_report_machine_state_pack_chan(system_id, component_id, chan, msg, report_machine_state->fault_code, report_machine_state->tracking, report_machine_state->rx_rf_state, report_machine_state->tx_rf_state, report_machine_state->acu_temperature, report_machine_state->acu_voltage, report_machine_state->acu_electricity, report_machine_state->tx_temperature, report_machine_state->tx_voltage, report_machine_state->tx_electricity, report_machine_state->rx_temperature, report_machine_state->rx_voltage, report_machine_state->rx_electricity);
}

/**
 * @brief Send a report_machine_state message
 * @param chan MAVLink channel to send the message
 *
 * @param fault_code  系统故障码
 * @param tracking  跟踪状态
 * @param rx_rf_state  接收射频状态，0x00关闭，0x01打开
 * @param tx_rf_state  发射射频状态，0x00关闭，0x01打开
 * @param acu_temperature  ACU工作温度*100（°）
 * @param acu_voltage  ACU工作电压*100（V）
 * @param acu_electricity  ACU工作电流*100（A）
 * @param tx_temperature  发射天线工作温度*100（°）
 * @param tx_voltage  发射天线工作电压*100（V）
 * @param tx_electricity  发射天线工作电流*100（A）
 * @param rx_temperature  接收天线工作温度*100（°）
 * @param rx_voltage  接收天线工作电压*100（V）
 * @param rx_electricity  接收天线工作电流*100（A）
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_report_machine_state_send(mavlink_channel_t chan, uint32_t fault_code, uint32_t tracking, uint8_t rx_rf_state, uint8_t tx_rf_state, int16_t acu_temperature, int16_t acu_voltage, int16_t acu_electricity, const int16_t *tx_temperature, const int16_t *tx_voltage, const int16_t *tx_electricity, const int16_t *rx_temperature, const int16_t *rx_voltage, const int16_t *rx_electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN];
    _mav_put_uint32_t(buf, 0, fault_code);
    _mav_put_uint32_t(buf, 4, tracking);
    _mav_put_int16_t(buf, 8, acu_temperature);
    _mav_put_int16_t(buf, 10, acu_voltage);
    _mav_put_int16_t(buf, 12, acu_electricity);
    _mav_put_uint8_t(buf, 110, rx_rf_state);
    _mav_put_uint8_t(buf, 111, tx_rf_state);
    _mav_put_int16_t_array(buf, 14, tx_temperature, 8);
    _mav_put_int16_t_array(buf, 30, tx_voltage, 8);
    _mav_put_int16_t_array(buf, 46, tx_electricity, 8);
    _mav_put_int16_t_array(buf, 62, rx_temperature, 8);
    _mav_put_int16_t_array(buf, 78, rx_voltage, 8);
    _mav_put_int16_t_array(buf, 94, rx_electricity, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE, buf, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
#else
    mavlink_report_machine_state_t packet;
    packet.fault_code = fault_code;
    packet.tracking = tracking;
    packet.acu_temperature = acu_temperature;
    packet.acu_voltage = acu_voltage;
    packet.acu_electricity = acu_electricity;
    packet.rx_rf_state = rx_rf_state;
    packet.tx_rf_state = tx_rf_state;
    mav_array_memcpy(packet.tx_temperature, tx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_voltage, tx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.tx_electricity, tx_electricity, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_temperature, rx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_voltage, rx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet.rx_electricity, rx_electricity, sizeof(int16_t)*8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE, (const char *)&packet, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
#endif
}

/**
 * @brief Send a report_machine_state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_report_machine_state_send_struct(mavlink_channel_t chan, const mavlink_report_machine_state_t* report_machine_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_report_machine_state_send(chan, report_machine_state->fault_code, report_machine_state->tracking, report_machine_state->rx_rf_state, report_machine_state->tx_rf_state, report_machine_state->acu_temperature, report_machine_state->acu_voltage, report_machine_state->acu_electricity, report_machine_state->tx_temperature, report_machine_state->tx_voltage, report_machine_state->tx_electricity, report_machine_state->rx_temperature, report_machine_state->rx_voltage, report_machine_state->rx_electricity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE, (const char *)report_machine_state, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_report_machine_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t fault_code, uint32_t tracking, uint8_t rx_rf_state, uint8_t tx_rf_state, int16_t acu_temperature, int16_t acu_voltage, int16_t acu_electricity, const int16_t *tx_temperature, const int16_t *tx_voltage, const int16_t *tx_electricity, const int16_t *rx_temperature, const int16_t *rx_voltage, const int16_t *rx_electricity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, fault_code);
    _mav_put_uint32_t(buf, 4, tracking);
    _mav_put_int16_t(buf, 8, acu_temperature);
    _mav_put_int16_t(buf, 10, acu_voltage);
    _mav_put_int16_t(buf, 12, acu_electricity);
    _mav_put_uint8_t(buf, 110, rx_rf_state);
    _mav_put_uint8_t(buf, 111, tx_rf_state);
    _mav_put_int16_t_array(buf, 14, tx_temperature, 8);
    _mav_put_int16_t_array(buf, 30, tx_voltage, 8);
    _mav_put_int16_t_array(buf, 46, tx_electricity, 8);
    _mav_put_int16_t_array(buf, 62, rx_temperature, 8);
    _mav_put_int16_t_array(buf, 78, rx_voltage, 8);
    _mav_put_int16_t_array(buf, 94, rx_electricity, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE, buf, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
#else
    mavlink_report_machine_state_t *packet = (mavlink_report_machine_state_t *)msgbuf;
    packet->fault_code = fault_code;
    packet->tracking = tracking;
    packet->acu_temperature = acu_temperature;
    packet->acu_voltage = acu_voltage;
    packet->acu_electricity = acu_electricity;
    packet->rx_rf_state = rx_rf_state;
    packet->tx_rf_state = tx_rf_state;
    mav_array_memcpy(packet->tx_temperature, tx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet->tx_voltage, tx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet->tx_electricity, tx_electricity, sizeof(int16_t)*8);
    mav_array_memcpy(packet->rx_temperature, rx_temperature, sizeof(int16_t)*8);
    mav_array_memcpy(packet->rx_voltage, rx_voltage, sizeof(int16_t)*8);
    mav_array_memcpy(packet->rx_electricity, rx_electricity, sizeof(int16_t)*8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_REPORT_MACHINE_STATE, (const char *)packet, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_MIN_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE REPORT_MACHINE_STATE UNPACKING


/**
 * @brief Get field fault_code from report_machine_state message
 *
 * @return  系统故障码
 */
static inline uint32_t mavlink_msg_report_machine_state_get_fault_code(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field tracking from report_machine_state message
 *
 * @return  跟踪状态
 */
static inline uint32_t mavlink_msg_report_machine_state_get_tracking(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field rx_rf_state from report_machine_state message
 *
 * @return  接收射频状态，0x00关闭，0x01打开
 */
static inline uint8_t mavlink_msg_report_machine_state_get_rx_rf_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  110);
}

/**
 * @brief Get field tx_rf_state from report_machine_state message
 *
 * @return  发射射频状态，0x00关闭，0x01打开
 */
static inline uint8_t mavlink_msg_report_machine_state_get_tx_rf_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  111);
}

/**
 * @brief Get field acu_temperature from report_machine_state message
 *
 * @return  ACU工作温度*100（°）
 */
static inline int16_t mavlink_msg_report_machine_state_get_acu_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field acu_voltage from report_machine_state message
 *
 * @return  ACU工作电压*100（V）
 */
static inline int16_t mavlink_msg_report_machine_state_get_acu_voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field acu_electricity from report_machine_state message
 *
 * @return  ACU工作电流*100（A）
 */
static inline int16_t mavlink_msg_report_machine_state_get_acu_electricity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field tx_temperature from report_machine_state message
 *
 * @return  发射天线工作温度*100（°）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_tx_temperature(const mavlink_message_t* msg, int16_t *tx_temperature)
{
    return _MAV_RETURN_int16_t_array(msg, tx_temperature, 8,  14);
}

/**
 * @brief Get field tx_voltage from report_machine_state message
 *
 * @return  发射天线工作电压*100（V）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_tx_voltage(const mavlink_message_t* msg, int16_t *tx_voltage)
{
    return _MAV_RETURN_int16_t_array(msg, tx_voltage, 8,  30);
}

/**
 * @brief Get field tx_electricity from report_machine_state message
 *
 * @return  发射天线工作电流*100（A）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_tx_electricity(const mavlink_message_t* msg, int16_t *tx_electricity)
{
    return _MAV_RETURN_int16_t_array(msg, tx_electricity, 8,  46);
}

/**
 * @brief Get field rx_temperature from report_machine_state message
 *
 * @return  接收天线工作温度*100（°）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_rx_temperature(const mavlink_message_t* msg, int16_t *rx_temperature)
{
    return _MAV_RETURN_int16_t_array(msg, rx_temperature, 8,  62);
}

/**
 * @brief Get field rx_voltage from report_machine_state message
 *
 * @return  接收天线工作电压*100（V）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_rx_voltage(const mavlink_message_t* msg, int16_t *rx_voltage)
{
    return _MAV_RETURN_int16_t_array(msg, rx_voltage, 8,  78);
}

/**
 * @brief Get field rx_electricity from report_machine_state message
 *
 * @return  接收天线工作电流*100（A）
 */
static inline uint16_t mavlink_msg_report_machine_state_get_rx_electricity(const mavlink_message_t* msg, int16_t *rx_electricity)
{
    return _MAV_RETURN_int16_t_array(msg, rx_electricity, 8,  94);
}

/**
 * @brief Decode a report_machine_state message into a struct
 *
 * @param msg The message to decode
 * @param report_machine_state C-struct to decode the message contents into
 */
static inline void mavlink_msg_report_machine_state_decode(const mavlink_message_t* msg, mavlink_report_machine_state_t* report_machine_state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    report_machine_state->fault_code = mavlink_msg_report_machine_state_get_fault_code(msg);
    report_machine_state->tracking = mavlink_msg_report_machine_state_get_tracking(msg);
    report_machine_state->acu_temperature = mavlink_msg_report_machine_state_get_acu_temperature(msg);
    report_machine_state->acu_voltage = mavlink_msg_report_machine_state_get_acu_voltage(msg);
    report_machine_state->acu_electricity = mavlink_msg_report_machine_state_get_acu_electricity(msg);
    mavlink_msg_report_machine_state_get_tx_temperature(msg, report_machine_state->tx_temperature);
    mavlink_msg_report_machine_state_get_tx_voltage(msg, report_machine_state->tx_voltage);
    mavlink_msg_report_machine_state_get_tx_electricity(msg, report_machine_state->tx_electricity);
    mavlink_msg_report_machine_state_get_rx_temperature(msg, report_machine_state->rx_temperature);
    mavlink_msg_report_machine_state_get_rx_voltage(msg, report_machine_state->rx_voltage);
    mavlink_msg_report_machine_state_get_rx_electricity(msg, report_machine_state->rx_electricity);
    report_machine_state->rx_rf_state = mavlink_msg_report_machine_state_get_rx_rf_state(msg);
    report_machine_state->tx_rf_state = mavlink_msg_report_machine_state_get_tx_rf_state(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN? msg->len : MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN;
        memset(report_machine_state, 0, MAVLINK_MSG_ID_REPORT_MACHINE_STATE_LEN);
    memcpy(report_machine_state, _MAV_PAYLOAD(msg), len);
#endif
}
