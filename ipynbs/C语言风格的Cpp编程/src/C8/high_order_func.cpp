
#include <stdio.h>
#include <math.h>

int sum_int(int a,int b){
    return a>b ?0:a + sum_int((a+1),b);
}

int cube(int x){
    return (int)pow(x,3);
}
int sum_func(int a,int b,int (*const func)(int)){
    return a>b ?0:(*func)(a) + sum_func((a+1),b,func);
}

int sum_cube(int a,int b){
    int (*const ptr_func)(int) = cube;
    return sum_func(a,b,ptr_func);
}
float sum_func_stp(int a,int b,float (*const func)(int) , int (*const step)(int)){
    return a>b ?0:(*func)(a) + sum_func_stp((*step)(a),b,func,step);
}
float func(int x){
    return 1.0/(x*(x+2));
}
int stp(int x){
    return x+4;
}
float sum_pi(int a,int b){
    float (*const ptr_func)(int) = func;
    int (*const ptr_stp)(int) = stp;
    return sum_func_stp(a,b,ptr_func,ptr_stp);
}


int main(void) {
    int top = 5,bottom = 1;
    printf("range(%d,%d) = %d\n", bottom,top,sum_int(bottom,top));
    printf("range(%d,%d)的立方和为%d\n", bottom,top,sum_cube(bottom,top));
    printf("sum_pi(%d,%d)为%f\n", bottom,top,sum_pi(bottom,top));
}