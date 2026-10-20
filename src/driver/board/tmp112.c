/*
 * tmp112.c
 *
 *  Created on: 2021年4月26日
 *      Author: Administrator
 */

#include "tmp112.h"

#define DEBUG_TAG "tmp112,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../core/debug.h"

#ifdef USING_TMP112

static tmp112_config_t tmp112_config[] =
{
#ifdef TMP112_0_CONFIG
    TMP112_0_CONFIG,
#endif
#ifdef TMP112_1_CONFIG
    TMP112_1_CONFIG,
#endif
};

static tmp112_t tmp112[sizeof(tmp112_config)/sizeof(tmp112_config[0])];

static ssize_t __tmp112_pread(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    uint8_t send_buffer[1];
    uint8_t recv_buffer[1];
    float *temperature = (float*)buffer;
    tmp112_t *sensor = container_of(device, tmp112_t, device);

    if(size < sizeof(float)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //设置寄存器地址
    send_buffer[0] = 0x00;
    if(ps_i2c_master_send(sensor->i2c, send_buffer, 1, sensor->config->slave_addr) != 1) {
//    lutao modifiy 2022-- 4R is error
//    	LOG_E("write data failed");
        return ERR_IO;
    }
    //读温度寄存器，温度采用12位补码表示且位于高位
    memset(recv_buffer, 0, sizeof(recv_buffer));
    if(ps_i2c_master_recv(sensor->i2c, recv_buffer, 1, sensor->config->slave_addr) != 1) {
    	//    lutao modifiy 2022-- 4R is error
    	//        LOG_E("read data failed");

        return ERR_IO;
    }

    //温度数据12位表示，左对齐补码
//    recv_buffer[1] = recv_buffer[1] >> 4;
//    temp = ((recv_buffer[0] << 4) | recv_buffer[1]);
//    if(temp & 0x800) {
//        temp |= 0xF000;
//    }
    *temperature = recv_buffer[0];

    return sizeof(float);
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = __tmp112_pread,
    .pwrite = NULL,
    .control = NULL,
};

int32_t tmp112_init(void)
{
    uint32_t i;
    int32_t result;

    for(i=0; i<sizeof(tmp112_config)/sizeof(tmp112_config[0]); i++) {
        tmp112[i].config = &tmp112_config[i];

        //打开对应的I2C总线
        tmp112[i].i2c = device_open(tmp112[i].config->bus_name, DEVICE_RDWR);
        if(!tmp112[i].i2c) {
            LOG_E("open i2c failed");
            return ERR_ROUTINE;
        }

        //注册TMP112设备
        tmp112[i].device.magic = TMP112_MAGIC;
        tmp112[i].device.ops = &ops;
        result = device_register(&tmp112[i].device, tmp112[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }
    return ERR_OK;
}

#endif /* USING_TMP112 */
