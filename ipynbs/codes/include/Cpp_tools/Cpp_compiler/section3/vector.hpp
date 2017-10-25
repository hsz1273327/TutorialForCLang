#ifndef BINARY_VECTOR_HPP
#define BINARY_VECTOR_HPP//一般是文件名的大写 头文件结尾写上一行
namespace Vector{
class BinaryVector{
public:
    float x;
    float y;
    
    BinaryVector();
    BinaryVector(float,float);
    BinaryVector(int,float);
    BinaryVector(int,int);
    BinaryVector(float,int);
    
    float mod();
    float mul(BinaryVector&);
    BinaryVector add(BinaryVector&);
    
    float operator*(BinaryVector&);
    BinaryVector operator+(BinaryVector&);
};
}

#endif