/*
 * test_axi_pwm.c
 *
 *  Created on: 2021年6月24日
 *      Author: Administrator
 */

#include "../driver/driver.h"


void test_axi_pwm(void)
{
    uint32_t period;
    uint32_t compare;

    //开启风扇电源
    power_on(FAN_12V_POWER);
    power_on(FAN_SYS_POWER);

    axi_capture_enable(INPUT_CHANNEL1);
    axi_capture_enable(INPUT_CHANNEL4);
    axi_capture_enable(INPUT_CHANNEL7);

    printf("enable fan1\r\n");
    axi_pwm_enable(PWM_CHANNEL1);
    axi_pwm_set_compare(PWM_CHANNEL1, 5000);
    vTaskDelay(20);
    printf("enable fan4\r\n");
    axi_pwm_enable(PWM_CHANNEL4);
    axi_pwm_set_compare(PWM_CHANNEL4, 5000);
    vTaskDelay(20);
    printf("enable fan7\r\n");
    axi_pwm_enable(PWM_CHANNEL7);
    axi_pwm_set_compare(PWM_CHANNEL7, 5000);
    vTaskDelay(20);

    while(1)
    {
        for(compare=5000; compare<(100000-500); )
        {
            printf("duty cycle %f\r\n", compare/100000.0*100);
            axi_capture_get_period(INPUT_CHANNEL1, &period);
            printf("fan1 speed %f\r\n", (1000000.0f/(period/142.0f))/2.0f);
            axi_capture_get_period(INPUT_CHANNEL4, &period);
            printf("fan4 speed %f\r\n", (1000000.0f/(period/142.0f))/2.0f);
            axi_capture_get_period(INPUT_CHANNEL7, &period);
            printf("fan7 speed %f\r\n", (1000000.0f/(period/142.0f))/2.0f);

            compare += 500;
            axi_pwm_set_compare(PWM_CHANNEL1, compare);
            axi_pwm_set_compare(PWM_CHANNEL4, compare);
            axi_pwm_set_compare(PWM_CHANNEL7, compare);
            vTaskDelay(50);
        }
    }
}
