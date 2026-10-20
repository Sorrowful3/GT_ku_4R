/**
 * @file    debug_print.h
 * @brief   实现调试输出相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2022年2月17日
 */

#ifndef __DEBUG_PRINT_H_
#define __DEBUG_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "xil_printf.h"

/**
 * 调试输出等级
 * 需要用户在包含debug.h文件前定义DEBUG_LEVEL，若未定义则默认为WARNING_OUTPUT
 */
#define DEBUG_OUTPUT    3
#define INFO_OUTPUT     2
#define WARNING_OUTPUT  1
#define ERROR_OUTPUT    0

/* 调试输出回调函数原型 */
typedef void (*custom_print)(const char *str);

void debug_print_raw(int32_t level, const char *fmt, ...);
void debug_print_line(int32_t level, const char *lvl_name, const char *tag, const char *fmt, ...);
void debug_print_custom_set(custom_print print);
void debug_print_set_level(int32_t level);

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_PRINT_H_ */
