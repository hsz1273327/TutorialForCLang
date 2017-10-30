#include <stdio.h>
#include "binary_vector.hpp"
#include "binary_operator.hpp"
#include "unary_operator.hpp"

int main(void){
    Vector::BINARY_VECTOR A = {1,2},B = {3,4};
    printf("mod(A)=%f\n",Vector::mod(A));
    printf("mod(B)=%f\n",Vector::mod(B));
    printf("mul(A,B)=%f\n",Vector::mul(A,B));
    Vector::BINARY_VECTOR result = Vector::add(A,B);
    printf("add(A,B)=<%f,%f>\n",result.x,result.y);
}