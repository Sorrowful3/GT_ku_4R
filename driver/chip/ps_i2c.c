/**
 * @file    ps_i2c.c
 * @brief   实现PS端IIC模块驱动
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年4月26日
 */

#include "ps_i2c.h"

#define DEBUG_TAG "i2c,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_PS_I2C

/* iic配置参数列表 */
static ps_i2c_config_t ps_i2c_config[] =
{
#ifdef PS_I2C0_CONFIG
    PS_I2C0_CONFIG,
#endif
#ifdef PS_I2C1_CONFIG
    PS_I2C1_CONFIG,
#endif
};

/* iic设备句柄 */
static ps_i2c_t ps_i2c[sizeof(ps_i2c_config)/sizeof(ps_i2c_config[0])];

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = NULL,
    .pwrite = NULL,
    .control = NULL,
};

/**
 * @function nane: ps_i2c_init
 * @author: liaofei
 * @data: 2021年4月26日
 * @input param:
 *              无
 * @output param:
 *              return 成功返回ERR_OK，失败返回相应的错误码
 * @description:
 * 初始化PS端IIC控制器
 * @modeify log:
 * 2021年4月26日 liaofei 初次创建
 */
int32_t ps_i2c_init(void)
{
    uint32_t i;
    int32_t result;
    XIicPs_Config *config;

    /* 使能IIC控制器时钟 */
    *((uint32_t*)0xF800012C) |= 0x01<<18;
    *((uint32_t*)0xF800012C) |= 0x01<<19;

    for(i=0; i<sizeof(ps_i2c)/sizeof(ps_i2c[0]); i++) {
        ps_i2c[i].config = &ps_i2c_config[i];

        /* 创建互斥锁 */
        ps_i2c[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!ps_i2c[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }

        /* 初始化I2C控制器 */
        config = XIicPs_LookupConfig(ps_i2c[i].config->device_id);
        if (!config) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        result = XIicPs_CfgInitialize(&ps_i2c[i].i2c, config, config->BaseAddress);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return -1;
        }
        result = XIicPs_SelfTest(&ps_i2c[i].i2c);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        /* 配置IIC总线时钟 */
        result = XIicPs_SetSClk(&ps_i2c[i].i2c, ps_i2c[i].config->clk_hz);
        if (result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }

        /* 注册I2C设备 */
        ps_i2c[i].device.magic = PS_I2C_MAGIC;
        ps_i2c[i].device.ops = &ops;
        result = device_register(&ps_i2c[i].device, ps_i2c[i].config->name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }
    return ERR_OK;
}

/**
 * @function nane: ps_i2c_master_send
 * @author: liaofei
 * @data: 2021年4月26日
 * @input param:
 *              device 设备句柄，这里应该是一个iic的设备句柄
 *              buffer 欲发送的数据
 *              lenght 欲发送的字节数
 *              slave_addr 从机地址
 * @output param:
 *              return 成功返回实际发送的字节数，失败返回相应的错误码
 * @description:
 * 通过IIC总线向IIC从设备发送数据
 * @modeify log:
 * 2021年4月26日 liaofei 初次创建
 */
ssize_t ps_i2c_master_send(device_t *device, uint8_t *buffer, size_t lenght, uint16_t slave_addr)
{
    int32_t result;
    ps_i2c_t *i2c = container_of(device, ps_i2c_t, device);

    if((!buffer) || (lenght == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, PS_I2C_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(i2c->lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    /* 等待I2C空闲 */
    while(XIicPs_BusIsBusy(&i2c->i2c));
    /* 发送数据 */
    result = XIicPs_MasterSendPolled(&i2c->i2c, buffer, lenght, slave_addr);

    /* 解锁 */
    if(xSemaphoreGiveRecursive(i2c->lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    if(result != XST_SUCCESS) {
  //    lutao modifiy 2022-- 4R is error
//       LOG_E("master send failed");
        return ERR_IO;
    }

    return lenght;
}

/**
 * @function nane: ps_i2c_master_recv
 * @author: liaofei
 * @data: 2021年4月26日
 * @input param:
 *              device 设备句柄，这里应该是一个iic的设备句柄
 *              buffer接收数据的缓存
 *              lenght 欲接收的字节数
 *              slave_addr 从机地址
 * @output param:
 *              return 成功返回实际读取的字节数，失败返回相应的错误码
 * @description:
 * 通过IIC总线从IIC从设备读取数据
 * @modeify log:
 * 2021年4月26日 liaofei 初次创建
 */
ssize_t ps_i2c_master_recv(device_t *device, uint8_t *buffer, size_t lenght, uint16_t slave_addr)
{
    int32_t status;
    ps_i2c_t *i2c = container_of(device, ps_i2c_t, device);

    if((!buffer) || (lenght == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(device_check(device, PS_I2C_MAGIC) != ERR_OK) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    /* 加锁 */
    if(xSemaphoreTakeRecursive(i2c->lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    /* 等待I2C空闲 */
    while(XIicPs_BusIsBusy(&i2c->i2c));
    /* 读取从机数据 */
    status = XIicPs_MasterRecvPolled(&i2c->i2c, buffer, lenght, slave_addr);

    /* 解锁 */
    if(xSemaphoreGiveRecursive(i2c->lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    if(status != XST_SUCCESS) {
    	 //    lutao modifiy 2022-- 4R is error
//        LOG_E("master recv failed");
        return ERR_IO;
    }

    return lenght;
}

#endif /* USING_PS_I2C */
