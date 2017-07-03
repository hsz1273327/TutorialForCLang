
#include <stdio.h>
#include <math.h>
const double N  = 2.0;

double sqrt_my(double a,double EPS){
    double x,last_x;
    x = a/2;
    do{
        last_x = x;
        x = (last_x + a/last_x)/2;
    }while(fabs(last_x - x) > EPS);
    return x;
}

int main(void){
    printf("根号2是%lf\n",sqrt_my(N,0.0001));
    return 0;
}