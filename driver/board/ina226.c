/*
 * ina226.c
 *
 *  Created on: 2021年8月11日
 *      Author: Administrator
 */

#include "ina226.h"

#define DEBUG_TAG "ina226,%d", __LINE__
#define DEBUG_LEVEL WARNING_OUTPUT
#include "../core/debug.h"

#ifdef USING_INA226

static ina226_config_t ina226_config[] =
{
#ifdef INA226_0_CONFIG
    INA226_0_CONFIG,
#endif
#ifdef INA226_1_CONFIG
    INA226_1_CONFIG,
#endif
};

static ina226_t ina226[sizeof(ina226_config)/sizeof(ina226_config[0])];

static ssize_t __ina226_pread(device_t *device, void *buffer, size_t size, uint32_t pos)
{
    uint8_t reg_addr;
    uint16_t tmp;
    uint8_t recv_buffer[2];
    ina226_data_t *data = (ina226_data_t*)buffer;
    ina226_t *sensor = container_of(device, ina226_t, device);

    if(size < sizeof(ina226_data_t)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //读电压
    reg_addr = 0x02;
    if(ps_i2c_master_send(sensor->i2c, &reg_addr, 1, sensor->config->slave_addr) != 1) {
        LOG_E("write data failed");
        return ERR_IO;
    }
    memset(recv_buffer, 0, sizeof(recv_buffer));
    if(ps_i2c_master_recv(sensor->i2c, recv_buffer, 2, sensor->config->slave_addr) != 2) {
        LOG_E("read data failed");
        return ERR_IO;
    }
    tmp = (recv_buffer[0] << 8) | recv_buffer[1];
    data->voltage = tmp * 1.25f / 1000.0f;

    //采样电阻分压值
    reg_addr = 0x01;
    if(ps_i2c_master_send(sensor->i2c, &reg_addr, 1, sensor->config->slave_addr) != 1) {
        LOG_E("write data failed");
        return ERR_IO;
    }
    memset(recv_buffer, 0, sizeof(recv_buffer));
    if(ps_i2c_master_recv(sensor->i2c, recv_buffer, 2, sensor->config->slave_addr) != 2) {
        LOG_E("read data failed");
        return ERR_IO;
    }
    tmp = (recv_buffer[0] << 8) | recv_buffer[1];
    data->electricity = (tmp * 2.5f / 1000000.0f) / sensor->config->r_sampling;

    //计算功率
    data->power = data->electricity * data->voltage;

    return sizeof(ina226_data_t);
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = __ina226_pread,
    .pwrite = NULL,
    .control = NULL,
};

int32_t ina226_init(void)
{
    uint32_t i;
    int32_t result;
    uint8_t send_buffer[3];

    for(i=0; i<sizeof(ina226_config)/sizeof(ina226_config[0]); i++) {
        ina226[i].config = &ina226_config[i];

        //打开对应的I2C总线
        ina226[i].i2c = device_open(ina226[i].config->bus_name, DEVICE_RDWR);
        if(!ina226[i].i2c) {
            LOG_E("open i2c failed");
            return ERR_ROUTINE;
        }

        //配置校准寄存器
        send_buffer[0] = 0x05;
        send_buffer[1] = 0x08;
        send_buffer[2] = 0x00;
        if(ps_i2c_master_send(ina226[i].i2c, send_buffer, 3, ina226[i].config->slave_addr) != 3) {
            LOG_E("write data failed");
            return ERR_IO;
        }

        //注册ina226设备
        ina226[i].device.magic = INA226_MAGIC;
        ina226[i].device.ops = &ops;
        result = device_register(&ina226[i].device, ina226[i].config->dev_name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }
    return ERR_OK;
}

#endif /* USING_INA226 */
