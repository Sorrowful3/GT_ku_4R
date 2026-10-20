/*
 * antenna_v0_offset1.c
 *
 *  Created on: 2021年10月22日
 *      Author: Administrator
 */

#include "antenna_v0_offset1.h"

#define DEBUG_TAG   "antenna_v0_offset1,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V0
#ifdef ANTENNA_V0_USING_POLARITY_OFFSET

/* 数据存储顺序
 * 线极化接收，左旋接收，右旋接收，线极化发射，左旋发射，右旋发射
 *
**/

#define MAX_THETA		60.0f

#define THETA_STEP		5.0f
#define PHI_STEP		5.0f
#define POL_STEP		0.5f

#define LINEAR_POLARZA_POL_NUMBER           1                                  //每个极化点对应一个数据
#define LINEAR_POLARZA_THETA_NUMBER         (360*LINEAR_POLARZA_POL_NUMBER)    //每个离轴点对应360个极化点
#define LINEAR_POLARZA_PHI_NUMBER           (13*LINEAR_POLARZA_THETA_NUMBER)   //每个方位点对应13个离轴点
#define LINEAR_POLARZA_FREQ_NUMBER          (72*LINEAR_POLARZA_PHI_NUMBER)     //每个频率点对应72个方位点

#define CIRCULAR_POLARZA_THETA_NUMBER       1                                    //每个离轴点对应一个数据
#define CIRCULAR_POLARZA_PHI_NUMBER         (13*CIRCULAR_POLARZA_THETA_NUMBER)   //每个方位点对应13个离轴点
#define CIRCULAR_POLARZA_FREQ_NUMBER        (72*CIRCULAR_POLARZA_PHI_NUMBER)     //每个频率点对应72个方位点

#define LINEAR_POLARZA_DATA_OFFSET          0                                                                   //线极化偏移
#define LEFT_HAND_CIRCULAR_DATA_OFFSET      (LINEAR_POLARZA_DATA_OFFSET+12*LINEAR_POLARZA_FREQ_NUMBER)          //左旋偏移，线极化偏移+线极化个数
#define RIGHT_HAND_CIRCULAR_DATA_OFFSET     (LEFT_HAND_CIRCULAR_DATA_OFFSET+12*CIRCULAR_POLARZA_FREQ_NUMBER)    //右旋偏移，左旋偏移+左旋个数
#define TX_DATA_OFFSET                      (RIGHT_HAND_CIRCULAR_DATA_OFFSET+12*CIRCULAR_POLARZA_FREQ_NUMBER)   //发射数据偏移，右旋偏移+右旋个数

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

    if(TorR == 0xff) {
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

static uint32_t pol_2_point(float pol)
{
	pol += POL_STEP / 2.0f;
	pol = fmodf(pol, 180.0f);
	return (uint32_t)(pol / POL_STEP);
}

int32_t antenna_v0_offset1_find(uint8_t TorR, const antenna_v0_point_t *point, const antenna_v0_offset_info_t *data_info, antenna_v0_offset_t *data)
{
	uint32_t freq_point;	//频率
	uint32_t theta_point;	//离轴角
	uint32_t phi_point;		//方位角
	uint32_t pol_point;		//极化角
	uint32_t offset;
    const antenna_v0_offset_t *offset_data = (antenna_v0_offset_t*)data_info->load_addr;

	if(!offset_data) {
		LOG_E("Uninitialized");
        return ERR_INVAL;
	}

	//复位参数
	data->phase = 0.0f;
	data->range = 0.0f;

    if(point->polarity_type == ANTENNA_V0_COMPLETE_POLARZA_MODE) {
        freq_point = freq_2_point(TorR, point->freq);
        theta_point = theta_2_point(point->theta);
        phi_point = phi_2_point(point->phi);
        pol_point = pol_2_point(point->polarity);

        offset = freq_point * LINEAR_POLARZA_FREQ_NUMBER
                + phi_point * LINEAR_POLARZA_PHI_NUMBER
                + theta_point * LINEAR_POLARZA_THETA_NUMBER
                + pol_point * LINEAR_POLARZA_POL_NUMBER;

        offset += LINEAR_POLARZA_DATA_OFFSET;
    }
    else if((point->polarity_type == ANTENNA_V0_LEFT_POLARZA_MODE) || (point->polarity_type == ANTENNA_V0_RIGHT_POLARZA_MODE)) {
        freq_point = freq_2_point(TorR, point->freq);
        theta_point = theta_2_point(point->theta);
        phi_point = phi_2_point(point->phi);

        offset = freq_point * CIRCULAR_POLARZA_FREQ_NUMBER
                + phi_point * CIRCULAR_POLARZA_PHI_NUMBER
                + theta_point * CIRCULAR_POLARZA_THETA_NUMBER;

        if(point->polarity_type == ANTENNA_V0_LEFT_POLARZA_MODE) {
            offset += LEFT_HAND_CIRCULAR_DATA_OFFSET;
        }
        if(point->polarity_type == ANTENNA_V0_RIGHT_POLARZA_MODE) {
            offset += RIGHT_HAND_CIRCULAR_DATA_OFFSET;
        }
    }
    else {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(TorR == ANTENNA_V0_TX) {
        offset += TX_DATA_OFFSET;
    }

#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("%s: freq %s%d.%02d, phi %s%d.%02d, theta %s%d.%02d, polarity %s%d.%02d, polarity type 0x%02x, offset %d\r\n", \
            TorR==ANTENNA_V0_RX?"T":"R", \
            point->freq>=0?"":"-", (int32_t)fabsf32(point->freq), (int32_t)(fabsf32(point->freq-(int32_t)(point->freq))*100), \
            point->phi>=0?"":"-", (int32_t)fabsf32(point->phi), (int32_t)(fabsf32(point->phi-(int32_t)(point->phi))*100), \
            point->theta>=0?"":"-", (int32_t)fabsf32(point->theta), (int32_t)(fabsf32(point->theta-(int32_t)(point->theta))*100), \
            point->polarity>=0?"":"-", (int32_t)fabsf32(point->polarity), (int32_t)(fabsf32(point->polarity-(int32_t)(point->polarity))*100), \
            point->polarity_type, \
            offset);
#endif
	if(offset > data_info->size/sizeof(antenna_v0_offset_t)) {
		LOG_E("over range");
		return ERR_INVAL;
	}

	*data = offset_data[offset];
#if DEBUG_LEVEL >= DEBUG_OUTPUT
    xil_printf("range %s%d.%02d, phase %s%d.%02d\r\n", \
            data->range>=0?"":"-", (int32_t)fabsf32(data->range), (int32_t)(fabsf32(data->range-(int32_t)(data->range))*100), \
            data->phase>=0?"":"-", (int32_t)fabsf32(data->phase), (int32_t)(fabsf32(data->phase-(int32_t)(data->phase))*100));
#endif
    return ERR_OK;
}

#endif  /* ANTENNA_V0_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V0 */
