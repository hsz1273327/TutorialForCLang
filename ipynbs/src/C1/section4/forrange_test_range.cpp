#include <vector>
#include <iostream>
#include "range.hpp"
 
    
using util::lang::range;
                
int main()
{
 
    std::cout << "step1: ";
    for (auto i: range(1,10)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    
    std::cout << "step3: ";
    for (auto i: range(1,10).step(3)) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}