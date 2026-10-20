/**
 * @file    gic.h
 * @brief   对FreeRtos的GIC驱动进行扩展
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月10日
 */

#ifndef __GIC_H_
#define __GIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xscugic.h"

#include "../driver_config.h"

void generate_software_intr(uint32_t interrupt_id, uint32_t cpu_id);
#ifdef USING_CORE1
void intr_map_to_cpu1(uint32_t interrupt_id);
#else
#define intr_map_to_cpu1(interrupt_id)
#endif
void set_intr_priority_trigger(uint32_t interrupt_id, uint8_t priority, uint8_t trigger);

#ifdef __cplusplus
}
#endif

#endif /* __GIC_H_ */
