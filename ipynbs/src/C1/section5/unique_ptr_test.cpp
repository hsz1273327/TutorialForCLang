#include <iostream>
#include <string>
#include <memory>
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
int main(){
    //智能指针的创建 
    unique_ptr<int> u_i; //创建空智能指针
    u_i.reset(new int(3)); //绑定动态对象  
    cout<< *u_i << endl;
    unique_ptr<int> u_i2(new int(4));//创建时指定动态对象  
    cout<< *u_i2 << endl;
    //所有权的变化  
    int *p_i = u_i2.release(); //释放所有权  
    
    unique_ptr<string> u_s(new string("abc"));  
    cout<< *u_s << endl;
    unique_ptr<string> u_s2 = std::move(u_s); //所有权转移(通过移动语义)，u_s所有权转移后，变成“空指针”  
    cout<< *u_s2 << endl;
    u_s2=nullptr;//显式销毁所指对象，同时智能指针变为空指针。与u_s2.reset()等价  
}