/*
 * platform.c
 *
 *  Created on: 2021年3月1日
 *      Author: Administrator
 */


#include "platform.h"

#ifdef START_CORE1
static void start_cpu1(void)
{
    //向"0Xffffffff0"地址写入 CORE1代码的起始地址
    Xil_Out32(0xfffffff0, CORE1_TEXT_ADDR);
    //内存屏障，等待之前的内存访问结束
    dmb();
    //通过"SEV"指令唤醒CPU1并跳转至相应的程序
    __asm__("sev");
}
#endif

#ifdef USING_TRANSDUCER
static void transducer_callbk(device_t *device, const uint8_t *command, uint32_t lenght)
{
    mavlink_transducer_broadcast(command, lenght);
}
#endif

#ifdef USING_BEACON
static void beacon_callbk(device_t *device, uint8_t *command, uint32_t lenght)
{
    mavlink_beacon_broadcast(command, lenght);
}
#endif

int32_t platform_init(void)
{
    int32_t result;
    device_t *transducer;
    device_t *beacon;
    device_t *noe_m8;

    /* 片级初始化 */
#ifdef USING_PS_GPIO
    result = ps_gpio_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_GPIO
    result = axi_gpio_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_PS_OCM
    result = ps_ocm_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_DMA
    result = axi_dma_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_UARTLITE
    /*串口总线初始化*/
    result = axi_uartlite_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_UART550
    result = axi_uart550_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_PS_I2C
    result = ps_i2c_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_PWM
    result = axi_pwm_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_AXI_CAPTURE
    result = axi_capture_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_PS_WDT
    result = ps_wdt_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_PS_SPI
    result = ps_spi_bus_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

    /* 板级初始化 */
#ifdef USING_POWER
    result = power_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_RAV260
    result = rav260_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_TRANSDUCER
    result = transducer_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_BEACON
    result = beacon_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_ANTENNA_HAL
    result = antenna_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_TMP112
    result = tmp112_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_INA220
    result = ina220_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_INA226
    result = ina226_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_ADIS16465
    result = adis16465_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

#ifdef USING_NOE_M8
    result = noe_m8_init();
    if(result != ERR_OK) {
        return result;
    }
#endif

    /* 启动核1 */
#ifdef START_CORE1
    start_cpu1();
#endif

    //等待上电结束
    vTaskDelay(2000);

    //对UBLOX M8进行默认配置
    noe_m8 = device_open("noe_m8_0", DEVICE_RDWR);
    if(!noe_m8) {
        return ERR_IO;
    }
    noe_m8_produce_cfg(noe_m8);
    device_close(noe_m8);

    /* 应用组件初始化 */
    result = adis16465_imu1_init("adis16465_0");
    if(result != ERR_OK) {
        return result;
    }

    result = device_monitor_init();
    if(result != ERR_OK) {
        return result;
    }

    result = fan_control_init();
    if(result != ERR_OK) {
        return result;
    }

    result = satmov1_init();
    if(result != ERR_OK) {
        return result;
    }

    result = mavlink_top_init();
    if(result != ERR_OK) {
        return result;
    }


    debug_print_custom_set(mavlink_print);

#ifdef USING_TRANSDUCER
    transducer = device_open("transducer0", DEVICE_RDWR);
    if(!transducer) {
        return ERR_IO;
    }
    transducer_set_receive_callbk(transducer, transducer_callbk);
    device_close(transducer);
#endif

#ifdef USING_BEACON
    beacon = device_open("beacon0", DEVICE_RDWR);
    if(!beacon) {
        return ERR_IO;
    }
    beacon_set_receive_callbk(beacon, beacon_callbk);
    device_close(beacon);
#endif

#ifdef USING_ANTENNA_V0
    antenna_v0_set_callbk(mavlink_antenna_broadcast);
#endif
#ifdef USING_ANTENNA_V1
    antenna_v1_set_callbk(mavlink_antenna_broadcast);
#endif

    //注册校准完成回调函数
    satmov1_set_currect_callbk(mavlink_currect_yaw_complete);
    //等待初始化结束
    vTaskDelay(20000);

#ifdef USING_PS_WDT

    result = ps_wdt_start();
    if(result != ERR_OK) {
        return result;
    }
#endif

    //启动对象程序
    satmov1_start();

    return ERR_OK;
}
