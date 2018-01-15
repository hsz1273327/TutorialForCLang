/** 
* # 栈
* @file                              stack.h
* @brief                             栈实现
* @detial  实现了一个栈结构,操作有:
* + 清空栈
* + 入栈
* + 出栈
* @par 依赖  except.h
*            assert.h
*            memory.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#ifndef STACK_INCLUDED
#define STACK_INCLUDED

// ## 定义的结构

/** 
* @struct                             Exception_T
* @brief                              异常结构
*/
struct Stack_S;

typedef struct Stack_S *Stack_T;

/** 
* @fn              T     Stack_new  (void)
* @return           Stack_T         返回栈实例  
* @brief   实例化一个栈
*/
extern Stack_T     Stack_new  (void);

/** 
* @fn     int   Stack_isempty(Stack_T)
* @brief  检查是否为空栈
* @params[stk]                  Stack_T            要清空的栈实例  
* @return                       int                返回 0,1也就是true/false
*/
extern int   Stack_isempty(Stack_T);

/** 
* @fn     void  Stack_push (Stack_T, void *)
* @brief 入栈
* @params[stk]                  Stack_T            对象要入的栈  
* @params[x]                    void*              要入栈的对象
*/
extern void  Stack_push (Stack_T, void *);

/** 
* @fn     void *Stack_pop  (T stk)
* @brief 出栈
* @params[stk]                  Stack_T            要推出内容的栈 
* @return                       void*              返回出栈对象的指针 
*/
extern void *Stack_pop  (Stack_T);

/** 
* @fn    void  Stack_free (T *)
* @brief 抛出异常
* @params[stk]                        Stack_T*     要推出内容的栈 
*/
extern void  Stack_free (Stack_T *);

#endif