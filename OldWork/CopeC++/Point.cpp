#include "Point.hpp"
#include "Globals.hpp"

namespace g{ extern unsigned int windowWidth, windowHeight; }

void convCenter(Point& p){
    p.x = float(p.x) - float(g::windowWidth)  / 2.0f;
    p.y = float(p.y) - float(g::windowHeight) / 2.0f;
}

std::pair<float, float> getVector(Point p){
    convCenter(p);
    std::pair<float, float> tmp2(float(p.x) / (float(g::windowWidth) / 2.0f), (float(p.y) / (float(g::windowHeight) / 2.0f)) * -1);
    return tmp2;
}

void print(const Point& p, std::string name){
    if (name.size())
        std::cout << g::getDebugCount() << ": " << name << " = (" << p.x << ", " << p.y << ")\n";
    else
        std::cout << g::getDebugCount() << ": (" << p.x << ", " << p.y << ")\n";
}

void convTopLeft(Point& p){
    p.x = float(p.x) + float(g::windowWidth)  / 2.0f;
    p.y = float(p.y) + float(g::windowHeight) / 2.0f;
}

std::pair<float, float> getUnnormalizedVector(Point p){
    std::pair<float, float> tmp(float(p.x), float(p.y) * -1);
    return tmp;
}

bool operator==(const Point& l, const Point& r){
    return l.x == r.x and l.y == r.y;
}

bool operator!=(const Point& l, const Point& r){
    return l.x != r.x or l.y != r.y;
}

/*
#include "Globals.hpp"
#include <iterator>

std::pair<float, float> Point::getVector(){
    Point tmp = this->convCenter();
    std::pair<float, float> tmp2(float(tmp.x) / (float(g::windowWidth) / 2.0f), (float(tmp.y) / (float(g::windowHeight) / 2.0f)) * -1);
    return tmp2;
}

void Point::print(std::string name){
    if (name.size())
        std::cout << g::getDebugCount() << ": " << name << " = (" << x << ", " << y << ")\n";
    else
        std::cout << g::getDebugCount() << ": (" << x << ", " << y << ")\n";
}

Point Point::convCenter(){
    Point tmp(float(this->x) - float(g::windowWidth) / 2.0f, float(this->y) - float(g::windowHeight) / 2.0f);
    return tmp;
}

Point Point::convTopLeft(){
    Point tmp(float(x) + float(g::windowWidth) / 2.0f, float(y) + float(g::windowHeight) / 2.0f);
    return tmp;
}

std::pair<float, float> Point::getUnnormalizedVector(){
    std::pair<float, float> tmp2(float(x), float(y) * -1);
    return tmp2;
}

bool Point::operator==(const Point& r){
    return this->x == r.x and this->y == r.y;
}

bool Point::operator!=(const Point& r){
    return this->x != r.x or this->y != r.y;
}
*/