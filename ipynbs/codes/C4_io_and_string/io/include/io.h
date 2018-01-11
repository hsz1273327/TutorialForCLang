#ifndef IO_INCLUDED
#define IO_INCLUDED
#include <stdio.h>
#include "except.h"

extern const Except_T IO_Open_Failed; ///< 文件打开错误

extern const Except_T IO_Buffer_Error; ///< 缓冲区错误


/** 
* @fn                void Except_init(void)
* @brief                  异常处理块的初始化
*/
extern int IO_debug(const char *format, ...)