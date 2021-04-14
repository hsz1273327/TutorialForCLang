#include "binary_vector.hpp"
#include <cmath>


namespace vector{
    float BinaryVector::mod(){
        auto result = std::sqrt(std::pow(this->x,2)+std::pow(this->y,2));
        return result;
    }
    BinaryVector BinaryVector::add(const BinaryVector& that){
        auto x = this->x + that.x;
        auto y = this->y + that.y;
        BinaryVector b(x,y);
        return b;
    }
    float BinaryVector::mul(const BinaryVector& that){
        float result = this->x * that.x + this->y * that.y;
        return result;
    }
}
