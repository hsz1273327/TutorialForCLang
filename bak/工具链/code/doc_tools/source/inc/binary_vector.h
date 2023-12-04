#ifndef BINARY_VECTOR_H
#define BINARY_VECTOR_H//一般是文件名的大写 头文件结尾写上一行

#define T BINARY_VECTOR

typedef struct {
    float x;
    float y;
} T;

extern float VEC_mod(T);
extern T VEC_add(T,T);
extern float VEC_mul(T ,T);

#undef T

#endif