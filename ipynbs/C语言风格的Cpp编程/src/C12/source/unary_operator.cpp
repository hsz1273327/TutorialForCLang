#include "unary_operator.h"
float mod(BINARY_VECTOR a){
    float result = sqrt(a.x*a.x+a.y*a.y);
    return result;
}