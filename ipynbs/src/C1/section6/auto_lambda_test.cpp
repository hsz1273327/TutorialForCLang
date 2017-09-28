
#include <iostream>
#include <string>

using std::string;
 
int main(){
    string s1 = "123";
    string s2 = "123";
    auto func = [](auto x,auto y){return x+y;};
    std::cout << "func1: " << func(6,4) << '\n'<< std::endl; 
    std::cout << "func2: " << func(6,0.5) << '\n'<< std::endl; 
    std::cout << "func2: " << func(1.5,0.5) << '\n'<< std::endl; 
    std::cout << "func2: " << func(s1,s2) << '\n'<< std::endl; 
}