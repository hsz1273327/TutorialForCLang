#include <iostream>
#include "helloworld.hpp"
using std::cout;
using std::endl;
using Hello::hello_name;
int main(){
    cout << hello_name("Cpp!") <<endl;
    return 0;
}