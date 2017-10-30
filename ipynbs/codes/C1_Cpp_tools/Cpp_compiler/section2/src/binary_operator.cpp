#include "binary_operator.hpp"
Vector::BINARY_VECTOR Vector::add(Vector::BINARY_VECTOR a,Vector::BINARY_VECTOR b){
    Vector::BINARY_VECTOR result = {a.x+b.x,a.y+b.y};
    return result;
}
float Vector::mul(Vector::BINARY_VECTOR a,Vector::BINARY_VECTOR b){
    float result = a.x*b.x+a.y*b.y;
    return result;
}