#include <iostream>
#include <map>
#include <functional>
using namespace std;

// 普通函数
int add(int i, int j) { return i + j; }
// lambda表达式
auto mod = [](int i, int j){return i % j; };
// 函数对象类
struct divide{
    int operator() (int denominator, int divisor){
        return denominator / divisor;
    }
};

int main(int argc, char *argv[]){
    map<char, function<int(int, int)>> binops = {
        { '+', add },
        { '-', minus<int>() },
        { '*', [](int i, int j){return i - j; } },
        { '/', divide() },
        { '%', mod }
    };
    cout << binops['+'](10, 5) << endl;
    cout << binops['-'](10, 5) << endl;
    cout << binops['*'](10, 5) << endl;
    cout << binops['/'](10, 5) << endl;
    cout << binops['%'](10, 5) << endl;
    system("pause");
    return 0;
}