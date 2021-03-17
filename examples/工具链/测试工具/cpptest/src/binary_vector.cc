#include "binary_vector.h"
#include <cmath>


namespace vector{
    float BinaryVector::mod(){
        auto result = std::sqrt(std::power(a->x,2)+std::power(a->y,2));
        return result;
    }
    BinaryVector BinaryVector::add(BinaryVector* that){
        auto x = a->x + b->x;
        auto y = a->y + b->y;
        BinaryVector b(x,y);
        return b;
    }
    float BinaryVector::mul(BinaryVector* that){
        float result = a->x * b->x + a->y * b->y;
        return result;
    }
}
