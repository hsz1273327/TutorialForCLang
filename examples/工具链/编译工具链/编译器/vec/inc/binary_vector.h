/* $Id$ */
#ifndef BINARY_VECTOR_H
#define BINARY_VECTOR_H//一般是文件名的大写 头文件结尾写上一行

struct BINARY_VECTOR {
    float x;
    float y;
};

typedef struct BINARY_VECTOR *BINARY_VECTOR_P;

BINARY_VECTOR_P VEC_new(void);
BINARY_VECTOR_P VEC_init(float x,float y);
void VEC_del(BINARY_VECTOR_P);

float VEC_mod(BINARY_VECTOR_P);
BINARY_VECTOR_P VEC_add(BINARY_VECTOR_P,BINARY_VECTOR_P);
float VEC_mul(BINARY_VECTOR_P ,BINARY_VECTOR_P);


#endif