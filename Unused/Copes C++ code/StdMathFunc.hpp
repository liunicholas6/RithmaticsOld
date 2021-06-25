#pragma once

#include "Line.hpp"

// This is to be able to have a reversed for-based range loop
template <typename T>
struct reversion_wrapper { T& iterable; };
template <typename T>
auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }
template <typename T>
auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }
template <typename T>
reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }

// These all assume y is inverted (i.e. origin = top left corner)

template <typename T>
T getMin (T a, T b);

template <typename T>
T getMax (T a, T b);

template <typename T>
T abs(T num);

template <typename T>
T abs(T num){
    if (num < 0)
        num *= -1;
    return num;
}

// Get the distance between 2 points
double getDist(Point a, Point b);

// Get the slope between 2 points 
//  (gets the angle between point b, and an imaginary line drawn right along the x axis of point a, counterclockwise)
double getSlope(Point a, Point b, bool giveRadians = true);

template <typename T>
T getAverage(const std::vector<T>& averagers);

template <typename T>
T getAverage(const std::vector<T>& averagers){
    T total = 0;
    for (auto i: averagers){
       total += i;
    }
    return total / averagers.size();
}

Point getAverage(const std::vector<Point>& averagers);

template <typename T>
bool isCloseEnough(T a, T b, T threshold);

template <typename T>
bool isCloseEnough(T a, T b, T threshold){
    return (abs<T>(a - b) < threshold) ? true: false;
}

bool isCloseEnough(Point a, Point b, double threshold);

int getLeftmostPoint(std::vector<Point> data);
int getRightmostPoint(std::vector<Point> data);
int getTopmostPoint(std::vector<Point> data);
int getBottommostPoint(std::vector<Point> data);

Point adjPoint(Point pt);

// Compare 2 vectors of points and get how much on average they're different
double getAverageDeviation(std::vector<Point> opt, std::vector<Point> subOpt);

// Exactly the same as getAverageDeviation(), but punishes distance from the line exponentially
double getExponentialAverageDeviation(std::vector<Point> opt, std::vector<Point> subOpt, double exp);

// Same as getAverageDeviation(), but totals the distance instead of averaging it
double getTotalDeviation(std::vector<Point> opt, std::vector<Point> subOpt);

// Returns the index of the point in subOpt that deviates the most
std::pair<int, double> getGreatestDeviation(std::vector<Point> opt, std::vector<Point> subOpt);

// Gets the points furthest from each other in the given vector
std::pair<int, int> findFurthestPoints(std::vector<Point> data, int efficency = 1);

// Gets the point in coparator that is furthest from target
std::pair<int, double> findFurthestPoint(Point target, std::vector<Point> comparator);

// Opposite of findFurthestPoint()
std::pair<int, double> findClosestPoint(Point target, std::vector<Point> comparator);

// Calculates the length of the given line (even if it's curvy)
double getLineLength(const Line& lucy);
double getLineLength(std::vector<Point> data);

// Rotates any line to be straight along the x axis
void rotateStraight(Line& lucy, bool isSine = true);

void rotate(Line& lucy, double degrees, bool radians = true);

// Returns true if the first hump goes up instead of down
bool getHumps(const std::vector<Point>& lineData, std::vector<int>& topHumps, std::vector<int>& bottomHumps);


