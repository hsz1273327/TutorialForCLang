/** 
* @file                              except.h
* @brief                             异常处理
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include <setjmp.h>

// ## 定义的结构

/** 
* @struct                             Exception_T
* @brief                              异常结构
*/
typedef struct Except_T {
    const char* reason; ///< reason   str   原因字符串
} Except_T;


typedef struct Except_Frame Except_Frame;

/** 
* @struct                             Exception_Frame
* @brief                              异常帧结构
*/
struct Except_Frame {
    Except_Frame* prev; ///< 异常帧指针
    jmp_buf env; ///< 用于处理嵌套异常
    const char* file; ///< 报异常文件
    int line; ///< 报异常的行号
    const Except_T* exception; ///< 异常实例指针
};

/** 
* @enum                            
* @brief                    异常的4个状态
*/
enum {
    Except_entered = 0, ///< 进入异常处理块状态
    Except_raised, ///< 抛出异常处理状态
    Except_handled, ///< 异常处理中状态
    Except_finalized ///< 异常处理块结束状态
};

extern Except_Frame* Except_stack; ///<           实例化一个异常栈
extern const Except_T Assert_Failed; ///<   定义Assert_Failed异常

// ## 定义函数

/** 
* @fn     void Except_raise(const Exception_T *,const char *,int)
* @brief 抛出异常
* @params[e]                          Except_T*        要抛出的异常   
* @params[file]                       char*        抛出异常的文件名  
* @params[line]                       int          抛出的异常所在行   
*/
void Except_raise(const Except_T* e, const char* file, int line);

#ifdef WIN32
#include <windows.h>

extern int Except_index; ///< 异常mulu

/** 
* @fn                void Except_init(void)
* @brief                  异常处理块的初始化
*/
extern void Except_init(void);

/** 
* @fn                               void Except_push(Except_Frame* fp)
* @params[fp]                       Except_Frame*          要抛出的异常    
* @brief     将异常压入栈
*/
extern void Except_push(Except_Frame* fp);

/** 
* @fn                                            void Except_pop(void) 
* @brief     将异常从栈中取出
*/
extern void Except_pop(void);
#endif


#ifdef WIN32
/* $Id$ */
/** 
* @def                                RAISE(e)
* @brief                              抛出异常
*/
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)

/** 
* @def                                RERAISE
* @brief                              重复抛出异常
*/
#define RERAISE Except_raise(Except_frame.exception, \
    Except_frame.file, Except_frame.line)
/** 
* @def                                RETURN
* @brief                              返回
*/
#define RETURN               \
    switch (Except_pop(), 0) \
    default:                 \
        return
/** 
* @def                                TRY
* @brief                              Try关键字
*/
#define TRY                                     \
    do {                                        \
        volatile int Except_flag;               \
        Except_Frame Except_frame;              \
        if (Except_index == -1)                 \
            Except_init();                      \
        Except_push(&Except_frame);             \
        Except_flag = setjmp(Except_frame.env); \
        if (Except_flag == Except_entered) {

/** 
* @def                                EXCEPT(e)
* @brief                              EXCEPT关键字
*/
#define EXCEPT(e)                            \
    if (Except_flag == Except_entered)       \
        Except_pop();                        \
    }                                        \
    else if (Except_frame.exception == &(e)) \
    {                                        \
        Except_flag = Except_handled;

/** 
* @def                                ELSE
* @brief                              ELSE关键字
*/
#define ELSE                           \
    if (Except_flag == Except_entered) \
        Except_pop();                  \
    }                                  \
    else                               \
    {                                  \
        Except_flag = Except_handled;

/** 
* @def                                FINALLY
* @brief                              FINALLY关键字
*/
#define FINALLY                            \
    if (Except_flag == Except_entered)     \
        Except_pop();                      \
    }                                      \
    {                                      \
        if (Except_flag == Except_entered) \
            Except_flag = Except_finalized;

/** 
* @def                                END_TRY
* @brief                              END_TRY关键字
*/
#define END_TRY                        \
    if (Except_flag == Except_entered) \
        Except_pop();                  \
    }                                  \
    if (Except_flag == Except_raised)  \
        RERAISE;                       \
    }                                  \
    while (0)

#else

/** 
* @def                                RAISE(e)
* @brief                              抛出异常
*/
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)

/** 
* @def                                RERAISE
* @brief                              重复抛出异常
*/
#define RERAISE Except_raise(Except_frame.exception, \
    Except_frame.file, Except_frame.line)

/** 
* @def                                RETURN
* @brief                              返回
*/
#define RETURN                                    \
    switch (Except_stack = Except_stack->prev, 0) \
    default:                                      \
        return

/** 
* @def                                TRY
* @brief                              Try关键字
*/
#define TRY                                     \
    do {                                        \
        volatile int Except_flag;               \
        Except_Frame Except_frame;              \
        Except_frame.prev = Except_stack;       \
        Except_stack = &Except_frame;           \
        Except_flag = setjmp(Except_frame.env); \
        if (Except_flag == Except_entered) {

/** 
* @def                                EXCEPT(e)
* @brief                              EXCEPT关键字
*/
#define EXCEPT(e)                            \
    if (Except_flag == Except_entered)       \
        Except_stack = Except_stack->prev;   \
    }                                        \
    else if (Except_frame.exception == &(e)) \
    {                                        \
        Except_flag = Except_handled;

/** 
* @def                                ELSE
* @brief                              ELSE关键字
*/
#define ELSE                               \
    if (Except_flag == Except_entered)     \
        Except_stack = Except_stack->prev; \
    }                                      \
    else                                   \
    {                                      \
        Except_flag = Except_handled;

/** 
* @def                                FINALLY
* @brief                              FINALLY关键字
*/
#define FINALLY                            \
    if (Except_flag == Except_entered)     \
        Except_stack = Except_stack->prev; \
    }                                      \
    {                                      \
        if (Except_flag == Except_entered) \
            Except_flag = Except_finalized;

/** 
* @def                                END_TRY
* @brief                              END_TRY关键字
*/
#define END_TRY                            \
    if (Except_flag == Except_entered)     \
        Except_stack = Except_stack->prev; \
    }                                      \
    if (Except_flag == Except_raised)      \
        RERAISE;                           \
    }                                      \
    while (0)
#endif

#endif