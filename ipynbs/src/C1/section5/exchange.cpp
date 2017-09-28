#include <stdio.h>

void exchange(int* ptr_a,int* ptr_b){
    int temp;
    temp = *ptr_a;
    *ptr_a = *ptr_b;
    *ptr_b = temp;
    return;
}

int main(void) {
    int a = 78,b = 28;
    printf("before exchange a=%d,b=%d\n",a,b);
    exchange(&a,&b);
    printf("after exchange a=%d,b=%d\n",a,b);
    return 0;
}