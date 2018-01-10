/** 
* # 内存处理
* 用法参考c++中的方式
*```C
* ```
* @file                              memory.h
* @brief                             堆中内存控制
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

/// ## 异常 

extern const Except_T Mem_Failed; ///< 内存操作失败

/// ## 函数接口

/** 
* @fn    int memcmp(const void *, const void *, size_t)
* @params[s1]             void *      要对比的第一段内存指针
* @params[s2]             void *      要对比的第二段内存指针
* @params[n]              size_t      要对比长度
* @return                 int         相当于bool
* @brief                              对比两端内存是否一致
*/
extern int Mem_memcmp(const void *, const void *, size_t);

/** 
* @fn    void *memmove(void *, const void *, size_t)
* @params[s1]             void *      目标地址指针
* @params[s2]             void *      要移动的对象指针
* @params[n]              size_t      申请时的代码所在行
* @return                 void*       复制内容到的地址指针 
* @brief                              将内存从一段内存移动到另一段内存
*/
extern void *Mem_memmove(void *, const void *, size_t);

/** 
* @fn    void *Mem_alloc (long nbytes, const char *file, int line)
* @params[nbytes]             long      开辟的空间大小 
* @params[file]               char*     申请时的代码文件名
* @params[line]               int       申请时的代码所在行
* @return                     void*     该区域的首地址  
* @brief                                申请堆上的空间,同malloc
*/
extern void *Mem_alloc (long, const char *, int);

/** 
* @fn    void *Mem_calloc(long count, long nbytes, const char *file, int line)
* @params[count]              long      开辟连续空间块的个数
* @params[nbytes]             long      每个空间块开辟的空间大小 
* @params[file]               char*     申请时的代码文件名
* @params[line]               int       申请时的代码所在行
* @return                     void*     该区域的首地址  
* @brief                                申请堆上的空间,同calloc
*/
extern void *Mem_calloc(long, long, const char *, int);

/** 
* @fn    void Mem_free(void *ptr, const char *file, int line)
* @params[ptr]                viod*     对象的指针 
* @params[file]               char*     申请时的代码文件名
* @params[line]               int       申请时的代码所在行
* @brief                                释放堆上的空间,同free
*/
extern void Mem_free(void *, const char *, int);

/** 
* @fn    void *Mem_resize(void *ptr, long nbytes, const char *file, int line)
* @params[ptr]                viod*     对象的指针 
* @params[nbytes]             long      开辟的空间大小 
* @params[file]               char*     申请时的代码文件名
* @params[line]               int       申请时的代码所在行
* @return                     void*     新空间区域的首地址  
* @brief                                复制指定对象指针上的内容并重新分配一块指定大小的空间给这个对象
*/
extern void *Mem_resize(void *ptr, long nbytes,
	const char *file, int line);

/// ## 宏

/** 
* @def    ALLOC(nbytes)
* @params[nbytes]             long      开辟的空间大小 
* @return                     void*     新空间区域的首地址  
* @brief                                申请堆上的空间,同malloc
*/
#define ALLOC(nbytes) \
	Mem_alloc((nbytes), __FILE__, __LINE__)

/** 
* @def    CALLOC(count, nbytes)
* @params[count]              long      开辟连续空间块的个数
* @params[nbytes]             long      每个空间块开辟的空间大小 
* @return                     void*     新空间区域的首地址  
* @brief                                申请堆上的空间,同calloc
*/	
#define CALLOC(count, nbytes) \
	Mem_calloc((count), (nbytes), __FILE__, __LINE__)

/** 
* @def    NEW(p)
* @params[p]                  type      结构名
* @return                     void*     新空间区域的首地址  
* @brief                                实例化一个结构       
*/
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))

/** 
* @def    NEW0(p)
* @params[p]                  type      结构名
* @params[ptr]                viod*     新空间区域的首地址
* @brief                                构造结构实例的数组
*/
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))

/** 
* @def    FREE(ptr)
* @params[ptr]                viod*     对象的指针 
* @brief                                释放对象的空间,同free
*/
#define FREE(ptr) ((void)(Mem_free((ptr), \
	__FILE__, __LINE__), (ptr) = 0))

/** 
* @fn    RESIZE(ptr, nbytes)
* @params[ptr]                viod*     对象的指针 
* @params[nbytes]             long      开辟的空间大小
* @return                     void*     新空间区域的首地址  
* @brief                                复制指定对象指针上的内容并重新分配一块指定大小的空间给这个对象
*/
#define RESIZE(ptr, nbytes) 	((ptr) = Mem_resize((ptr), \
	(nbytes), __FILE__, __LINE__))
#endif