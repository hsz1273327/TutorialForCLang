#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;
        
int My_add(int a, int b){  
    return (a + b);  
}  
  
string My_add(string a, string b){  
    return (a + b);  
}  
  
int main(){  
    auto m = 10;  
    auto n = 20;  
    auto ret = My_add(m, n);
    cout << ret << endl;
    string x = "asdfg";

    string y = "qwert";
    string re = My_add(x, y);
    cout << re << endl;
    return 0;  
}  