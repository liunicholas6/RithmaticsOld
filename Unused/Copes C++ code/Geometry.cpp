#include "Geometry.hpp"
// #include "Globals.hpp"
// #include "StdMathFunc.hpp"

#include <math.h>
#include <string>
#include <vector>


#define LINE_FORBIDDENCE_PRECISION 100.f
// In getExponentialAvergeDeviation(), use this value to punish distace
#define DISTANCE_PENALTY_EXPONENT 1.5
// How many radius samples are taken (the greater the number, the more accurate, but slower it's going to be)
//  Put line->lineData->size() for max accuracy
#define MAX_RADIUS_SAMPLES line->lineData->size()
// Defines how many points forward or backward to check when assuming the circle's radius based on it's size
//  Used in efficent getAverageRadius()
#define OFFSET_CHECK_DISTANCE 10
// Defines the step of the slow way of calculating getFurthestPoints() (smaller is slower, but more accurate)
#define FIND_RADIUS_SLOW_EFFICENCY 1


// Generate vectors filled with the points required to draw the "perfect" shape
// The ordered flag toggles whether to order the points for nice drawing or not
std::vector<Point> Geometry::genOptCircle(Point center, double radius, bool ordered){
    std::vector<Point> opt;
    // main, Bottom Right, Bottom Left, Top Left: starts at the Top Right
    std::vector<Point> optTR1, optBR1, optBL1, optTL1, optTR2, optBR2, optBL2, optTL2;

    // Mid-Point Circle Drawing Algorithm -- Works, but draws the points out of order, making OpenGL draw them funny
    int x = radius, y = 0; 
    
    opt.push_back(Point(center.x + radius, center.y));
    if (ordered) // To make OpenGL draw the starting gap
       opt.push_back(Point(center.x + radius, center.y + 1));

    // When radius is zero only a single point will be printed 
    if (radius > 0){
        // opt.push_back(Point(x + center.x, -y + center.y));
        // opt.push_back(Point(y + center.x, x + center.y));
        // opt.push_back(Point(-y + center.x, x + center.y));
    }
    
    // Initialising the value of P
    int P = 1 - radius; 
    while (x > y){
        y++; 
        
        // Mid-point is inside or on the perimeter 
        if (P <= 0) 
            P = P + 2*y + 1; 
        // Mid-point is outside the perimeter 
        else{
            x--; 
            P = P + 2*y - 2*x + 1; 
        } 
        
        // All the perimeter points have already been printed 
        if (x < y) 
            break; 
        
        // Printing the generated point and its reflection
        // in the other octants after translation
        if (ordered){
            optTR1.push_back(Point(x + center.x, y + center.y));
            optTL1.push_back(Point(-x + center.x, y + center.y));
            optBR1.push_back(Point(x + center.x, -y + center.y));
            optBL1.push_back(Point(-x + center.x, -y + center.y));
        }
        else{
            opt.push_back(Point(x + center.x, y + center.y));
            opt.push_back(Point(-x + center.x, y + center.y));
            opt.push_back(Point(x + center.x, -y + center.y));
            opt.push_back(Point(-x + center.x, -y + center.y));
        }
        
        // If the generated point is on the line x = y then the perimeter points have already been printed
        if (x != y){
            if (ordered){
                optTR2.push_back(Point(y + center.x, x + center.y));
                optTL2.push_back(Point(-y + center.x, x + center.y));
                optBR2.push_back(Point(y + center.x, -x + center.y));
                optBL2.push_back(Point(-y + center.x, -x + center.y));
            }
            else{
                opt.push_back(Point(y + center.x, x + center.y));
                opt.push_back(Point(-y + center.x, x + center.y));
                opt.push_back(Point(y + center.x, -x + center.y));
                opt.push_back(Point(-y + center.x, -x + center.y));
            }
        } 
    }

    if (ordered){
        // Now assemble all those points properly
        // This ordering works, but it looks messed up. 
        //  I think I messed up the octant naming or something.
        for(auto it: optBR1)
            opt.push_back(it);
        for(auto it: reverse(optBR2))
            opt.push_back(it);
        for(auto it: optBL2)
            opt.push_back(it);
        for(auto it: reverse(optBL1))
            opt.push_back(it);

        opt.push_back(Point(center.x - radius, center.y));

        for(auto it: optTL1)
            opt.push_back(it);
        for(auto it: reverse(optTL2))
            opt.push_back(it);
        for(auto it: optTR2)
            opt.push_back(it);
        for(auto it: reverse(optTR1))
            opt.push_back(it);
    }
    else
        opt.push_back(Point(center.x - radius, center.y));

    if (ordered)
       opt.push_back(Point(center.x + radius, center.y - 1));

    // opt.push_back(Point(center.x + radius, center.y));

    /*
        errors between octants:
        TL1/TR1 and TL1/TL2

    */


    /*
       8        1               TR 1
           xxx                              
    7   x       x   2           TR 2
      x           x
      x           x
      x           x
    6   x       x   3           BR 1
           xxx
       5         4              BR 2 
    */

    /* Bresenham’s Algorithm -- Works, same problem
    int x = 0, y = radius; 
    int d = 3 - 2 * radius; 

    opt.push_back(Point(center.x + x, center.y + y)); 
    opt.push_back(Point(center.x - x, center.y + y)); 
    opt.push_back(Point(center.x + x, center.y - y)); 
    opt.push_back(Point(center.x - x, center.y - y)); 
    opt.push_back(Point(center.x + y, center.y + x)); 
    opt.push_back(Point(center.x - y, center.y + x)); 
    opt.push_back(Point(center.x + y, center.y - x)); 
    opt.push_back(Point(center.x - y, center.y - x));

    while (y >= x){
        // for each pixel we will draw all eight pixels 
        x++; 
        
        // check for decision parameter and correspondingly update d, x, y 
        if (d > 0){
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 

        opt.push_back(Point(center.x + x, center.y + y)); 
           ^
        opt.push_back(Point(center.x + x, center.y - y)); 
        opt.push_back(Point(center.x - x, center.y - y)); 
        opt.push_back(Point(center.x + y, center.y + x)); 
        opt.push_back(Point(center.x - y, center.y + x)); 
        opt.push_back(Point(center.x + y, center.y - x)); 
        opt.push_back(Point(center.x - y, center.y - x));
    }
    */
    
    /* Organize the points to be in order
    std::vector<Point> organizedOpt;
    /
           xxx
        x       x
      x           x
      x           x
      x           x
        x       x
           xxx
           ^
       start here
    /

    // Get the bottommost point
    assert(opt.size());
    Point bottomLeft;
    for(auto it: opt)
        if(it.y > bottomLeft.y)
            if(it.x < bottomLeft.x)
                bottomLeft = it;

    organizedOpt.push_back(bottomLeft);
    */

    return opt;
}


std::vector<Point> Geometry::genOptLine(Point start, Point end){
    std::vector<Point> opt;

    /* Native line drawing algorithm - doesn't currently work
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    bool positiveSlope = start.x > end.x;

    for (int x = start.x; positiveSlope ? (x >= end.x) : (x <= end.x); positiveSlope ? --x : ++x){
        int y = ((start.y + dy) * (x - start.x)) / dx;
        _log(std::to_string(x) + ", " + std::to_string(y));
        opt.push_back(Point(x, y));
    }
    */

    // https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm) 

    double x, y, dx, dy, step;
    int i;

    double x1 = start.x, y1 = start.y, x2 = end.x, y2 = end.y;

    dx = (x2 - x1);
    dy = (y2 - y1);
    if (abs(dx) >= abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
    dx = dx / step;
    dy = dy / step;
    x = x1;
    y = y1;
    i = 1;
    while (i <= step) {
        // putpixel(x, y, 5);
        opt.push_back(Point(x, y));
        x = x + dx;
        y = y + dy;
        i = i + 1;
    }

    return opt;
}


std::vector<Point> Geometry::genOptEllipse(){
    std::vector<Point> opt;
    return opt;
}

/** WARNING:
* This fuction does not clamp the length of the sine wave between the start and end points. They are there to get the proper angle
* of the wave. You have to input the correct number of cycles yourself to get the right length. */
std::vector<Point> Geometry::genOptSine(Point start, Point end, double wavelength, double amplitude, double cycles, double phaseShift, bool niceLooking){
    /**************** Formula of a Sine Wave *****************
    * @p A amplitude
    * @p f cycles
    * @p t time
    * @p shift phase shift
    *
    ** y(t) = A * sin(2 * PI * f * t + shift)
    *                         or...
    ** y(time) = amplitude * sin(2 * PI * cycles * time + phaseShift)
    * P.S. This is wrong; it doesn't work.
    **********************************************************/

    // I love the internet so much
    // https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line

    std::vector<Point> opt;
    int x, y, dx, dy, newX, newY;
    double v = atan2(end.y - start.y, end.x - start.x) - (M_PI / 2);

    // Create the actual sine wave, then change the angle of that sine wave
    for(x = start.x; x < (wavelength * cycles) + start.x; ++x){
        // Calculate y based on x, via the sine equation
        y = amplitude * sin(((2 * M_PI) * (x + phaseShift)) / wavelength) + start.y;
        
        // Calculate how far off you are from the center
        dx = x - start.x;
        dy = y - start.y;
        
        // Rotate around the starting point
        newX = dx * cos(v) - dy * sin(v) + start.x;
        newY = dx * sin(v) + dy * cos(v) + start.y;

        // Add that point to the new vector
        opt.push_back(Point(newX, newY));
    }
    
    // This should NOT have been this difficult to figure out.
    /** //? This way works, so I want to keep it, but the other way is more efficent
        double x, y;
        double verticalShift = start.y;

        double v = atan2(end.y - start.y, end.x - start.x) - (M_PI / 2);
        
        y = amplitude * sin(((2 * M_PI) * (start.x + phaseShift)) / wavelength) + verticalShift;
        int newY = (start.x *  cos(v)) + (y * sin(v));
        int newX = (start.x * -sin(v)) + (y * cos(v));

        int xOffset = start.x - newX + 7;
        int yOffset = start.y - newY;
        

        // Create the actual sine wave, then change the angle of that sine wave
        for(x = start.x; x < (wavelength * cycles) + start.x; ++x){
            y = amplitude * sin(((2 * M_PI) * (x + phaseShift)) / wavelength) + verticalShift;
            int newY = (x *  cos(v)) + (y * sin(v));
            int newX = (x * -sin(v)) + (y * cos(v));
            opt.push_back(Point(newX + xOffset, newY + yOffset));
        }
    */

    // Because that only generates one point per x, draw lines between the points
    //  (actually looks slightly worse when drawn with OpenGL this way)
    if (not niceLooking){
        std::vector<Point> tmpOpt = opt;
        opt.clear();
        assert(tmpOpt.size() > 2);
        for(auto it = tmpOpt.begin() + 1; it < tmpOpt.end(); ++it){
            // opt.push_back(*(it - 1));
            std::vector<Point> tmp = genOptLine(*it, *(it - 1));
            appendVector(opt, tmp);
            // opt.push_back(*it);
        }
    }
    return opt;
}


double Geometry::getAccuracy(Line::Type bestGuess){
    switch(bestGuess){
        case LINE_DOT:
        case LINE_UNKNOWN:
        case LINE_MAKING:
        case LINE_REVOCATION:
            return NAN;
        case LINE_VIGOR:
        {
            double detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift, maxAmpDev;
            getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift, maxAmpDev);
            std::cout << g::getDebugCount() << ": " << std::fixed << std::setprecision(5)
                    // << "Wavelength = " << wavelength << "\n"
                    << "Detected Wavelength = " << detectedWavelength << "\n"
                    // << "Amplitude = " << amplitude << "\n"
                    << "Detected Amplitude = " << detectedAmplitude << '\n'
                    // << "Cycles = " << cycles << '\n'
                    << "Detected Cycles = " << detectedCycles << '\n'
                    << "Detected Phase Shift = " << detectedPhaseShift << '\n'
                    << "Detected Maximum Amplitude Deviation = " << maxAmpDev << '\n';

            if(detectedAmplitude == NAN or detectedCycles == NAN or detectedWavelength == NAN or detectedPhaseShift == NAN)
                return 0.;

            std::vector<Point> optSine = genOptSine(line->start, line->end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift, true);
            //                                                 neither is this value ||
            //          For sine, this isn't always super accurate \/                \/
            double answer = 100 - (getAverageDeviation(optSine, *line->lineData) / 1.5);// - (maxAmpDev / 3);

            logVal(getAverageDeviation(optSine, *line->lineData))
            logVal(maxAmpDev)
            logVal(answer)

            return answer;
            // return answer < 0 ? 0 : answer; // If it's negative, just say 0.
        }
        case LINE_WARDING:
        {
            double radius = getAverageRadius(false);
            Point  center = getCenter(radius);

            std::vector<Point> optCircle = genOptCircle(center, radius, false);

            double answer = 100 - getAverageDeviation(optCircle, *line->lineData) - (getGreatestDeviation(optCircle, *line->lineData).second * 2);

            return answer < 0 ? 0 : answer; // If it's negative, just say 0.
        }
        case LINE_FORBIDDENCE:
        {   // getAverageDeviation() here can be approximated with getDist(start, end) / getLineLength().
            std::vector<Point> optLine = genOptLine(line->start, line->end);
            // return LINE_FORBIDDENCE_PRECISION - getAverageDeviation(optLine) * LINE_FORBIDDENCE_PRECISION / 10;

            // logVal(getExponentialAverageDeviation(optLine))
            // logVal(getGreatestDeviation(optLine).second)
            // logVal(((getExponentialAverageDeviation(optLine) * -1) - getGreatestDeviation(optLine).second) * -1)

            double answer = 100 - getExponentialAverageDeviation(optLine, *line->lineData, DISTANCE_PENALTY_EXPONENT) - (getGreatestDeviation(optLine, *line->lineData).second * 2);

            return answer < 0 ? 0 : answer; // If it's negative, just say 0. 
        }
        default:
            break;
    }
}


double Geometry::getAverageRadius(bool efficent){
    std::vector<double> radii;
    int samples = MAX_RADIUS_SAMPLES;

    // The smarter way of doing things
    if (efficent){
        _log("Effeciently calculating radius...");
        auto sampler = line->lineData->begin();
        auto antiSampler = line->lineData->begin() + (lineData->size() / 2);

        //todo this doesn't work...
        while (samples){
            double finalDist = getDist(*sampler, *antiSampler);

            if (getDist(*sampler, *antiSampler) == finalDist)
                break;
            else if (getDist(*sampler, *(antiSampler + 1)) > finalDist)
                while(getDist(*sampler, *(++antiSampler)) > finalDist) { }
            else if (getDist(*sampler, *(antiSampler + 1)) > finalDist)
                while(getDist(*sampler, *(--antiSampler)) > finalDist) { }

            radii.push_back(getDist(*sampler, *antiSampler) / 2);

            --samples;
        }
    }
    // The easy way of doing things
    else{
        _log("Ineffeciently calculating radius...");
        for(auto it = line->lineData->begin(); it < line->lineData->end(); it += (line->lineData->size() / MAX_RADIUS_SAMPLES))
            radii.push_back(findFurthestPoint(*it, *line->lineData).second / 2);
    }

    return getAverage<double>(radii);
}

Point Geometry::getCenter(double radius){
    Point rightmost  = (*line->lineData)[getRightmostPoint(*line->lineData)];
    Point leftmost   = (*line->lineData)[getLeftmostPoint(*line->lineData)];
    Point topmost    = (*line->lineData)[getTopmostPoint(*line->lineData)];
    Point bottommost = (*line->lineData)[getBottommostPoint(*line->lineData)];
    Point rightSays (rightmost.x - radius, rightmost.y);
    Point leftSays  (leftmost.x  + radius, leftmost.y );
    Point topSays   (topmost.x   , topmost.y    + radius);
    Point bottomSays(bottommost.x, bottommost.y - radius);

    std::vector<Point> centers = {rightSays, leftSays, topSays, bottomSays};
    Point avg = getAverage(centers);
    return avg;
}

// Guesses what type of line this Line is
Line::Type Geometry::guessType(){

    assert(line->isFinished and not line->isNull());
    if (line->lineData->size() == 1)
        return LINE_DOT;

    else if (getDist(line->start, line->end) < getLineLength(*this->line) / 8) // 8 is arbitrary
        return LINE_WARDING;

    // The length of the line of a sine wave is approx. sqrt(pow(amp, 2) + pow(wav / 4, 2))
    else if (getDist(line->start, line->end) < getLineLength(*this->line) / 1.1) // 1.1 is arbitrary, but pretty good
        return LINE_VIGOR;

    else if (isCloseEnough<double>(getDist(line->start, line->end) / getLineLength(*line), 1., .3))
        return LINE_FORBIDDENCE;

    else if (false)
        return LINE_REVOCATION;

    else if (false)
        return LINE_MAKING;

    else 
        return LINE_UNKNOWN;
}

Line::Type Geometry::identify(){
    Type guessedType = guessType();

    double acc = getAccuracy(guessedType);

    line->lineType = guessedType;
    line->accuracy = acc;

    // logVal(getDist(line->start, line->end) / getLineLength())

    if (acc < 50.f)
        return LINE_UNKNOWN;
    else
        return guessedType;
}

#define LINEDATA (*line->lineData)

void Geometry::getSineData(double& amplitude, double& wavelength, double& cycles, double& phaseShift, double& maxAmpDev){
   
    rotateStraight(*line);

    // todo clamp between start and end distances somehow

    /** IMPORTANT: Remember that origin is the top left, so y is inverted from intuition  */

    std::vector<int> topHumps, bottomHumps;

    bool goesUpFirst = getHumps(LINEDATA, topHumps, bottomHumps);

    logVal(topHumps.size())
    logVal(bottomHumps.size())

    if (not topHumps.size() or not bottomHumps.size()){
        amplitude  = NAN;
        wavelength = NAN;
        cycles     = NAN;
        phaseShift = NAN;
        maxAmpDev  = NAN;
        return;
    }

    //* Calculate Cycles
        // The meat of the matter: the number of points from the first hump to the last hump
        // Get the indecies of the first and last humps
        int meatCountBegin = (goesUpFirst ? topHumps.front(): bottomHumps.front());
        int meatCountEnd   = (topHumps.back() > bottomHumps.back() ? topHumps.back(): bottomHumps.back());
        int meatCount      = meatCountEnd - meatCountBegin;

        double totalHumps = double(topHumps.size() + bottomHumps.size());

        // How many points aren't accounted for in the meat
        double totalFractionalCyclePoints = double(LINEDATA.size() - meatCount);

        // How many points are in each cycles, on average?
        double averagePointsPerCycle = meatCount / totalHumps;

        // How many partial cycles (beginning and the ending) are there?
        double fractionalCycles = totalFractionalCyclePoints / averagePointsPerCycle;

        cycles = (totalHumps + fractionalCycles) / 2;


    //* Calculate Phase Shift
        int preCyclePoints  = meatCountBegin;
        int postCyclePoints = LINEDATA.size() - meatCountEnd;

        // if ((preCyclePoints + postCyclePoints) )

        double preCycles  = double(preCyclePoints)  / double(averagePointsPerCycle);
        double postCycles = double(postCyclePoints) / double(averagePointsPerCycle);

        phaseShift = preCycles >= 1 ? --preCycles: preCycles;

        // Since we're calculating each induvidual hump seperately, we need to detect if the 
        //  partial part is partially going up or down. The sign represents the slope of the line.
        // if (not goesUpFirst)
        //     phaseShift *= -1;

        if (not goesUpFirst){
            phaseShift += averagePointsPerCycle * 2;
        }


    //* Calculate Amplitude
        // Calculate out all the amplitudes
        double topHumpAverageAmp = 0, bottomHumpAverageAmp = 0, averageAmp = 0;
        int    topHumpTotalAmp   = 0, bottomHumpTotalAmp = 0  , totalAmp   = 0;

        // Calculate the top amplitudes
        // for (auto i: topHumps){
        //     topHumpTotalAmp += LINEDATA[i].y - line->start.y;
        // }

        // Calculate the bottom amplitudes
        // for (auto i: bottomHumps){
        //     bottomHumpTotalAmp += LINEDATA[i].y - line->start.y;
        // }

        // logVal(totalHumps)

        double currentAmp = -1., penultimateAmpDev = 0.;
        for (int i = 0; i < totalHumps; i += 2){
            currentAmp = bottomHumps[i] - topHumps[i];

            totalAmp += currentAmp;
            if (currentAmp > maxAmpDev){
                // if (currentAmp != -1.)
                    // penultimateAmpDev = maxAmpDev;
                maxAmpDev = currentAmp;
            }
        }

        // Something to add for the future: add top vs bottom average amplitude comparison into the main algorithm

        // averageAmp = (topHumpTotalAmp + bottomHumpTotalAmp) /  totalHumps;
        averageAmp = double(totalAmp) / double(totalHumps);
        amplitude = fabs(averageAmp); // abs() here shouldn't be necissary.


    //* Calculate Maximum Amplitude Deviation
        // logVal(maxAmpDev)
        // logVal(penultimateAmpDev)

        maxAmpDev = maxAmpDev - amplitude;


    //* Calculate Wavelength
        wavelength = LINEDATA.size() / cycles;
}

#undef LINEDATA


Geometry::~Geometry(){
    // delete this->line;
}