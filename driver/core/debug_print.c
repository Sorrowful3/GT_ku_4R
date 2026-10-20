/**
 * @file    debug_print.c
 * @brief   实现调试输出相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2022年2月17日
 */

#include "debug_print.h"

/* 全局调试输出等级 */
static int32_t debug_level = DEBUG_OUTPUT;
/* 自定义调试输出回调函数 */
static custom_print vprint = NULL;

/**
 * @function nane: debug_print_raw
 * @author: liaofei
 * @data: 2022年2月17日
 * @input param:
 *              level 输出等级，值越小，优先级越高
 *              fmt 格式字符串
 *              ... 不定参数列表，与fmt格式化内容相关
 * @output param:
 *              无
 * @description: 
 * 输出调试字符串，其fmt和后面的不定参数参考printf函数
 * @modeify log: 
 * 2022年2月17日 liaofei 首次编写
 */
void debug_print_raw(int32_t level, const char *fmt, ...)
{
    int lenght;
    char *dest_buffer;
    va_list args;

    //检查参数是否合法
    if(!fmt) {
        return;
    }

    /* 检查调试输出等级优先级是否高于全局输出等级，值越小，优先级越高 */
    if(debug_level < level) {
        return;
    }

    /* 计算字符串长度 */
    va_start(args, fmt);
    lenght = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);

    /* 分配内存 */
    dest_buffer = (char*)pvPortMalloc(lenght);
    if(!dest_buffer) {
        return;
    }

    /* 将字符串格式到内存中 */
    va_start(args, fmt);
    vsnprintf(dest_buffer, lenght, fmt, args);
    va_end(args);

    /* 通过标准输出输出数据 */
    printf("%s", dest_buffer);

    /* 输出到自定义终端 */
    if(vprint) {
        vprint(dest_buffer);
    }

    //释放内存
    vPortFree(dest_buffer);
}

/**
 * @function nane: debug_print_line
 * @author: liaofei
 * @data: 2022年2月17日
 * @input param:
 *              level 输出等级，值越小，优先级越高
 *              lvl_name 输出等级对应字符串
 *              tag 标签
 *              fmt 格式字符串
 *              ... 不定参数列表，与fmt格式化内容相关
 * @output param:
 *              无
 * @description:
 * 输出调试字符串，其fmt和后面的不定参数参考printf函数
 * @modeify log:
 * 2022年2月17日 liaofei 首次编写
 */
void debug_print_line(int32_t level, const char *lvl_name, const char *tag, const char *fmt, ...)
{
    int lenght;
    char *fmt_buffer;
    char *dest_buffer;
    va_list args;

    //检查参数是否合法
    if((!lvl_name) || (!tag) || (!fmt)) {
        return;
    }

    /* 检查调试输出等级优先级是否高于全局输出等级，值越小，优先级越高 */
    if(debug_level < level) {
        return;
    }

    //计算格式字符串长度
    lenght = snprintf(NULL, 0, "[%s/%s]%s\r\n", lvl_name, tag, fmt) + 1;

    //分配存储格式字符串的保持
    fmt_buffer = (char*)pvPortMalloc(lenght);
    if(!fmt_buffer) {
        return;
    }

    //格式化格式字符串
    snprintf(fmt_buffer, lenght, "[%s/%s]%s\r\n", lvl_name, tag, fmt);

    //计算输出字符串长度
    va_start(args, fmt);
    lenght = vsnprintf(NULL, 0, fmt_buffer, args) + 1;
    va_end(args);

    //分配存储输出字符串的保持
    dest_buffer = (char*)pvPortMalloc(lenght);
    if(!dest_buffer) {
        vPortFree(fmt_buffer);
        return;
    }

    /* 将字符串格式到内存中 */
    va_start(args, fmt);
    vsnprintf(dest_buffer, lenght, fmt_buffer, args);
    va_end(args);

    /* 通过标准输出输出数据 */
    printf("%s", dest_buffer);

    /* 输出到自定义终端 */
    if(vprint) {
        vprint(dest_buffer);
    }

    //释放内存
    vPortFree(fmt_buffer);
    vPortFree(dest_buffer);
}

/**
 * @function nane: debug_print_custom_set
 * @author: liaofei
 * @data: 2022年2月17日
 * @input param:
 *              print 自定义调试输出函数
 * @output param:
 *              无
 * @description: 
 * 注册用户自定义的调试输出回调函数
 * @modeify log: 
 * 2022年2月17日 liaofei 首次编写
 */
void debug_print_custom_set(custom_print print)
{
    vprint = print;
}

/**
 * @function nane: debug_print_set_level
 * @author: liaofei
 * @data: 2022年2月17日
 * @input param:
 *              level 全局调试输出等级
 * @output param:
 *              无
 * @description: 
 * 设置全局调试输出等级
 * @modeify log: 
 * 2022年2月17日 liaofei 首次编写
 */
void debug_print_set_level(int32_t level)
{
    debug_level = level;
}
