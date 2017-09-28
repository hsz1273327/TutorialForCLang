#include <iostream>

using std::cout;
using std::endl;
        
int main(void){
    auto a = 5;
    cout<<"a=5 是"<<(a%2==0? "偶数":"奇数")<<"\n"<<endl;
    return 0;
}