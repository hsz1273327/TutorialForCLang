#include "unary_operator.hpp"
float Vector::mod(Vector::BINARY_VECTOR a){
    float result = sqrt(a.x*a.x+a.y*a.y);
    return result;
}