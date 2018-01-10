/** 
* # 栈
* @file                              stack.h
* @brief                             栈实现
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#ifndef STACK_INCLUDED
#define STACK_INCLUDED
#define T Stack_T

// ## 定义的结构

/** 
* @struct                             Exception_T
* @brief                              异常结构
*/
typedef struct T *T;

/** 
* @fn              T     Stack_new  (void)
* @return           Stack_T         返回栈实例  
* @brief   实例化一个栈
*/
extern T     Stack_new  (void);

/** 
* @fn     int   Stack_empty(Stack_T)
* @brief 清空栈
* @params[stk]                  Stack_T            要清空的栈实例  
* @return                       Stack_T            返回    
*/
extern int   Stack_empty(T);

/** 
* @fn     void  Stack_push (Stack_T, void *)
* @brief 入栈
* @params[stk]                  Stack_T            对象要入的栈  
* @params[x]                    void*              要入栈的对象
*/
extern void  Stack_push (T, void *);

/** 
* @fn     void *Stack_pop  (T stk)
* @brief 出栈
* @params[stk]                  Stack_T            要推出内容的栈 
* @return                       void*              返回出栈对象的指针 
*/
extern void *Stack_pop  (T);

/** 
* @fn    void  Stack_free (T *)
* @brief 抛出异常
* @params[stk]                        Stack_T*     要推出内容的栈 
*/
extern void  Stack_free (T *);
#undef T
#endif