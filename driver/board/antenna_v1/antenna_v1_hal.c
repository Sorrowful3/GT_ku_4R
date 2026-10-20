/*
 * antenna_v1_hal.c
 *
 *  Created on: 2021年9月16日
 *      Author: Administrator
 */

#include "antenna_v1_hal.h"

#ifdef USING_ANTENNA_V1

#define DEBUG_TAG "antenna_v1_hal,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../../core/debug.h"

static antenna_v1_beam_t rx_beam = {
    .polarity_type = ANTENNA_V1_LINEAR_POLARITY_VERTICAL,
    .polarity = 0.0f,
    .theta = 0.0f,
    .phi = 0.0f,
    .weight = 0,
    .freq = 12.457f,
    .nbr = 0,
    .polarity_mode = ANTENNA_V1_LINEAR_POLARITY_MODE,
};

static antenna_v1_beam_t tx_beam = {
    .polarity_type = ANTENNA_V1_LINEAR_POLARITY_HORIZONTALL,
    .polarity = 0.0f,
    .theta = 0.0f,
    .phi = 0.0f,
    .weight = 0,
    .freq = 14.457f,
    .nbr = 0,
    .polarity_mode = ANTENNA_V1_LINEAR_POLARITY_MODE,
};

//射频电源状态
static uint8_t rx_rf_vcc_state = ANTENNA_V1_OFF;
static uint8_t tx_rf_vcc_state = ANTENNA_V1_OFF;

static int32_t antenna_v1_polarity_switch(uint8_t TorR, uint8_t polarity)
{
    uint8_t old_polarity;
    int32_t result;
    uint8_t *polarity_mode;
    uint8_t *rf_vcc_state;

    if(TorR == ANTENNA_V1_RX) {
        polarity_mode = &rx_beam.polarity_mode;
        rf_vcc_state = &rx_rf_vcc_state;
    }
    else {
        polarity_mode = &tx_beam.polarity_mode;
        rf_vcc_state = &tx_rf_vcc_state;
    }
    //不需要切换极化
    if(*polarity_mode == polarity) {
        return ERR_OK;
    }
    if(*rf_vcc_state == ANTENNA_V1_OFF) {
        //修改极化
        *polarity_mode = polarity;
        return ERR_OK;
    }
    else {
        //缓存之前的极化
        old_polarity = *polarity_mode;
        //修改极化
        *polarity_mode = polarity;
        //重启射频电源
        result = antenna_v1_rf_vcc_off(TorR);
        if(result != ERR_OK) {
            *polarity_mode = old_polarity;
            return result;
        }
        result = antenna_v1_rf_vcc_on(TorR);
        if(result != ERR_OK) {
            *polarity_mode = old_polarity;
            return result;
        }
    }

    return ERR_OK;
}

int32_t antenna_v1_rf_vcc_on(uint8_t TorR)
{
    int32_t result;
    uint8_t *polarity_mode;
    uint8_t *rf_vcc_state;
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
        polarity_mode = &rx_beam.polarity_mode;
        rf_vcc_state = &rx_rf_vcc_state;
    }
    else {
        beam = &tx_beam;
        polarity_mode = &tx_beam.polarity_mode;
        rf_vcc_state = &tx_rf_vcc_state;
    }

    //电源状态不需要改变
    if(*rf_vcc_state == ANTENNA_V1_ON) {
        return ERR_OK;
    }
    //配置极化
    result = antenna_v1_config(TorR, *polarity_mode);
    if(result != ERR_OK) {
        return result;
    }
    //更新波束
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
    //发送极化补偿数据
    antenna_v1_offset(TorR, beam);
#endif
    result = antenna_v1_set_beam(TorR, beam, 1);
    if(result != ERR_OK) {
        return result;
    }
    //射频上电
    result = antenna_v1_ctrl_rf_vcc(TorR, ANTENNA_V1_SUBSET_ALL, ANTENNA_V1_ON);
    if(result != ERR_OK) {
        return result;
    }
    //改变电源状态
    *rf_vcc_state = ANTENNA_V1_ON;

    return ERR_OK;
}

int32_t antenna_v1_rf_vcc_off(uint8_t TorR)
{
    uint8_t old_state;
    int32_t result;
    uint8_t *rf_vcc_state;

    if(TorR == ANTENNA_V1_RX) {
        rf_vcc_state = &rx_rf_vcc_state;
    }
    else {
        rf_vcc_state = &tx_rf_vcc_state;
    }
    //电源状态不需要改变
    if(*rf_vcc_state == ANTENNA_V1_OFF) {
        return ERR_OK;
    }

    old_state = *rf_vcc_state;
    //改变电源状态
    *rf_vcc_state = ANTENNA_V1_OFF;
    __asm__ __volatile__("": : :"memory");
    //射频下电
    result = antenna_v1_ctrl_rf_vcc(TorR, ANTENNA_V1_SUBSET_ALL, ANTENNA_V1_OFF);
    if(result != ERR_OK) {
        *rf_vcc_state = old_state;
        return result;
    }

    return ERR_OK;
}

int32_t antenna_v1_rf_vcc_state(uint8_t TorR)
{
    uint8_t *rf_vcc_state;

    if(TorR == ANTENNA_V1_RX) {
        rf_vcc_state = &rx_rf_vcc_state;
    }
    else {
        rf_vcc_state = &tx_rf_vcc_state;
    }

    if(*rf_vcc_state == ANTENNA_V1_ON) {
        return 1;
    }

    return 0;
}

int32_t antenna_v1_set_freq(uint8_t TorR, float freq)
{
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }
    beam->freq = freq;

    return ERR_OK;
}

int32_t antenna_v1_get_freq(uint8_t TorR, float *freq)
{
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }
    *freq = beam->freq;

    return ERR_OK;
}

int32_t antenna_v1_set_polarity(uint8_t TorR, uint8_t polarity_type)
{
    uint8_t polarity_mode;
    int32_t result;
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    if(beam->polarity_type == polarity_type) {
        return ERR_OK;
    }

    switch(polarity_type) {
    case ANTENNA_V1_LINEAR_POLARITY_HORIZONTALL:
        //线极化水平参考
        polarity_mode = ANTENNA_V1_LINEAR_POLARITY_MODE;
        break;
    case ANTENNA_V1_LINEAR_POLARITY_VERTICAL:
        //线极化垂直参考
        polarity_mode = ANTENNA_V1_LINEAR_POLARITY_MODE;
        break;
    case ANTENNA_V1_LINEAR_POLARITY_HORIZONTAL_OLAY:
        //线极化仅开水平通道
        polarity_mode = ANTENNA_V1_HORIZONTALL_POLARITY_MODE;
        break;
    case ANTENNA_V1_LINEAR_POLARITY_VERTICAL_OLAY:
        //线极化仅开垂直通道
        polarity_mode = ANTENNA_V1_VERTICAL_POLARITY_MODE;
        break;
    case ANTENNA_V1_LEFT_HAND_CIRCULAR_POLARITY:
        //左旋
        polarity_mode = ANTENNA_V1_LEFT_POLARITY_MODE;
        break;
    case ANTENNA_V1_RIGHT_HAND_CIRCULAR_POLARITY:
        //右旋
        polarity_mode = ANTENNA_V1_RIGHT_POLARITY_MODE;
        break;
    default:
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    //修改极化方式
    result = antenna_v1_polarity_switch(TorR, polarity_mode);
    if(result != ERR_OK) {
        return result;
    }
    //修改极化类型
    beam->polarity_type = polarity_type;

    return ERR_OK;
}

int32_t antenna_v1_get_polarity(uint8_t TorR, uint8_t *polarity_type)
{
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }
    *polarity_type = beam->polarity_type;

    return ERR_OK;
}

int32_t antenna_v1_set_point(uint8_t TorR, float polarity, float phi, float theta)
{
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
        beam = &rx_beam;
    }
    else {
        beam = &tx_beam;
    }

    switch(beam->polarity_type) {
    case ANTENNA_V1_LINEAR_POLARITY_HORIZONTALL:
        beam->polarity = polarity;
        break;
    case ANTENNA_V1_LINEAR_POLARITY_VERTICAL:
        beam->polarity = fmodf(polarity+90.0f, 180.0f);
        break;
    case ANTENNA_V1_LINEAR_POLARITY_VERTICAL_OLAY:
        beam->polarity = 90.0f;
        break;
    case ANTENNA_V1_LINEAR_POLARITY_HORIZONTAL_OLAY:
    case ANTENNA_V1_LEFT_HAND_CIRCULAR_POLARITY:
    case ANTENNA_V1_RIGHT_HAND_CIRCULAR_POLARITY:
        beam->polarity = 0.0f;
        break;
    default:
        LOG_E("parameter error");
        return ERR_INVAL;
    }
//    //动中侦项目方位角294.5进行波束补偿  1度补偿
//    float a=0;
//    if((phi>=290.8)&&(phi<=298))
//    {
//    	float b=abs(phi-294.1235);
//    	if(b<1.6)
//    	{
//    	a=1.6*((2-b)/1.5);
//    	}
//    	else
//    	{
//        a=1*(1/b);
//    	}
//    	phi=phi-a;
//    }
    beam->phi = phi;
    beam->theta = theta;

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
    //发送极化补偿数据
    antenna_v1_offset(TorR, beam);
#endif
    //设置指向
    return antenna_v1_set_beam(TorR, beam, 0);
}

int32_t antenna_v1_get_point(uint8_t TorR, float *polarity, float *phi, float *theta)
{
    antenna_v1_beam_t *beam;

    if(TorR == ANTENNA_V1_RX) {
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

int32_t antenna_v1_update_beam(uint8_t TorR, const antenna_v1_beam_t *beam)
{
    int32_t result;
    antenna_v1_beam_t *temp;

    //配置极化
    result = antenna_v1_set_polarity(TorR, beam->polarity_type);
    if(result != ERR_OK) {
        return result;
    }
    //设置频率
    result = antenna_v1_set_freq(TorR, beam->freq);
    if(result != ERR_OK) {
        return result;
    }
    //开启电源
    result = antenna_v1_rf_vcc_on(TorR);
    if(result != ERR_OK) {
        return result;
    }

    //天线上电才配置指向
    if(antenna_v1_rf_vcc_state(TorR)) {
        if(TorR == ANTENNA_V1_RX) {
            temp = &rx_beam;
        }
        else {
            temp = &tx_beam;
        }
        temp->polarity = beam->polarity;
        temp->phi = beam->phi;
        temp->theta = beam->theta;
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
        //发送极化补偿数据
        antenna_v1_offset(TorR, temp);
#endif
        result = antenna_v1_set_beam(TorR, temp, 0);
        if(result != ERR_OK) {
            return result;
        }
    }

    return ERR_OK;
}

int32_t antenna_v1_selfcheck(uint8_t TorR, uint8_t subset)
{
    antenna_v1_version_t versions;

    //通过读版本检查是否正常
    return antenna_v1_read_versions(TorR, subset, &versions);
}

int32_t antenna_v1_init(void)
{
    uint8_t i;
    int32_t result;

#ifdef ANTENNA_V1_USING_POLARITY_OFFSET
    //初始化补偿数据
    result = antenna_v1_offset_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

    //初始化底层接口
    result = antenna_v1_port_init();
    if(result != ERR_OK) {
        return result;
    }

    //初始化控制协议
    result = antenna_v1_ctrl_init();
    if(result != ERR_OK) {
        return result;
    }

    //开启天线直流电源
    for(i = 0; (i<ANTENNA_V1_RX_TOTAL)||(i<ANTENNA_V1_TX_TOTAL); i++) {
        if(i < ANTENNA_V1_RX_TOTAL) {
            result = antenna_v1_power_on_nodelay(ANTENNA_V1_RX, i);
            if(result != ERR_OK) {
                return result;
            }
        }
        if(i < ANTENNA_V1_TX_TOTAL) {
            result = antenna_v1_power_on_nodelay(ANTENNA_V1_TX, i);
            if(result != ERR_OK) {
                return result;
            }
        }
        vTaskDelay(50);
    }
    //等待上电结束
    vTaskDelay(200);
#ifdef DEBUG
    //antenna_v1_rf_vcc_on(0x00);
    //波控复位
    antenna_v1_reset();
    //配置水平极化模式
    antenna_v1_config(0x00,0x03);
    //波束更新
    antenna_v1_update_beam(0x00,&rx_beam);
    //射频上电
    antenna_v1_rf_vcc_on(0x00);
    //射频下电
    antenna_v1_rf_vcc_off(0x00);
#endif
    //等待复位结束
    vTaskDelay(500);

    return ERR_OK;
}


#endif /* USING_ANTENNA_V1 */
