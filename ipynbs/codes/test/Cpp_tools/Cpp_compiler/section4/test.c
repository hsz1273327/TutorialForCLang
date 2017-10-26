#include <stdio.h>
#include "vector.h"

int main(){
    Vector__BinaryVector * A = Vector__BinaryVector_GetInstance();
    Vector__BinaryVector * B = Vector__BinaryVector_GetInstance();
    Vector__BinaryVector_setInstance(A,1.0,2.0);
    Vector__BinaryVector_setInstance(B,3.0,4.0);
    printf("mod(A)=%f\n",Vector__BinaryVector_mod(A));
    printf("mod(B)=%f\n",Vector__BinaryVector_mod(B));
    printf("mul(A,B)=%f\n",Vector__BinaryVector_mul(A,B));
    Vector__BinaryVector * result = Vector__BinaryVector_add(A,B);
    //printf("add(A,B)=<%f,%f>\n",Vector__BinaryVector_getInstanceX(result),Vector__BinaryVector_getInstanceY(result));
}