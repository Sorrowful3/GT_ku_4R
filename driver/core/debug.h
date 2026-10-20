/**
 * @file    debug.h
 * @brief   实现调试输出相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#ifndef __DEBUG_H_
#define __DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "debug_print.h"

/* 默认调试输出等级 */
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL     WARNING_OUTPUT
#endif

/* 默认调试输出标签 */
#ifndef DEBUG_TAG
#define DEBUG_TAG   "DBG"
#endif

/**
 * @function nane: DBG_LOG_LINE_INTR
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              debug_level 输出等级
 *              lvl_name 输出等级标签
 *              fmt 格式化字符串
 *              ... 不定参数列表
 * @output param:
 *              无
 * @description:
 * 在任务中输出一行调试字符串
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
#define DBG_LOG_LINE_TASK(debug_level, lvl_name, fmt, ...)                      \
do{                                                                             \
    int lenght;                                                                 \
    char *tag_buffer;                                                           \
                                                                                \
    lenght = snprintf(NULL, 0, DEBUG_TAG) + 1;                                  \
                                                                                \
    tag_buffer = (char*)pvPortMalloc(lenght);                                   \
    if(!tag_buffer) {                                                           \
        break;                                                                  \
    }                                                                           \
    snprintf(tag_buffer, lenght, DEBUG_TAG);                                    \
                                                                                \
    debug_print_line(debug_level, lvl_name, tag_buffer, fmt, ##__VA_ARGS__);    \
                                                                                \
    vPortFree(tag_buffer);                                                      \
}while(0)

/**
 * @function nane: DBG_LOG_LINE_INTR
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              debug_level 输出等级
 *              lvl_name 输出等级标签
 *              fmt 格式化字符串
 *              ... 不定参数列表
 * @output param:
 *              无
 * @description:
 * 在中断中输出一行调试字符串
 * @modeify log:
 * 2021年3月1日 liaofei 首次编写
 */
#define DBG_LOG_LINE_INTR(debug_level, lvl_name, fmt, ...)              \
do{                                                                     \
    printf("[");                                                        \
    printf(lvl_name);                                                   \
    printf("/");                                                        \
    printf(DEBUG_TAG);                                                  \
    printf("]");                                                        \
    printf(fmt, ##__VA_ARGS__);                                         \
    printf("\r\n");                                                     \
}while(0)

/**
 * @function nane: DBG_LOG_LINE
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              debug_level 输出等级
 *              lvl_name 输出等级标签
 *              fmt 格式化字符串
 *              ... 不定参数列表
 * @output param:
 *              无
 * @description: 
 * 输出一行调试字符串
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
#define DBG_LOG_LINE(debug_level, lvl_name, fmt, ...)                           \
do{                                                                             \
    if(portICCRPR_RUNNING_PRIORITY_REGISTER < 255) {                            \
        DBG_LOG_LINE_INTR(debug_level, lvl_name, fmt, ##__VA_ARGS__);           \
    }                                                                           \
    else {                                                                      \
        DBG_LOG_LINE_TASK(debug_level, lvl_name, fmt, ##__VA_ARGS__);           \
    }                                                                           \
}while(0)

#if (DEBUG_LEVEL >= DEBUG_OUTPUT)
#define LOG_D(fmt, ...)     DBG_LOG_LINE(DEBUG_OUTPUT, "D", fmt, ##__VA_ARGS__)
#else
#define LOG_D(...)  {}
#endif

#if (DEBUG_LEVEL >= INFO_OUTPUT)
#define LOG_I(fmt, ...)     DBG_LOG_LINE(INFO_OUTPUT, "I", fmt, ##__VA_ARGS__)
#else
#define LOG_I(...)  {}
#endif

#if (DEBUG_LEVEL >= WARNING_OUTPUT)
#define LOG_W(fmt, ...)     DBG_LOG_LINE(WARNING_OUTPUT, "W", fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)  {}
#endif

#if (DEBUG_LEVEL >= ERROR_OUTPUT)
#define LOG_E(fmt, ...)     DBG_LOG_LINE(ERROR_OUTPUT, "E", fmt, ##__VA_ARGS__)
#else
#define LOG_E(...)  {}
#endif

//输出调试级别的原始字符串
#define LOG_D_RAW(fmt, ...) debug_print_raw(DEBUG_OUTPUT, fmt, ##__VA_ARGS__)
//输出信息级别的原始字符串
#define LOG_I_RAW(fmt, ...) debug_print_raw(INFO_OUTPUT, fmt, ##__VA_ARGS__)
//输出警告级别的原始字符串
#define LOG_W_RAW(fmt, ...) debug_print_raw(WARNING_OUTPUT, fmt, ##__VA_ARGS__)
//输出错误级别的原始字符串
#define LOG_E_RAW(fmt, ...) debug_print_raw(ERROR_OUTPUT, fmt, ##__VA_ARGS__)
//最高优先级输出原始调试信息
#define LOG_RAW             LOG_E_RAW

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H_ */
