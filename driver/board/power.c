/*
 * power.c
 *
 *  Created on: 2021年3月19日
 *      Author: Administrator
 */

#include "power.h"

#define DEBUG_TAG "power,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_POWER

static power_info_t power_info[] = POWER_INFO;
static uint32_t power_on_list[] = POWER_ON_LIST;

/*
 * 初始化AXI GPIO为输出
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t axi_power_init(power_info_t *info)
{
#ifdef USING_AXI_GPIO
    return axi_gpio_config(info->gpio_number, AXI_GPIO_MODE_OUTPUT);
#else
    return ERR_INVAL;
#endif
}

/*
 * 控制AXI GPIO开启相应电源
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t axi_power_on(power_info_t *info)
{
#ifdef USING_AXI_GPIO
    uint32_t value = AXI_GPIO_LOW;

    if(info->polarity == HIGH_LEVEL_VALID) {
        value = AXI_GPIO_HIGH;
    }
    return axi_gpio_write(info->gpio_number, value);
#else
    return ERR_INVAL;
#endif
}

/*
 * 控制AXI GPIO关闭相应电源
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t axi_power_off(power_info_t *info)
{
#ifdef USING_AXI_GPIO
    uint32_t value = AXI_GPIO_HIGH;

    if(info->polarity == HIGH_LEVEL_VALID) {
        value = AXI_GPIO_LOW;
    }
    return axi_gpio_write(info->gpio_number, value);
#else
    return ERR_INVAL;
#endif
}

/*
 * 初始化PS GPIO为输出
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t ps_power_init(power_info_t *info)
{
#ifdef USING_PS_GPIO
    return ps_gpio_config(info->gpio_number, PS_GPIO_MODE_OUTPUT);
#else
    return ERR_INVAL;
#endif
}

/*
 * 控制PS GPIO开启相应电源
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t ps_power_on(power_info_t *info)
{
#ifdef USING_PS_GPIO
    uint32_t value = PS_GPIO_LOW;

    if(info->polarity == HIGH_LEVEL_VALID) {
        value = PS_GPIO_HIGH;
    }
    return ps_gpio_write(info->gpio_number, value);
#else
    return ERR_INVAL;
#endif
}

/*
 * 控制PS GPIO关闭相应电源
 *
 * @param info 电源网络句柄
 *
 * @return 成功返回EOK
 */
static int32_t ps_power_off(power_info_t *info)
{
#ifdef USING_PS_GPIO
    uint32_t value = PS_GPIO_HIGH;

    if(info->polarity == HIGH_LEVEL_VALID) {
        value = PS_GPIO_LOW;
    }
    return ps_gpio_write(info->gpio_number, value);
#else
    return ERR_INVAL;
#endif
}

/*
 * 初始化电源控制IO
 *
 * @return 成功返回EOK
 */
int32_t power_init(void)
{
    uint32_t i;
    int32_t result;

    //初始化控制引脚
    for(i=0; i<sizeof(power_info)/sizeof(power_info[0]); i++) {
        if(power_info[i].is_axi_ctrl == AXI_GPIO_CTRL) {
            result = axi_power_init(&power_info[i]);
        }
        else {
            result = ps_power_init(&power_info[i]);
        }
        if(result != ERR_OK) {
            return result;
        }
    }

    //开启电源
    for(i=0; i<sizeof(power_on_list)/sizeof(power_on_list[0]); i++) {
        result = power_on(power_on_list[i]);
        if(result != ERR_OK) {
            return result;
        }
    }

    return ERR_OK;
}

/*
 * 开启相应外设的电源
 *
 * @param number 电源网络编号
 *
 * @return 成功返回EOK
 */
int32_t power_on(uint32_t number)
{
    int32_t result;

    if(number == IGNORE_POWER) {
        return ERR_OK;
    }

    if(number > sizeof(power_info)/sizeof(power_info[0])) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(power_info[number].is_axi_ctrl == AXI_GPIO_CTRL) {
        result = axi_power_on(&power_info[number]);
    }
    else {
        result = ps_power_on(&power_info[number]);
    }
    return result;
}

/*
 * 关闭相应外设的电源
 *
 * @param number 电源网络编号
 *
 * @return 成功返回EOK
 */
int32_t power_off(uint32_t number)
{
    int32_t result;

    if(number == IGNORE_POWER) {
        return ERR_OK;
    }

    if(number > sizeof(power_info)/sizeof(power_info[0])) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(power_info[number].is_axi_ctrl == AXI_GPIO_CTRL) {
        result = axi_power_off(&power_info[number]);
    }
    else {
        result = ps_power_off(&power_info[number]);
    }
    return result;
}

#endif /* USING_POWER */
