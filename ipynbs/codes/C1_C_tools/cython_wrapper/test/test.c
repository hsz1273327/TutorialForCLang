#include <stdio.h>
#include "binary_vector.h"


int main(void){
    BINARY_VECTOR_P A = VEC_new();
    A->x = 1;
    A->y = 2;
    BINARY_VECTOR_P B = VEC_new();
    B->x = 3;
    B->y = 4;
    printf("mod(A)=%f\n",VEC_mod(A));
    printf("mod(B)=%f\n",VEC_mod(B));
    printf("mul(A,B)=%f\n",VEC_mul(A,B));
    BINARY_VECTOR_P result = VEC_add(A,B);
    printf("add(A,B)=<%f,%f>\n",result->x,result->y);
    VEC_del(A);
    VEC_del(B);
    VEC_del(result);
}