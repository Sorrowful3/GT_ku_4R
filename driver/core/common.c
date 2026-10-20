/**
 * @file    common.c
 * @brief   实现了一些常用的公共函数
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#include "common.h"

/* 1byte数据的最低位置位的位图表 */
const int8_t __lowest_bit_bitmap[] =
{
    /* x取0x0~0xF*/
    /* 0x */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 1x */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 2x */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 3x */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 4x */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 5x */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 6x */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 7x */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 8x */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 9x */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Ax */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Bx */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Cx */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Dx */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Ex */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* Fx */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @function nane: device_ffs
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              value 一个32为数
 * @output param:
 *              return 返回value中最低一位1的位索引
 * @description: 
 * 获取一个32位数中最低一位1的位索引
 * @modeify log: 
 * 2021年3月1日 liaofei 初次创建
 */
int32_t device_ffs(uint32_t value)
{
    if (value == 0) {
        return ERR_INVAL;
    }

    if (value & 0xff) {
        return __lowest_bit_bitmap[value & 0xff];
    }

    if (value & 0xff00) {
        return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 8;
    }

    if (value & 0xff0000) {
        return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 16;
    }

    return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 24;
}

/**
 * @function nane: byte_swap_4
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              src 字节序交换前的数据
 * @output param:
 *              dst 进行字节序交换后的数据
 * @description: 
 * 对4字节数据进行字节序交换
 * @modeify log: 
 * 2021年3月1日 liaofei 初次创建
 */
void byte_swap_4(void *dst, const void *src)
{
    uint8_t *d;
    uint8_t s[4];

    if((!dst) || (!src)) {
        return;
    }

    d = (uint8_t *)dst;
    memcpy(s, src, sizeof(s));
    d[0] = s[3];
    d[1] = s[2];
    d[2] = s[1];
    d[3] = s[0];
}

/**
 * @function nane: byte_swap_2
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              src 字节序交换前的数据
 * @output param:
 *              dst 进行字节序交换后的数据
 * @description: 
 * 对2字节数据进行字节序交换
 * @modeify log: 
 * 2021年3月1日 liaofei 初次创建
 */
void byte_swap_2(void *dst, const void *src)
{
    uint8_t *d;
    uint8_t s[2];

    if((!dst) || (!src)) {
        return;
    }

    d = (uint8_t *)dst;
    memcpy(s, src, sizeof(s));
    d[0] = s[1];
    d[1] = s[0];
}

/**
 * @function nane: crc16
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              data 要计算CRC的数据
 *              lenght 数据字节数
 *              init_value CRC初始值
 * @output param:
 *              return CRC16值
 * @description: 
 * 计算CRC16值
 * @modeify log: 
 * 2021年3月1日 liaofei 初次创建
 */
uint16_t crc16(const uint8_t *data, uint32_t lenght, uint16_t init_value)
{
    uint8_t i;
    uint16_t crc = init_value;

    if(!data) {
        return 0;
    }

    while(lenght--)
    {
        crc ^= (uint16_t)(*(data++)) << 8;
        for(i = 0; i<8; i++) {
            if(crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }
            else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}
