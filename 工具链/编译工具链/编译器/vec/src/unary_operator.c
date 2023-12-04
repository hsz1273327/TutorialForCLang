#include "binary_vector.h"
#include <math.h>
#define T BINARY_VECTOR_P
float VEC_mod(T a){
    float result = sqrt(a->x*a->x+a->y*a->y);
    return result;
}
#undef T