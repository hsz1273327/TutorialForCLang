/**
 * @file                              assert.h
 * @brief                             断言库
 * @detail  断言库,抛出一个异常结构体的实例
 * @version                            1.2
 * @par Copyright (c) 1994,1995,1996,1997 by David R. Hanson
 * @par LICENSE                        MIT
 * @par requirements:                  except.h
 * @par update:                        hsz
 * @par update date:                   2018-01-18
 */
#ifndef SOURCE_INCLUDE_ASSERT_H_
#define SOURCE_INCLUDE_ASSERT_H_

#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "except.h"

/**
 * @fn                               void assert(int e)
 * @param [e]     int                要断言的表达式
 * @brief                            断言函数
 * @detial  断言表达式的结果不是0,如果是0代表错误,会抛出一个Assert_Faile
 */
extern void assert(int e);

/**
 * @def                               assert(e)
 * @param [e]                         int          要断言的表达式
 * @brief                             断言宏
 * @detial  断言表达式的结果不是0,如果是0代表错误,会抛出一个Assert_Faile
 */
#define assert(e) ((void)((e) || (RAISE(Assert_Failed), 0)))
#endif  // NDEBUG
#endif  // SOURCE_INCLUDE_ASSERT_H_
