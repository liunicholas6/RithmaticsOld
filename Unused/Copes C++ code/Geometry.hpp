#pragma once

#include "Globals.hpp"
#include "Line.hpp"
#include "StdMathFunc.hpp"

#include <vector>

class Geometry:public Line{
private:
    Line* line;

    // Guesses what type of line this Line is so we can test and see
    Type guessType();

    double getAccuracy(Line::Type bestGuess);

    // Based on how these are calculated, it's much easier to do them together.
    // void getSineData(double& amplitude, double& wavelength, double& cycles, double& phaseShift);

    // Used for circle and maybe ellipse
    double getAverageRadius(bool efficent = true);
    
    // Get the most likely center based on the radius of the circle
    Point getCenter(double radius);

public:
    Geometry():        line(nullptr)     {}; // only use with generator functions
    Geometry(Line* l): line(l)           {};
    ~Geometry();

    // These can be used to manually create lines (that's why they're public)
    // Generate vectors filled with the points required to draw the "perfect" shape
    std::vector<Point> genOptCircle(Point center, double radius, bool ordered = false);
    std::vector<Point> genOptLine(Point start, Point end);
    std::vector<Point> genOptEllipse();
    /** WARNING:
    * This fuction does not clamp the length of the sine wave between the start and end points. They are there to get the proper angle
    * of the wave. You have to input the correct number of cycles yourself to get the right length. */
    std::vector<Point> genOptSine(Point start, Point end, double wavelength, double amplitude, double cycles = 2., double phaseShift = 7., bool niceLooking = false);


    //* These are here solely for debuggin purpouses
    void getSineData(double& amplitude, double& wavelength, double& cycles, double& phaseShift, double& maxAmpDev);

    Type identify();
};