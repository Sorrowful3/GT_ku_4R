/*
 * antenna_v0_msg.c
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#include "antenna_V0_msg.h"

#define DEBUG_TAG   "antenna_V0_msg,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V0

#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V0_ENABLE_MATE_DEBUG
static uint8_t mate_table[] = {0x0B, 0x1B, 0x29};
#endif /* ANTENNA_V0_ENABLE_MATE_DEBUG */
#endif /* DEBUG_LEVEL >= DEBUG_OUTPUT */

antenna_v0_parse_state_t antenna_v0_parse_byte(antenna_v0_msg_t *msg, uint8_t data)
{
    uint16_t crc;

    if(!msg) {
        LOG_E("parameter error");
        return ANTENNA_PARSE_BAD;
    }

    if(msg->current_lenght >= ANTENNA_V0_BIG_PACK_LENGHT) {
        return ANTENNA_PARSE_BAD;
    }

    msg->load[msg->current_lenght] = data;
    msg->current_lenght++;

    switch(msg->current_lenght)
    {
        case 1:                                                         //帧头第1字节
            if(msg->load[0] == 0xAA) {
                return  ANTENNA_PARSE_INCOMPLETE;
            }
            return ANTENNA_PARSE_BAD;
        case 2:                                                         //帧头第2字节
            if(msg->load[1] == 0x55) {
                return ANTENNA_PARSE_INCOMPLETE;
            }
            return ANTENNA_PARSE_BAD;
        case 3:                                                         //帧长第字节（高8位）
            msg->frame_lenght = (uint16_t)((msg->load[2]) << 8);
            return ANTENNA_PARSE_INCOMPLETE;
        case 4:                                                         //帧长第2字节（低8位）
            msg->frame_lenght |= (uint16_t)(msg->load[3]);
            if(msg->frame_lenght <= ANTENNA_V0_BIG_PACK_LENGHT) {
                return ANTENNA_PARSE_INCOMPLETE;
            }
            return ANTENNA_PARSE_BAD;
        default:
            if(msg->current_lenght < msg->frame_lenght) {
                return ANTENNA_PARSE_INCOMPLETE;
            }

            //进行CRC校验
            crc = crc16(msg->load,  msg->frame_lenght-2, 0xffff);
            if(crc != (((uint16_t)(msg->load[msg->frame_lenght-2]) << 8) | (uint16_t)(msg->load[msg->frame_lenght-1]))) {
                //CRC校验失败
                return ANTENNA_PARSE_BAD;
            }
            //提取命令码
            msg->msg_id = msg->load[4];
            //调试输出
#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V0_ENABLE_MATE_DEBUG
            for(uint16_t i=0; i<sizeof(mate_table)/sizeof(mate_table[0]); i++) {
                if(msg->msg_id == mate_table[i]) {
                    xil_printf("R:\r\n");
                    for(uint16_t j=0; j<msg->frame_lenght; j++) {
                        xil_printf("%02x ", msg->load[j]);
                    }
                    xil_printf("\r\n");
                    break;
                }
            }
#else
            xil_printf("R:\r\n");
            for(uint16_t i=0; i<msg->frame_lenght; i++) {
                xil_printf("%02x ", msg->load[i]);
            }
            xil_printf("\r\n");
#endif
#endif

            return ANTENNA_PARSE_OK;
    }
}

ssize_t antenna_v0_msg_2_buffer(antenna_v0_msg_t *msg, uint8_t *buffer)
{
    uint16_t crc;

    if((!msg) || (msg->frame_lenght > ANTENNA_V0_BIG_PACK_LENGHT)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //将msg的载荷补充完整
    //帧头
    msg->load[0] = 0xAA;
    msg->load[1] = 0x55;
    //帧长
    LITTLE16_2_BIG16(&msg->load[2], &msg->frame_lenght);
    //命令码
    msg->load[4] = msg->msg_id;
    //CRC
    crc = crc16(msg->load,  msg->frame_lenght-2, 0xFFFF);
    LITTLE16_2_BIG16(&msg->load[msg->frame_lenght - 2], &crc);

    //将载荷拷贝到buffer
    if(buffer) {
        memcpy(buffer, msg->load, msg->frame_lenght);
    }
#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V0_ENABLE_MATE_DEBUG
    for(uint16_t i=0; i<sizeof(mate_table)/sizeof(mate_table[0]); i++) {
        if(msg->msg_id == mate_table[i]) {
            xil_printf("T:\r\n");
            for(int i=0; i<msg->frame_lenght; i++) {
                xil_printf("%02x ", msg->load[i]);
            }
            xil_printf("\r\n");
            break;
        }
    }
#else
    xil_printf("T:\r\n");
    for(int i=0; i<msg->frame_lenght; i++) {
        xil_printf("%02x ", msg->load[i]);
    }
    xil_printf("\r\n");
#endif
#endif

    return (ssize_t)msg->frame_lenght;
}

int32_t antenna_v0_msg_send(antenna_v0_msg_t *msg)
{
    ssize_t lenght;
    ssize_t result;

    if(!msg) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //开始发送
    antenna_v0_port_write_start();

    //序列化
    lenght = antenna_v0_msg_2_buffer(msg, NULL);

    //发送数据到天线
    result = antenna_v0_port_write(msg->load, lenght);

    //发送完成
    antenna_v0_port_write_end();

    if(result != lenght) {
        return ERR_IO;
    }
    return ERR_OK;
}

int32_t antenna_v0_beam_pack(const antenna_v0_beam_tx_t *beam, antenna_v0_msg_t *msg)
{
    uint16_t polarity;
    uint16_t theta;
    uint16_t phi;
    uint32_t freq;
    uint32_t freq_big;

    if((!beam) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    polarity = (uint16_t)((beam->polarity * 100.0f) + 0.5);
    theta = (uint16_t)((beam->theta * 100.0f) + 0.5);
    phi = (uint16_t)((beam->phi * 100.0f) + 0.5);
    freq = (uint32_t)((beam->freq * 39321.6f) + 0.5f);
    LITTLE32_2_BIG32(&freq_big, &freq);
    freq_big = freq_big >> 8;

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x22;

    LITTLE16_2_BIG16(&msg->load[5], &polarity);
    LITTLE16_2_BIG16(&msg->load[7], &theta);
    LITTLE16_2_BIG16(&msg->load[9], &phi);
    msg->load[11] = beam->weight;
    msg->load[12] = beam->TorR;
    msg->load[15] = beam->polarity_mode;
    memcpy(&msg->load[16], &freq_big, 3);

    return ERR_OK;
}

int32_t antenna_v0_beam_send(const antenna_v0_beam_tx_t *beam)
{
    antenna_v0_msg_t msg;

    if(!beam) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_beam_pack(beam, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_beam_decode(const antenna_v0_msg_t *msg, antenna_v0_beam_rx_t *beam)
{
    if((!beam) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    beam->TorR = msg->load[5];
    beam->subset = msg->load[6];
    beam->error = msg->load[7];

    return ERR_OK;
}

int32_t antenna_v0_versions_pack(const antenna_v0_versions_tx_t *versions, antenna_v0_msg_t *msg)
{
    if((!versions) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x12;

    msg->load[5] = versions->TorR;
    msg->load[6] = versions->subset;

    return ERR_OK;
}

int32_t antenna_v0_versions_send(const antenna_v0_versions_tx_t *versions)
{
    antenna_v0_msg_t msg;

    if(!versions) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_versions_pack(versions, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_versions_decode(const antenna_v0_msg_t *msg, antenna_v0_versions_rx_t *versions)
{
    uint32_t version;

    if((!versions) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    versions->TorR = msg->load[5];
    versions->subset = msg->load[6];
    memcpy(&version, &msg->load[7], 3);
    BIG32_2_LITTLE32(&versions->version, &version);
    versions->version = versions->version >> 8;
    versions->baud = msg->load[10];

    return ERR_OK;
}

int32_t antenna_v0_rf_vcc_pack(const antenna_v0_rf_vcc_tx_t *rf_vcc, antenna_v0_msg_t *msg)
{
    if((!rf_vcc) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x36;

    msg->load[5] = rf_vcc->state;
    msg->load[6] = rf_vcc->polarity;    //0x00 垂直，0x01 水平，0xff 双极化
    msg->load[7] = rf_vcc->TorR;
    if(rf_vcc->polarity == 0x00) {
        msg->load[8] = 0x01;            //0x00 双极化 ，0x01 垂直，0x02 水平
    }
    else if(rf_vcc->polarity == 0x01) {
        msg->load[8] = 0x02;
    }
    else {
        msg->load[8] = 0x00;
    }

    return ERR_OK;
}

int32_t antenna_v0_rf_vcc_send(const antenna_v0_rf_vcc_tx_t *rf_vcc)
{
    antenna_v0_msg_t msg;

    if(!rf_vcc) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_rf_vcc_pack(rf_vcc, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_rf_vcc_decode(const antenna_v0_msg_t *msg, antenna_v0_rf_vcc_rx_t *rf_vcc)
{
    if((!rf_vcc) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    rf_vcc->state = msg->load[5];
    rf_vcc->polarity = msg->load[6];
    rf_vcc->TorR = msg->load[7];
    rf_vcc->subset = msg->load[8];

    return ERR_OK;
}

int32_t antenna_v0_reboot_pack(const antenna_v0_reboot_tx_t *reboot, antenna_v0_msg_t *msg)
{
    if((!reboot) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x11;

    return ERR_OK;
}

int32_t antenna_v0_reboot_send(const antenna_v0_reboot_tx_t *reboot)
{
    antenna_v0_msg_t msg;

    if(!reboot) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_reboot_pack(reboot, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_selfcheck_pack(const antenna_v0_selfcheck_tx_t *selfcheck, antenna_v0_msg_t *msg)
{
    if((!selfcheck) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x21;

    msg->load[5] = selfcheck->TorR;
    msg->load[6] = selfcheck->subset;

    return ERR_OK;
}

int32_t antenna_v0_selfcheck_send(const antenna_v0_selfcheck_tx_t *selfcheck)
{
    antenna_v0_msg_t msg;

    if(!selfcheck) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_selfcheck_pack(selfcheck, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_selfcheck_decode(const antenna_v0_msg_t *msg, antenna_v0_selfcheck_rx_t *selfcheck)
{
    if((!selfcheck) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    selfcheck->TorR = msg->load[5];
    selfcheck->subset = msg->load[6];
    selfcheck->total_frame = msg->load[7];
    selfcheck->current_frame = msg->load[8];
    BIG16_2_LITTLE16(&selfcheck->check_data[0], &msg->load[9]);
    BIG16_2_LITTLE16(&selfcheck->check_data[1], &msg->load[12]);
    BIG16_2_LITTLE16(&selfcheck->check_data[2], &msg->load[15]);
    BIG16_2_LITTLE16(&selfcheck->check_data[3], &msg->load[18]);

    return ERR_OK;
}

int32_t antenna_v0_temperature_pack(const antenna_v0_temperature_tx_t *temperature, antenna_v0_msg_t *msg)
{
    if((!temperature) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x13;

    msg->load[5] = temperature->TorR;
    msg->load[6] = temperature->subset;
    memcpy(&msg->load[10], temperature->chip_number, 9);

    return ERR_OK;
}

int32_t antenna_v0_temperature_send(const antenna_v0_temperature_tx_t *temperature)
{
    antenna_v0_msg_t msg;

    if(!temperature) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_temperature_pack(temperature, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_temperature_decode(const antenna_v0_msg_t *msg, antenna_v0_temperature_rx_t *temperature)
{
    uint32_t i;

    if((!temperature) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    temperature->TorR = msg->load[5];
    temperature->subset = msg->load[6];
    memcpy(temperature->board_temperature, &msg->load[7], 3);
    memcpy(temperature->chip_temperature, &msg->load[10], 9);

    //数据格式转换
    for(i=0; i<9; i++) {
        temperature->chip_temperature[i] = (int8_t)(543.5f - (0.844f * temperature->chip_temperature[i] * 4.0f));
    }

    return ERR_OK;
}

int32_t antenna_v0_power_pack(const antenna_v0_power_tx_t *power, antenna_v0_msg_t *msg)
{
    if((!power) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x18;

    msg->load[5] = power->TorR;
    msg->load[6] = power->subset;

    return ERR_OK;
}

int32_t antenna_v0_power_send(const antenna_v0_power_tx_t *power)
{
    antenna_v0_msg_t msg;

    if(!power) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_power_pack(power, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_power_decode(const antenna_v0_msg_t *msg, antenna_v0_power_rx_t *power)
{
    uint16_t total_electricity;
    uint16_t total_voltage;
    uint16_t total_power;

    if((!power) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    power->TorR = msg->load[5];
    power->subset = msg->load[6];
    BIG16_2_LITTLE16(&total_electricity, &msg->load[7]);
    BIG16_2_LITTLE16(&total_voltage, &msg->load[9]);
    BIG16_2_LITTLE16(&total_power, &msg->load[11]);

    power->total_electricity = total_electricity * 0.009765625f;
    power->total_voltage = total_voltage / 8.0f * 4.0f / 1000.0f;
    power->total_power = total_power * 20.0f * 0.009765625f;

    return ERR_OK;
}

int32_t antenna_v0_voltage_pack(const antenna_v0_voltage_tx_t *voltage, antenna_v0_msg_t *msg)
{
    if((!voltage) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x1A;

    msg->load[5] = voltage->TorR;
    msg->load[6] = voltage->subset;

    return ERR_OK;
}

int32_t antenna_v0_voltage_send(const antenna_v0_voltage_tx_t *voltage)
{
    antenna_v0_msg_t msg;

    if(!voltage) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_voltage_pack(voltage, &msg);
    return antenna_v0_msg_send(&msg);
}

int32_t antenna_v0_voltage_decode(const antenna_v0_msg_t *msg, antenna_v0_voltage_rx_t *voltage)
{
    uint8_t vdd1_00;
    uint8_t vdd1_01;
    uint8_t maam_vdd;
    uint8_t maam_vg;

    if((!voltage) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    voltage->TorR = msg->load[5];
    voltage->subset = msg->load[6];
    vdd1_00 = msg->load[7];
    vdd1_01 = msg->load[8];
    maam_vdd = msg->load[9];
    maam_vg = msg->load[10];

    voltage->vdd1_00 = vdd1_00 / 256.0f;
    voltage->vdd1_01 = vdd1_01 / 256.0f;
    voltage->maam_vdd = maam_vdd / 256.0f;
    voltage->maam_vg = maam_vg / 256.0f;

    return ERR_OK;
}

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
int32_t antenna_v0_offset_pack(const antenna_v0_offset_tx_t *offset, antenna_v0_msg_t *msg)
{
    uint16_t v_range;
    int16_t v_phase;
    uint16_t h_range;
    int16_t h_phase;

    if((!offset) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //转换校准数据
    if(offset->range >= 0.0f) {
        h_range = (uint16_t)(offset->range * 2.0f + 0.5f);
        v_range = 0;
    }
    else {
        h_range = 0;
        v_range = (uint16_t)((-offset->range) * 2.0f + 0.5f);
    }
    if(offset->phase >= 0.0f) {
        v_phase = (int16_t)(offset->phase * 64.0f + 0.5f);
    }
    else {
        v_phase = (int16_t)(offset->phase * 64.0f - 0.5f);
    }
    h_phase = 0;

    memset(msg, 0, sizeof(antenna_v0_msg_t));

    msg->frame_lenght = ANTENNA_V0_SMALL_PACK_LENGHT;
    msg->msg_id = 0x29;

    msg->load[5] = offset->TorR;
    msg->load[6] = 0x00;
    //垂直幅度相位
    msg->load[7] = (uint8_t)(v_range>>8);
    msg->load[8] = (uint8_t)v_range;
    msg->load[9] = (uint8_t)(v_phase>>8);
    msg->load[10] = (uint8_t)v_phase;
    //水平幅度相位
    msg->load[11] = (uint8_t)(h_range>>8);
    msg->load[12] = (uint8_t)h_range;
    msg->load[13] = (uint8_t)(h_phase>>8);
    msg->load[14] = (uint8_t)h_phase;

    return ERR_OK;
}

int32_t antenna_v0_offset_send(const antenna_v0_offset_tx_t *offset)
{
    antenna_v0_msg_t msg;

    if(!offset) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v0_offset_pack(offset, &msg);
    return antenna_v0_msg_send(&msg);
}
#endif

#endif /* USING_ANTENNA_V0 */
