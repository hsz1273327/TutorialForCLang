
#include <stdio.h>

int main(void){
    int a = 5,b = 5;
    a++;
    ++b;
    printf("a:%d,b:%d\n",a,b);
    a = b++;
    printf("after a=b++\na:%d,b:%d\n",a,b);
    a = ++b;
    printf("after a=++b\na:%d,b:%d\n",a,b);
    return 0;
}