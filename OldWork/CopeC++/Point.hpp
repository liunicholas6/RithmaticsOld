#pragma once

#include <utility>
// #include <set>
#include <tuple>
#include <string>

// typedef std::pair<int, int> Point;
// using std::pair<int, int>.first = 



// now we can use x_pos and y_pos instead of pair_name.first and pair_name.second
// Another benefit with this is if you ever change the function to return a tuple tie() will also work with that.

// With C++17 we now have structured bindings which allow you to declare and bind multiple variables to the return of the function.
// This work with arrays, tuple/pair like objects and struct/classes (as long as they meet some requirments). 
// Using structured bindings in this case lets use convert the above example into

// auto [x_pos, y_pos] = function_that_returns_pair_of_cords();
// You can also do

// auto& [x_pos, y_pos] = cords;

// int x = 0, y = 0;

// std::pair<int, int> pointFunc(){
//     return std::pair<int, int>(x, y);
// }

// std::tie(x, y) = pointFunc();

// auto& [x, y] = pointFunc();

class Point{
public:
    int x, y;
    bool isNull = false;

    Point():
        x(0), 
        y(0),
        isNull(true) 
        {};
    Point(int x, int y): 
        x(x), 
        y(y),
        isNull(false)
        {};

};

std::pair<float, float> getVector(Point p);
std::pair<float, float> getUnnormalizedVector(Point p);

void convCenter(Point& p);
void convTopLeft(Point& p);

void print(const Point& p, std::string name = "");

bool operator==(const Point& l, const Point& r);
bool operator!=(const Point& l, const Point& r);


/*
struct x{
    template<class Derived, class Type, std::size_t N>
    struct getter {
        Type& x() & {
            return std::get<N>(static_cast<Derived&>(*this)); 
        }
        Type&& x() && { 
            return std::get<N>(static_cast<Derived&&>(*this));
        }
        const Type& x() const & { 
            return std::get<N>(static_cast<const Derived&>(*this));
        }
        const Type&& x() const && { 
            return std::get<N>(static_cast<const Derived&&>(*this)); 
        }
    };
};

struct y{
    template<class Derived, class Type, std::size_t N>
    struct getter {
        Type& y() & { 
            return std::get<N>(static_cast<Derived&>(*this)); 
        }
        Type&& y() && { 
            return std::get<N>(static_cast<Derived&&>(*this)); 
        }
        const Type& y() const & { 
            return std::get<N>(static_cast<const Derived&>(*this)); 
        }
        const Type&& y() const && { 
            return std::get<N>(static_cast<const Derived&&>(*this)); 
        }
    };
};

template<class, class, class...> struct collect;
template<class Derived, std::size_t... Ns, class... Tags>
struct collect<Derived, std::index_sequence<Ns...>, Tags...>
      : Tags::template getter<Derived, std::tuple_element_t<Ns, Derived>, Ns>...{};

template<class Base, class... Tags>
struct tagged : Base, collect<tagged<Base, Tags...>, 
                              std::index_sequence_for<Tags...>, Tags...> {
    using Base::Base;
    // extra polish for swap and converting from other tagged's.
};

namespace std
{
    template<typename Base, typename...Tags>
    struct tuple_size<tagged<Base, Tags...>>
      : tuple_size<Base>
    {};

    template<size_t N, typename Base, typename...Tags>
    struct tuple_element<N, tagged<Base, Tags...>>
      : tuple_element<N, Base>
    {};
}

using Point = tagged<std::pair<int, int>, x, y>;
*/


// int x, y;

// std::tie(x, y) = function_that_returns_pair_of_cords();

// auto& [x, y] = cords;

/* 
#include "Globals.hpp"

namespace g{ extern unsigned int windowWidth, windowHeight; }



//* Defualt origin is top-left corner of the window!!!

class Point{
private:
public:
    int x, y;
    bool isNull;

    Point():
        x(0), 
        y(0),
        isNull(true) 
        {};
    Point(int x, int y): 
        x(x), 
        y(y),
        isNull(false)
        {};

    void print(std::string name = "");
    std::pair<float, float> getVector();
    std::pair<float, float> getUnnormalizedVector();
    Point convCenter();
    Point convTopLeft();

    bool operator==(const Point& r);
    bool operator!=(const Point& r);
}; */