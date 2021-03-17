/* $Id$ */
#ifndef BINARY_VECTOR_H
#define BINARY_VECTOR_H//一般是文件名的大写 头文件结尾写上一行

namespace vector{
    class BinaryVector{
        public:
            float x;
            float y;
            BinaryVector(float x,float y): x(x),y(y) {};
            float mod();
            BinaryVector add(BinaryVector*);
            float mul(BINARY_VECTOR_P*);
    };
}

#endif