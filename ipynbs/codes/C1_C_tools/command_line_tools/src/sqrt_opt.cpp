#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <math.h>
using std::string;
using std::cout;
using std::endl;

int main(int argc, char **argv)  {  
    int ch; 
    double result;
    opterr = 0;  
    string version = "0.1.0";
    while ((ch = getopt(argc,argv,"s:v\n"))!=-1){  
        switch(ch){  
            case 'v':  
                cout<<("option v: %s\n",version)<<endl;  
                break;  
            case 's':
                result = sqrt(atoi(optarg));
                cout<<("%f\n",result)<<endl;
                break;
            default:  
                cout<<"wrong args\n"<<endl;  
        }  
    }  
    return 0;
}  