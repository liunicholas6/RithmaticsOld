#include "StdMathFunc.hpp"
#include "Globals.hpp"
#include "Point.hpp"

#include <math.h>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
T getMax (T a, T b) {
  T result;
  result = (a > b) ? a: b;
  return (result);
}

template <typename T>
T getMin (T a, T b){
  T result;
  result = (a < b) ? a: b;
  return (result);
}


double getLineLength(const Line& lucy){
    double total = 0.f;

    for (int i = 1; i < lucy.lineData->size(); ++i)
        total += getDist((*lucy.lineData)[i], (*lucy.lineData)[i - 1]);

    return total + 1;
}


double getLineLength(std::vector<Point> data){
    double total = 0.f;

    for (int i = 1; i < data.size(); ++i)
        total += getDist(data[i], data[i - 1]);

    return total + 1;
}


Point getAverage(const std::vector<Point>& averagers){
    Point total;
    for (auto i: averagers){
        total.x += i.x;
        total.y += i.y;
    }
    return Point(total.x / averagers.size(), total.y / averagers.size());
}


double getDist(Point a, Point b){
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    // return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Gets the angle between the line that goes between the 2 points, and the right hand side x axis
double getSlope(Point a, Point b, bool giveRadians){
    double angle = -atan2(b.y - a.y, b.x - a.x);
    if (angle < 0){
        angle += 2 * M_PI;// + abs(angle);
    }
    return giveRadians ? angle: glm::degrees(angle);

    // You both suck.
    // return (double(b.y) - double(a.y)) / (double(b.x) - double(a.x)); // The traditional slope equation
    // return (double(a.y) - double(b.y)) / (double(b.x) - double(a.x)); // The slope equation for origin at top left corner
}


bool isCloseEnough(Point a, Point b, double threshold){
    return (fabs(getDist(a, b)) < threshold) ? true: false;
}


int getLeftmostPoint(std::vector<Point> data){
    int smallestX = 10000, index;
    for(int i = 0; i < data.size(); ++i){
        if (data[i].x < smallestX){
            smallestX = data[i].x;
            index = i;
        }
    }
    return index;
}


int getRightmostPoint(std::vector<Point> data){
    int largestX = 0, index;
    for(int i = 0; i < data.size(); ++i){
        if (data[i].x > largestX){
            largestX = data[i].x;
            index = i;
        }
    }
    return index;
}


int getTopmostPoint(std::vector<Point> data){
    int smallestY = 10000, index;
    for(int i = 0; i < data.size(); ++i){
        if (data[i].y < smallestY){
            smallestY = data[i].y;
            index = i;
        }
    }
    return index;
}


int getBottommostPoint(std::vector<Point> data){
    int largestY = 0, index;
    for(int i = 0; i < data.size(); ++i){
        if (data[i].y > largestY){
            largestY = data[i].y;
            index = i;
    // First time reaching 1,000 lines of code!
        }
    }
    return index;
}

// Gets the points furthest from each other in the vector
std::pair<int, int> findFurthestPoints(std::vector<Point> data, int efficency){
    // I'm just going to assume it's at least vaugely a circle shape
    int a, b, i = 0;

    for (; i < data.size(); i += efficency){

    }

    return std::pair<int, int>(a, b);
}

// Finds the furthest point in the list to what it's given
std::pair<int, double> findFurthestPoint(Point target, std::vector<Point> comparator){
    int index = 0;
    double finalDist = 0, current;

    for (int i = 0; i < comparator.size(); ++i){
        current = getDist(target, comparator[i]);
        if (current > finalDist){
            finalDist = current;
            index = i;
        }
    }
    // logVal(finalDist)
    return std::pair<int, double>(index, finalDist);
}

// Finds the closest point in the list to what it's given
std::pair<int, double> findClosestPoint(Point target, std::vector<Point> comparator){
    int index = 0;
    double finalDist = 10000, current;

    for (int i = 0; i < comparator.size(); ++i){
        current = getDist(target, comparator[i]);
        if (current < finalDist){
            finalDist = current;
            index = i;
        }
    }
    // logVal(finalDist)
    return std::pair<int, double>(index, finalDist);
}


double getAverageDeviation(std::vector<Point> opt, std::vector<Point> subOpt){
    int i = 0;
    double netDist = 0.f;
    for(; i < subOpt.size(); ++i){
        netDist += fabs(findClosestPoint(subOpt[i], opt).second);
    }
    // logVal(this->subOpt.size())
    // logVal(i)
    // logVal(netDist)
    // logVal(netDist / i)
    return netDist / i; // The average distance of the closest points
}

// Exactly the same as getAverageDeviation(), but punishes distance from the line exponentially
double getExponentialAverageDeviation(std::vector<Point> opt, std::vector<Point> subOpt, double exp){
    int i = 0;
    double netDist = 0.f;
    for(; i < subOpt.size(); ++i){
        netDist += pow(fabs(findClosestPoint(subOpt[i], opt).second), exp);
    }
    // logVal(this->subOpt.size())
    // logVal(i)
    // logVal(netDist)
    // logVal(netDist / i)
    return netDist / i; // The average distance of the closest points
}


double getTotalDeviation(std::vector<Point> opt, std::vector<Point> subOpt){
    double netDist = 0.f;
    for(int i = 0; i < subOpt.size(); ++i){
        netDist += findClosestPoint(subOpt[i], opt).second;
    }
    // logVal(netDist)
    return netDist;
}


std::pair<int, double> getGreatestDeviation(std::vector<Point> opt, std::vector<Point> subOpt){
    double currentDist = 0.f, finalDist = 0.f;
    int index = 0;

    for(int i = 0; i < subOpt.size(); ++i){
        currentDist = findClosestPoint(subOpt[i], opt).second;
        if (currentDist > finalDist){
            finalDist = currentDist;
            index = i;
        }
    }

    return std::pair<int, double>(index, finalDist);
}


void rotate(Line& lucy, double angle, bool radians){
    if (not radians)
        angle = glm::radians(angle);

    assert(not lucy.isNull());    

    std::vector<Point> angleAdj;

    //* The GLM Method
    // Create the rotation matrix
    // glm::mat4 rotateMat= glm::mat4(1.0f);
    // rotateMat = glm::rotate(rotateMat, float(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Translate the points
    // glm::vec4 rotated = rotateMat * glm::vec4(lucy.start.x, lucy.start.y * -1, 0.0f, 1.0f);

    int dx, dy, newX, newY;

    // I love the internet so much
    // https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line

    //* Actually do the moving
    for (auto i: *lucy.lineData){
        dx = i.x - lucy.start.x;
        dy = i.y - lucy.start.y;
        newX = dx * cos(angle) - dy * sin(angle) + lucy.start.x;
        newY = dx * sin(angle) + dy * cos(angle) + lucy.start.y;

        // glm::vec4 rotated = rotateMat * glm::vec4(i.x, i.y * -1, 0.0f, 1.0f);

        angleAdj.push_back(Point(newX, newY));
    }

    std::vector<Point>* tmp = new std::vector<Point>(angleAdj);
    lucy.lineData = tmp;
    lucy.end = lucy.lineData->back();
    // lucy.end.print("Generated end point");
    // assert(lucy.start.x == lucy.lineData->front().x and lucy.start.y == lucy.lineData->front().y);
}


void rotateStraight(Line& lucy, bool isSine){
    // float angle = atan2(lucy.end.y - lucy.start.y, lucy.end.x - lucy.start.x) - (M_PI / 2);
    double angle = getSlope(lucy.start, lucy.end);
    rotate(lucy, angle, true);

    //todo This algorithm Definitely needs some work, but it works for now.
    if (isSine){
        std::vector<int> topHumps, bottomHumps;

        bool firstHumpHigh = getHumps(*lucy.lineData, topHumps, bottomHumps);
        
        logVal(topHumps.size())
        // logVal(bottomHumps.size())

        
        // logVal(bottomHumps.size())

        double totalTopHumpSlopes  = 0, totalBottomHumpSlopes  = 0;
        double averageTopHumpSlope = 0, averageBottomHumpSlope = 0;

        // getTopmostPoint(*lucy.lineData);

        for(auto it = topHumps.begin() + 1; it < topHumps.end(); ++it){
            // Get the slope between the first hump, and all the other humps
            totalTopHumpSlopes += getSlope((*lucy.lineData)[topHumps[0]], (*lucy.lineData)[*it]);
        }

        // logVal(totalTopHumpSlopes)
        // logVal(ave)

        averageTopHumpSlope = totalTopHumpSlopes / double(topHumps.size());
    /* 
        for(auto i: bottomHumps){
            // Get the slope between the first hump, and all the other humps
            totalBottomHumpSlopes += getSlope((*lucy.lineData)[bottomHumps[0]], (*lucy.lineData)[i]);
        }
        averageBottomHumpSlope = averageBottomHumpSlope / double(bottomHumps.size());
    */
        
        // logVal(totalBottomHumpSlopes)

        // logVal(averageTopHumpSlope)
        // logVal(averageBottomHumpSlope)

        if (firstHumpHigh){
            averageTopHumpSlope    = (2 * M_PI) - averageTopHumpSlope;
            // averageBottomHumpSlope = (2 * M_PI) - averageBottomHumpSlope;

        }
        else{
            averageTopHumpSlope    -= M_PI;
            // averageBottomHumpSlope -= M_PI;
        }

        _print(---------) _endl
        // logVal(averageTopHumpSlope)
        // logVal(averageBottomHumpSlope)

        // double averageSlope = (averageTopHumpSlope + averageBottomHumpSlope) / 2;


        rotate(lucy, -averageTopHumpSlope, false);
    }
}

// Returns true if the first hump goes up instead of down
bool getHumps(const std::vector<Point>& lineData, std::vector<int>& topHumps, std::vector<int>& bottomHumps){
    int prevY, index = 0;
    bool goesUpFirst = true;
    auto itPrev = lineData.begin();
    auto it = lineData.begin() + 1;

    // First, figure out if the sine is going up or down first
    do{
        // You're going up
        if (it->y < itPrev->y){
            prevY = 100000;
            break;
        }
        
        // You're going down
        if (it->y > itPrev->y){
            prevY = 0;
            goesUpFirst = false;
            // Starts looking for the lowest point first, instead of the highest point first
            goto startGoingDown;
        }
    }
    while (itPrev == it);


    // Go up until you start going down, and mark that index
    // Then go down until you start going up, and mark that index
    while (index < lineData.size() - 1){ 
        while (lineData[index].y <= prevY){
            prevY = lineData[index].y;
            ++index;

            if(index >= lineData.size())
                return goesUpFirst;
        }

        // For robustness (i.e. if a point goes forward then immediately back)
        if (lineData[index - 1].y >= lineData[index + 1].y)
            topHumps.push_back(index - 1);


        startGoingDown:
        while (lineData[index].y >= prevY){
            prevY = lineData[index].y;
            ++index;

            if(index >= lineData.size())
                return goesUpFirst;
        }

        // For robustness (i.e. if a point goes forward then immediately back)
        if (lineData[index - 1].y <= lineData[index + 1].y)
            bottomHumps.push_back(index - 1);
    }

    // The logic should never get to this point
    assert(false);
    return goesUpFirst;
}

#include "StdMathFunc.tcc"