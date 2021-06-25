#include "Line.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>



void Line::draw(){
    // glDrawArrays(GL_LINES, 0, lineData.size() * 2 + 4);
}

Line::Type Line::identifyLine(){
    Geometry geo(this);

    return geo.identify();
}

const int Line::getDataLen(){
    int len = lineData->size();

    // check if start/end are empty, if so, add them
    // if(start.isNull or isFinished){
    //     ++len;
    //     if(end.isNull or isFinished)
    //         ++len;
    // }

    return len;
}

void Line::finish(Point end, bool identify){
    this->end = end;
    isFinished = true;

    if (not lineData->size()){
        lineData->push_back(start);
        lineType = LINE_DOT;
    }
    else if (identify){
        identifyLine();
        printAccDebug();
        // update();
    }
}

void Line::finish(Point end, const std::vector<Point> data, bool identify){
    finish(end, identify);
    lineData = new std::vector<Point>(data);
}

std::vector<Point>* Line::getData(){
    std::vector<Point>* tmp = new std::vector<Point>;
    tmp = lineData;

    // check if start/end are empty, if so, add them
    if(start.isNull or isFinished){
        tmp->insert(tmp->begin(), start);
        if(end.isNull or isFinished)
            tmp->push_back(end);
    }
    return tmp;
}

void Line::erase(){
    start = Point();
    end = Point();
    accuracy = -2;
    // isIdentified = false;
    isFinished = false;
    // lineData->clear();
    // vertices->clear();
}

const void Line::printAcc(){
    std::string name;

    switch(lineType){
        case Line::LINE_DOT:         name = "dot";                 break;
        case Line::LINE_UNKNOWN:     name = "unknown";             break;
        case Line::LINE_MAKING:      name = "Line of Making";      break;
        case Line::LINE_REVOCATION:  name = "Line of Revocation";  break;
        case Line::LINE_VIGOR:       name = "Line of Vigor";       break;
        case Line::LINE_WARDING:     name = "Line of Warding";     break;
        case Line::LINE_FORBIDDENCE: name = "Line of Forbiddence"; break;
    }

    if (accuracy >= 50.f)
        std::cout << g::getDebugCount() << ": " << "I'm  " << std::setprecision(1) << std::fixed << accuracy << "% sure that is a " << name << ".\n";
    else if (lineType == LINE_UNKNOWN)
        std::cout << g::getDebugCount() << ": " << "I have no idea what that is supposed to be.\n";
    else 
        std::cout << g::getDebugCount() << ": " << "That is not a " << name << ". (I'm " << std::setprecision(1) << std::fixed << 50.f - accuracy << "% sure)\n";
        // std::cout << g::getDebugCount() << ": " << "That is not a " << name << ". (" << std::setprecision(1) << std::fixed << accuracy << "%)\n";
}

const void Line::printAccDebug(){
    std::string name;

    switch(lineType){
        case Line::LINE_DOT:         name = "a dot";                 break;
        case Line::LINE_UNKNOWN:     name = "unknown";               break;
        case Line::LINE_MAKING:      name = "a Line of Making";      break;
        case Line::LINE_REVOCATION:  name = "a Line of Revocation";  break;
        case Line::LINE_VIGOR:       name = "a Line of Vigor";       break;
        case Line::LINE_WARDING:     name = "a Line of Warding";     break;
        case Line::LINE_FORBIDDENCE: name = "a Line of Forbiddence"; break;
    }

    std::cout << g::getDebugCount() << ": " << "The line is " << name << ", and it's accuracy is " << std::fixed << std::setprecision(5) << accuracy << ".\n";
}

std::vector<float>* Line::update(Color playersColor, bool isDot){
    // I hate operator overloads
    if (this->lineColor != playersColor){
    // if (this-> lineColor.r != playersColor.r or this-> lineColor.g != playersColor.g or this-> lineColor.b != playersColor.b or this-> lineColor.a != playersColor.a){
        this->lineColor = playersColor;
        vertices->clear(); 
        // this->vertices = new std::vector<float>(reUpdate(*lineData, playersColor));
        // return this->vertices;
        // return vertices;
    }

    if (isDot) lineType = LINE_DOT;
    // if (isFinished){
        // identifyLine();
        // printAcc();
    // }

    int startSize = vertices->size();

    // if (isFinished and not lineData->size()){
    //     assert(start.x == end.x and start.y == end.y);
    //     lineData->push_back(start);
    // }

    if(!(isFinished) and (vertices->size() != (lineData->size() * 6))){
        for(auto it = lineData->begin() + (vertices->size() / 6); it != lineData->end(); ++it){
            addVertices(getVector(*it));
        }
    }
    else if (isFinished){
        // addVertices(start.getVector());
        if (lineData->size() > 1){
            for(auto it = lineData->begin() + (vertices->size() / 6); it < lineData->end(); ++it){
                addVertices(getVector(*it));
            }
        }
        // size == 1
        else if(lineData->size()){
            addVertices(getVector(start));
            // addVertices(Point(start.x    , start.y + 1).getVector());
            // addVertices(Point(start.x + 1, start.y + 1).getVector());
            // addVertices(Point(start.x + 1, start.y    ).getVector());
        }
        // addVertices(end.getVector());
    }

    // logVal(lineData->size())
    // logVal(vertices->size())
    // logVal(isFinished)

    // Create a new subvector of the stuff we just added
    // Cream: Just scraping the good stuff off the top
    std::vector<float>* cream = new std::vector<float>;

    // This loop was WAY harder than it should have been.
    for (int i = startSize; i < vertices->size(); ++i)
        cream->push_back((*vertices)[i]);

    return cream;
}

void Line::addVertices(std::pair<float, float> coord){
    vertices->push_back(coord.first);
    vertices->push_back(coord.second);
    vertices->push_back(lineColor.r);
    vertices->push_back(lineColor.g);
    vertices->push_back(lineColor.b);
    vertices->push_back(lineColor.a);
}

const bool Line::isNull(){
    return start.isNull and accuracy == -2;
}

void Line::append(Point where){
    lineData->push_back(where);
}

void Line::reUpdate(){
    vertices->clear();
    // I hate operator overloads
    // if (lineData->front() != start)
    //     lineData->insert(lineData->begin(), start);
    // if (lineData->back() != end)
    //     lineData->push_back(end);

    // Becuase things can't just be easy...
    for(int i = 0; i < lineData->size(); ++i)
        addVertices(getVector((*lineData)[i]));

    // if (lineData->size())
        // for(auto it: *lineData)
            // addVertices(it.getVector());
}

std::vector<float> Line::reUpdate(std::vector<Point> line, const Color& color){
    // I hate operator overloads
    // if (lineData->front() != start)
    //     lineData->insert(lineData->begin(), start);
    // if (lineData->back() != end)
    //     lineData->push_back(end);

    std::vector<float> returnMe;

    // Becuase things can't just be easy...
    for(int i = 0; i < line.size(); ++i){
        returnMe.push_back(getVector(line[i]).first);
        returnMe.push_back(getVector(line[i]).second);
        returnMe.push_back(color.r);
        returnMe.push_back(color.g);
        returnMe.push_back(color.b);
        returnMe.push_back(color.a);
    }

    // if (lineData->size())
        // for(auto it: *lineData)
            // addVertices(it.getVector());
    return returnMe;
}


const void Line::print(std::string name){
    unsigned long count = g::getDebugCount();
    
    short length = floor(log10(count)) + 4;

    std::string filler(length, ' ');

    std::cout << count  << ": {" << (name.size() ? name + "::": "") << "Line:\n"
              << filler << "Start = (" << start.x << ", " << start.y << ")\n"
              << filler << "End = (" << end.x << ", " << end.y << ")\n"
              << filler << "Line Data Size = " << lineData->size() << "\n"
              << filler << "Vertices Size = " << vertices->size() << " (/ 6 = " << vertices->size() / 6 << ")\n"
              << filler << "Type = " << getTypeName() << "\n"
              << filler << "Accuracy = " << std::fixed << std::setprecision(3) << accuracy << "\n"
              << filler << "Line Color = [" << lineColor.r << ", " << lineColor.g << ", " << lineColor.b << ", " << lineColor.a << "]\n"
              << filler << (isFinished ? "Is Finished": "Is Not Finished") << " and\n"
              << filler << (isNull() ? "Is Null": "Is Not Null") << "\n}\n";
}

const std::string Line::getTypeName(Type type){
    switch(type){
        case Line::LINE_DOT:         return "dot";
        case Line::LINE_UNKNOWN:     return "unknown";
        case Line::LINE_MAKING:      return "Line of Making";
        case Line::LINE_REVOCATION:  return "Line of Revocation";
        case Line::LINE_VIGOR:       return "Line of Vigor";
        case Line::LINE_WARDING:     return "Line of Warding";
        case Line::LINE_FORBIDDENCE: return "Line of Forbiddence";
    }
}

const std::string Line::getTypeName(){
    switch(this->lineType){
        case Line::LINE_DOT:         return "dot";
        case Line::LINE_UNKNOWN:     return "unknown";
        case Line::LINE_MAKING:      return "Line of Making";
        case Line::LINE_REVOCATION:  return "Line of Revocation";
        case Line::LINE_VIGOR:       return "Line of Vigor";
        case Line::LINE_WARDING:     return "Line of Warding";
        case Line::LINE_FORBIDDENCE: return "Line of Forbiddence";
    }
}


// void Line::operator=(const Line& r){
//     this->start = r.start;
//     this->end   = r.end;
//     this->lineData = r.lineData;
//     this->vertices = r.vertices;
//     this->lineType = r.lineType;
//     this->accuracy = r.accuracy;
//     this->lineColor  = r.lineColor;
//     this->isFinished = r.isFinished;
// }


void Line::init() {}

Line::~Line() {}

/* 
std::vector<float> Player::update(){
    if (needsUpdate()){ // if we've added some lines since we've last checked
        g::log("Player is updating their lines");
        assert(numLines < lines.size());

        for(auto it = lines.begin() + numLines; it != lines.end(); ++it){
            std::vector<Point> data = it->getData();
            //    data.insert(data.end(), currentLine.begin(), currentLine.end());
            std::vector<Point> tmp = currentLine.getData();
            appendVector(data, tmp);
            
            for (int i = 0; i < data.size(); ++i){
                std::array<float, 2> pointArray = data[i].getVector();
                // std::cout << pointArray[0] << ", " << pointArray[1] << "\n";
                vertices.push_back(pointArray[0]);
                vertices.push_back(pointArray[1]);
                vertices.push_back(lineColor.r);
                vertices.push_back(lineColor.g);
                vertices.push_back(lineColor.b);
                vertices.push_back(lineColor.a);
            }
        }
    }
    // return what you just added, so we can add it to Arena's buffer vector as well
    std::vector<float> tmp2;
    tmp2.insert(tmp2.begin(), vertices.begin() + ((lines.size() - numLines) * 6), vertices.end());
    numLines = lines.size();
    // g::printVector(tmp2, 6);
    g::log("added " + std::to_string((lines.size() - numLines) * 6) + " points to the buffer vector", 4);
    logVal(lines.size());
    logVal(vertices.size());
    logVal(numLines);
    
    return tmp2;
}
 */

