
#include <iostream>
using namespace std;

struct Divide{
    int operator() (int denominator, int divisor){
        return denominator / divisor;
    }
};

class Add{
    public:  
    int operator() (int x, int y){
        return x + y;
    }
};

int main(int argc, char *argv[]){
    //Add add;
    //struct Divide divide;
    cout << Add()(10, 5) << endl;
    cout << Divide()(10, 5) << endl;
    return 0;
}