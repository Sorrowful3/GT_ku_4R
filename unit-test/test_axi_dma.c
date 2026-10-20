/*
 * test_axi_dma.c
 *
 *  Created on: 2021年4月22日
 *      Author: Administrator
 */


#include "../driver/driver.h"

#define TEST_SIZE 8192

static uint8_t rx_buffer[TEST_SIZE];
static uint8_t tx_buffer[TEST_SIZE];
static uint64_t tx_total = 0;
static uint64_t rx_total = 0;

static void test_thread(void *arg)
{
    uint8_t magic;
    uint32_t i;
    size_t size;
    ssize_t result;
    device_t *dma;

    dma = device_open((char*)arg, DEVICE_RDWR);
    if(!dma) {
        printf("open uart failed\r\n");
        vTaskDelete(NULL);
    }

    magic = 0;
    while(1)
    {
        magic++;
        for(i=0; i<sizeof(tx_buffer); i++) {
            tx_buffer[i] = (uint8_t)(i + magic);
        }

        result = device_write(dma, tx_buffer, sizeof(tx_buffer));
        if(result != sizeof(tx_buffer)) {
            continue;
        }
        tx_total += result;

        for(size=0; size<sizeof(rx_buffer); ) {
            result = device_read(dma, &rx_buffer[size], sizeof(rx_buffer)-size);
            if(result <= 0) {
                continue;
            }
            size += result;
            rx_total += result;
        }

        for(i=0; i<sizeof(tx_buffer); i++) {
            if(tx_buffer[i] != rx_buffer[i]) {
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
        vTaskDelay(1000);
        printf("tx_total = %fMB\r\n", tx_total/1024.0/1024.0);
        printf("rx_total = %fMB\r\n", rx_total/1024.0/1024.0);
        printf("\r\n");
    }
}

void test_axi_dma(char *dma_name)
{
    TaskHandle_t xTask;

    xTaskCreate( test_thread,
                 "test_thread",
                 1024,
                 (void*)dma_name,
                 tskIDLE_PRIORITY + 2,
                 &xTask );

    xTaskCreate( print_thread,
                 "print_thread",
                 1024,
                 (void*)dma_name,
                 tskIDLE_PRIORITY + 2,
                 &xTask );
}




