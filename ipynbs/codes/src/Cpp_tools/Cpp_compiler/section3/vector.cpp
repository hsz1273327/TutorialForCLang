#include <cmath>
#include "vector.hpp"
Vector::BinaryVector::BinaryVector(){
    x=0.0;
    y=0.0;
}
                
Vector::BinaryVector::BinaryVector(float in_x,float in_y){
    x=in_x;
    y=in_y;
}
Vector::BinaryVector::BinaryVector(int in_x,int in_y){
    x=static_cast<float>(in_x);
    y=static_cast<float>(in_y);
}
Vector::BinaryVector::BinaryVector(int in_x,float in_y){
    x=static_cast<float>(in_x);
    y=in_y;
}
Vector::BinaryVector::BinaryVector(float in_x,int in_y){
    x=in_x;
    y=static_cast<float>(in_y);
}
                
float Vector::BinaryVector::mod(){
    float result = sqrt(x*x+y*y);
    return result;
}
float Vector::BinaryVector::mul(const Vector::BinaryVector & that_vector){
    float result = x*that_vector.x+y*that_vector.y;
    return result;
}
                
    
Vector::BinaryVector Vector::BinaryVector::add(const Vector::BinaryVector & that_vector){
    Vector::BinaryVector result(x+that_vector.x,y+that_vector.y);
    return result;
    
}
                            
float Vector::BinaryVector::operator*(const Vector::BinaryVector& that_vector){
    return mul(that_vector);
}
Vector::BinaryVector Vector::BinaryVector::operator+(const Vector::BinaryVector& that_vector){
    return add(that_vector);
}