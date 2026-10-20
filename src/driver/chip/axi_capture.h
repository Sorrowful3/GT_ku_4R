/**
 * @file    axi_capture.h
 * @brief   实现输入捕获IP核驱动
 * @author  liaofei
 * @version v2.1.0
 * @data
 */

#ifndef __AXI_CAPTURE_H_
#define __AXI_CAPTURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_AXI_CAPTURE

#include "xparameters.h"
#include "xil_io.h"
#include "input_capture.h"

#define CAPTURE_BASEADDR        XPAR_INPUT_CAPTURE_0_S00_AXI_BASEADDR

#define CAPTURE_ENABLE_REG      INPUT_CAPTURE_S00_AXI_SLV_REG0_OFFSET
#define CAPTURE_MAX_PERIOD_REG  INPUT_CAPTURE_S00_AXI_SLV_REG1_OFFSET
#define CAPTURE_PERIOD0_REG     INPUT_CAPTURE_S00_AXI_SLV_REG2_OFFSET
#define CAPTURE_PERIOD1_REG     INPUT_CAPTURE_S00_AXI_SLV_REG3_OFFSET
#define CAPTURE_PERIOD2_REG     INPUT_CAPTURE_S00_AXI_SLV_REG4_OFFSET
#define CAPTURE_PERIOD3_REG     INPUT_CAPTURE_S00_AXI_SLV_REG5_OFFSET
#define CAPTURE_PERIOD4_REG     INPUT_CAPTURE_S00_AXI_SLV_REG6_OFFSET
#define CAPTURE_PERIOD5_REG     INPUT_CAPTURE_S00_AXI_SLV_REG7_OFFSET
#define CAPTURE_PERIOD6_REG     INPUT_CAPTURE_S00_AXI_SLV_REG8_OFFSET
#define CAPTURE_PERIOD7_REG     INPUT_CAPTURE_S00_AXI_SLV_REG9_OFFSET
#define CAPTURE_PERIOD8_REG     INPUT_CAPTURE_S00_AXI_SLV_REG10_OFFSET
#define CAPTURE_PERIOD9_REG     INPUT_CAPTURE_S00_AXI_SLV_REG11_OFFSET
#define CAPTURE_PERIOD10_REG    INPUT_CAPTURE_S00_AXI_SLV_REG12_OFFSET
#define CAPTURE_PERIOD11_REG    INPUT_CAPTURE_S00_AXI_SLV_REG13_OFFSET
#define CAPTURE_PERIOD12_REG    INPUT_CAPTURE_S00_AXI_SLV_REG14_OFFSET
#define CAPTURE_PERIOD13_REG    INPUT_CAPTURE_S00_AXI_SLV_REG15_OFFSET
#define CAPTURE_PERIOD14_REG    INPUT_CAPTURE_S00_AXI_SLV_REG16_OFFSET
#define CAPTURE_PERIOD15_REG    INPUT_CAPTURE_S00_AXI_SLV_REG17_OFFSET

#define INPUT_CHANNEL0      0x0001
#define INPUT_CHANNEL1      0x0002
#define INPUT_CHANNEL2      0x0004
#define INPUT_CHANNEL3      0x0008
#define INPUT_CHANNEL4      0x0010
#define INPUT_CHANNEL5      0x0020
#define INPUT_CHANNEL6      0x0040
#define INPUT_CHANNEL7      0x0080
#define INPUT_CHANNEL8      0x0100
#define INPUT_CHANNEL9      0x0200
#define INPUT_CHANNEL10     0x0400
#define INPUT_CHANNEL11     0x0800
#define INPUT_CHANNEL12     0x1000
#define INPUT_CHANNEL13     0x2000
#define INPUT_CHANNEL14     0x4000
#define INPUT_CHANNEL15     0x8000
#define INPUT_CHANNEL_MASK  0Xffff

int32_t axi_capture_init(void);
int32_t axi_capture_set_max_period(uint32_t period);
int32_t axi_capture_enable(uint32_t channel);
int32_t axi_capture_disable(uint32_t channel);
int32_t axi_capture_get_period(uint32_t channel, uint32_t period[]);

#endif /* USING_AXI_CAPTURE */

#ifdef __cplusplus
}
#endif



#endif /* __AXI_CAPTURE_H_ */
