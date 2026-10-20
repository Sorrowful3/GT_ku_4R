/*
 * test_uart.c
 *
 *  Created on: 2021年4月22日
 *      Author: Administrator
 */

#include "../driver/driver.h"

static void test_thread(void *arg)
{
    ssize_t size;
    device_t *uart;
    char buf[100];

    uart = device_open((char*)arg, DEVICE_RDWR);
    if(!uart) {
        printf("open uart failed\r\n");
        vTaskDelete(NULL);
    }

    while(1)
    {
        memset(buf, 0, 100);
        size = device_read(uart, buf, 99);
        if(size <= 0) {
            continue;
        }
        device_write(uart, buf, size);
    }
}

void test_uart(char *uart_name)
{
    TaskHandle_t xTask;

    xTaskCreate( test_thread,
                 "test_thread",
                 1024,
                 (void*)uart_name,
                 tskIDLE_PRIORITY + 2,
                 &xTask );
}

