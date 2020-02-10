/* $Id$ */
/** 
* @file         binary_vector.h
* @brief        binary_vector接口文件
* @author       author 
* @date     2018-1-8
* @version  dev
* @par Copyright (c):  
*       XXX公司 
* @par History:          
*   version: author, date, desc\n 
*/
#ifndef BINARY_VECTOR_H
#define BINARY_VECTOR_H//一般是文件名的大写 头文件结尾写上一行
/** 
* @struct       BINARY_VECTOR
* @brief        vector结构
*/  
struct BINARY_VECTOR {
    float x;
    float y;
};


typedef struct BINARY_VECTOR *BINARY_VECTOR_P;

/** 
* @fn BINARY_VECTOR_P VEC_new(void)
* @brief 实例化一个BINARY_VECTOR结构体
* @return  BINARY_VECTOR_P 新结构体对象的指针
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
BINARY_VECTOR_P VEC_new(void);

/** 
* @fn BINARY_VECTOR_P VEC_init(float,float)
* @brief 带初始化的实例化一个BINARY_VECTOR结构体
* @param[x] x值
* @param[y] y值
* @return  BINARY_VECTOR_P 新结构体对象的指针 
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
BINARY_VECTOR_P VEC_init(float x,float y);

/** 
* @fn void VEC_del(BINARY_VECTOR_P)
* @brief 释放一个结构体对象
* @param[vector]   要释放的结构体对象
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
void VEC_del(BINARY_VECTOR_P);

/** 
* @fn float VEC_mod(BINARY_VECTOR_P)
* @brief 求模 
* @param[vector]   要求模的向量  
* @return  float 模的值
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
float VEC_mod(BINARY_VECTOR_P);
/** 
* @fn BINARY_VECTOR_P VEC_add(BINARY_VECTOR_P,BINARY_VECTOR_P)
* @brief 求两个向量的和
* @param[vector]   求和的第一个向量
* @param[vector]   求和的第二个向量  
* @return  BINARY_VECTOR 相加后的向量
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
BINARY_VECTOR_P VEC_add(BINARY_VECTOR_P,BINARY_VECTOR_P);
/** 
* @fn float VEC_mul(BINARY_VECTOR_P ,BINARY_VECTOR_P)
* @brief 求积
* @param[vector]   求积的第一个向量
* @param[vector]   求积的第二个向量  
* @return  float 内积的值
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
float VEC_mul(BINARY_VECTOR_P ,BINARY_VECTOR_P);
#endif