
#include <stdio.h>
#include <string.h>
typedef struct{
    char name[10];
    char sex[2];
    int age;
    float weight;
    float height;
} Child;

int main(void) {
    Child Tom = {"Tom","m",8,48.5,1.5};
    Child Sam;
    strcpy(Sam.name,"Sam");
    strcpy(Sam.sex,"m");
    Sam.age = 7;
    Sam.weight = 45.7;
    Sam.height = 1.57;
    printf("%s 身高%fm\n",Tom.name,Tom.height);
    printf("%s 体重%fkg\n",Sam.name,Sam.weight);
    return 0;
}