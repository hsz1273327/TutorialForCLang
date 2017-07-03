#include <iostream>
#include <vector>
#include <math.h>
using std::cout;
using std::endl;
using std::vector;

int quadratic_sum(vector<int> &list){
    auto sum=0;
    auto quadratic = [](int x)->decltype(sum){return pow(x,2);};

    for(auto i :list){
        sum += quadratic(i);
    }
    return sum;
}

int main(){
    vector<int> val = {1,2,3,4,5};
    cout<< quadratic_sum(val) <<endl;
    return 0;
}