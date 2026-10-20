/*
 * adis16465.h
 *
 *  Created on: 2022年4月6日
 *      Author: Administrator
 */

#ifndef __ADIS16465_H_
#define __ADIS16465_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sleep.h"

#include "../core/core_include.h"
#include "../chip/chip_include.h"
#include "../driver_config.h"

#ifdef USING_ADIS16465

//ADIS16465 registers
#define DIAG_STAT       0x02
#define X_GYRO_LOW      0x04
#define X_GYRO_OUT      0x06
#define Y_GYRO_LOW      0x08
#define Y_GYRO_OUT      0x0A
#define Z_GYRO_LOW      0x0C
#define Z_GYRO_OUT      0x0E
#define X_ACCL_LOW      0x10
#define X_ACCL_OUT      0x12
#define Y_ACCL_LOW      0x14
#define Y_ACCL_OUT      0x16
#define Z_ACCL_LOW      0x18
#define Z_ACCL_OUT      0x1A
#define TEMP_OUT        0x1C
#define TIME_STAMP      0x1E
#define DATA_CNTR       0x22
#define X_DELTANG_LOW   0x24
#define X_DELTANG_OUT   0x26
#define Y_DELTANG_LOW   0x28
#define Y_DELTANG_OUT   0x2A
#define Z_DELTANG_LOW   0x2C
#define Z_DELTANG_OUT   0x2E
#define X_DELTVEL_LOW   0x30
#define X_DELTVEL_OUT   0x32
#define Y_DELTVEL_LOW   0x34
#define Y_DELTVEL_OUT   0x36
#define Z_DELTVEL_LOW   0x38
#define Z_DELTVEL_OUT   0x3A
#define FILT_CTRL       0x5C
#define RANG_MDL        0x5E
#define MSC_CTRL        0x60
#define UP_SCALE        0x62
#define DEC_RATE        0x64
#define NULL_CNFG       0x66
#define GLOB_CMD        0x68
#define PROD_ID         0x72


#define ADIS16465_DEV_CTRL_GET_CONFIG (DEVICE_USER_CTRL+1)

typedef struct{
    int32_t gyro_x;    /* 陀螺仪x轴原始数据 */
    int32_t gyro_y;    /* 陀螺仪y轴原始数据 */
    int32_t gyro_z;    /* 陀螺仪z轴原始数据 */
    int32_t acc_x;     /* 加速度计x轴原始数据 */
    int32_t acc_y;     /* 加速度计y轴原始数据 */
    int32_t acc_z;     /* 加速度计z轴原始数据 */
    int16_t temp;      /* 温度原始数据 */
}adis16465_original_t;

typedef struct{
    const char  *name;          /* 设备名称 */
    const char  *spi_bus;       /* SPI总线名称 */
    uint32_t    spi_cs;         /* SPI片选*/
    uint32_t    cs_mode;        /* SPI片选模式*/
    uint16_t    output_rate;    /* 输出速率 */
    uint32_t    rst_io;         /* 复位引脚 */
}adis16465_config_t;

typedef struct{
    uint16_t    output_rate;    /* 输出速率，hz/s */
    float       acc_scale;      /* 加速度计分辨率 ，g/lsb*/
    float       acc_ratio;      /* 数据放大倍率 */
    float       gyro_scale;     /* 陀螺仪分辨率，°/s/lsb */
    float       gyro_ratio;      /* 数据放大倍率 */
    float       temp_scale;     /* 温度分辨率 ，°/lsb*/
}adis16465_info_t;

typedef struct{
    device_t            device;     /* 继承于device_t */

    adis16465_config_t  *config;    /* 暂存配置传输 */
    ps_spi_client_t     spi_client; /* 继承ps_spi_client_t */
    SemaphoreHandle_t   lock;       /* 互斥锁 */
    adis16465_info_t    info;       /* 基本信息 */
}adis16465_t;

int32_t adis16465_init(void);

#endif /* USING_ADIS16465 */

#ifdef __cplusplus
}
#endif

#endif /* __ADIS16465_H_ */
