/*
 * antenna_v1_offset.c
 *
 *  Created on: 2021年6月5日
 *    Author: Administrator
 */

#include "antenna_v1_offset.h"
#include "antenna_v1_offset0.h"
#include "antenna_v1_offset1.h"

#define DEBUG_TAG   "antenna_v1_offset,%d", __LINE__
#define DEBUG_LEVEL INFO_OUTPUT
#include "../../core/debug.h"

#ifdef USING_ANTENNA_V1
#ifdef ANTENNA_V1_USING_POLARITY_OFFSET

static const antenna_v1_offset_t *offset_data = NULL;
static antenna_v1_offset_info_t data_info;

static uint16_t sum16(const uint8_t *data, uint32_t lenght, uint16_t init_val)
{
    uint32_t i;
    uint16_t sum = init_val;

    for(i=0; i<lenght; i++) {
        sum += data[i];
    }
    return sum;
}

int32_t antenna_v1_offset_init(void)
{
    uint16_t sum;

    data_info = *((antenna_v1_offset_info_t*)0x30000000);
    //校准数据不大于256MB
    if(data_info.size > 1024*1024*256) {
        LOG_E("correct data invalid");
        return ERR_INVAL;
    }

    //进行和校验
    if(data_info.sum_flag) {
        sum = sum16((uint8_t *)data_info.load_addr, data_info.size, 0xffff);
        if(sum != data_info.sum) {
            LOG_E("correct data CRC checkout failed");
            return ERR_INVAL;
        }
    }

    LOG_I("load_addr = %lx", data_info.load_addr);
    LOG_I("size = %ld", data_info.size);
    if(data_info.sum_flag) {
        LOG_I("SUM = %x", sum);
    }

    offset_data = (antenna_v1_offset_t*)data_info.load_addr;
    return ERR_OK;
}

int32_t antenna_v1_offset_find(uint8_t TorR, const antenna_v1_point_t *point, antenna_v1_offset_t *data)
{
	if(data_info.size < 64696320) {
		return antenna_v1_offset0_find(TorR, point, &data_info, data);
	}
	else {
		return antenna_v1_offset1_find(TorR, point, &data_info, data);
	}
    return ERR_OK;
}

#endif  /* ANTENNA_V1_USING_POLARITY_OFFSET */
#endif /* USING_ANTENNA_V1 */

