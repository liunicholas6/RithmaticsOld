#include "Player.hpp"
#include "Globals.hpp"

#include <string>

// calculate vertices every time, instead of storing them (less efficent)
/*
    std::vector<float> Player::getVertices(){
        if (numLines != lines.size()){ // if we've added some lines since we've last checked
            std::vector<float> returnArray;
            int i = 0;

            for(auto l: lines){
            std::vector<Point> data = l.getData();
            //    data.insert(data.end(), currentLine.begin(), currentLine.end());
            appendVector(data, currentLine);
                for (i = 0; i < data.size(); ++i){
                    auto pointArray = data[i].getVector();
                    // std::cout << pointArray[0] << ", " << pointArray[1] << "n";
                    returnArray.push_back(pointArray[0]);
                    returnArray.push_back(pointArray[1]);
                    returnArray.push_back(lineColor.r);
                    returnArray.push_back(lineColor.g);
                    returnArray.push_back(lineColor.b);
                    returnArray.push_back(lineColor.a);
                }
            }
        return returnArray;
    }
*/


std::vector<float>* Player::update(){
    // _print(I got to the beginning of player::update)
    std::vector<float>* updated =  new std::vector<float>;

    // If the last line is still being drawn
    if (lines->size() and not lines->back().isFinished and not lines->back().isNull()){
        std::vector<float>* tmp = lines->back().update(drawColor);
        switch(tmp->size()){
            case 0:
                break;
            case 1:
                vertices->push_back((*tmp)[0]); break;
            default:
                vertices->insert(vertices->end(), tmp->begin(), tmp->end()); break;
        }
        switch(tmp->size()){
            case 0:
                break;
            case 1:
                updated->push_back((*tmp)[0]); break;
            default:
                updated->insert(updated->end(), tmp->begin(), tmp->end()); break;
        }
        delete tmp;
    }

    // Assumes that you haven't added any lines to the current stack in drawing the current line
    if ((vertices->size() != (lines->size() * 6)) and lines->back().isFinished){
        for(auto it = lines->begin() + (vertices->size() / 6); it < lines->end(); ++it){
            if (not it->isNull()){
                std::vector<float>* tmp = it->update(drawColor);
                switch(tmp->size()){
                    case 0:
                        break;
                    case 1:
                        vertices->push_back((*tmp)[0]); break;
                    default:
                        vertices->insert(vertices->end(), tmp->begin(), tmp->end()); break;
                }
                switch(tmp->size()){
                    case 0:
                        break;
                    case 1:
                        updated->push_back((*tmp)[0]); break;
                    default:
                        updated->insert(updated->end(), tmp->begin(), tmp->end()); break;
                }
                delete tmp;
            }
        }
    }

    // Check all the lines to see that they haven't been deleted
    for (int i = 0; i < lines->size(); ++i){
        if ((*lines)[i].isNull()){
            _log("Deleting the line at index " + std::to_string(i));
            _log("(There are " + std::to_string(lines->size()) + " lines)");
            lines->erase(lines->begin() + i);

            int offset = 0;
            // Now erase it's vertices
            for (int k = 0; k < i; ++k){
                offset += (*lines)[k].getDataLen();
            }
            vertices->erase(vertices->begin() + (offset * 6), vertices->begin() + (offset * 6) + (*lines)[i].getDataLen());
        }

        // While we're iterating through all the lines, check for dots...
        if((*lines)[i].lineData->size() == 1 and not (*lines)[i].vertices->size() and (*lines)[i].isFinished and not (*lines)[i].isNull()){
            std::vector<float>* tmp = (*lines)[i].update(drawColor, true);
                switch(tmp->size()){
                    case 0:
                        break;
                    case 1:
                        vertices->push_back((*tmp)[0]); break;
                    default:
                        vertices->insert(vertices->end(), tmp->begin(), tmp->end()); break;
                }
                switch(tmp->size()){
                    case 0:
                        break;
                    case 1:
                        updated->push_back((*tmp)[0]); break;
                    default:
                        updated->insert(updated->end(), tmp->begin(), tmp->end()); break;
                }
                delete tmp;
        }
    }

    return updated;
}


void Player::init() {}

Player::~Player() {}