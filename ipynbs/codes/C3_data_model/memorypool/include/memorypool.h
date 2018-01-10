/** 
* # 内存处理
* 用法参考c++中的方式
*```C
* ```
* @file                              memorypool.h
* @brief                             内存池操作
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/

/* $Id$ */
#ifndef MEMORYPOOL_INCLUDED
#define MEMORYPOOL_INCLUDED
#include "except.h"
#define T MemoryPool_T


/** 
* @struct                            MemoryPool_T
* @brief                             内存池结构,内存池描述一大块内存
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
typedef struct T *T;

/// ## 异常 
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
extern T MemoryPool_new(void);

/** 
* @fn         void MemoryPool_dispose(MemoryPool_T *)
* @params[ap]      MemoryPool_T*    要释放的内存池指针
* @brief                             释放内存池关联的所有内存,并
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void MemoryPool_dispose(T *);

/** 
* @fn          void *MemoryPool_alloc (MemoryPool_T, long, const char *, int)
* @params[memory_pool]      MemoryPool_T    目标内存池
* @params[nbytes]           long            开辟内存单个的大小
* @params[file]             char*           文件
* @params[line]             int             执行所在行
* @return      void*                 新空间区域的首地址
* @brief                             在内存池中申请一份内存连续的序列,类似malloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *MemoryPool_alloc (T memory_pool, long nbytes,
	const char *file, int line);

/** 
* @fn         void *MemoryPool_calloc(MemoryPool_T, long, long, const char *, int line)
* @params[memory_pool]      MemoryPool_T    目标内存池
* @params[count]            long            开辟内存序列的个数
* @params[nbytes]           long            开辟内存单个的大小
* @params[file]             char*           文件
* @params[line]             int             执行所在行
* @return      void*                 新空间区域的首地址
* @brief                             在内存池中申请一份内存连续的序列,类似calloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *MemoryPool_calloc(T memory_pool, long count,
	long nbytes, const char *file, int line);

/** 
* @fn         void MemoryPool_free(MemoryPool_T)
* @params[memory_pool]      MemoryPool_T     要释放的内存池对象
* @brief                             释放内存池全部内容
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void MemoryPool_free(T);
#undef T
#endif