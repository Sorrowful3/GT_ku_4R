/*
 * mavlink_chan.c
 *
 *  Created on: 2020年9月3日
 *      Author: Administrator
 */

#include "mavlink_chan.h"

#define DEBUG_TAG "mavlink_chan,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "../driver/core/debug.h"

typedef struct{
    uint8_t                 use_flag;
    uint8_t                 type;
    SemaphoreHandle_t       r_mutex;
    SemaphoreHandle_t       w_mutex;
    mavlink_port_t          *ops;
}mavlink_chan_t;

static mavlink_chan_t mavlink_chan[MAVLINK_COMM_NUM_BUFFERS];
static SemaphoreHandle_t alloc_mutex;

int32_t mavlink_chan_init(void)
{
    uint32_t i;

    alloc_mutex = xSemaphoreCreateRecursiveMutex();
    if(!alloc_mutex) {
        LOG_E("create mutex failed");
        return ERR_NOMEM;
    }

    for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
        //复位通道参数
        mavlink_chan[i].use_flag = 0;
        mavlink_chan[i].type = UNKNOWN_TYPE;
        mavlink_chan[i].ops = NULL;

        //创建互斥锁
        mavlink_chan[i].r_mutex = xSemaphoreCreateRecursiveMutex();
        if(!mavlink_chan[i].r_mutex) {
            LOG_E("create mutex failed");
            return ERR_NOMEM;
        }

        mavlink_chan[i].w_mutex = xSemaphoreCreateRecursiveMutex();
        if(!mavlink_chan[i].w_mutex) {
            LOG_E("create mutex failed");
            return ERR_NOMEM;
        }
    }

    return ERR_OK;
}

int32_t mavlink_chan_alloc(mavlink_channel_t *chan, mavlink_port_t *ops, uint8_t type)
{
    uint32_t i;
    int32_t result;

    if((!chan) || (!ops)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //获取通道分配保护锁
    xSemaphoreTakeRecursive(alloc_mutex, portMAX_DELAY);

    //遍历所有通道
    for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
        //判断通道是否空闲
        if(!mavlink_chan[i].use_flag) {
            //打开通道
            if(ops->open) {
                result = ops->open((struct mavlink_port*)ops);
                if(result != ERR_OK) {
                    xSemaphoreGiveRecursive(alloc_mutex);
                    LOG_E("open chan failed");
                    return result;
                }
            }
            //返回获取到的通道号
            *chan = (mavlink_channel_t)i;
            //获取相应通道
            mavlink_chan[i].use_flag = 1;
            mavlink_chan[i].ops = ops;
            mavlink_chan[i].type = type;
            break;
        }
    }

    //释放通道分配保护锁
    xSemaphoreGiveRecursive(alloc_mutex);

    if(i >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) {
        LOG_E("alloc chan failed");
        return ERR_FULL;
    }
    return ERR_OK;
}

int32_t mavlink_chan_free(mavlink_channel_t chan)
{
    int32_t result;

    if(chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    //获取通道分配保护锁
    xSemaphoreTakeRecursive(alloc_mutex, portMAX_DELAY);
    //获取读写锁，确保读写操作完成
    xSemaphoreTakeRecursive(mavlink_chan[chan].r_mutex, portMAX_DELAY);
    xSemaphoreTakeRecursive(mavlink_chan[chan].w_mutex, portMAX_DELAY);

    if(!mavlink_chan[chan].use_flag) {
        //释放读写锁
        xSemaphoreGiveRecursive(mavlink_chan[chan].r_mutex);
        xSemaphoreGiveRecursive(mavlink_chan[chan].w_mutex);

        //释放通道分配保护锁
        xSemaphoreGiveRecursive(alloc_mutex);
        return ERR_INVAL;
    }

    //关闭通道
    if((mavlink_chan[chan].ops) && (mavlink_chan[chan].ops->close)) {
        result = mavlink_chan[chan].ops->close((struct mavlink_port*)mavlink_chan[chan].ops);
        if(result != ERR_OK) {
            //释放读写锁
            xSemaphoreGiveRecursive(mavlink_chan[chan].r_mutex);
            xSemaphoreGiveRecursive(mavlink_chan[chan].w_mutex);

            //释放通道分配保护锁
            xSemaphoreGiveRecursive(alloc_mutex);
            LOG_E("close chan failed");
            return result;
        }
    }
    //复位通道
    mavlink_chan[chan].use_flag = 0;
    mavlink_chan[chan].ops = NULL;
    mavlink_chan[chan].type = UNKNOWN_TYPE;

    //释放读写锁
    xSemaphoreGiveRecursive(mavlink_chan[chan].r_mutex);
    xSemaphoreGiveRecursive(mavlink_chan[chan].w_mutex);

    //释放通道分配保护锁
    xSemaphoreGiveRecursive(alloc_mutex);

    return ERR_OK;
}

ssize_t mavlink_chan_read(mavlink_channel_t chan, void *data, size_t size, TickType_t timeout)
{
    ssize_t result;

    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    xSemaphoreTakeRecursive(mavlink_chan[chan].r_mutex, portMAX_DELAY);

    if(mavlink_chan[chan].use_flag) {
        if((mavlink_chan[chan].type != UNKNOWN_TYPE) &&
                (mavlink_chan[chan].type != VIRTUAL_TYPE) &&
                (mavlink_chan[chan].ops) &&
                (mavlink_chan[chan].ops->read)) {
            result = mavlink_chan[chan].ops->read((struct mavlink_port*)mavlink_chan[chan].ops, data, size, timeout);
        }
        else if(mavlink_chan[chan].type == VIRTUAL_TYPE) {
            result = 0;
        }
        else {
            result = ERR_IO;
        }
    }
    else  {
        result = ERR_IO;
    }

    xSemaphoreGiveRecursive(mavlink_chan[chan].r_mutex);

    return result;
}

ssize_t mavlink_chan_write(mavlink_channel_t chan, const void *data, size_t size)
{
    ssize_t result;

    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    xSemaphoreTakeRecursive(mavlink_chan[chan].w_mutex, portMAX_DELAY);

    if(mavlink_chan[chan].use_flag) {
        if((mavlink_chan[chan].type != UNKNOWN_TYPE) &&
                (mavlink_chan[chan].type != VIRTUAL_TYPE) &&
                (mavlink_chan[chan].ops) &&
                (mavlink_chan[chan].ops->write)) {
            result = mavlink_chan[chan].ops->write((struct mavlink_port*)mavlink_chan[chan].ops, data, size);
        }
        else if(mavlink_chan[chan].type == VIRTUAL_TYPE) {
            result = 0;
        }
        else {
            result = ERR_IO;
        }
    }
    else  {
        result = ERR_IO;
    }

    xSemaphoreGiveRecursive(mavlink_chan[chan].w_mutex);

    return result;
}

int32_t mavlink_type_chan_write(const void *data, size_t size, uint8_t type)
{
    uint32_t i;

    if((!data) || (size == 0) || (type == UNKNOWN_TYPE)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    if(type == VIRTUAL_TYPE) {
        return ERR_OK;
    }

    for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
        if((mavlink_chan[i].type == type) &&
                (mavlink_chan[i].ops) &&
                (mavlink_chan[i].ops->write)) {
            mavlink_chan_write((mavlink_channel_t)i, data, size);
        }
    }
    return ERR_OK;
}

int32_t mavlink_all_chan_write(const void *data, size_t size)
{
    uint32_t i;

    if((!data) || (size == 0)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    for(i=0; i<sizeof(mavlink_chan)/sizeof(mavlink_chan[0]); i++) {
        if((mavlink_chan[i].type != UNKNOWN_TYPE) &&
                (mavlink_chan[i].type != VIRTUAL_TYPE) &&
                (mavlink_chan[i].ops) &&
                (mavlink_chan[i].ops->write)) {
            mavlink_chan_write((mavlink_channel_t)i, data, size);
        }
    }
    return ERR_OK;
}

int32_t mavlink_chan_write_pack(mavlink_channel_t chan, const mavlink_message_t *msg)
{
    ssize_t result;
    size_t lenght = 0;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (!msg)) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(buffer, 0, sizeof(buffer));
    //序列化
    lenght = mavlink_msg_to_send_buffer(buffer, msg);
    //发送
    result = mavlink_chan_write(chan, buffer, lenght);
    if(result != (ssize_t)lenght) {
        return ERR_IO;
    }
    else {
        return ERR_OK;
    }
}

int32_t mavlink_type_chan_write_pack(const mavlink_message_t *msg, uint8_t type)
{
    size_t lenght = 0;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    if(!msg) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(buffer, 0, sizeof(buffer));
    //序列化
    lenght = mavlink_msg_to_send_buffer(buffer, msg);
    //发送
    return mavlink_type_chan_write(buffer, lenght, type);
}

int32_t mavlink_all_chan_write_pack(const mavlink_message_t *msg)
{
    size_t lenght = 0;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    if(!msg) {
        LOG_E("parameter error");
        return ERR_INVAL;
    }

    memset(buffer, 0, sizeof(buffer));
    //序列化
    lenght = mavlink_msg_to_send_buffer(buffer, msg);
    //发送
    return mavlink_all_chan_write(buffer, lenght);
}

void mavlink_chan_send_begin(mavlink_channel_t chan, uint16_t lenght)
{
    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (lenght == 0)) {
        LOG_E("parameter error");
        return ;
    }

    xSemaphoreTakeRecursive(mavlink_chan[chan].w_mutex, portMAX_DELAY);
}

void mavlink_chan_send_bytes(mavlink_channel_t chan, const void *data, uint16_t lenght)
{
    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (!data) || (lenght == 0)) {
        LOG_E("parameter error");
        return ;
    }

    if(mavlink_chan[chan].use_flag) {
        if((mavlink_chan[chan].type != UNKNOWN_TYPE) &&
                (mavlink_chan[chan].type != VIRTUAL_TYPE) &&
                (mavlink_chan[chan].ops) &&
                (mavlink_chan[chan].ops->write)) {
            mavlink_chan[chan].ops->write((struct mavlink_port*)mavlink_chan[chan].ops, data, lenght);
        }
    }

}

void mavlink_chan_send_end(mavlink_channel_t chan, uint16_t lenght)
{
    if((chan >= sizeof(mavlink_chan)/sizeof(mavlink_chan[0])) || (lenght == 0)) {
        LOG_E("parameter error");
        return ;
    }

    xSemaphoreGiveRecursive(mavlink_chan[chan].w_mutex);
}

