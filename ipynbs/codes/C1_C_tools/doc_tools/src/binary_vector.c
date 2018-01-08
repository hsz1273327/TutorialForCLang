#include "binary_vector.h"
#include <math.h>
#define T BINARY_VECTOR
float VEC_mod(T a){
    float result = sqrt(a.x*a.x+a.y*a.y);
    return result;
}

T  VEC_add(T a,T b){
    BINARY_VECTOR result = {a.x+b.x,a.y+b.y};
    return result;
}

float VEC_mul(T a,T b){
    float result = a.x*b.x+a.y*b.y;
    return result;
}
#undef T