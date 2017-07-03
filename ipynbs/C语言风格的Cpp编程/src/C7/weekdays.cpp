
#include <stdio.h>

enum Weekday {MON,TUE,WED,THU,FRI,SAT,SUN};

int main(void) {
    enum Weekday firstday = TUE;
    printf("first day is %d\n",firstday );
    return 0;
}