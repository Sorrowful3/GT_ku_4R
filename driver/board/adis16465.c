/*
 * adis16465.c
 *
 *  Created on: 2022年4月6日
 *      Author: Administrator
 */

#include "adis16465.h"

#define DEBUG_TAG "adis16465,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_ADIS16465

static adis16465_config_t config[] = {
#ifdef ADIS16465_0
    ADIS16465_0,
#endif
#ifdef ADIS16465_1
    ADIS16465_1,
#endif
};
static adis16465_t adis16465[sizeof(config)/sizeof(config[0])];

static int32_t _adis16465_read_reg16(adis16465_t *adis16465, uint8_t addr, uint16_t *data)
{
    ssize_t num;
    uint16_t send_buffer[4];
    uint16_t recv_buffer[4];
    ps_spi_msg_t msg[1];

    memset(send_buffer, 0, sizeof(send_buffer));
    send_buffer[0] = addr & (~0x80);
    send_buffer[1] = addr & (~0x80);
    memset(recv_buffer, 0, sizeof(recv_buffer));
    msg[0].send_buf = send_buffer;
    msg[0].recv_buf = recv_buffer;
    msg[0].cs_take = 1;
    msg[0].cs_release = 1;
    msg[0].length = sizeof(send_buffer);

    num = ps_spi_client_transfer(&adis16465->spi_client, msg, 1);
    if(num != 1) {
        return ERR_IO;
    }

    BIG16_2_LITTLE16(data, &recv_buffer[2]);

    return ERR_OK;
}

static int32_t _adis16465_write_reg16(adis16465_t *adis16465, uint8_t addr, uint16_t data)
{
    ssize_t num;
    uint16_t send_buffer[1];
    uint16_t recv_buffer[1];
    ps_spi_msg_t msg[1];

    send_buffer[0] = ((0xff & data) << 8) | (addr | 0x80);
    memset(recv_buffer, 0, sizeof(recv_buffer));
    msg[0].send_buf = send_buffer;
    msg[0].recv_buf = recv_buffer;
    msg[0].cs_take = 1;
    msg[0].cs_release = 1;
    msg[0].length = sizeof(send_buffer);
    num = ps_spi_client_transfer(&adis16465->spi_client, msg, 1);
    if(num != 1) {
        return ERR_IO;
    }

    usleep(10);

    send_buffer[0] = (0xff00 & data) | ((addr + 1) | 0x80);
    memset(recv_buffer, 0, sizeof(recv_buffer));
    msg[0].send_buf = send_buffer;
    msg[0].recv_buf = recv_buffer;
    msg[0].cs_take = 1;
    msg[0].cs_release = 1;
    msg[0].length = sizeof(send_buffer);
    num = ps_spi_client_transfer(&adis16465->spi_client, msg, 1);
    if(num != 1) {
        return ERR_IO;
    }

    return ERR_OK;
}

static int32_t _adis16465_init(adis16465_t *adis16465)
{
    uint16_t value;
    uint16_t rate;

    //硬件复位
    if(adis16465->config->rst_io != ADIS16465_IGNORE_RST) {
        if(ps_gpio_config(adis16465->config->rst_io, PS_GPIO_MODE_OUTPUT) != ERR_OK) {
            LOG_E("hardware reset adis16465 failed");
        }
        ps_gpio_write(adis16465->config->rst_io, PS_GPIO_LOW);
        vTaskDelay(5);
        ps_gpio_write(adis16465->config->rst_io, PS_GPIO_HIGH);
        vTaskDelay(500);
    }

    //软件复位
    value = 0x01 << 7;
    _adis16465_write_reg16(adis16465, GLOB_CMD, value);
    vTaskDelay(320);

    //检查复位结果
    _adis16465_read_reg16(adis16465, MSC_CTRL, &value);
    if(value != 0xc1) {
        LOG_E("software reset adis16465 failed");
        return ERR_IO;
    }

//    //使用内部时钟，使能陀螺仪的线性补偿，使能冲击点对准，数据有效时DR为高
//    value = 0xC1;
//    _adis16465_write_reg16(adis16465, MSC_CTRL, value);
//    vTaskDelay(10);

    //配置滤波器，阶数为2^0
    value = 0x00;
    _adis16465_write_reg16(adis16465, FILT_CTRL, value);
    vTaskDelay(10);

    //配置输出速率
    rate = adis16465->config->output_rate;
    if(rate > 2000) {
        rate = 2000;
    }
    if(rate == 0) {
        rate = 1;
    }
    value = 2000 / rate -1;
    _adis16465_write_reg16(adis16465, DEC_RATE, value);
    vTaskDelay(10);
    _adis16465_read_reg16(adis16465, DEC_RATE, &rate);
    if(value != rate) {
        LOG_E("set adis16465 out rate failed");
        return ERR_IO;
    }

    adis16465->info.output_rate = adis16465->config->output_rate;
    adis16465->info.acc_scale = 0.25f/1000.0f;
    adis16465->info.acc_ratio = 65536.0f;
    _adis16465_read_reg16(adis16465, RANG_MDL, &value);
    value = value >> 2;
    if(value == 0x00) {
        //ADIS16465-1
        adis16465->info.gyro_scale = 1.0f/160.0f;
    }
    else if(value == 0x01) {
        //ADIS16465-2
        adis16465->info.gyro_scale = 1.0f/40.0f;
    }
    else {
        //ADIS16465-3
        adis16465->info.gyro_scale = 1.0f/10.0f;
    }
    adis16465->info.gyro_ratio = 65536.0f;
    adis16465->info.temp_scale = 0.1f;

    //读取ID
    _adis16465_read_reg16(adis16465, PROD_ID, &value);
    if(value != 0x4051) {
        LOG_E("check adis16465 failed");
        return ERR_IO;
    }

    //Flash memory test
    value = 0x01 << 4;
    _adis16465_write_reg16(adis16465, GLOB_CMD, value);
    vTaskDelay(32);
    _adis16465_read_reg16(adis16465, DIAG_STAT, &value);
    if(value != 0x00) {
        LOG_E("Flash memory test failed");
        return ERR_IO;
    }

    //Sensor self test
    value = 0x01 << 2;
    _adis16465_write_reg16(adis16465, GLOB_CMD, value);
    vTaskDelay(15);
    _adis16465_read_reg16(adis16465, DIAG_STAT, &value);
    if(value != 0x00) {
        LOG_E("Sensor self test failed");
        return ERR_IO;
    }

    return ERR_OK;
}

static int imu_cnt;

static int32_t _adis16465_read(adis16465_t *adis16465, adis16465_original_t *original_data)
{
    uint8_t i;
    uint8_t addr;
    int32_t result;
    uint16_t buffer[13];

    for(i=0, addr=X_GYRO_LOW; i<13; i++, addr+=2) {
        result = _adis16465_read_reg16(adis16465, addr, &buffer[i]);
        if(result != ERR_OK) {
            return result;
        }
    }

    original_data->gyro_x = ((uint32_t)buffer[1] << 16) | buffer[0];
    original_data->gyro_y = ((uint32_t)buffer[3] << 16) | buffer[2];
    original_data->gyro_z = ((uint32_t)buffer[5] << 16) | buffer[4];

    original_data->acc_x = ((uint32_t)buffer[7] << 16) | buffer[6];
    original_data->acc_y = ((uint32_t)buffer[9] << 16) | buffer[8];
    original_data->acc_z = ((uint32_t)buffer[11] << 16) | buffer[10];

    if (imu_cnt > 1000) {
//    	LOG_I("ax %d,zy %d,az %d", original_data->acc_x, original_data->acc_y, original_data->acc_z);
    	imu_cnt = 0;
    }
    else {
    	imu_cnt++;
    }

    original_data->temp = buffer[12];

    return ERR_OK;
}

static ssize_t _adis16465_pread(device_t *device, void *usr_buffer, size_t size, uint32_t pos)
{
    int32_t result;
    adis16465_original_t *original_data = (adis16465_original_t*)usr_buffer;
    adis16465_t *adis16465 = container_of(device, adis16465_t, device);

    if(size < sizeof(adis16465_original_t)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //加锁
    if(xSemaphoreTakeRecursive(adis16465->lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    //读取传感器原始数据
    result = _adis16465_read(adis16465, original_data);

    //解锁
    if(xSemaphoreGiveRecursive(adis16465->lock) != pdPASS) {
        LOG_E("unlock failed");
    }

    if(result == ERR_OK){
        return sizeof(adis16465_original_t);
    }
    return result;
}

static int32_t _adis16465_control(device_t *device, uint32_t command, void *user_arg)
{
    adis16465_info_t *info = (adis16465_info_t*)user_arg;
    adis16465_t *adis16465 = container_of(device, adis16465_t, device);

    switch(command) {
    case ADIS16465_DEV_CTRL_GET_CONFIG:
        if(!info) {
            return ERR_INVAL;
        }
        *info = adis16465->info;
        break;
    default:
        return ERR_INVAL;
    }
    return ERR_OK;
}

static struct device_ops _ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = _adis16465_pread,
    .pwrite = NULL,
    .control = _adis16465_control,
};
int32_t adis16465_init(void)
{
    int32_t result;
    uint32_t i;
    uint32_t j;
    ps_spi_client_config_t spi_config;

    for(i=0; i<sizeof(config)/sizeof(config[0]); i++) {
        //初始化配置参数
        adis16465[i].config = &config[i];

        //初始化SPI
        spi_config.mode = PS_SPI_BUS_CLOCK_MODE3;
        spi_config.data_width = 8;
        spi_config.clk_hz = 1500000;
        spi_config.cs_mode = adis16465[i].config->cs_mode;
        spi_config.cs_pin = adis16465[i].config->spi_cs;
        spi_config.cs_delay = 0;
        spi_config.cs_interval = 16;
        result = ps_spi_client_init(&adis16465[i].spi_client, adis16465[i].config->spi_bus, &spi_config);
        if(result != ERR_OK) {
            LOG_E("register adis16465 failed");
            return result;
        }

        //初始化ADIS16465
        for(j=0; j<5; j++) {
            result = _adis16465_init(&adis16465[i]);
            if(result == ERR_OK) {
                break;
            }
        }
        if(result != ERR_OK) {
            LOG_E("init adis16465 failed");
            return result;
        }

        //创建互斥锁
        adis16465[i].lock= xSemaphoreCreateRecursiveMutex();
        if(!adis16465[i].lock) {
            LOG_E("create mutex lock failed");
            return ERR_NOMEM;
        }

        //注册adis16465设备
        adis16465[i].device.magic = ADIS16465_MAGIC;
        adis16465[i].device.ops = &_ops;
        result = device_register(&adis16465[i].device, adis16465[i].config->name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register adis16465 failed");
            return result;
        }
    }

    return ERR_OK;
}

#endif /* USING_ADIS16465 */
