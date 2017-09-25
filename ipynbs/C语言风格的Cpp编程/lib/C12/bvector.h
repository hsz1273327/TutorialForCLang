#ifndef BVECTOR_H
#define BVECTOR_H//一般是文件名的大写 头文件结尾写上一行


typedef struct {
    float x;
    float y;
} BINARY_VECTOR;

BINARY_VECTOR add(BINARY_VECTOR,BINARY_VECTOR);
float mul(BINARY_VECTOR,BINARY_VECTOR);
float mod(BINARY_VECTOR);

#endif
