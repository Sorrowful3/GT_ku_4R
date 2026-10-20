/*
 * antenna_v1_msg.c
 *
 *  Created on: 2021年9月5日
 *      Author: Administrator
 */

#include "antenna_v1_msg.h"

#define DEBUG_TAG   "antenna_v1_msg,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V1

#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V1_ENABLE_MATE_DEBUG
static uint8_t mate_table[] = {0x27, 0x1B, 0x18, 0x1a, 0x21, 0x26, 0x36};
#endif /* ANTENNA_V1_ENABLE_MATE_DEBUG */
#endif /* DEBUG_LEVEL >= DEBUG_OUTPUT */

antenna_v1_parse_state_t antenna_v1_parse_byte(antenna_v1_msg_t *msg, uint8_t data)
{
    uint16_t crc;

    if(!msg) {
        LOG_E("parameter error");
        return ANTENNA_V1_PARSE_BAD;
    }

    //msg最大长度为ANTENNA_V1_BIG_PACK_LENGHT
    if(msg->current_lenght >= ANTENNA_V1_BIG_PACK_LENGHT) {
        return ANTENNA_V1_PARSE_BAD;
    }

    msg->load[msg->current_lenght] = data;
    msg->current_lenght++;

    switch(msg->current_lenght)
    {
        case 1:                                                         //帧头第1字节
            if(msg->load[0] == 0xAA) {
                return  ANTENNA_V1_PARSE_INCOMPLETE;
            }
            return ANTENNA_V1_PARSE_BAD;
        case 2:                                                         //帧头第2字节
            if(msg->load[1] == 0x55) {
                return ANTENNA_V1_PARSE_INCOMPLETE;
            }
            return ANTENNA_V1_PARSE_BAD;
        case 3:                                                         //帧长第字节（高8位）
            msg->frame_lenght = (uint16_t)((msg->load[2]) << 8);
            return ANTENNA_V1_PARSE_INCOMPLETE;
        case 4:                                                         //帧长第2字节（低8位）
            msg->frame_lenght |= (uint16_t)(msg->load[3]);
            if(msg->frame_lenght <= ANTENNA_V1_BIG_PACK_LENGHT) {
                return ANTENNA_V1_PARSE_INCOMPLETE;
            }
            return ANTENNA_V1_PARSE_BAD;
        default:
            if(msg->current_lenght < msg->frame_lenght) {
                return ANTENNA_V1_PARSE_INCOMPLETE;
            }

            //进行CRC校验
            crc = crc16(msg->load,  msg->frame_lenght-2, 0xffff);
            if(crc != (((uint16_t)(msg->load[msg->frame_lenght-2]) << 8) | (uint16_t)(msg->load[msg->frame_lenght-1]))) {
                //CRC校验失败
                return ANTENNA_V1_PARSE_BAD;
            }
            //提取命令码
            msg->msg_id = msg->load[4];

            //调试输出
#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V1_ENABLE_MATE_DEBUG
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
            return ANTENNA_V1_PARSE_OK;
    }
}

ssize_t antenna_v1_msg_2_buffer(antenna_v1_msg_t *msg, uint8_t *buffer)
{
    uint16_t crc;

    if((!msg) || (msg->frame_lenght > ANTENNA_V1_BIG_PACK_LENGHT)) {
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
    LITTLE16_2_BIG16(&msg->load[msg->frame_lenght-2], &crc);

    //将载荷拷贝到buffer
    if(buffer) {
        memcpy(buffer, msg->load, msg->frame_lenght);
    }

#if DEBUG_LEVEL >= DEBUG_OUTPUT
#ifdef ANTENNA_V1_ENABLE_MATE_DEBUG
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

int32_t antenna_v1_msg_send(antenna_v1_msg_t *msg)
{
    ssize_t lenght;
    ssize_t result;

    if(!msg) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //开始发送
    antenna_v1_port_write_start();

    //序列化
    lenght = antenna_v1_msg_2_buffer(msg, NULL);

    //发送数据到天线
    result = antenna_v1_port_write(msg->load, lenght);

    //发送完成
    antenna_v1_port_write_end();

    if(result != lenght) {
        return ERR_IO;
    }
    return ERR_OK;
}

int32_t antenna_v1_init_check_pack(const antenna_v1_init_check_tx_t *init_check, antenna_v1_msg_t *msg)
{
    if((!init_check) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x09;

    msg->load[5] = init_check->TorR;
    msg->load[6] = init_check->subset;

    return ERR_OK;
}

int32_t antenna_v1_init_check_send(const antenna_v1_init_check_tx_t *init_check)
{
    antenna_v1_msg_t msg;

    if(!init_check) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_init_check_pack(init_check, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_init_check_decode(const antenna_v1_msg_t *msg, antenna_v1_init_check_rx_t *init_check)
{
    if((!msg) || (!init_check)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    init_check->TorR = msg->load[5];
    init_check->subset = msg->load[6];
    init_check->state = msg->load[7];
    init_check->init_flag = msg->load[8];

    return ERR_OK;
}

int32_t antenna_v1_comm_error_decode(const antenna_v1_msg_t *msg, antenna_v1_comm_error_rx_t *comm_error)
{
    if((!msg) || (!comm_error)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    comm_error->TorR = msg->load[5];
    comm_error->subset = msg->load[6];
    comm_error->fault_code = msg->load[7];

    return ERR_OK;
}

int32_t antenna_v1_reset_pack(const antenna_v1_reset_tx_t *reset, antenna_v1_msg_t *msg)
{
    if((!reset) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x11;

    return ERR_OK;
}

int32_t antenna_v1_reset_send(const antenna_v1_reset_tx_t *reset)
{
    antenna_v1_msg_t msg;

    if(!reset) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_reset_pack(reset, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_reset_decode(const antenna_v1_msg_t *msg, antenna_v1_reset_rx_t *reset)
{
    if((!msg) || (!reset)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    reset->TorR = msg->load[5];
    reset->subset = msg->load[6];

    return ERR_OK;
}

int32_t antenna_v1_versions_pack(const antenna_v1_versions_tx_t *versions, antenna_v1_msg_t *msg)
{
    if((!versions) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x12;

    msg->load[5] = versions->TorR;
    msg->load[6] = versions->subset;

    return ERR_OK;
}

int32_t antenna_v1_versions_send(const antenna_v1_versions_tx_t *versions)
{
    antenna_v1_msg_t msg;

    if(!versions) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_versions_pack(versions, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_versions_decode(const antenna_v1_msg_t *msg, antenna_v1_versions_rx_t *versions)
{
    if((!msg) || (!versions)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    versions->TorR = msg->load[5];
    versions->subset = msg->load[6];
    versions->major = msg->load[7];
    versions->minor = msg->load[8];
    versions->amend = msg->load[9];
    memcpy(&versions->data, &msg->load[10], 3);
    versions->antenna_type = msg->load[13];
    versions->work_freq = msg->load[14];
    versions->work_type = msg->load[15];
    versions->antenna_type = msg->load[13];
    memcpy(&versions->channel_nbr, &msg->load[16], 2);
    versions->large_version = msg->load[18];
    versions->small_edition = msg->load[19];

    return ERR_OK;
}

int32_t antenna_v1_power_pack(const antenna_v1_power_tx_t *power, antenna_v1_msg_t *msg)
{
    if((!power) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x1A;

    msg->load[5] = power->TorR;
    msg->load[6] = power->subset;

    return ERR_OK;
}

int32_t antenna_v1_power_send(const antenna_v1_power_tx_t *power)
{
    antenna_v1_msg_t msg;

    if(!power) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_power_pack(power, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_power_decode(const antenna_v1_msg_t *msg, antenna_v1_power_rx_t *power)
{
    int32_t i;

    if((!msg) || (!power)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(power, 0x00, sizeof(antenna_v1_power_rx_t));

    power->TorR = msg->load[5];
    power->subset = msg->load[6];
    power->total = msg->load[7];
    power->current = msg->load[8];
    if(power->current == 0x01) {
        BIG16_2_LITTLE16(&power->total_electricity, &msg->load[12]);
        BIG16_2_LITTLE16(&power->total_voltage, &msg->load[14]);
        for(i=0; i<2; i++) {
            BIG16_2_LITTLE16(&power->xadc_voltage[i], &msg->load[16+i*2]);
        }
    }
    else {
        for(i=0; i<4; i++) {
            BIG16_2_LITTLE16(&power->xadc_voltage[i], &msg->load[9+i*2]);
        }
    }

    return ERR_OK;
}

int32_t antenna_v1_beam_pack(const antenna_v1_beam_tx_t *beam, antenna_v1_msg_t *msg)
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

    polarity = (uint16_t)((beam->polarity * 100.0f) + 0.5f);
    theta = (uint16_t)((beam->theta * 100.0f) + 0.5f);
    phi = (uint16_t)((beam->phi * 100.0f) + 0.5f);
    freq = (uint32_t)((beam->freq * 39321.6f) + 0.5f);
    LITTLE32_2_BIG32(&freq_big, &freq);
    freq_big = freq_big >> 8;

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x22;

    LITTLE16_2_BIG16(&msg->load[5], &polarity);
    LITTLE16_2_BIG16(&msg->load[7], &theta);
    LITTLE16_2_BIG16(&msg->load[9], &phi);
    msg->load[11] = beam->weight;
    msg->load[12] = beam->TorR;
    memcpy(&msg->load[16], &freq_big, 3);

    return ERR_OK;
}

int32_t antenna_v1_beam_send(const antenna_v1_beam_tx_t *beam)
{
    antenna_v1_msg_t msg;

    if(!beam) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_beam_pack(beam, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_beam_decode(const antenna_v1_msg_t *msg, antenna_v1_beam_rx_t *beam)
{
    if((!msg) || (!beam)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    beam->TorR = msg->load[5];
    beam->subset = msg->load[6];
    beam->result = msg->load[7];

    return ERR_OK;
}

int32_t antenna_v1_config_pack(const antenna_v1_config_tx_t *config, antenna_v1_msg_t *msg)
{
    if((!config) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x26;

    msg->load[5] = config->TorR;
    msg->load[7] = config->polarity_type;

    return ERR_OK;
}

int32_t antenna_v1_config_send(const antenna_v1_config_tx_t *config)
{
    antenna_v1_msg_t msg;

    if(!config) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_config_pack(config, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_config_decode(const antenna_v1_msg_t *msg, antenna_v1_config_rx_t *config)
{
    if((!msg) || (!config)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    config->TorR = msg->load[5];
    config->subset = msg->load[6];
    config->result = msg->load[7];

    return ERR_OK;
}

int32_t antenna_v1_config_abnormal_decode(const antenna_v1_msg_t *msg, antenna_v1_config_abnormal_rx_t *config_abnormal)
{
    if((!msg) || (!config_abnormal)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    config_abnormal->TorR = msg->load[5];
    config_abnormal->subset = msg->load[6];
    config_abnormal->state = msg->load[7];
    config_abnormal->error = msg->load[8];
    config_abnormal->hint = msg->load[9];

    return ERR_OK;
}

int32_t antenna_v1_rf_vcc_pack(const antenna_v1_rf_vcc_tx_t *rf_vcc, antenna_v1_msg_t *msg)
{
    if((!rf_vcc) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x36;

    msg->load[5] = rf_vcc->TorR;
    msg->load[6] = rf_vcc->subset;
    msg->load[7] = rf_vcc->sw_state;

    return ERR_OK;
}

int32_t antenna_v1_rf_vcc_send(const antenna_v1_rf_vcc_tx_t *rf_vcc)
{
    antenna_v1_msg_t msg;

    if(!rf_vcc) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_rf_vcc_pack(rf_vcc, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_rf_vcc_decode(const antenna_v1_msg_t *msg, antenna_v1_rf_vcc_rx_t *rf_vcc)
{
    if((!msg) || (!rf_vcc)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    rf_vcc->TorR = msg->load[5];
    rf_vcc->subset = msg->load[6];
    rf_vcc->sw_state = msg->load[7];
    rf_vcc->result = msg->load[8];

    return ERR_OK;
}

int32_t antenna_v1_temperature_pack(const antenna_v1_temperature_tx_t *temperature, antenna_v1_msg_t *msg)
{
    if((!temperature) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(msg, 0, sizeof(antenna_v1_msg_t));
    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
    msg->msg_id = 0x3C;

    msg->load[5] = temperature->TorR;
    msg->load[6] = temperature->subset;
    LITTLE16_2_BIG16(&msg->load[10], &temperature->chip_number);

    return ERR_OK;
}

int32_t antenna_v1_temperature_send(const antenna_v1_temperature_tx_t *temperature)
{
    antenna_v1_msg_t msg;

    if(!temperature) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_temperature_pack(temperature, &msg);
    return antenna_v1_msg_send(&msg);
}

int32_t antenna_v1_temperature_decode(const antenna_v1_msg_t *msg, antenna_v1_temperature_rx_t *temperature)
{
    if((!msg) || (!temperature)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    temperature->TorR = msg->load[5];
    temperature->subset = msg->load[6];
    memcpy(temperature->board_temperature, &msg->load[8], 3);
    temperature->chip_temperature = msg->load[7];

    return ERR_OK;
}

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
int32_t antenna_v1_offset_pack(const antenna_v1_offset_tx_t *offset, antenna_v1_msg_t *msg)
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

    memset(msg, 0, sizeof(antenna_v1_msg_t));

    msg->frame_lenght = ANTENNA_V1_SMALL_PACK_LENGHT;
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

int32_t antenna_v1_offset_send(const antenna_v1_offset_tx_t *offset)
{
    antenna_v1_msg_t msg;

    if(!offset) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    antenna_v1_offset_pack(offset, &msg);
    return antenna_v1_msg_send(&msg);
}
#endif

#endif /* USING_ANTENNA_V1 */
