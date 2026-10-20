/*
 * fan_control.c
 *
 *  Created on: 2021年5月6日
 *      Author: Administrator
 */

#include "fan_control.h"

#define DEBUG_TAG "fan_control,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../driver/core/debug.h"

static fan_control_parameter_t control_parameter[] = FAN_CONTROL_PARAMETER;
static fan_control_info_t control_info[] = FAN_CONTROL_INFO;

static float read_temperature(fan_control_info_t *control_info)
{
    device_state_t device_state;

    get_device_state(&device_state);

    switch(control_info->number) {
    case 0:
        return device_state.tx_temperature[0];
    case 1:
        return device_state.rx_temperature[0];
    default:
        return 60.0f;
    }
}

static void control_thread(void *arg)
{
    uint32_t i;
    uint32_t j;
    uint32_t period;
    float step;
    float offset;
    float temperature;

    while(1)
    {
        for(i=0; i<sizeof(control_info)/sizeof(control_info[0]); i++) {
            //读取温度
            temperature = read_temperature(&control_info[i]);

            //根据温度查询占空比
            control_info[i].ctrl_foc = control_parameter[0].duty_cycle;
            for(j=0; j<sizeof(control_parameter)/sizeof(control_parameter[0])-1; j++) {
                if((temperature >= control_parameter[j].temperature) && (temperature < control_parameter[j+1].temperature)) {
                    step =  control_parameter[j+1].temperature - control_parameter[j].temperature;
                    offset = temperature - control_parameter[j].temperature;
                    control_info[i].ctrl_foc = control_parameter[j].duty_cycle * (1.0f - offset / step) + control_parameter[j+1].duty_cycle * (offset / step);
                    break;
                }
            }
            if(temperature >= control_parameter[sizeof(control_parameter)/sizeof(control_parameter[0])-1].temperature) {
                control_info[i].ctrl_foc = control_parameter[sizeof(control_parameter)/sizeof(control_parameter[0])-1].duty_cycle;
            }

            //设置占空比
          //  axi_pwm_set_compare(control_info[i].pwm_channel, (PWM_CLK_MHZ*PWM_PERIOD)-(uint32_t)((PWM_CLK_MHZ*PWM_PERIOD)*(control_info[i].ctrl_foc/100.0f)));
//           if(i==0)
            axi_pwm_set_compare(control_info[i].pwm_channel, 60000);
//           else
//        	 axi_pwm_set_compare(control_info[i].pwm_channel, 0);
            //获取风扇转速
            axi_capture_get_period(control_info[i].capture_channel, &period);
            taskENTER_CRITICAL();
            control_info[i].speed = (uint32_t)((1000000.0f / (period / (float)CAPTURE_CLK_MHZ)) / 2.0f);
            taskEXIT_CRITICAL();
        }

        //每隔500ms调节一次转速
        vTaskDelay(500);
    }
}

int32_t fan_control_init(void)
{
    uint32_t i;
    int32_t result;
    TaskHandle_t handle;
    portBASE_TYPE xresult;

    //开启风扇电源
    result = power_on(FAN_12V_POWER);
    if(result != ERR_OK) {
        LOG_E("open power failed");
        return result;
    }
    result = power_on(FAN_SYS_POWER);
    if(result != ERR_OK) {
        LOG_E("open power failed");
        return result;
    }

    //设置PWM周期
    axi_pwm_set_period(PWM_CLK_MHZ * PWM_PERIOD);
    //设置最大捕获周期
    axi_capture_set_max_period(CAPTURE_CLK_MHZ * 1000000);

    //使能PWM和脉冲捕获
    for(i=0; i<sizeof(control_info)/sizeof(control_info[0]); i++) {
        //使能脉冲捕获
        result = axi_capture_enable(control_info[i].capture_channel);
        if(result != ERR_OK) {
            LOG_E("enable input capture failed");
            return result;
        }
        //设置占空比为0
        result = axi_pwm_set_compare(control_info[i].pwm_channel, 0x00);
        if(result != ERR_OK) {
            LOG_E("set PWM failed");
            return result;
        }
        //使能PWM输出
        result = axi_pwm_enable(control_info[i].pwm_channel);
        if(result != ERR_OK) {
            LOG_E("enable PWM failed");
            return result;
        }
    }

    xresult = xTaskCreate(control_thread,
                         "control_thread",
                         2048,
                         NULL,
                         tskIDLE_PRIORITY + 3,
                         &handle );
    if(xresult != pdPASS) {
        LOG_E("create task fan_control_thread failed\r\n");
        return ERR_NOMEM;
    }
    return ERR_OK;
}

int32_t fan_get_speed(uint32_t number, uint32_t *speed)
{
    if((number >= sizeof(control_info)/sizeof(control_info[0])) || (!speed)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    taskENTER_CRITICAL();
    *speed = control_info[number].speed;
    taskEXIT_CRITICAL();

    return ERR_OK;
}
