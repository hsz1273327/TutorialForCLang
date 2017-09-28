#include <stdio.h>


int main(void){
    int max_elem = 0;
    int matrix[3][2] = {{1,4},{2,5},{3,6}};
    for (int i = 0;i<3;i++){
        for (int j = 0;j<2;j++){
            if (matrix[i][j]>=max_elem) {
                max_elem = matrix[i][j];
            }
        }
    }
    printf("max element is %d\n",max_elem);
    return 0;
}
