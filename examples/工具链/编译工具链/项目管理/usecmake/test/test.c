#include <stdio.h>
#include "binary_vector.h"


int main(void){
    struct BINARY_VECTOR A = {1,2},B = {3,4};
    printf("mod(A)=%f\n",VEC_mod(&A));
    printf("mod(B)=%f\n",VEC_mod(&B));
    printf("mul(A,B)=%f\n",VEC_mul(&A,&B));
    BINARY_VECTOR_P result = VEC_add(&A,&B);
    printf("add(A,B)=<%f,%f>\n",result->x,result->y);
}