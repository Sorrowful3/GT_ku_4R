/*
 * ps_spi.h
 *
 *  Created on: 2022年4月2日
 *      Author: Administrator
 */

#ifndef __PS_SPI_H_
#define __PS_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../driver_config.h"
#include "ps_gpio.h"

#ifdef USING_PS_SPI

#include "sleep.h"
#include "xspips.h"
#include "xparameters.h"
#include "xplatform_info.h"

//时钟相位
#define PS_SPI_BUS_PHASE_1EDGE     (0<<0)
#define PS_SPI_BUS_PHASE_2EDGE     (1<<0)
//时钟极性
#define PS_SPI_BUS_POLARITY_LOW    (0<<1)
#define PS_SPI_BUS_POLARITY_HIGH   (1<<1)
//片选
#define PS_SPI_BUS_NO_CS           (1<<4)

#define PS_SPI_BUS_MODE_MASK (PS_SPI_BUS_PHASE_2EDGE | PS_SPI_BUS_POLARITY_HIGH | PS_SPI_BUS_MSB |  PS_SPI_BUS_CS_HIGH | PS_SPI_BUS_NO_CS)

#define PS_SPI_BUS_CLOCK_MODE0    (PS_SPI_BUS_POLARITY_LOW | PS_SPI_BUS_PHASE_1EDGE)
#define PS_SPI_BUS_CLOCK_MODE1    (PS_SPI_BUS_POLARITY_LOW | PS_SPI_BUS_PHASE_2EDGE)
#define PS_SPI_BUS_CLOCK_MODE2    (PS_SPI_BUS_POLARITY_HIGH | PS_SPI_BUS_PHASE_1EDGE)
#define PS_SPI_BUS_CLOCK_MODE3    (PS_SPI_BUS_POLARITY_HIGH | PS_SPI_BUS_PHASE_2EDGE)

#define PS_SPI_BUS_GPIO_CS        0x01
#define PS_SPI_BUS_CONTROLLER_CS  0x02

#define PS_SPI_CLIENT_CONFIG_DEFAULT            \
{                                               \
    .mode  = PS_SPI_BUS_CLOCK_MODE0,            \
    .data_width = 8,                            \
    .cs_mode = PS_SPI_BUS_GPIO_CS,              \
    .clk_hz  = 20*1000*1000,                    \
}

typedef struct{
    const char  *name;      /* SPI控制器设备名称 */
    uint16_t    device_id;  /* 设备ID */
    uint32_t    intr_id;    /* 控制器中断ID */
    uint32_t    clk_hz;     /* 控制器总线时钟频率 */
}ps_spi_bus_config_t;

struct ps_spi_client;
typedef struct ps_spi_bus{
    device_t                device;         /* 继承于device_t */
    SemaphoreHandle_t       lock;           /* 互斥锁 */
    SemaphoreHandle_t       sync;           /* 传输完成同步信号量 */
    uint32_t                intr_state;     /* 中断状态 */
    struct ps_spi_client    *owner;         /* 此总线当前绑定的客户端 */
    ps_spi_bus_config_t     *bus_config;
    XSpiPs                  spi_instance;   /* SPI控制器 */
}ps_spi_bus_t;

typedef struct{
    const void *send_buf;
    void       *recv_buf;
    size_t     length;

    uint8_t    cs_take;
    uint8_t    cs_release;
}ps_spi_msg_t;

typedef struct {
    uint8_t  mode;          /* 模式 */
    uint8_t  data_width;    /* 数据宽度 */
    uint32_t clk_hz;        /* 时钟频率 */
    uint32_t cs_mode;       /* 片选模式，PS_SPI_BUS_CONTROLLER_CS表示采用控制器的片选信号*/
    uint32_t cs_pin;        /* 片选引脚 */
    uint32_t cs_delay;      /* 片选延迟 */
    uint32_t cs_interval;   /* 片选间隔 */
}ps_spi_client_config_t;

typedef struct ps_spi_client{
    ps_spi_bus_t            *bus;           /* ps_spi总线 */
    ps_spi_client_config_t  client_config;  /* ps_spi客户端的配置 */
    void                    *private_data;  /* 预留 */
}ps_spi_client_t;

int32_t ps_spi_bus_init(void);

//当片选模式是PS_SPI_BUS_CONTROLLER_CS时一次只能传输一个msg
ssize_t ps_spi_bus_transfer(ps_spi_bus_t *bus, ps_spi_msg_t msgs[], size_t num);
ps_spi_bus_t *ps_spi_bus_find(const char *name);
int32_t ps_spi_client_init(ps_spi_client_t *client, const char *bus_name, ps_spi_client_config_t *config);
int32_t ps_spi_client_config(ps_spi_client_t *client, ps_spi_client_config_t *config);
ssize_t ps_spi_client_transfer(ps_spi_client_t *client, ps_spi_msg_t msgs[], size_t num);

#endif /* USING_PS_SPI */

#ifdef __cplusplus
}
#endif

#endif /* __PS_SPI_H_ */
