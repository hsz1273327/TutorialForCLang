/** 
* @file                              memorypool.h
* @brief                             内存池操作
* @detial  内存池操作,有一定的风险
* @par 依赖     except.h
*               assert.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/

/* $Id$ */
#ifndef MEMORYPOOL_INCLUDED
#define MEMORYPOOL_INCLUDED
#include "except.h"

/** 
* @typedef      MemoryPool_T
* @param [prev]   struct MemoryPool_T *      下一段池头地址    
* @param [avail]  char *  本段内存中第一个空闲位置
* @param [limit]  char *  本段结束位置,从avail到limit之前的空间可以用于分配    
* @brief                             内存池结构,内存池描述一大块内存
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
typedef struct MemoryPool_T *MemoryPool_T;



extern const Except_T MemoryPool_NewFailed; ///< 内存池new操作失败
extern const Except_T MemoryPool_Failed; ///< 内存池操作失败

/** 
* @fn         MemoryPool_T MemoryPool_new(void)
* @return     MemoryPool_T           实例化的内存池对象
* @brief                             在堆上构造一个内存池
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern MemoryPool_T MemoryPool_new(void);

/** 
* @fn         void MemoryPool_dispose(MemoryPool_T *)
* @param [ap]      MemoryPool_T*    要释放的内存池指针
* @brief                             释放内存池关联的所有内存,并
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void MemoryPool_dispose(MemoryPool_T *);

/** 
* @fn          void *MemoryPool_alloc (MemoryPool_T, long, const char *, int)
* @param [memory_pool]      MemoryPool_T    目标内存池
* @param [nbytes]           long            开辟内存单个的大小
* @param [file]             const char*     文件
* @param [line]             int             执行所在行
* @return      void*                 新空间区域的首地址
* @brief                             在内存池中申请一份内存连续的序列,类似malloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *MemoryPool_alloc (MemoryPool_T, long, const char *, int);

/** 
* @fn         void *MemoryPool_calloc(MemoryPool_T, long, long, const char *, int line)
* @param [memory_pool]      MemoryPool_T    目标内存池
* @param [count]            long            开辟内存序列的个数
* @param [nbytes]           long            开辟内存单个的大小
* @param [file]             const char*     文件
* @param [line]             int             执行所在行
* @return      void*                 新空间区域的首地址
* @brief                             在内存池中申请一份内存连续的序列,类似calloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *MemoryPool_calloc(MemoryPool_T, long, long, const char *, int);

/** 
* @fn         void MemoryPool_free(MemoryPool_T)
* @param [memory_pool]      MemoryPool_T     要释放的内存池对象
* @brief                             释放内存池全部内容
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void MemoryPool_free(MemoryPool_T);

#endif