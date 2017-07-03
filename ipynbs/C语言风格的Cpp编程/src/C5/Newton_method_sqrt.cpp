

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
const double EPS  = 0.0000001;

double newton_method(double a,int n){
    double x=0,last_x=0;
    if (a>0){
        x = a/n;
        for(int i = 0;i < 500;i++){
            last_x = x;
            x = last_x - last_x*(1-a*pow(last_x,-n))/n;
            if(fabs(last_x - x) < EPS){
                break;
            }
        }
        return x;
    } else if (a == 0){
        return 0;
    } else if (a == 1){
        return 1;
    } else{
        return -1;
    }

}

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("参数错误,参数有%d个,请传入要开方的数和开多少次方\n",argc-1);
        return 0;
    }else{
        double a= atof(argv[1]),result = 0;
        int n = atoi(argv[2]);
        result = newton_method(a,n);
        if (result < 0){
            printf("%lf无法在实数范围内求开方\n",a);
            return 0;
        }else{
            printf("%lf的%d次方根是%lf\n",a,n,result);
            return 0;
        }
    }
}