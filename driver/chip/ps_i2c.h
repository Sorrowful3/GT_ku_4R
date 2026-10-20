/**
 * @file    ps_i2c.h
 * @brief   实现PS端IIC模块驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年4月26日
 */

#ifndef __PS_I2C_H_
#define __PS_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/core_include.h"
#include "../driver_config.h"

#ifdef USING_PS_I2C

#include "xparameters.h"
#include "xiicps.h"

typedef struct{
    const char *name;   /* IIC控制器设备名称 */
    uint16_t device_id; /* 设备ID */
    uint32_t clk_hz;    /* 总线时钟频率 */
}ps_i2c_config_t;

typedef struct{
    device_t            device;     /* 继承device_t */
    SemaphoreHandle_t   lock;       /* 互斥锁 */

    XIicPs              i2c;        /* IIC句柄 */
    ps_i2c_config_t     *config;    /* IIC配置参数 */
}ps_i2c_t;


int32_t ps_i2c_init(void);
ssize_t ps_i2c_master_send(device_t *device, uint8_t *buffer, size_t lenght, uint16_t slave_addr);
ssize_t ps_i2c_master_recv(device_t *device, uint8_t *buffer, size_t lenght, uint16_t slave_addr);

#endif /* USING_PS_I2C */

#ifdef __cplusplus
}
#endif

#endif /* __PS_I2C_H_ */
