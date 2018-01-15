/** 
* @file                              arith.h
* @brief                             整型数值计算库
* @detail  这个库可以单独使用,可以用于一些哈希表的计算
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/

/* $Id$ */
#ifndef ARITH_INCLUDED
#define ARITH_INCLUDED

/** 
* @fn     int Arith_max(int x, int y)
* @brief                               比较两个整数的大小,返回大的那个
* @param [x]                 int       比较的整数  
* @param [y]                 int       比较的整数  
* @return                    int       返回大的那个
*/
extern int Arith_max(int, int);

/** 
* @fn     int Arith_min(int x, int y)
* @brief                               比较两个整数的大小,返回小的那个
* @param [x]                 int       比较的整数   
* @param [y]                 int       比较的整数
* @return                    int       返回小的那个
*/
extern int Arith_min(int, int);

/** 
* @fn     int Arith_div(int x, int y)
* @brief                               整数除法,返回不超过实数z的最大整数
* @param [x]                 int       除数   
* @param [y]                 int       被除数
* @return                    int       返回超过实数z的最大整数
*/
extern int Arith_div(int x, int y);

/** 
* @fn     int Arith_mod(int x, int y)
* @brief                               整数除法取余数
* @param [x]                 int       除数   
* @param [y]                 int       被除数
* @return                    int       x-y*Arith_div(x,y)
*/
extern int Arith_mod(int x, int y);

/** 
* @fn     int Arith_max(int x, int y)
* @brief                               整数除法,返回不小于x/y的实数商的最小整数
* @param [x]                 int       除数   
* @param [y]                 int       被除数
* @return                    int       返回不小于x/y的实数商的最小整数
*/
extern int Arith_ceiling(int x, int y);

/** 
* @fn     int Arith_max(int x, int y)
* @brief                               整数除法,返回不大于x/y的实数商的最大整数
* @param [x]                 int       除数   
* @param [y]                 int       被除数
* @return                    int       返回不大于x/y的实数商的最大整数
*/
extern int Arith_floor  (int x, int y);