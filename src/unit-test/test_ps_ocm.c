/*
 * ps_ocm_test.c
 *
 *  Created on: 2021年4月23日
 *      Author: Administrator
 */

#include "../driver/driver.h"
#include "test_ps_ocm.h"

#ifdef USING_CORE0

static uint64_t rx_total = 0;
static uint64_t tx_total = 0;

static uint8_t rx_buffer[1024];
static uint8_t tx_buffer[1024];

static void test_thread(void *arg)
{
    uint8_t magic;
    int32_t i;
    size_t size;
    ssize_t result;
    device_t *ocm_dev;

    ocm_dev = device_open("ps_ocm", DEVICE_RDWR);
    if(!ocm_dev) {
        printf("open ps_ocm failed\n");
        vTaskDelete(NULL);
    }

    magic = 0;
    while(1)
    {
        magic++;
        for(i=0; i<(int32_t)sizeof(tx_buffer); i++) {
            tx_buffer[i] = i + magic;
        }

        result = device_write(ocm_dev, tx_buffer, sizeof(tx_buffer));
        if(result != (ssize_t)sizeof(tx_buffer)) {
            continue;
        }
        tx_total += result;

        for(size=0;  size<sizeof(rx_buffer); ) {
            result = device_read(ocm_dev, &rx_buffer[size], sizeof(rx_buffer)-size);
            if(result <= 0) {
                continue;
            }
            size += result;
            rx_total += result;
        }

        for(i=0; i<sizeof(rx_buffer); i++) {
            if(rx_buffer[i] != tx_buffer[i]) {
                printf("check failed\r\n");
                break;
            }
        }
    }
}

static void print_thread(void *arg)
{
    while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);
        printf("tx_total = %fMB\r\n", tx_total/1024.0/1024.0);
        printf("rx_total = %fMB\r\n", rx_total/1024.0/1024.0);
        printf("\r\n");
    }
}

void test_ps_ocm(void)
{
    TaskHandle_t xTask;

    xTaskCreate(test_thread,
                (const char *)"test_thread",
                2048,
                NULL,
                tskIDLE_PRIORITY + 2,
                &xTask );

    xTaskCreate(print_thread,
                (const char *)"print_thread",
                2048,
                NULL,
                tskIDLE_PRIORITY + 3,
                &xTask );
}

#else

static uint8_t buffer[1024];

static void test_thread(void *arg)
{
    device_t *ocm_dev;
    ssize_t tmp_size;

    ocm_dev = device_open("ps_ocm", DEVICE_RDWR);
    if(!ocm_dev) {
        printf("open ps_ocm failed\r\n");
        vTaskDelete(NULL);
    }

    while(1)
    {
        //从OCM读数据
        tmp_size = device_read(ocm_dev, &buffer[0], sizeof(buffer));
        //写回到OCM
        if(tmp_size > 0) {
            device_write(ocm_dev, buffer, tmp_size);
        }
    }
}

void test_ps_ocm(void)
{
    TaskHandle_t xTask;

    xTaskCreate(test_thread,
                (const char *)"test_thread",
                2048,
                NULL,
                tskIDLE_PRIORITY + 2,
                &xTask);
}

#endif /* THIS_CORE0 */


