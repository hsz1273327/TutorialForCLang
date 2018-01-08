#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
const char * version = "0.1.0";

int main(int argc, char **argv)  {  
    int ch; 
    double result;
    opterr = 0;
    while ((ch = getopt(argc,argv,"s:v\n"))!=-1){  
        switch(ch){  
            case 'v':
                printf("option v: %s\n",version);
                break;  
            case 's':
                result = sqrt(atoi(optarg));
                printf("%f\n",result);
                break;
            default:
                printf("wrong args\n");
        }  
    }  
    return 0;
}  