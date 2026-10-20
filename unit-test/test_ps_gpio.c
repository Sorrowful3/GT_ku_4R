/*
 * test_ps_gpio.c
 *
 *  Created on: 2021年4月23日
 *      Author: Administrator
 */


#include "../driver/driver.h"

#ifdef USING_PS_GPIO

#define LED0 7
#define LED1 8

#define KEY0 12
#define KEY1 11

static void gpio_hdr(void *arg)
{
    uint32_t key = (uint32_t)arg;

    if(key == KEY0) {
        printf("PS KEY0 interrupt\n");
    }
    else if(key == KEY1) {
        printf("PS KEY1 interrupt\n");
    }
}

static void test_thread(void *arg)
{
    //LED默认点亮
    ps_gpio_write(LED0, PS_GPIO_HIGH);
    ps_gpio_write(LED1, PS_GPIO_HIGH);
    //配置LED为输出
    ps_gpio_config(LED0, PS_GPIO_MODE_OUTPUT);
    ps_gpio_config(LED1, PS_GPIO_MODE_OUTPUT);

    //配置引脚为中断
    ps_gpio_attach_irq(KEY0, PS_GPIO_IRQ_FALLING, gpio_hdr, (void *)KEY0);
    ps_gpio_attach_irq(KEY1, PS_GPIO_IRQ_FALLING, gpio_hdr, (void *)KEY1);

    //LED闪烁
    while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);

        ps_gpio_write(LED0, PS_GPIO_LOW);
        ps_gpio_write(LED1, PS_GPIO_LOW);

        vTaskDelay(1000/portTICK_RATE_MS);

        ps_gpio_write(LED0, PS_GPIO_HIGH);
        ps_gpio_write(LED1, PS_GPIO_HIGH);
    }
}


void test_ps_gpio(void)
{
    TaskHandle_t handle;
    portBASE_TYPE result;

    //创建启动线程
    result = xTaskCreate(test_thread,
                         "test_thread",
                         1024,
                         NULL,
                         tskIDLE_PRIORITY + 2,
                         &handle );
    if(result != pdPASS) {
        printf("create task test_thread failed\r\n");
    }
}

#endif /* USING_PS_GPIO */



