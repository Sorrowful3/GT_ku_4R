/*
 * antenna_V0_hal.c
 *
 *  Created on: 2021年4月19日
 *      Author: Administrator
 */

#include "antenna_V0_hal.h"

#define DEBUG_TAG "antenna_V0_hal,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V0


//垂直极化
#define VERTICAL_RF_VCC    0X00
//水平极化
#define HORIZONTAL_RF_VCC  0X01
//双极化
#define DOUBLE_RF_VCC      0XFF
//射频供电关
#define RF_VCC_OFF      0x00
//射频供电开
#define RF_VCC_ON       0xff

static antenna_v0_beam_t rx_beam = {
    .polarity_type = ANTENNA_V0_LINEAR_POLARITY_VERTICAL,
    .polarity = 0.0f,
    .theta = 0.0f,
    .phi = 0.0f,
    .weight = 0,
    .polarity_mode = ANTENNA_V0_COMPLETE_POLARZA_MODE,
    .freq = 12.457f,
};

static antenna_v0_beam_t tx_beam = {
    .polarity_type = ANTENNA_V0_LINEAR_POLARITY_HORIZONTALL,
    .polarity = 0.0f,
    .theta = 0.0f,
    .phi = 0.0f,
    .weight = 0,
    .polarity_mode = ANTENNA_V0_COMPLETE_POLARZA_MODE,
    .freq = 14.2216f,
};

static antenna_v0_rf_vcc_t rx_rf_vcc = {
    .state = RF_VCC_OFF,
    .polarity = DOUBLE_RF_VCC,
};

static antenna_v0_rf_vcc_t tx_rf_vcc = {
    .state = RF_VCC_OFF,
    .polarity = DOUBLE_RF_VCC,
};

static int32_t antenna_v0_rf_vcc_switch(uint8_t TorR, uint8_t polarity)
{
    uint8_t old_polarity;
    int32_t result;
    antenna_v0_rf_vcc_t *rf_vcc;

    if(TorR == ANTENNA_V0_RX) {
        rf_vcc = &rx_rf_vcc;
    }
    else {
        rf_vcc = &tx_rf_vcc;
    }

    //电源极化不用修改
    if(rf_vcc->polarity == polarity) {
        return ERR_OK;
    }

    if(rf_vcc->state == RF_VCC_OFF) {
        //修改电源极化
        rf_vcc->polarity = polarity;
    }
    else {
        old_polarity = rf_vcc->polarity;
        //先关闭射频电源
        rf_vcc->polarity = DOUBLE_RF_VCC;
        rf_vcc->state = RF_VCC_OFF;
        result = antenna_v0_ctrl_rf_vcc(TorR, rf_vcc);
        if(result != ERR_OK) {
            rf_vcc->polarity = old_polarity;
            rf_vcc->state = RF_VCC_ON;
            return result;
        }
        //修改电源极化并开启电源
        rf_vcc->polarity = polarity;
        rf_vcc->state = RF_VCC_ON;
        result = antenna_v0_ctrl_rf_vcc(TorR, rf_vcc);
        if(result != ERR_OK) {
            rf_vcc->polarity = old_polarity;
            rf_vcc->state = RF_VCC_OFF;
            return result;
        }
    }
    return ERR_OK;
}

int32_t antenna_v0_rf_vcc_on(uint8_t TorR)
{
    int32_t result;
    antenna_v0_rf_vcc_t *rf_vcc;

    if(TorR == ANTENNA_V0_RX) {
        rf_vcc = &rx_rf_vcc;
    }
    else {
        rf_vcc = &tx_rf_vcc;
    }

    if(rf_vcc->state != RF_VCC_ON) {
        rf_vcc->state = RF_VCC_ON;
        result = antenna_v0_ctrl_rf_vcc(TorR, rf_vcc);
        if(result != ERR_OK) {
            rf_vcc->state = RF_VCC_OFF;
            return result;
        }
    }

    return ERR_OK;
}

int32_t antenna_v0_rf_vcc_off(uint8_t TorR)
{
    int32_t result;
    antenna_v0_rf_vcc_t *rf_vcc;

    if(TorR == ANTENNA_V0_RX) {
        rf_vcc = &rx_rf_vcc;
    }
    else {
        rf_vcc = &tx_rf_vcc;
    }

    if(rf_vcc->state != RF_VCC_OFF) {
        rf_vcc->state = RF_VCC_OFF;
        result = antenna_v0_ctrl_rf_vcc(TorR, rf_vcc);
        if(result != ERR_OK) {
            rf_vcc->state = RF_VCC_ON;
            return result;
        }
    }

    return ERR_OK;
}

int32_t antenna_v0_rf_vcc_state(uint8_t TorR)
{
    antenna_v0_rf_vcc_t *rf_vcc;

    if(TorR == ANTENNA_V0_RX) {
        rf_vcc = &rx_rf_vcc;
    }
    else {
        rf_vcc = &tx_rf_vcc;
    }

    if(rf_vcc->state == RF_VCC_ON) {
        return 1;
    }

    return 0;
}

int32_t antenna_v0_set_freq(uint8_t TorR, float freq)
{
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    beam->freq = freq;

    return ERR_OK;
}

int32_t antenna_v0_get_freq(uint8_t TorR, float *freq)
{
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    *freq = beam->freq;

    return ERR_OK;
}

int32_t antenna_v0_set_polarity(uint8_t TorR, uint8_t polarity)
{
    uint8_t vcc_polarity;
    uint8_t beam_polarity;
    int32_t result;
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    if(beam->polarity_type == polarity) {
        return ERR_OK;
    }

    switch(polarity) {
    case ANTENNA_V0_LINEAR_POLARITY_HORIZONTALL:
        //全极化，水平参考
        beam_polarity = ANTENNA_V0_COMPLETE_POLARZA_MODE;
        //要极化合成，水平和垂直电源均需要开启
        vcc_polarity = DOUBLE_RF_VCC;
        break;
    case ANTENNA_V0_LINEAR_POLARITY_VERTICAL:
        //全极化，垂直参考
        beam_polarity = ANTENNA_V0_COMPLETE_POLARZA_MODE;
        //要极化合成，水平和垂直电源均需要开启
        vcc_polarity = DOUBLE_RF_VCC;
        break;
    case ANTENNA_V0_LINEAR_POLARITY_HORIZONTAL_OLAY:
        //全极化，但仅使用水平方向
        beam_polarity = ANTENNA_V0_COMPLETE_POLARZA_MODE;
        //不需要极化合成，仅开水平通道
        vcc_polarity = HORIZONTAL_RF_VCC;
        break;
    case ANTENNA_V0_LINEAR_POLARITY_VERTICAL_OLAY:
        //全极化，但仅使用垂直方向
        beam_polarity = ANTENNA_V0_COMPLETE_POLARZA_MODE;
        //不需要极化合成，仅开垂直通道
        vcc_polarity = VERTICAL_RF_VCC;
        break;
    case ANTENNA_V0_LEFT_HAND_CIRCULAR_POLARITY:
        //左旋极化
        beam_polarity = ANTENNA_V0_LEFT_POLARZA_MODE;
        //要极化合成，水平和垂直电源均需要开启
        vcc_polarity = DOUBLE_RF_VCC;
        break;
    case ANTENNA_V0_RIGHT_HAND_CIRCULAR_POLARITY:
        //右旋极化
        beam_polarity = ANTENNA_V0_RIGHT_POLARZA_MODE;
        //要极化合成，水平和垂直电源均需要开启
        vcc_polarity = DOUBLE_RF_VCC;
        break;
    default:
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //切换电源极化方式
    result = antenna_v0_rf_vcc_switch(TorR, vcc_polarity);
    if(result != ERR_OK) {
        return result;
    }
    //修改波束极化方式
    beam->polarity_mode = beam_polarity;
    //修改极化类型
    beam->polarity_type = polarity;

    return ERR_OK;
}

int32_t antenna_v0_get_polarity(uint8_t TorR, uint8_t *polarity_type)
{
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    *polarity_type = beam->polarity_type;
    return ERR_OK;
}

int32_t antenna_v0_set_point(uint8_t TorR, float polarity, float phi, float theta)
{
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    switch(beam->polarity_type) {
    case ANTENNA_V0_LINEAR_POLARITY_HORIZONTALL:
        beam->polarity = polarity;
        break;
    case ANTENNA_V0_LINEAR_POLARITY_VERTICAL:
        beam->polarity = fmodf(polarity+90.0f, 180.0f);
        break;
    case ANTENNA_V0_LINEAR_POLARITY_VERTICAL_OLAY:
        beam->polarity = 90.0f;
        break;
    case ANTENNA_V0_LINEAR_POLARITY_HORIZONTAL_OLAY:
    case ANTENNA_V0_LEFT_HAND_CIRCULAR_POLARITY:
    case ANTENNA_V0_RIGHT_HAND_CIRCULAR_POLARITY:
        beam->polarity = 0.0f;
        break;
    default:
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    beam->phi = phi;
    beam->theta = theta;

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
    //发送极化补偿数据
    antenna_v0_offset(TorR, beam);
#endif
    //设置指向
    return antenna_v0_set_beam(TorR, beam, 0);
}

int32_t antenna_v0_get_point(uint8_t TorR, float *polarity, float *phi, float *theta)
{
    antenna_v0_beam_t *beam;

    if(TorR == ANTENNA_V0_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    *polarity = beam->polarity;
    *phi = beam->phi;
    *theta = beam->theta;

    return ERR_OK;
}

int32_t antenna_v0_update_beam(uint8_t TorR, const antenna_v0_beam_t *beam)
{
    int32_t result;
    antenna_v0_beam_t *temp;

    //配置极化
    result = antenna_v0_set_polarity(TorR, beam->polarity_type);
    if(result != ERR_OK) {
        return result;
    }
    //设置频率
    result = antenna_v0_set_freq(TorR, beam->freq);
    if(result != ERR_OK) {
        return result;
    }
    //开启电源
    result = antenna_v0_rf_vcc_on(TorR);
    if(result != ERR_OK) {
        return result;
    }

    //天线上电才配置指向
    if(antenna_v0_rf_vcc_state(TorR)) {
        if(TorR == ANTENNA_V0_RX) {
            temp = &rx_beam;
        }
        else {
            temp = &tx_beam;
        }
        temp->polarity = beam->polarity;
        temp->phi = beam->phi;
        temp->theta = beam->theta;
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
        //发送极化补偿数据
        antenna_v0_offset(TorR, temp);
#endif
        //设置指向
        result = antenna_v0_set_beam(TorR, temp, 0);
        if(result != ERR_OK) {
            return result;
        }
    }

    return ERR_OK;
}

int32_t antenna_v0_selfcheck(uint8_t TorR, uint8_t subset)
{
    antenna_v0_version_t versions;

    //通过读版本检查是否正常
    return antenna_v0_read_versions(TorR, subset, &versions);
}

int32_t antenna_v0_init(void)
{
    uint8_t i;
    int32_t result;

#ifdef ANTENNA_V0_USING_POLARITY_OFFSET
    //初始化补偿数据
    result = antenna_v0_offset_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

    //初始化底层接口
    result = antenna_v0_port_init();
    if(result != ERR_OK) {
        return result;
    }

    //初始化控制协议
    result = antenna_v0_ctrl_init();
    if(result != ERR_OK) {
        return result;
    }

    //开启天线直流电源
    for(i = 0; (i<ANTENNA_V0_RX_TOTAL)||(i<ANTENNA_V0_TX_TOTAL); i++) {
        if(i < ANTENNA_V0_RX_TOTAL) {
            result = antenna_v0_power_on_nodelay(ANTENNA_V0_RX, i);
            if(result != ERR_OK) {
                return result;
            }
        }
        if(i < ANTENNA_V0_TX_TOTAL) {
            result = antenna_v0_power_on_nodelay(ANTENNA_V0_TX, i);
            if(result != ERR_OK) {
                return result;
            }
        }
        vTaskDelay(20);
    }
    //等待天线上电结束
    vTaskDelay(50);

    //波控复位
    antenna_v0_reboot();
    //等待复位结束
    vTaskDelay(50);

    return ERR_OK;
}

#endif /* USING_ANTENNA_V0 */
