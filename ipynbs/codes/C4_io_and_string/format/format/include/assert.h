/** 
* @file                              assert.h
* @brief                             断言库
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "except.h"

/** 
* @fn                               void assert(int e)
* @param [e]                        int          错误码,0代表错误,其他表述正确  
* @brief                            断言函数
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void assert(int e);

/** 
* @def                               assert(e)
* @param [e]                         int          错误码,0代表错误,其他表述正确  
* @brief                             断言宏
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
#define assert(e) ((void)((e) || (RAISE(Assert_Failed), 0)))
#endif