/** 
* # 断言库
* 用法参考python中的断言:
*```C
*assert(1==0)
* ```
* @file                              assert.h
* @brief                             断言库
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
*/

/* $Id$ */
#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "except.h"
/** 
* @fn                               void assert(int e)
* @params[e]                        int          错误码   
* @brief                            断言函数
*/
extern void assert(int e);
/** 
* @def                               assert(e) 
* @brief                             断言宏
*/
#define assert(e) ((void)((e)||(RAISE(Assert_Failed),0)))
#endif