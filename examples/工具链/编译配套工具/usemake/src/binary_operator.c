#include "binary_vector.h"
#define T BINARY_VECTOR_P
T VEC_add(T a, T b) {
    float x = a->x + b->x;
    float y = a->y + b->y;
    T result;
    result = VEC_new();
    result->x = x;
    result->y = y;
    return result;
}

float VEC_mul(T a, T b) {
    float result = a->x * b->x + a->y * b->y;
    return result;
}
#undef T