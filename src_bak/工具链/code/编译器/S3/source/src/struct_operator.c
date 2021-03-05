#include "binary_vector.h"
#include <stdlib.h>
#define T BINARY_VECTOR_P
T VEC_new(void){
    T ptr;
    ptr = (T) malloc(((long)sizeof *(ptr)));
    ptr->x = 0.0;
    ptr->y = 0.0;
    return ptr;
}

T VEC_init(float x,float y){
    T ptr;
    ptr = (T) malloc(((long)sizeof *(ptr)));
    ptr->x = x;
    ptr->y = y;
    return ptr;
}

void VEC_del(T ptr){
    if (ptr) free(ptr);
}
#undef T