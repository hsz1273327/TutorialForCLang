#include <vector>
#include <iostream>
 
int main()
{
    std::vector<int> c { 1,2,3,4,5,6,7 };
    int x = 5;
 
    std::cout << "c: ";
    for (auto i: c) {
        if (i%2 == 0){
            std::cout << i << ' ';
        }    
    }
    std::cout << '\n';
}