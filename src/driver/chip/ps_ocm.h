/*
 * ps_ocm.h
 *
 *  Created on: 2021年3月8日
 *      Author: Administrator
 */

#ifndef __PS_OCM_H_
#define __PS_OCM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gic.h"

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_PS_OCM

#include "xil_mmu.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xil_cache.h"

#define PS_OCM_GET_MSG_SIZE (DEVICE_USER_CTRL+1)

#define CORE1_ID    2
#define CORE0_ID    1

typedef struct{
    int32_t lenght;                 //数据有效长度
    uint8_t msg[PS_OCM_MSG_SIZE];   //数据缓存
}__attribute__((packed))ocm_msg_t;

#define CORE0_TO_CORE1_MSG  ((ocm_msg_t *volatile)PS_OCM_BASE_ADDRESS)
#define CORE1_TO_CORE0_MSG  ((ocm_msg_t *volatile)(PS_OCM_BASE_ADDRESS + sizeof(ocm_msg_t)))

typedef struct{
    device_t                device;

    fifo_t                  *rx_fifo;

    fifo_t                  *tx_fifo;
    SemaphoreHandle_t       tx_lock;
    SemaphoreHandle_t       tx_sync;
    int32_t                 tx_idle;
}ps_ocm_t;

int32_t ps_ocm_init(void);

#endif /* USING_PS_OCM */

#ifdef __cplusplus
}
#endif

#endif /* __PS_OCM_H_ */
