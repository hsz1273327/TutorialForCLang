#include <iostream>
using std::cout;
using std::endl;
        
void F(int a){  
    cout<<a<<endl;  
}  
 
void F(int *p){  
    assert(p != NULL);  
 
    cout<< p <<endl;  
}  
 
int main(){  
 
    int *p = nullptr;  
    int *q = NULL;  
    bool equal = ( p == q ); // equal的值为true，说明p和q都是空指针  
    int a = nullptr; // 编译失败，nullptr不能转型为int  
    F(0); // 在C++98中编译失败，有二义性；在C++11中调用F（int）  
    F(nullptr);  
 
    return 0;  
} 