/** 
* @file                              memory.h
* @brief                             堆中内存控制
* @detial  内存操作库,主要是封装了malloc和free这几个函数,以规避一些常见的异常.
* 有两种实现,一种不做异常检验一种进行
* @par 依赖        except.h
*                  assert.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/

/* $Id$ */
#include <string.h>
#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED
#include "except.h"

extern const Except_T Mem_Failed; ///< 内存操作失败

/** 
* @fn    int Mem_memcmp(const void *, const void *, size_t)
* @param [s1]             void *      要对比的第一段内存指针
* @param [s2]             void *      要对比的第二段内存指针
* @param [n]              size_t      要对比长度
* @return                 int         相当于bool
* @brief                              对比两端内存是否一致
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern int Mem_memcmp(const void *, const void *, size_t);

/** 
* @fn    void *Mem_memmove(void *, const void *, size_t)
* @param [s1]             void *      目标地址指针
* @param [s2]             void *      要移动的对象指针
* @param [n]              size_t      申请时的代码所在行
* @return                 void*       复制内容到的地址指针 
* @brief                              将内存从一段内存移动到另一段内存
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *Mem_memmove(void *, const void *, size_t);

/** 
* @fn    void *Mem_alloc (long nbytes, const char *file, int line)
* @param [nbytes]             long      开辟的空间大小 
* @param [file]               char*     申请时的代码文件名
* @param [line]               int       申请时的代码所在行
* @return                     void*     该区域的首地址  
* @brief                                申请堆上的空间,同malloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *Mem_alloc (long, const char *, int);

/** 
* @fn    void *Mem_calloc(long count, long nbytes, const char *file, int line)
* @param [count]              long      开辟连续空间块的个数
* @param [nbytes]             long      每个空间块开辟的空间大小 
* @param [file]               char*     申请时的代码文件名
* @param [line]               int       申请时的代码所在行
* @return                     void*     该区域的首地址  
* @brief                                申请堆上的空间,同calloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *Mem_calloc(long, long, const char *, int);

/** 
* @fn    void Mem_free(void *ptr, const char *file, int line)
* @param [ptr]                viod*     对象的指针 
* @param [file]               char*     申请时的代码文件名
* @param [line]               int       申请时的代码所在行
* @brief                                释放堆上的空间,同free
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void Mem_free(void *, const char *, int);

/** 
* @fn    void *Mem_resize(void *ptr, long nbytes, const char *file, int line)
* @param [ptr]                viod*     对象的指针 
* @param [nbytes]             long      开辟的空间大小 
* @param [file]               char*     申请时的代码文件名
* @param [line]               int       申请时的代码所在行
* @return                     void*     新空间区域的首地址  
* @brief                                复制指定对象指针上的内容并重新分配一块指定大小的空间给这个对象
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
extern void *Mem_resize(void *ptr, long nbytes,
	const char *file, int line);

/** 
* @def    ALLOC(nbytes)
* @param [nbytes]             long      开辟的空间大小 
* @return                     void*     新空间区域的首地址  
* @brief                                申请堆上的空间,同malloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
#define ALLOC(nbytes) \
	Mem_alloc((nbytes), __FILE__, __LINE__)

/** 
* @def    CALLOC(count, nbytes)
* @param [count]              long      开辟连续空间块的个数
* @param [nbytes]             long      每个空间块开辟的空间大小 
* @return                     void*     新空间区域的首地址  
* @brief                                申请堆上的空间,同calloc
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/	
#define CALLOC(count, nbytes) \
	Mem_calloc((count), (nbytes), __FILE__, __LINE__)

/** 
* @def    NEW(p)
* @param [p]                  type      结构名
* @return                     void*     新空间区域的首地址  
* @brief                                实例化一个结构  
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT     
*/
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))

/** 
* @def    NEW0(p)
* @param [p]                  type      结构名
* @param [ptr]                viod*     新空间区域的首地址
* @brief                                构造结构实例的数组
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))

/** 
* @def    FREE(ptr)
* @param [ptr]                viod*     对象的指针 
* @brief                                释放对象的空间,同free
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
#define FREE(ptr) ((void)(Mem_free((ptr), \
	__FILE__, __LINE__), (ptr) = 0))

/** 
* @fn    RESIZE(ptr, nbytes)
* @param [ptr]                viod*     对象的指针 
* @param [nbytes]             long      开辟的空间大小
* @return                     void*     新空间区域的首地址  
* @brief                                复制指定对象指针上的内容并重新分配一块指定大小的空间给这个对象
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
#define RESIZE(ptr, nbytes) 	((ptr) = Mem_resize((ptr), \
	(nbytes), __FILE__, __LINE__))
#endif