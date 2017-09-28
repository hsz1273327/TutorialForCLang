#include <iostream>

using std::cout;
using std::endl;
        
int main(void){
    auto a = 5,b = 5;
    a++;
    ++b;
    cout<<"a:"<<a<<"b:"<<b<<"\n"<<endl;
    a = b++;
    cout<<"after a=b++\na:" << a << ",b:" <<b<<"\n"<<endl;
    a = ++b;
    cout<<"after a=++b\na:" << a << ",b:" <<b<<"\n"<<endl;
    return 0;
}