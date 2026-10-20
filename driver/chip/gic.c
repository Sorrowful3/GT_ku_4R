/**
 * @file    gic.c
 * @brief   对FreeRtos的GIC驱动进行扩展
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月10日
 */

#include "gic.h"

extern XScuGic xInterruptController;

/**
 * @function nane: generate_software_intr
 * @author: liaofei
 * @data: 2021年3月10日
 * @input param:
 *              interrupt_id 中断号
 *              cpu_id CPU核号，0x01 核0,0x02 核1
 * @output param:
 *              无
 * @description:
 * 向指定CPU产生软中断
 * @modeify log:
 * 2021年3月10日 liaofei 初次创建
 */
void generate_software_intr(uint32_t interrupt_id, uint32_t cpu_id)
{
    XScuGic_SoftwareIntr(&xInterruptController, interrupt_id, cpu_id);
}

#ifdef USING_CORE1
/**
 * @function nane:
 * @author: liaofei
 * @data: 2021年3月10日
 * @input param:
 *              interrupt_id 中断号
 * @output param:
 *              无
 * @description:
 * 将中断映射到核1
 * @modeify log:
 * 2021年3月10日 liaofei 初次创建
 */
void intr_map_to_cpu1(uint32_t interrupt_id)
{
    XScuGic_InterruptMaptoCpu(&xInterruptController, 1, interrupt_id);
}
#endif

/**
 * @function nane: set_intr_priority_trigger
 * @author: liaofei
 * @data: 2021年3月10日
 * @input param:
 *              interrupt_id 中断号
 *              priority 优先级，0优先级最高，0xF8(248)最低，共有32个优先级，分别是0,8,16,32,40, …, 248
 *              trigger  触发方式 ；只对SPI（共享）中断有效，其他类型的中断无效，01b高电平有效，11b上升沿有效
 * @output param:
 *              无
 * @description:
 * 设置中断优先级和触发方式
 * @modeify log:
 * 2021年3月10日 liaofei 初次创建
 */
void set_intr_priority_trigger(uint32_t interrupt_id, uint8_t priority, uint8_t trigger)
{
    XScuGic_SetPriorityTriggerType(&xInterruptController, interrupt_id, priority, trigger);
}

