/** 
* @file                              format.h
* @brief                             字符串格式化
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#ifndef FMT_INCLUDED
#define FMT_INCLUDED
#include <stdarg.h>
#include <stdio.h>
#include "except.h"

/** 
* @typedef          va_list_box
* @param [ap]   va_list      参数序列  
* @brief                             参数序列类
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
typedef struct va_list_box {
	va_list ap; ///< 
} va_list_box;

/** 
* @typedef          Format_T
* @param [ap]   va_list      参数序列  
* @brief   定义了一个与每个格式化代码关联的格式转换函数的类型
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @detail 定义了一个与每个格式化代码关联的格式转换函数的类型\
这个定义的类型实际是一个函数,其形式为:
`void (*)(int,va_list_box,int(*)(int,void*),void*,unsigned char* ,int,int)`
*/
typedef void (*Format_T)(int code, va_list_box *box,
	int put(int c, void *cl), void *cl,
	unsigned char flags[256], int width, int precision);

extern char *Fmt_flags;///<格式化标识

extern const Except_T Fmt_Overflow;///<缓冲溢出异常

/** 
* @fn     void Fmt_fmt(int (*)(int, void *), void *, const char *, ...)
* @brief                                                 格式化函数
* @param [put]    int (*)(int,void *)                函数指针 ,这个函数用于格式化每个字符串
* @param [c1]     void *c                            指定输出的流 
* @param [fmt]    const char *                       格式化用的字符串
* @param [...]                                       额外的参数序列 
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void Fmt_fmt (int(*)(int, void *), void *, const char *, ...);

/** 
* @fn     void Fmt_vfmt(int (*)(int , void *), void *, const char *, va_list_box *)
* @brief                                              
* @param [put]         int (*)(int,void *)          函数指针 ,这个函数用于格式化每个字符串
* @param [c1]          void *c                      指定输出的流 
* @param [fmt]         const char *                 格式化用的字符串
* @param [box]         va_list_box *                存放额外参数的结构
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT  
*/
extern void Fmt_vfmt(int (*)(int, void *), void *, const char *, va_list_box *);

extern void Fmt_print (const char *fmt, ...);

extern void Fmt_fprint(FILE *stream,
	const char *fmt, ...);

extern int Fmt_sfmt   (char *buf, int size,
	const char *fmt, ...);

extern int Fmt_vsfmt(char *buf, int size,
	const char *fmt, va_list_box *box);

extern char *Fmt_string (const char *fmt, ...);

extern char *Fmt_vstring(const char *fmt, va_list_box *box);

extern Format_T Fmt_register(int code, Format_T cvt);

extern void Fmt_putd(const char *str, int len,
	int put(int c, void *cl), void *cl,
	unsigned char flags[256], int width, int precision);

extern void Fmt_puts(const char *str, int len,
	int put(int c, void *cl), void *cl,
	unsigned char flags[256], int width, int precision);

#endif
