#include <stdio.h>
#include "vector.hpp"

using Vector::BinaryVector;

int main(){
    BinaryVector A(1,2),B(3,4);
    printf("mod(A)=%f\n",A.mod());
    printf("mod(B)=%f\n",B.mod());
    printf("mul(A,B)=%f\n",A*B);
    BinaryVector result = A+B;
    printf("add(A,B)=<%f,%f>\n",result.x,result.y);
}