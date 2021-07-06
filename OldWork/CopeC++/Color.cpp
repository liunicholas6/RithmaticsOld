#include "Color.hpp"
#include <string>
#include <system_error>

float Color::round(float var, int digits) {
    // 37.66666 * 100 =3766.66 
    // 3766.66 + .5 =3767.16    for rounding off value 
    // then type cast to int so value is 3767 
    // then divided by 100 so the value converted into 37.67 
    float value = int((var * (digits * 10) + .5));
    return float(value / (digits * 10));
}

// bool Color::operator==(const Color& r){
//     return this->r == r.r and this->g == r.g and this->b == r.b and this->a == r.a;
// }

bool Color::operator!=(const Color& r){
    return this->r != r.r or this->g != r.g or this->b != r.b or this->a != r.a;
}