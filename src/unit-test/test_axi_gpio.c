/*
 * test_axi_gpio.c
 *
 *  Created on: 2021年4月23日
 *      Author: Administrator
 */

#include "../driver/driver.h"

#ifdef USING_AXI_GPIO

#define LED0 0
#define LED1 1

#define KEY0 32
#define KEY1 33
#define KEY_CHANNEL AXI_GPIO_CHANNEL2

static void gpio_hdr(void *arg)
{
    uint32_t key = (uint32_t)arg;

    if(key == KEY0) {
        printf("AXI KEY0 interrupt\n");
    }
    else if(key == KEY1) {
        printf("AXI KEY1 interrupt\n");
    }
}

static void test_thread(void *arg)
{
    //LED默认点亮
    axi_gpio_write(LED0, AXI_GPIO_HIGH);
    axi_gpio_write(LED1, AXI_GPIO_HIGH);
    //配置LED为输出
    axi_gpio_config(LED0, AXI_GPIO_MODE_OUTPUT);
    axi_gpio_config(LED1, AXI_GPIO_MODE_OUTPUT);

    //配置引脚为中断
    axi_gpio_attach_irq(KEY0, AXI_GPIO_IRQ_LOW, gpio_hdr, (void *)KEY0);
    axi_gpio_attach_irq(KEY1, AXI_GPIO_IRQ_LOW, gpio_hdr, (void *)KEY1);
    axi_gpio_irq_enable(KEY_CHANNEL);

    //LED闪烁
    while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);

        axi_gpio_write(LED0, AXI_GPIO_LOW);
        axi_gpio_write(LED1, AXI_GPIO_LOW);

        vTaskDelay(1000/portTICK_RATE_MS);

        axi_gpio_write(LED0, AXI_GPIO_HIGH);
        axi_gpio_write(LED1, AXI_GPIO_HIGH);
    }
}

void test_axi_gpio(void)
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
        printf("create task ps_pin_test failed\r\n");
    }
}
#endif /* USING_AXI_PIN */




