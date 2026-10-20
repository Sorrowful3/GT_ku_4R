/*
 * ps_spi.c
 *
 *  Created on: 2022年4月2日
 *      Author: Administrator
 */

#include "ps_spi.h"

#define DEBUG_TAG "ps spi,%d", __LINE__
#define DEBUG_LEVEL DEBUG_OUTPUT
#include "../core/debug.h"

#ifdef USING_PS_SPI

#define spi_send_byte(BaseAddress, Data) XSpiPs_Out32((BaseAddress) + (u32)XSPIPS_TXD_OFFSET, (u32)(Data))

#define spi_recv_byte(BaseAddress) XSpiPs_In32((u32)((BaseAddress) + (u32)XSPIPS_RXD_OFFSET))

typedef ps_spi_client_config_t ps_spi_config_t;

/* spi配置参数列表 */
static ps_spi_bus_config_t spi_config[] =
{
#ifdef PS_SPI0_CONFIG
    PS_SPI0_CONFIG,
#endif
#ifdef PS_SPI1_CONFIG
    PS_SPI1_CONFIG,
#endif
};

/* spi设备句柄 */
static ps_spi_bus_t spi_bus[sizeof(spi_config)/sizeof(spi_config[0])];

static void _ps_spi_handler(void *arg)
{
    BaseType_t task_woken;
    ps_spi_bus_t *spi_bus = (ps_spi_bus_t*)arg;
    XSpiPs *spi_instance = &spi_bus->spi_instance;

    /* 验证形参，判断是否是一个SPI句柄 */
    if((!spi_bus) || (device_check(&spi_bus->device, PS_SPI_MAGIC) != ERR_OK) || (!spi_instance)) {
        return;
    }

    //获取中断状态
    spi_bus->intr_state = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_SR_OFFSET);
    //清除中断标志
    XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_SR_OFFSET, (spi_bus->intr_state & XSPIPS_IXR_WR_TO_CLR_MASK));
    //暂时关闭中断
    XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_IDR_OFFSET, XSPIPS_IXR_DFLT_MASK);

    /* 产生同步信号 */
    task_woken = pdFALSE;
    xSemaphoreGiveFromISR(spi_bus->sync, &task_woken);
    /* 进行一次中断级任务调度 */
    portYIELD_FROM_ISR(task_woken);
}

static uint32_t _2n(uint32_t x)
{
    uint32_t i;
    uint32_t temp;

    if(x == 0) {
        return 1;
    }
    for(temp=2, i=1; i<x; i++) {
        temp *= 2;
    }

    return temp;
}

static int32_t _get_prescaler(ps_spi_bus_t *spi_bus, uint32_t *prescaler, uint32_t clock)
{
    uint32_t source_clock;
    const uint32_t table[] = {XSPIPS_CLK_PRESCALE_4, XSPIPS_CLK_PRESCALE_8, XSPIPS_CLK_PRESCALE_16,
            XSPIPS_CLK_PRESCALE_32, XSPIPS_CLK_PRESCALE_64, XSPIPS_CLK_PRESCALE_128,
            XSPIPS_CLK_PRESCALE_256};

    source_clock = spi_bus->bus_config->clk_hz;
    for(*prescaler=0; *prescaler<sizeof(table)/sizeof(table[0]); (*prescaler)++) {
        if((source_clock / _2n(*prescaler+2)) <= clock) {
            *prescaler = table[*prescaler];
            break;
        }
    }
    if((*prescaler == 0) && ((source_clock/_2n(*prescaler+1))>clock)) {
        *prescaler = XSPIPS_CLK_PRESCALE_256;
    }

    return ERR_OK;
}

static int32_t _spi_bus_config(ps_spi_bus_t *spi_bus, ps_spi_config_t *config)
{
    uint32_t options;
    uint32_t prescaler;
    uint32_t reg_config;
    XSpiPs *spi_instance = &spi_bus->spi_instance;

    if(config->data_width != 8) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //手动启动，主模式，手动片选
    options = XSPIPS_MANUAL_START_OPTION | XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION;
    if(config->mode & PS_SPI_BUS_POLARITY_HIGH) {
        //时钟空闲为高电平
        options |= XSPIPS_CLK_ACTIVE_LOW_OPTION;
    }
    if(config->mode & PS_SPI_BUS_PHASE_2EDGE) {
        //第二个边沿有效
        options |= XSPIPS_CLK_PHASE_1_OPTION;
    }
    XSpiPs_SetOptions(&spi_bus->spi_instance, options);

    //配置时钟
    _get_prescaler(spi_bus, &prescaler, config->clk_hz);
    XSpiPs_SetClkPrescaler(&spi_bus->spi_instance, prescaler);

    //配置硬件片选
    if(config->cs_mode & PS_SPI_BUS_CONTROLLER_CS) {
        XSpiPs_SetSlaveSelect(&spi_bus->spi_instance, config->cs_pin);
        //释放片选
        reg_config = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET);
        reg_config |= XSPIPS_CR_SSCTRL_MASK;
        XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET, reg_config);
        usleep(20);
    }

    return ERR_OK;
}

static int32_t _spi_bus_xfer_msg(ps_spi_bus_t *spi_bus, ps_spi_msg_t *msg)
{
    uint8_t temp;
    uint32_t reg_config;
    uint32_t transfer_count;
    size_t remaining_bytes;
    const uint8_t *send_buffer = (const uint8_t *)msg->send_buf;
    uint8_t *recv_buffer = (uint8_t *)msg->recv_buf;
    XSpiPs *spi_instance = &spi_bus->spi_instance;

    if(msg->length == 0) {
        return ERR_OK;
    }
    if(((!msg->recv_buf) && (!msg->send_buf))) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //复位信号量
    xSemaphoreTake(spi_bus->sync, 0);
    //初始剩余字节数
    remaining_bytes = msg->length;
    //清除中断标志
    XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_SR_OFFSET, XSPIPS_IXR_WR_TO_CLR_MASK);

    for(; remaining_bytes>0; ) {
        //复位此次传输的字节数
        transfer_count = 0;

        //将数据写入发送FIFO
        for(; (remaining_bytes > 0U) && (transfer_count < XSPIPS_FIFO_DEPTH); ) {
            spi_send_byte(spi_instance->Config.BaseAddress, *send_buffer);
            send_buffer++;
            remaining_bytes--;
            transfer_count++;
        }

        //使能中断
        XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_IER_OFFSET, XSPIPS_IXR_DFLT_MASK);

        //启动传输
        reg_config = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET);
        reg_config |= XSPIPS_CR_MANSTRT_MASK;
        XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET, reg_config);

        //等待中断
        if(xSemaphoreTake(spi_bus->sync, spi_bus->device.receive_timeout) != pdPASS) {
            LOG_E("take cplt failed");
            return ERR_IO;
        }

        if(spi_bus->intr_state & XSPIPS_IXR_MODF_MASK) {
            //模式错误
            XSpiPs_Abort(spi_instance);
            LOG_E("Mode Fault");
            return ERR_IO;
        }
        if(spi_bus->intr_state & XSPIPS_IXR_RXOVR_MASK) {
            //接收上溢
            XSpiPs_Abort(spi_instance);
            LOG_E("Rx FIFO Overrun");
            return ERR_IO;
        }
        if(spi_bus->intr_state & XSPIPS_IXR_TXUF_MASK){
            //发送上溢
            XSpiPs_Abort(spi_instance);
            LOG_E("Tx FIFO Underflow");
            return ERR_IO;
        }
        //发送FIFO低于阈值，阈值默认为1，所以这里认为发送完成
        if(spi_bus->intr_state & XSPIPS_IXR_TXOW_MASK) {
            //从接收FIFO读数据
            for(; transfer_count>0;) {
                temp = (uint8_t)spi_recv_byte(spi_instance->Config.BaseAddress);
                if(recv_buffer) {
                    *recv_buffer = temp;
                    recv_buffer++;
                }
                transfer_count--;
            }
        }
    }

    return ERR_OK;
}

static ssize_t _spi_bus_xfer(ps_spi_bus_t *spi_bus, ps_spi_msg_t msgs[], size_t num)
{
    int32_t result;
    uint32_t reg_config;
    uint32_t not_empty;
    size_t i;
    uint32_t cs_interval = spi_bus->owner->client_config.cs_interval;
    uint32_t cs_delay = spi_bus->owner->client_config.cs_delay;
    uint32_t cs_pin = spi_bus->owner->client_config.cs_pin;
    uint32_t no_cs = spi_bus->owner->client_config.mode & PS_SPI_BUS_NO_CS;
    uint32_t cs_pin_mode = spi_bus->owner->client_config.cs_mode & PS_SPI_BUS_GPIO_CS;
    XSpiPs *spi_instance = &spi_bus->spi_instance;

    /* 加锁 */
    if(xSemaphoreTakeRecursive(spi_bus->lock, portMAX_DELAY) != pdTRUE ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    //清空接收FIFO
    not_empty = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_SR_OFFSET) & XSPIPS_IXR_RXNEMPTY_MASK;
    for(; not_empty; ) {
        if(spi_recv_byte(spi_instance->Config.BaseAddress)) {
            ;
        }
        not_empty = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_SR_OFFSET) & XSPIPS_IXR_RXNEMPTY_MASK;
    }

    //使能控制器
    XSpiPs_Enable(spi_instance);

    for(i=0; i<num; i++) {
        //获取片选
        if(!(no_cs) && (msgs[i].cs_take)) {
            if(cs_pin_mode) {
                //将硬件片选设置为无效
                reg_config = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET);
                reg_config |= XSPIPS_CR_SSCTRL_MASK;
                XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET, reg_config);
                //设置软件片选
                ps_gpio_write(cs_pin,PS_GPIO_LOW);
            }
            else {
                reg_config = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET);
                reg_config &= (u32)(~XSPIPS_CR_SSCTRL_MASK);
                reg_config |= spi_instance->SlaveSelect;
                XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET, reg_config);
            }
            //片选延时
            if(cs_delay > 0) {
                usleep(cs_delay);
            }
        }

        //传输一个msg
        result = _spi_bus_xfer_msg(spi_bus, &msgs[i]);

        //释放片选
        if(!(no_cs) && (msgs[i].cs_release)) {
            //片选延时
            if(cs_delay > 0) {
                usleep(cs_delay);
            }
            if(cs_pin_mode) {
                ps_gpio_write(cs_pin, PS_GPIO_HIGH);
            }
            else {
                reg_config = XSpiPs_ReadReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET);
                reg_config |= XSPIPS_CR_SSCTRL_MASK;
                XSpiPs_WriteReg(spi_instance->Config.BaseAddress, XSPIPS_CR_OFFSET, reg_config);
            }
            //片选最小间隔
            if(cs_interval > 0) {
                usleep(cs_interval);
            }
        }

        //检查传输结果
        if(result != ERR_OK) {
            break;
        }
    }

    //禁止控制器
    XSpiPs_Disable(spi_instance);

    /* 解锁 */
    if(xSemaphoreGiveRecursive(spi_bus->lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    return i;
}

int32_t ps_spi_client_init(ps_spi_client_t *client, const char *bus_name, ps_spi_client_config_t *config)
{
    int32_t result;

    if((!client) || (!bus_name) || (!config)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if((!(config->mode & PS_SPI_BUS_NO_CS)) && (config->cs_mode & PS_SPI_BUS_GPIO_CS)) {
        //配置片选引脚为输出
        result = ps_gpio_config(config->cs_pin, PS_GPIO_MODE_OUTPUT);
        if(result != ERR_OK) {
            LOG_E("cs pin config failed");
            return result;
        }
        //设置片选为无效
        ps_gpio_write(config->cs_pin, PS_GPIO_HIGH);
    }

    client->bus = ps_spi_bus_find(bus_name);
    if(!client->bus) {
        LOG_E("find spi bus failed");
        return ERR_EMPTY;
    }
    client->client_config = *config;

    return ERR_OK;
}

int32_t ps_spi_client_config(ps_spi_client_t *client, ps_spi_client_config_t *config)
{
    int32_t result;
    ps_spi_bus_t *spi_bus;

    if((!client) || (!config)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    spi_bus = client->bus;
    if(!spi_bus) {
        LOG_E("no bus bound");
        return ERR_IO;
    }

    //存储配置数据
    client->client_config.mode = config->mode;
    client->client_config.data_width = config->data_width;
    client->client_config.clk_hz = config->clk_hz;

    //加锁
    if(xSemaphoreTakeRecursive(spi_bus->lock, portMAX_DELAY) != pdPASS ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    //配置spi总线
    if(spi_bus->owner == client) {
        //配置SPI总线
        result = _spi_bus_config(spi_bus, &client->client_config);
        if(result != ERR_OK) {
            xSemaphoreGiveRecursive(spi_bus->lock);
            LOG_E("configer bud failed");
            return result;
        }
    }

    //解锁
    if(xSemaphoreGiveRecursive(spi_bus->lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    return ERR_OK;

}

ssize_t ps_spi_client_transfer(ps_spi_client_t *client, ps_spi_msg_t msgs[], size_t num)
{
    int32_t result;
    ssize_t lenght;
    ps_spi_bus_t *spi_bus;

    if((!client) || (!msgs) || (num == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    spi_bus = client->bus;
    if(!spi_bus) {
        LOG_E("no bus bound");
        return ERR_IO;
    }

    //加锁
    if(xSemaphoreTakeRecursive(spi_bus->lock, portMAX_DELAY) != pdPASS ) {
        LOG_E("lock failed");
        return ERR_ROUTINE;
    }

    //配置总线
    if(spi_bus->owner != client) {
        //配置SPI总线
        result = _spi_bus_config(spi_bus, &client->client_config);
        if(result != ERR_OK) {
            xSemaphoreGiveRecursive(spi_bus->lock);
            LOG_E("configer bud failed");
            return result;
        }
        spi_bus->owner = client;
    }

    //传输消息包
    lenght = _spi_bus_xfer(spi_bus, msgs, num);

    //解锁
    if(xSemaphoreGiveRecursive(spi_bus->lock) != pdPASS) {
        LOG_E("unlock failed");
        return ERR_ROUTINE;
    }

    return lenght;
}

ssize_t ps_spi_bus_transfer(ps_spi_bus_t *bus, ps_spi_msg_t msgs[], size_t num)
{
    if((!bus) || (!msgs) || (num == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }
    if(!bus->owner) {
        LOG_E("no client is bound");
        return ERR_IO;
    }

    return ps_spi_client_transfer(spi_bus->owner, msgs, num);
}

ps_spi_bus_t *ps_spi_bus_find(const char *name)
{
    device_t *spi;

    spi = device_find(name);
    if(!spi) {
        return NULL;
    }
    return container_of(spi, ps_spi_bus_t, device);
}

static struct device_ops ops =
{
    .init = NULL,
    .open = NULL,
    .release = NULL,
    .pread = NULL,
    .pwrite = NULL,
    .control = NULL,
};
int32_t ps_spi_bus_init(void)
{
    uint32_t i;
    int32_t result;
    XSpiPs_Config *SpiConfig;

    /* 使能SPI控制器时钟 */
    *((uint32_t*)0xF800012C) |= (0x01 << 15) | (0x01 << 14);
    *((uint32_t*)0xF8000158) = 0x00000603U;

    for(i=0; i<sizeof(spi_bus)/sizeof(spi_bus[0]); i++) {
        spi_bus[i].bus_config = &spi_config[i];
        spi_bus[i].owner = NULL;

        /* 创建同步信号量 */
        spi_bus[i].sync = xSemaphoreCreateBinary();
        if(!spi_bus[i].sync) {
            LOG_E("create sync failed");
            return ERR_NOMEM;
        }
        /* 创建互斥量 */
        spi_bus[i].lock = xSemaphoreCreateRecursiveMutex();
        if(!spi_bus[i].lock) {
            LOG_E("create lock failed");
            return ERR_NOMEM;
        }

        //初始化SPI控制器
        SpiConfig = XSpiPs_LookupConfig(spi_bus[i].bus_config->device_id);
        if(!SpiConfig) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        result = XSpiPs_CfgInitialize(&spi_bus[i].spi_instance, SpiConfig, SpiConfig->BaseAddress);
        if(result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }
        result = XSpiPs_SelfTest(&spi_bus[i].spi_instance);
        if(result != XST_SUCCESS) {
            LOG_E("initialize failed");
            return ERR_IO;
        }

        /* 注册GIC中断回调函数 */
        xPortInstallInterruptHandler(spi_bus[i].bus_config->intr_id, (Xil_ExceptionHandler)_ps_spi_handler, (void *)&spi_bus[i]);
        /* 将GIC中断映射到CPU1 */
        intr_map_to_cpu1(spi_bus[i].bus_config->intr_id);
        /* 使能GIC的中断号 */
        vPortEnableInterrupt(spi_bus[i].bus_config->intr_id);
//        //注册SPI中断回调函数
//        XSpiPs_SetStatusHandler(&spi_bus[i].spi_instance, (void *)&spi_bus[i], _ps_spi_handler);

        /* 注册SPI设备 */
        spi_bus[i].device.magic = PS_SPI_MAGIC;
        spi_bus[i].device.ops = &ops;
        result = device_register(&spi_bus[i].device, spi_bus[i].bus_config->name, DEVICE_RDWR);
        if(result != ERR_OK) {
            LOG_E("register failed");
            return result;
        }
    }

    return ERR_OK;
}

#endif /* USING_PS_SPI */

