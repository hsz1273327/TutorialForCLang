#include <stdio.h>

int main(void){
    int a = 5;
    printf("a=5 是%s\n",(a%2==0? "偶数":"奇数"));
    return 0;
}