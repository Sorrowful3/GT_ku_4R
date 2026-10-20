/*
 * antenna_v1_offset0.c
 *
 *  Created on: 2021年10月22日
 *      Author: Administrator
 */

#include "antenna_v1_offset0.h"

#define DEBUG_TAG   "antenna_v1_offset0,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V1
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET

/* 数据存储顺序
 * 线极化接收，线极化发射
 *
**/
#define MAX_THETA       60.0f

#define THETA_STEP      5.0f
#define PHI_STEP        5.0f
#define POL_STEP        0.5f

#define POL_NUMBER      1                   //每个极化点对应一个数据
#define THETA_NUMBER    (360*POL_NUMBER)    //每个离轴点对应360个极化点
#define PHI_NUMBER      (13*THETA_NUMBER)   //每个方位点对应13个离轴点
#define FREQ_NUMBER     (72*PHI_NUMBER)     //每个频率点对应72个方位点
#define TX_DATA_OFFSET  (12*FREQ_NUMBER)    //发射补偿数据起始

static const float tx_freq_point[] =
{
        13.74f, 13.82f, 13.90f, 13.98f, 14.03516f, 14.10f,
        14.16f, 14.22f, 14.28f, 14.36f, 14.42f, 14.48f,
};
static const float rx_freq_point[] =
{
        10.7883f, 10.9550f, 11.1217f, 11.2884f, 11.4551f, 11.6218f,
        11.7885f, 11.9552f, 12.1219f, 12.2886f, 12.4553f, 12.6220f,
};

static uint32_t freq_2_point(uint8_t TorR, float freq)
{
    uint32_t i;
    uint32_t point;
    uint32_t lenght;
    float abs_min;
    float tmp;
    const float *temp_p;

    if(TorR == ANTENNA_V1_TX) {
        //使用发射频点
        temp_p = tx_freq_point;
        lenght = sizeof(tx_freq_point)/sizeof(tx_freq_point[0]);;
    }
    else {
        //使用接收频点
        temp_p = rx_freq_point;
        lenght = sizeof(rx_freq_point)/sizeof(rx_freq_point[0]);;
    }

    point = 0;
    abs_min = 1000.0f;
    for(i=0; i<lenght; i++) {
        tmp = fabsf(temp_p[i] - freq);
        if(abs_min > tmp) {
            abs_min = tmp;
            point = i;
        }
        else {
            break;
        }
    }
    return point;
}

static uint32_t theta_2_point(float theta)
{
    theta += THETA_STEP / 2.0f;
    if(theta > MAX_THETA) {
        theta = MAX_THETA;
    }
    theta = fmodf(theta, 360.0f);
    return (uint32_t)(theta / THETA_STEP);
}

static uint32_t phi_2_point(float phi)
{
    phi += PHI_STEP / 2.0f;
    phi = fmodf(phi, 360.0f);
    return (uint32_t)(phi / PHI_STEP);
}

static uint32_t polarity_2_point(float pol)
{
    pol += POL_STEP / 2.0f;
    pol = fmodf(pol, 180.0f);
    return (uint32_t)(pol / POL_STEP);
}

int32_t antenna_v1_offset0_find(uint8_t TorR, const antenna_v1_point_t *point, const antenna_v1_offset_info_t *data_info, antenna_v1_offset_t *data)
{
    uint32_t freq_point;    //频率
    uint32_t theta_point;   //离轴角
    uint32_t phi_point;     //方位角
    uint32_t pol_point;     //极化角
    uint32_t offset;
    const antenna_v1_offset_t *offset_data = (antenna_v1_offset_t*)data_info->load_addr;

    if(!offset_data) {
        LOG_E("Uninitialized ");
        return ERR_INVAL;
    }

    freq_point = freq_2_point(TorR, point->freq);
    theta_point = theta_2_point(point->theta);
    phi_point = phi_2_point(point->phi);
    pol_point = polarity_2_point(point->polarity);

    offset = freq_point * FREQ_NUMBER
            + phi_point * PHI_NUMBER
            + theta_point * THETA_NUMBER
            + pol_point * POL_NUMBER;

    if(TorR == ANTENNA_V1_TX) {
        offset += TX_DATA_OFFSET; //4043520
    }


#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("%02x %s%d.%02d %s%d.%02d %s%d.%02d %s%d.%02d\r\n", TorR, \
            point->freq>=0?"":"-", (int32_t)fabsf32(point->freq), (int32_t)(fabsf32(point->freq-(int32_t)(point->freq))*100), \
            point->phi>=0?"":"-", (int32_t)fabsf32(point->phi), (int32_t)(fabsf32(point->phi-(int32_t)(point->phi))*100), \
            point->theta>=0?"":"-", (int32_t)fabsf32(point->theta), (int32_t)(fabsf32(point->theta-(int32_t)(point->theta))*100), \
            point->polarity>=0?"":"-", (int32_t)fabsf32(point->polarity), (int32_t)(fabsf32(point->polarity-(int32_t)(point->polarity))*100));
#endif
    if(offset > data_info->size/sizeof(antenna_v1_offset_t)) {
        LOG_E("over range");
        return ERR_INVAL;
    }

    *data = offset_data[offset];
#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("%s%d.%02d %s%d.%02d\r\n", data->range>=0?"":"-", (int32_t)fabsf32(data->range), (int32_t)(fabsf32(data->range-(int32_t)(data->range))*100), \
            data->phase>=0?"":"-", (int32_t)fabsf32(data->phase), (int32_t)(fabsf32(data->phase-(int32_t)(data->phase))*100));
#endif
    return ERR_OK;
}

#endif  /* ANTENNA_V1_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V1 */
