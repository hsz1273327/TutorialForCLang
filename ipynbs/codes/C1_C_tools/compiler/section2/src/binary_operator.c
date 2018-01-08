#include "binary_vector.h"
#define T BINARY_VECTOR
T  VEC_add(T a,T b){
    BINARY_VECTOR result = {a.x+b.x,a.y+b.y};
    return result;
}
float VEC_mul(T a,T b){
    float result = a.x*b.x+a.y*b.y;
    return result;
}
#undef T