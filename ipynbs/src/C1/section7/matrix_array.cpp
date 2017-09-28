#include <stdio.h>
#include <array>


int main(void){
    auto max_elem = 0;
    std::array<std::array<int, 2>, 3> matrix{std::array<int, 2>{1,4},
                                             std::array<int, 2>{2,5},
                                             std::array<int, 2>{3,6}};
    for (const auto& ary : matrix) {
        for (const auto& item : ary) {
            if (item >= max_elem) {
                max_elem = item;
            }
        }
    }
    printf("max element is %d\n",max_elem);
    return 0;
}