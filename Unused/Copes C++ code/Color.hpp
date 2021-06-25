#pragma once

// #include <cmath>
// using std::round;


class Color{
private:
    float round(float var, int digits);
public:
    float r, g, b, a;
    Color() {};
    Color(float r, float g, float b, float a = 1.0f): 
        r(r), 
        g(g), 
        b(b), 
        a(a) 
        {};
    Color(int r, int g, int b, int a = 255):
        r(round(float(r) / 255.0f, 1)), 
        g(round(float(g) / 255.0f, 1)), 
        b(round(float(b) / 255.0f, 1)), 
        a(round(float(a) / 255.0f, 1)) 
        {};

    // bool operator==(const Color& r);
    bool operator!=(const Color& r);
};

/*
unsigned float r, g, b, a;
    Color() {};
    Color(unsigned float r, unsigned float g, unsigned float b, unsigned float a): r(r), g(g), b(b), a(a) {};
    Color(unsigned int   r, unsigned int   g, unsigned int   b, unsigned int   a):
*/