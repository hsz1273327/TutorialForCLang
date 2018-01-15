/** 
* # 原子
* @file                              atom.h
* @brief                             原子实现
* @detial  实现了一个原子结构,原子是一个指针,指向一个唯一的不可变的序列,
* 序列包含0个或者多个字节,多数原子都指向0结尾的字符串,但也可以指向任一字
*节序列的指针.任意原子都只会出现一次.
* 原子的一个用处通过比较两个指针就可以比较两个字节需略是否相等.
* 另一优点是节省空间,因为任意序列都只会出现一次.因此经常被用作数据结构中的键.
* 原子创建后它在整个执行时间内都存在.
* + 清空栈
* + 入栈
* + 出栈
* @par 依赖  assert.h
*            memory.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#ifndef ATOM_INCLUDED
#define ATOM_INCLUDED

/** 
* @fn              int   Atom_length(const char *str)
* @param[str]      const char *    一个原子
* @return          int             返回参数原子长度
* @brief   通过输入一个原子查找序列并获得长度
*/
extern       int   Atom_length(const char *);

/** 
* @fn             const char *Atom_new   (const char *str, int len)
* @param[str]      const char *       指向字节序列的指针
* @param[len]      int                字节序列的长度
* @return          const char *       返回字节序列的原子
* @brief   为一个字节序列创建的原子
* @detail 这个函数不会返回NULL指针,且原子总是以0字符结束,Atom_new在必要时会添加0字符
*/
extern const char *Atom_new   (const char *, int);

/** 
* @fn              const char *Atom_string(const char *str)
* @param[str]      const char *        字符串
* @return          const char *        返回字符串的原子
* @brief   为一个字符串创建序列,字符串必须以0字符结尾
*/
extern const char *Atom_string(const char *);

/** 
* @fn             const char *Atom_int   (long n)
* @param[n]       long
* @return         const char *         返回栈实例  
* @brief   返回以字符串表示的长整数的原子
*/
extern const char *Atom_int   (long);
#endif