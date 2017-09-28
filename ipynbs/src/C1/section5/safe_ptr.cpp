#include <stdio.h>

int main(void) {
    const int a = 78,b = 28;
    int c = 18;
    const int* p = &a;
    printf("p:%d\n",*p);
    //*p = 58//报错,*p无法重新复制
    p = &b;//可以给p重新复制,
    printf("p:%d\n",*p);
    return 0;
}