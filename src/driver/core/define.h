/*
 * define.h
 *
 *  Created on: 2021年3月10日
 *      Author: Administrator
 */

#ifndef __DEFINE_H_
#define __DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

//错误码定义
#define ERR_OK         0       /* 没有错误 */
#define ERR_ROUTINE    (-1)    /* 常规错误 */
#define ERR_TIMEOUT    (-2)    /* 超时 */
#define ERR_FULL       (-3)    /* 资源满 */
#define ERR_EMPTY      (-4)    /* 资源空 */
#define ERR_NOMEM      (-5)    /* 分配内存失败 */
#define ERR_NOSUP      (-6)    /* 系统不支持 */
#define ERR_BUSY       (-7)    /* 忙 */
#define ERR_IO         (-8)    /* I/O错误 */
#define ERR_INVAL      (-9)    /* 无效  */
#define ERR_TRYAGAIN   (-10)   /* 再试一次  */
#define ERR_DISCONNECT (-11)   /* 断开连接 */

#if defined(_WIN64)
typedef __int64 ssize_t;
#elif defined(_WIN32)
typedef int ssize_t;
#endif

//对其配置
#define DEVICE_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

#ifdef __cplusplus
}
#endif

#endif /* __DEFINE_H_ */
