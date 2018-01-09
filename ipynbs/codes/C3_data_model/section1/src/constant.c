
#include <stdio.h>

const float Pi = 3.1415;

int main(void){
    int r;
    float s;
    r = 2;
    s = Pi*r*r;
    printf("半径2m圆的面积为:%f\n",s);

    //Pi = 3.14;
    printf("Pi=%f\n",Pi);
    return 0;
}