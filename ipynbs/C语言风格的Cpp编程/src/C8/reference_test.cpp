#include<iostream>
using std::cout;
using std::endl;
        
int main(){
    int a=5;
    int &b=a;
    b=6;
    cout<<"a="<<a<<",b="<<b<<endl;//a=6,b=6
    int c=7;
    b=c;
    cout<<"a="<<a<<",b="<<b<<endl;//a=7,b=7
    return 0;
}