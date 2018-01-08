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
typedef struct {
    float x; ///< 头一位
    float y; ///< 尾一位
} BINARY_VECTOR;

/** 
* @fn float VEC_mod(BINARY_VECTOR)
* @brief 求模 
* @param[vector]   要求模的向量  
* @return  float 模的值
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
extern float VEC_mod(BINARY_VECTOR);

/** 
* @fn BINARY_VECTOR VEC_add(BINARY_VECTOR,BINARY_VECTOR)
* @brief 求两个向量的和
* @param[vector]   求和的第一个向量
* @param[vector]   求和的第二个向量  
* @return  BINARY_VECTOR 相加后的向量
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
extern BINARY_VECTOR VEC_add(BINARY_VECTOR,BINARY_VECTOR);

/** 
* @fn float VEC_mul(BINARY_VECTOR ,BINARY_VECTOR)
* @brief 求积
* @param[vector]   求积的第一个向量
* @param[vector]   求积的第二个向量  
* @return  float 内积的值
* @par 修改日志 
*      XXX于201X-XX-XX创建 
*/
extern float VEC_mul(BINARY_VECTOR ,BINARY_VECTOR);

#endif