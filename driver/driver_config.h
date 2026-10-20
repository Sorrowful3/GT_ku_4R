/*
 * driver_config.h
 *
 *  Created on: 2023年9月1日
 *      Author: LIJIWEN
 */

#ifndef __DRIVER_CONFIG_H_
#define __DRIVER_CONFIG_H_

//#define USING_CORE0
#define USING_CORE1

#ifdef USING_CORE0
#define START_CORE1
#define CORE1_TEXT_ADDR 0x3e000000
#endif

//最长设备名称
#define MAX_NAME_LENGHT 32

#define USING_PS_GPIO

//版本
#define KU_DOWNCONV_CONT_LO_GAIN_CONTROLLABLE       1   // 1: Ku下变频-连续本振-增益可控
#define KA_DOWNCONV_CONT_LO_GAIN_CONTROLLABLE       2  	// 2: Ka下变频-连续本振-增益可控
#define KU_UPDOWNCONV_FIXED_LO_GAIN_CONTROLLABLE    3 	// 3: Ku上下一体变频-固定本振-增益可控
#define KA_UPDOWNCONV_FIXED_LO_GAIN_CONTROLLABLE    4 	// 4: Ka上下一体变频-固定本振-增益可控
#define KU_DOWNCONV_FIXED_LO_GAIN_CONTROLLABLE      5 // 5: Ku下变频-固定本振-增益可控
#define KA_DOWNCONV_FIXED_LO_GAIN_CONTROLLABLE    	6 //: Ka下变频-固定本振-增益可控
#define KU_DOWNCONV_CONT_LO_GAIN_UNCONTROLLABLE     7 // 7: Ku下变频-连续本振-增益不可控
#define KU_DOWNCONV_FIXED_LO_GAIN_UNCONTROLLABLE    8 // 8: Ku下变频-固定本振-增益不可控
#define KU_UPDOWNCONV_FIXED_LO_GAIN_UNCONTROLLABLE  9 // 9: Ku上下一体变频-固定本振-增益不可控
#define KA_UPDOWNCONV_FIXED_LO_GAIN_UNCONTROLLABLE  10// 10: Ka上下一体变频-固定本振-增益不可控
#define KU_UPDOWNCONV_CONT_LO_GAIN_CONTROLLABLE     11  // 11: Ku上下一体变频-连续本振-增益可控
#define KA_UPDOWNCONV_CONT_LO_GAIN_CONTROLLABLE     12  // 12: Ka上下一体变频-连续本振-增益可控
#define KU_UPDOWNCONV_CONT_LO_GAIN_UNCONTROLLABLE   13  // 13: Ku上下一体变频-连续本振-增益不可控
#define KA_UPDOWNCONV_CONT_LO_GAIN_UNCONTROLLABLE  	14 	// 14: Ka上下一体变频-连续本振-增益不可控


// 定义天线枚举类型
#define  GPS_DUAL_6222       	1      // 1: 6222双GPS天线
#define   GPS_SINGLE_6222    	2     // 2: 6222单GPS天线
#define   XIAN_INERTIAL      	3    // 3: 西安惯导
#define  GPS_CLOSED_LOOP_6222  	4   // 4: 6222闭环
#define  GPS_DUAL_6222_FIXED    5  // 5: 6222双GPS天线-固定站

// 定义天线枚举类型
#define   imu_6222_ML_STATIC      	1      // 静
#define   imu_6222_ML_ACTIVE    	2     // 动


#define _Inc_  FY
#define _AT_   1//侦收天线
#define _Pl_ 	1 //4-768R
#define _PS_  1 //单波束
#define _ML_  imu_6222_ML_ACTIVE
#define _Tr_   KU_DOWNCONV_CONT_LO_GAIN_CONTROLLABLE
#define _IMU_   GPS_DUAL_6222
#define _AU_   1
#define _VER_  2.0.1

#define Inc  Inc
#define AT   AT
#define Pl 	Pl
#define PS  PS
#define ML  ML
#define Tr   Tr
#define IMU  IMU
#define AU   AU
#define VER   V

// 辅助宏，用于将宏名转换为字符串
#define STRINGIFY(x) #x
// 将宏名转换为字符串的宏
#define STR(x) #x

// 主宏，用于组合 key 和 value
//#define CONCAT_KEY_VALUE(key, value) STRINGIFY(key) ":" STR(value)
#define CONCAT_KEY_VALUE(key, value) STR(value)

// 最后，定义一个宏来组合所有的键值对
#define ALL_KEYS_VALUES CONCAT_KEY_VALUE(Inc, _Inc_) "-"\
						CONCAT_KEY_VALUE(AT, _AT_)"-"\
						CONCAT_KEY_VALUE(Pl, _Pl_)"-"\
						CONCAT_KEY_VALUE(PS, _PS_)"-"\
						CONCAT_KEY_VALUE(ML, _ML_)"-"\
						CONCAT_KEY_VALUE(Tr, _Tr_)"-"\
						CONCAT_KEY_VALUE(IMU, _IMU_)"-"\
						CONCAT_KEY_VALUE(AU, _AU_)"-"\
						CONCAT_KEY_VALUE(VER, _VER_)
//版本号
#define FIRWARE_VERSION  ALL_KEYS_VALUES

//#define USING_AXI_GPIO
#define USING_AXI_GPIO_INTERRUPT
#define CHANNEL1_DEFAULF_OUTPUT     0X00000140
#define CHANNEL2_DEFAULF_OUTPUT     0X00
#define AXI_GPIO_DEVICE_ID          XPAR_GPIO_0_DEVICE_ID
#define AXI_GPIO_INTERRUPT_ID       XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR

#define USING_PS_OCM
#define PS_OCM_MAGIC            0x01
#define PS_OCM_DEVICE_NAME      "ps_ocm"
#define SOFT_IRQ_CORE1_TO_CORE0 14
#define SOFT_IRQ_CORE0_TO_CORE1 15
#define PS_OCM_MSG_SIZE         1024
#define PS_OCM_BASE_ADDRESS     0xFFFF0000
#define PS_OCM_TX_FIFO_SIZE     8192
#define PS_OCM_RX_FIFO_SIZE     8192

#define USING_AXI_DMA
#define AXI_DMA_MAGIC   0x02
#define DMA_ALIGN 512
#define RX_BUFFER_SIZE  DEVICE_ALIGN(8192, DMA_ALIGN)
#define TX_BUFFER_SIZE  DEVICE_ALIGN(8192, DMA_ALIGN)
#define AXI_DMA0_CONFIG                                         \
{                                                               \
    .device_name = "axi_dma0",                                  \
    .device_id = XPAR_AXIDMA_0_DEVICE_ID,                       \
    .rx_intr_id = XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,      \
    .tx_intr_id = XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR,      \
    .rx_fifo_size = (size_t)1024*8,                             \
}

#define USING_AXI_UARTLITE
#define AXI_UARTLITE_MAGIC  0x03
#define AXI_UARTLITE0_CONFIG                    \
{                                               \
    .device_name = "axi_uart0",                 \
    .device_id = XPAR_UARTLITE_0_DEVICE_ID,     \
    .intr_id = XPAR_FABRIC_UARTLITE_0_VEC_ID,   \
    .rx_fifo_size = (size_t)2048,               \
    .tx_fifo_size = (size_t)2048,               \
}
#define AXI_UARTLITE1_CONFIG                    \
{                                               \
    .device_name = "axi_uart1",                 \
    .device_id = XPAR_UARTLITE_1_DEVICE_ID,     \
    .intr_id = XPAR_FABRIC_UARTLITE_1_VEC_ID,   \
    .rx_fifo_size = (size_t)512,                \
    .tx_fifo_size = (size_t)512,                \
}
#define AXI_UARTLITE2_CONFIG                   \
{                                               \
    .device_name = "axi_uart2",                 \
    .device_id = XPAR_UARTLITE_2_DEVICE_ID,     \
    .intr_id = XPAR_FABRIC_UARTLITE_2_VEC_ID,   \
    .rx_fifo_size = (size_t)512,                \
    .tx_fifo_size = (size_t)512,                \
}
//#define AXI_UARTLITE3_CONFIG                    \
//{                                               \
//    .device_name = "axi_uart3",                 \
//    .device_id = XPAR_UARTLITE_3_DEVICE_ID,     \
//    .intr_id = XPAR_FABRIC_UARTLITE_3_VEC_ID,   \
//    .rx_fifo_size = (size_t)512,                \
//    .tx_fifo_size = (size_t)512,                \
//}
//
//#define AXI_UARTLITE4_CONFIG                    \
//{                                               \
//    .device_name = "axi_uart4",                 \
//    .device_id = XPAR_UARTLITE_4_DEVICE_ID,     \
//    .intr_id = XPAR_FABRIC_UARTLITE_4_VEC_ID,   \
//    .rx_fifo_size = (size_t)512,                \
//    .tx_fifo_size = (size_t)512,                \
//}

//#define AXI_UARTLITE5_CONFIG                    \
//{                                               \
//    .device_name = "axi_uart5",                 \
//    .device_id = XPAR_UARTLITE_5_DEVICE_ID,     \
//    .intr_id = XPAR_FABRIC_UARTLITE_5_VEC_ID,   \
//    .rx_fifo_size = (size_t)512,                \
//    .tx_fifo_size = (size_t)512,                \
//}

#define USING_AXI_UART550
#define AXI_UART550_MAGIC  0x04
#define AXI_UART5500_CONFIG                     \
{                                               \
    .device_name = "axi_uart3",                 \
                                                \
    .device_id = XPAR_UARTNS550_0_DEVICE_ID,    \
    .intr_id = XPAR_FABRIC_UARTNS550_0_VEC_ID,  \
                                                \
    .baud_rate = 460800,                        \
    .data_bits = AXI_UART550_DATA_BITS_8,       \
    .parity = AXI_UART550_PARITY_NONE,          \
    .stop_bits = AXI_UART550_STOP_BITS_1,       \
                                                \
    .rx_fifo_size = (size_t)1024,               \
    .tx_fifo_size = (size_t)1024,               \
}

#define AXI_UART5501_CONFIG                     \
{                                               \
    .device_name = "axi_uart4",                 \
                                                \
    .device_id = XPAR_UARTNS550_1_DEVICE_ID,    \
    .intr_id = XPAR_FABRIC_UARTNS550_1_VEC_ID,  \
                                                \
    .baud_rate = 460800,                        \
    .data_bits = AXI_UART550_DATA_BITS_8,       \
    .parity = AXI_UART550_PARITY_NONE,          \
    .stop_bits = AXI_UART550_STOP_BITS_1,       \
                                                \
    .rx_fifo_size = (size_t)1024,               \
    .tx_fifo_size = (size_t)1024,               \
}

#define USING_PS_I2C
#define PS_I2C_MAGIC  0x05
#define PS_I2C0_CONFIG                      \
{                                           \
    .name = "ps_i2c0",                      \
    .device_id = XPAR_XIICPS_0_DEVICE_ID,   \
    .clk_hz = 400000,                       \
}
#define PS_I2C1_CONFIG                      \
{                                           \
    .name = "ps_i2c1",                      \
    .device_id = XPAR_XIICPS_1_DEVICE_ID,   \
    .clk_hz = 400000,                       \
}

#define USING_PS_SPI
#define PS_SPI_MAGIC  0x06
#define PS_SPI0_CONFIG                              \
{                                                   \
    .name = "ps_spi0",                              \
    .device_id = XPAR_XSPIPS_0_DEVICE_ID,           \
    .intr_id = XPAR_XSPIPS_0_INTR,                  \
    .clk_hz = (uint32_t)(166.666672f*1000*1000),    \
}

#define USING_AXI_PWM
#define DEFAULT_PWM_PERIOD  0x10000

#define USING_AXI_CAPTURE
#define DEFAULT_MAX_CAPTURE  0x2FAF080

#define USING_PS_WDT
#define PS_WDT_LOAD_VALUE   0xFFFF0000


#define USING_POWER
#define POWER_INFO                                      \
{   /*电源网络0，PS GPIO0控制，高电平开 */                \
    /*电源网络1，PS GPIO1控制，高电平开 */                \
    {0, PS_GPIO_CTRL, 54+0, HIGH_LEVEL_VALID},          \
    {1, PS_GPIO_CTRL, 54+1, LOW_LEVEL_VALID},          \
    {2, PS_GPIO_CTRL, 54+2, HIGH_LEVEL_VALID},          \
    {3, PS_GPIO_CTRL, 54+3, HIGH_LEVEL_VALID},         \
    {4, PS_GPIO_CTRL, 54+4, HIGH_LEVEL_VALID},         \
    {5, PS_GPIO_CTRL, 54+5, HIGH_LEVEL_VALID},         \
    {6, PS_GPIO_CTRL, 54+6, HIGH_LEVEL_VALID},         \
    {7, PS_GPIO_CTRL, 54+7, HIGH_LEVEL_VALID},         \
    {8, PS_GPIO_CTRL, 54+8, LOW_LEVEL_VALID},         \
    {9, PS_GPIO_CTRL, 54+9, LOW_LEVEL_VALID},         \
    {10, PS_GPIO_CTRL, 54+10, LOW_LEVEL_VALID},        \
    {11, PS_GPIO_CTRL, 54+11, LOW_LEVEL_VALID},        \
    {12, PS_GPIO_CTRL, 54+12, HIGH_LEVEL_VALID},        \
    {13, PS_GPIO_CTRL, 54+13, LOW_LEVEL_VALID},         \
    {14, PS_GPIO_CTRL, 54+14, HIGH_LEVEL_VALID},         \
    {15, PS_GPIO_CTRL, 54+15, HIGH_LEVEL_VALID},         \
    {16, PS_GPIO_CTRL, 54+16, HIGH_LEVEL_VALID},        \
    {17, PS_GPIO_CTRL, 54+17, HIGH_LEVEL_VALID},        \
    {18, PS_GPIO_CTRL, 54+18, HIGH_LEVEL_VALID},        \
	{19, PS_GPIO_CTRL, 54+19, HIGH_LEVEL_VALID},        \
	{20, PS_GPIO_CTRL, 54+20, LOW_LEVEL_VALID},        \
	{21, PS_GPIO_CTRL, 54+21, LOW_LEVEL_VALID},        \
	{22, PS_GPIO_CTRL, 54+22, LOW_LEVEL_VALID},        \
	{23, PS_GPIO_CTRL, 54+23, LOW_LEVEL_VALID},        \
	{24, PS_GPIO_CTRL, 54+24, LOW_LEVEL_VALID},        \
}
//电源不需要控制
#define IGNORE_POWER        0xffffffff
//电源网络编号

//#define R0_POWER            2
//#define R1_POWER            3
//#define R2_POWER            0
//#define R3_POWER            1

#define R0_POWER            15
#define R1_POWER            16
#define R2_POWER            17
#define R3_POWER            18

#define FAN_SYS_POWER       IGNORE_POWER
#define FAN_12V_POWER      	1
#define VFD_12V_POWER       2
#define VFD_5V5_POWER       5
#define RAV260_POWER        6
#define DMJ_24V_POWER       0
#define MODEM_24V_POWER     7
#define OTHER_3V3_POWER     8
#define ROUTER_3V3_POWER    9
#define PA_12V_POWER        10
#define LED_RED_POWER       23
#define LED_GRE_POWER       24
#define NOE_M8_POWER        3
#define DDC_10MHZ_BUFF      14
#define PWR_EN_CLK10M       12

//初始化完成后需要默认打开的电源网络
#define POWER_ON_LIST   \
{                       \
    PA_12V_POWER,       \
    MODEM_24V_POWER,    \
    OTHER_3V3_POWER,    \
    ROUTER_3V3_POWER,   \
	LED_RED_POWER,      \
	LED_GRE_POWER,      \
	DDC_10MHZ_BUFF,     \
	PWR_EN_CLK10M,      \
}

//	R0_POWER,\
//	R1_POWER,\
//	R2_POWER,\
//	R3_POWER,\
//#define USING_RAV260
//#define RAV260_TIMEOUT  6
//#define RAV260_MAGIC    0x06
//#define RAV260_0_CONFIG             \
//{                                   \
//    .uart_name = "axi_uart3",       \
//    .dev_name = "IMU0",             \
//    .versions = 1,                  \
//    .power = RAV260_POWER,          \
//}



//#define USING_ANTENNA_V0
#define ANTENNA_V0_TIMEOUT     200
#define ANTENNA_V0_PORT_DEVICE "axi_dma0"
#define ANTENNA_V0_USE_CONVENIENCE_FUNCTIONS
//#define ANTENNA_V0_TX_ROTATE    90.0f

#define USING_ANTENNA_V1
#define ANTENNA_V1_TIMEOUT     20
#define ANTENNA_V1_PORT_DEVICE "axi_dma0"
#define ANTENNA_V1_USE_CONVENIENCE_FUNCTIONS
//#define ANTENNA_V1_TX_ROTATE    180.0f

#define USING_ANTENNA_HAL
#define ANTENNA_VERSION_V0      0X00
#define ANTENNA_VERSION_V1      0X01
/* 阵面版本 */
#define ANTENNA_RX_VERSION      ANTENNA_VERSION_V1
#define ANTENNA_TX_VERSION      ANTENNA_VERSION_V1
/* 阵面数量 */
//接收阵面的数量
#define ANTENNA_RX_TOTAL       4
//发射阵面的数量
#define ANTENNA_TX_TOTAL        0
/* 阵面电源控制定义 */
//接收阵面直流供电
#define ANTENNA_R0_VCC          R0_POWER
#define ANTENNA_R1_VCC          R1_POWER
#define ANTENNA_R2_VCC          R2_POWER
#define ANTENNA_R3_VCC          R3_POWER
//发射阵面直流供电
#define ANTENNA_T0_VCC          T0_POWER
#define ANTENNA_T1_VCC          T1_POWER
#define ANTENNA_T2_VCC          T2_POWER
#define ANTENNA_T3_VCC          T3_POWER

//西安惯导
#define XIAN_IMU_PARES

//北斗星通
//#define ANTENNA_IMU_SCAN    1

#define USING_TMP112
#define TMP112_MAGIC    0x09
#define TMP112_0_CONFIG                 \
{                                       \
    .bus_name = "ps_i2c0",              \
    .dev_name = "tmp112_0",             \
    .slave_addr = 0x71,                 \
}

//#define USING_INA220
//#define INA220_MAGIC    0x0A
//#define INA220_1_CONFIG                 \
//{                                       \
//    .bus_name = "ps_i2c1",              \
//    .dev_name = "ina220_1",             \
//    .slave_addr = 0x41,                 \
//    .r_sampling = 0.5f/1000.0f,         \
//}
//#define INA220_2_CONFIG                 \
//{                                       \
//    .bus_name = "ps_i2c1",              \
//    .dev_name = "ina220_2",             \
//    .slave_addr = 0x43,                 \
//    .r_sampling = 0.5f/1000.0f,         \
//}

//#define USING_INA226
//#define INA226_MAGIC    0x0B
//#define INA226_0_CONFIG                 \
//{                                       \
//    .bus_name = "ps_i2c1",              \
//    .dev_name = "ina226_0",             \
//    .slave_addr = 0x44,                 \
//    .r_sampling = 0.5f/1000.0f,         \
//}

//#define USING_ADIS16465
//#define ADIS16465_MAGIC  0x0C
//#define ADIS16465_IGNORE_RST 0xffffffff
//#define ADIS16465_0                         \
//{                                           \
//    .name = "adis16465_0",                  \
//    .spi_bus = "ps_spi0",                   \
//    .spi_cs = 0,                            \
//    .cs_mode = PS_SPI_BUS_CONTROLLER_CS,    \
//    .output_rate = 500,                     \
//    .rst_io = 54+21,                        \
//}

#define USING_NOE_M8
#define NOE_M8_MAGIC    0x0D
#define NOE_M8_TIMEOUT  200
#define NOE_M8_0_CONFIG         \
{                               \
    .uart_name = "axi_uart3",   \
    .dev_name = "noe_m8_0",     \
    .power = NOE_M8_POWER,      \
}

#define USING_TRANSDUCER
//#define FREQUENCY_POINT
#define TRANSDUCER_MAGIC    0x07
#define TRANSDUCER_TIMEOUT  100
#define TRANSDUCER_0_CONFIG         \
{                                   \
    .uart_name = "axi_uart0",       \
    .dev_name = "transducer0",      \
    .power12v = VFD_12V_POWER,      \
    .power5v5 = VFD_5V5_POWER,      \
}

#define USING_BEACON
#define BEACON_MAGIC    0x08
#define BEACON_TIMEOUT  1000
#define BEACON0_CONFIG              \
{                                   \
    .uart_name = "axi_uart1",       \
    .dev_name = "beacon0",          \
    .power = DMJ_24V_POWER,         \
}

#endif /* __DRIVER_CONFIG_H_ */
