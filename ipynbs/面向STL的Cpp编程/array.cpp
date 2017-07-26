#include<iostream>
#include<array>
using namespace std;
int main(){
    array<int,5> myarray={1,2,3,4,5};  
    cout <<"myarray="<<endl;  
    for (int x:myarray){  
        cout << x <<'\t';  
    }  
    cout << endl;
}