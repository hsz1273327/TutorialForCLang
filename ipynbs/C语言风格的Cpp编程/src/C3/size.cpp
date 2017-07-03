#include <stdio.h>

int main(void){
    long long a = 5;
    printf("a=5 占用%lu位\n",(sizeof(a)));
    return 0;
}