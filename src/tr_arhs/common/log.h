/*
 * log.h
 *
 *  Created on: 2021Äê4ÔÂ8ÈÕ
 *      Author: admin
 */

#ifndef SRC_PALNFORM_FREERTOS_INCLUDE_LOG_H_
#define SRC_PALNFORM_FREERTOS_INCLUDE_LOG_H_

#include <inttypes.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <stdarg.h>

#define _TR_LOG_LEVEL_DEBUG		0
#define _TR_LOG_LEVEL_INFO		1
#define _TR_LOG_LEVEL_WARN		2
#define _TR_LOG_LEVEL_ERROR		3
#define _TR_LOG_LEVEL_PANIC		4


// Used to silence unused variable warning
static inline void do_nothing(int level, ...)
{
	(void)level;
}


#define __tr__log_printcond(cond, ...)	    if (cond) printf(__VA_ARGS__)

/****************************************************************************
 * __px4_log_omit:
 * Compile out the message
 ****************************************************************************/
#define __tr_log_omit(level, FMT, ...)   do_nothing(level, ##__VA_ARGS__)

/****************************************************************************
 * __tr_log_named_cond:
 * Convert a message in the form:
 * 	TR_LOG_COND(__dbg_enabled, "val is %d", val);
 * to
 * 	printf("%-5s val is %d\n", "LOG", val);
 * if the first arg/condition is true.
 ****************************************************************************/
#define __tr_log_named_cond(name, cond, FMT, ...) \
	__tr__log_printcond(cond,\
			     "%s " \
			     FMT\
			     "\n" \
			     ,name, ##__VA_ARGS__\
			    )

/****************************************************************************
 * __tr_log_modulename:
 * Convert a message in the form:
 * 	TR_WARN("val is %d", val);
 * to
 * 	printf("%-5s [%s] val is %d\n", __px4_log_level_str[3],
 *		MODULENAME, val);
 ****************************************************************************/

enum COLOR
{
    COLOR_INFO = 32,   //COLOR_GREEN
    COLOR_WARN = 33,   //COLOR_YELLOW
    COLOR_ERROR = 31,  //COLOR_RED
    COLOR_FATAL = 36   //COLOR_CYAN
};

static int s_color[] = {0, COLOR_INFO, COLOR_WARN, COLOR_ERROR, COLOR_FATAL};

#define tr_log_modulename(level , fmt, ...) \
    printf("\033[%d;40m", s_color[level],fmt); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\033[0m")

#define __tr_log_modulename(level, fmt, ...) \
	do { \
		tr_log_modulename(level , fmt, ##__VA_ARGS__); \
	} while(0)


#define TR_DEBUG(FMT, ...) 	__tr_log_omit(_TR_LOG_LEVEL_DEBUG, FMT, ##__VA_ARGS__)

#define TR_LOG_NAMED(name, FMT, ...) 	        __tr_log_named_cond(name, true, FMT, ##__VA_ARGS__)
#define TR_LOG_NAMED_COND(name, cond, FMT, ...) __tr_log_named_cond(name, cond, FMT, ##__VA_ARGS__)

/****************************************************************************
 * Medium verbose settings for a default build
 ****************************************************************************/
#define TR_PANIC(FMT, ...)	__tr_log_modulename(_TR_LOG_LEVEL_PANIC, FMT, ##__VA_ARGS__)
#define TR_ERR(FMT, ...)	__tr_log_modulename(_TR_LOG_LEVEL_ERROR, FMT, ##__VA_ARGS__)
#define TR_WARN(FMT, ...) 	__tr_log_modulename(_TR_LOG_LEVEL_WARN,  FMT, ##__VA_ARGS__)
#define TR_INFO(FMT, ...) 	__tr_log_modulename(_TR_LOG_LEVEL_INFO,  FMT, ##__VA_ARGS__)

#endif /* SRC_PALNFORM_FREERTOS_INCLUDE_LOG_H_ */
