/*
 * antenna_hal.c
 *
 *  Created on: 2021年9月22日
 *      Author: Administrator
 */

#include "antenna_hal.h"

#define DEBUG_TAG "antenna_hal,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

int32_t antenna_rf_vcc_on(uint8_t TorR)
{
    //开启射频前必须先开启直流供电
    if(!antenna_power_state(TorR, ANTENNA_SUBSET_ALL)) {
        return ERR_IO;
    }

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_on(TorR);
#else
        return antenna_v1_rf_vcc_on(TorR);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_on(TorR);
#else
        return antenna_v1_rf_vcc_on(TorR);
#endif
    }
}

int32_t antenna_rf_vcc_off(uint8_t TorR)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_off(TorR);
#else
        return antenna_v1_rf_vcc_off(TorR);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_off(TorR);
#else
        return antenna_v1_rf_vcc_off(TorR);
#endif
    }
}

int32_t antenna_rf_vcc_state(uint8_t TorR)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_state(TorR);
#else
        return antenna_v1_rf_vcc_state(TorR);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_rf_vcc_state(TorR);
#else
        return antenna_v1_rf_vcc_state(TorR);
#endif
    }
}

int32_t antenna_set_freq(uint8_t TorR, float freq)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_freq(TorR, freq);
#else
        return antenna_v1_set_freq(TorR, freq);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_freq(TorR, freq);
#else
        return antenna_v1_set_freq(TorR, freq);
#endif
    }
}

int32_t antenna_get_freq(uint8_t TorR, float *freq)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_freq(TorR, freq);
#else
        return antenna_v1_get_freq(TorR, freq);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_freq(TorR, freq);
#else
        return antenna_v1_get_freq(TorR, freq);
#endif
    }
}

int32_t antenna_set_polarity(uint8_t TorR, uint8_t polarity_type)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_polarity(TorR, polarity_type);
#else
        return antenna_v1_set_polarity(TorR, polarity_type);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_polarity(TorR, polarity_type);
#else
        return antenna_v1_set_polarity(TorR, polarity_type);
#endif
    }
}

int32_t antenna_get_polarity(uint8_t TorR, uint8_t *polarity_type)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_polarity(TorR, polarity_type);
#else
        return antenna_v1_get_polarity(TorR, polarity_type);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_polarity(TorR, polarity_type);
#else
        return antenna_v1_get_polarity(TorR, polarity_type);
#endif
    }
}

int32_t antenna_set_point(uint8_t TorR, float polarity, float phi, float theta)
{
//	 LOG_W("--body_point:pol=%.3f",polarity);
    //设置波束前需要开射频
    if(!antenna_rf_vcc_state(TorR)) {
        return ERR_IO;
    }

    //phi不能为360°
    if(phi < 0.25f) {
        phi = 0.2f;
    }
    if(phi > 359.85f) {
        phi = 359.85f;
    }
    //polarity不能为180°
    if(polarity < 0.25f) {
        polarity = 0.2f;
    }
    if(polarity > 179.85f) {
        polarity = 179.85f;
    }
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_point(TorR, polarity, phi, theta);
#else
        return antenna_v1_set_point(TorR, polarity, phi, theta);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_set_point(TorR, polarity, phi, theta);
#else
        return antenna_v1_set_point(TorR, polarity, phi, theta);
#endif
    }
}

int32_t antenna_get_point(uint8_t TorR, float *polarity, float *phi, float *theta)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_point(TorR, polarity, phi, theta);
#else
        return antenna_v1_get_point(TorR, polarity, phi, theta);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_get_point(TorR, polarity, phi, theta);
#else
        return antenna_v1_get_point(TorR, polarity, phi, theta);
#endif
    }
}

int32_t antenna_update_beam(uint8_t TorR, const antenna_beam_t *beam)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_beam_t beam_v0;

        beam_v0.polarity_type = beam->polarity_type;
        beam_v0.freq = beam->freq;
        beam_v0.weight = beam->weight;
        beam_v0.phi = beam->phi;
        beam_v0.polarity = beam->polarity;
        beam_v0.theta = beam->theta;
        return antenna_v0_update_beam(TorR, &beam_v0);
#else
        antenna_v1_beam_t beam_v1;

        beam_v1.polarity_type = beam->polarity_type;
        beam_v1.freq = beam->freq;
        beam_v1.nbr = beam->nbr;
        beam_v1.phi = beam->phi;
        beam_v1.polarity = beam->polarity;
        beam_v1.theta = beam->theta;
        beam_v1.weight = beam->weight;
        return antenna_v1_update_beam(TorR, &beam_v1);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_beam_t beam_v0;

        beam_v0.polarity_type = beam->polarity_type;
        beam_v0.freq = beam->freq;
        beam_v0.weight = beam->weight;
        beam_v0.phi = beam->phi;
        beam_v0.polarity = beam->polarity;
        beam_v0.theta = beam->theta;
        return antenna_v0_update_beam(TorR, &beam_v0);
#else
        antenna_v1_beam_t beam_v1;

        beam_v1.polarity_type = beam->polarity_type;
        beam_v1.freq = beam->freq;
        beam_v1.nbr = beam->nbr;
        beam_v1.phi = beam->phi;
        beam_v1.polarity = beam->polarity;
        beam_v1.theta = beam->theta;
        beam_v1.weight = beam->weight;
        return antenna_v1_update_beam(TorR, &beam_v1);
#endif
    }
}

int32_t antenna_power_on(uint8_t TorR, uint8_t subset)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_on(TorR, subset);
#else
        return antenna_v1_power_on(TorR, subset);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_on(TorR, subset);
#else
        return antenna_v1_power_on(TorR, subset);
#endif
    }
}

int32_t antenna_power_off(uint8_t TorR, uint8_t subset)
{
    //关闭直流供电强必须先关闭射频供电
    if(antenna_rf_vcc_state(TorR)) {
        return ERR_IO;
    }

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_off(TorR, subset);
#else
        return antenna_v1_power_off(TorR, subset);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_off(TorR, subset);
#else
        return antenna_v1_power_off(TorR, subset);
#endif
    }
}

int32_t antenna_power_state(uint8_t TorR, uint8_t subset)
{
    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_state(TorR, subset);
#else
        return antenna_v1_power_state(TorR, subset);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_power_state(TorR, subset);
#else
        return antenna_v1_power_state(TorR, subset);
#endif
    }
}

int32_t antenna_read_power(uint8_t TorR, uint8_t subset, antenna_power_t *power)
{
    int32_t result;

    //开启直流后才能读取功率
    if(!antenna_power_state(TorR, subset)) {
        return ERR_IO;
    }

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_power_t power_v0;

        result = antenna_v0_read_power(TorR, subset, &power_v0);
        if(result != ERR_OK) {
            return result;
        }
        power->total_electricity = power_v0.total_electricity;
        power->total_voltage = power_v0.total_voltage;
        power->total_power = power_v0.total_power;
        return ERR_OK;
#else
        antenna_v1_power_t power_v1;

        result = antenna_v1_read_power(TorR, subset, &power_v1);
        if(result != ERR_OK) {
            return result;
        }
        power->total_electricity = power_v1.total_electricity;
        power->total_voltage = power_v1.total_voltage;
        power->total_power = power_v1.total_power;
        return ERR_OK;
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_power_t power_v0;

        result = antenna_v0_read_power(TorR, subset, &power_v0);
        if(result != ERR_OK) {
            return result;
        }
        power->total_electricity = power_v0.total_electricity;
        power->total_voltage = power_v0.total_voltage;
        power->total_power = power_v0.total_power;
        return ERR_OK;
#else
        antenna_v1_power_t power_v1;

        result = antenna_v1_read_power(TorR, subset, &power_v1);
        if(result != ERR_OK) {
            return result;
        }
        power->total_electricity = power_v1.total_electricity;
        power->total_voltage = power_v1.total_voltage;
        power->total_power = power_v1.total_power;
        return ERR_OK;
#endif
    }
}

int32_t antenna_read_temperature(uint8_t TorR, uint8_t subset, antenna_temperature_t *temperature)
{
    int32_t result;

    //开启直流后才能读温度
    if(!antenna_power_state(TorR, subset)) {
        return ERR_IO;
    }

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_temperature_t temperature_v0;

        result = antenna_v0_read_temperature(TorR, subset, NULL, &temperature_v0);
        if(result != ERR_OK) {
            return result;
        }
        temperature->board_temperature[0] = temperature_v0.board_temperature[0];
        temperature->board_temperature[1] = temperature_v0.board_temperature[1];
        temperature->board_temperature[2] = temperature_v0.board_temperature[2];
        return ERR_OK;
#else
        antenna_v1_temperature_t temperature_v1;

        result = antenna_v1_read_temperature(TorR, subset, 0x00, &temperature_v1);
        if(result != ERR_OK) {
            return result;
        }
        temperature->board_temperature[0] = temperature_v1.board_temperature[0];
        temperature->board_temperature[1] = temperature_v1.board_temperature[1];
        temperature->board_temperature[2] = temperature_v1.board_temperature[2];
        return ERR_OK;
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        antenna_v0_temperature_t temperature_v0;

        result = antenna_v0_read_temperature(TorR, subset, NULL, &temperature_v0);
        if(result != ERR_OK) {
            return result;
        }
        temperature->board_temperature[0] = temperature_v0.board_temperature[0];
        temperature->board_temperature[1] = temperature_v0.board_temperature[1];
        temperature->board_temperature[2] = temperature_v0.board_temperature[2];
        return ERR_OK;
#else
        antenna_v1_temperature_t temperature_v1;

        result = antenna_v1_read_temperature(TorR, subset, 0x00, &temperature_v1);
        if(result != ERR_OK) {
            return result;
        }
        temperature->board_temperature[0] = temperature_v1.board_temperature[0];
        temperature->board_temperature[1] = temperature_v1.board_temperature[1];
        temperature->board_temperature[2] = temperature_v1.board_temperature[2];
        return ERR_OK;
#endif
    }
}

int32_t antenna_selfcheck(uint8_t TorR, uint8_t subset)
{
    //直流供电开启后才能自检
    if(!antenna_power_state(TorR, subset)) {
        return ERR_IO;
    }

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_selfcheck(TorR, subset);
#else
        return antenna_v1_selfcheck(TorR, subset);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_selfcheck(TorR, subset);
#else
        return antenna_v1_selfcheck(TorR, subset);
#endif
    }
}

int32_t antenna_read_version(uint8_t TorR, uint8_t subset, uint32_t *version)
{
    //直流开启后才能读版本
    if(!antenna_power_state(TorR, subset)) {
        return ERR_IO;
    }

#ifdef USING_ANTENNA_V0
    antenna_v0_version_t v0_version;
#endif
#ifdef USING_ANTENNA_V1
    antenna_v1_version_t v1_version;
#endif

    if(TorR == ANTENNA_RX) {
#if ANTENNA_RX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_read_versions(TorR, subset, &v0_version);
#else
        return antenna_v1_read_versions(TorR, subset, &v1_version);
#endif
    }
    else {
#if ANTENNA_TX_VERSION == ANTENNA_VERSION_V0
        return antenna_v0_read_versions(TorR, subset, &v0_version);
#else
        return antenna_v1_read_versions(TorR, subset, &v1_version);
#endif
    }
}

int32_t antenna_init(void)
{
    int32_t result;

#ifdef USING_ANTENNA_V0
    result = antenna_v0_init();
    if(result != ERR_OK) {
        return result;
    }
#endif
#ifdef USING_ANTENNA_V1
    result = antenna_v1_init();
    if(result != ERR_OK) {
        return result;
    }
#endif
    return ERR_OK;
}
