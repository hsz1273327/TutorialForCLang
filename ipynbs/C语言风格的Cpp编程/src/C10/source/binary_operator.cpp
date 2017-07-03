
#include "binary_operator.h"
BINARY_VECTOR add(BINARY_VECTOR a,BINARY_VECTOR b){
    BINARY_VECTOR result = {a.x+b.x,a.y+b.y};
    return result;
}
float mul(BINARY_VECTOR a,BINARY_VECTOR b){
    float result = a.x*b.x+a.y*b.y;
    return result;
}