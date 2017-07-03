#include <stdio.h>
#include "bvector.h"


int main(void){
    BINARY_VECTOR A = {1,2},B = {3,4};
    printf("mod(A)=%f\n",mod(A));
    printf("mod(B)=%f\n",mod(B));
    printf("mul(A,B)=%f\n",mul(A,B));
    BINARY_VECTOR result = add(A,B);
    printf("add(A,B)=<%f,%f>\n",result.x,result.y);
}