#include "Arena.hpp"

#include <tuple>
// #include "Debug.hpp"

/* 
    std::vector<float> Arena::getLineVertices(){
    std::vector<float> mainVertices;

        for (auto p: players){
            std::vector<float> tmp = p.getVertices();
            mainVertices.insert(mainVertices.end(), tmp.begin(), tmp.end());


                // for (auto k: tmp)
                //     mainVertices.push_back(k);
        }

    // float* Arena::getLineVertices(){
    //     std::vector<float> mainVertices;

        // for (auto p: players){ 
    //         std::vector<float> tmp = players.front().getVertices();

    //             for (auto k: tmp)
    //                 mainVertices.push_back(k);
        // }

        // g::printVector(mainVertices, 5);

        g::log("mainVertices is has " + std::to_string(mainVertices.size()) + " items", 5);

        // auto result = mainVertices.data();
        // float result[mainVertices.size()];
        // std::copy(std::begin(mainVertices), std::end(mainVertices), result);
        return mainVertices;
    }
 */


void Arena::update(){
    // Iterate through all the players, call their update functions, 
    //  and add whatever they return to the main vector of vertices.
    for (auto p: *players){
        std::vector<float>* toAdd = p.second.update();
        if (not toAdd->empty()){
            appendHeapVector(vertices, toAdd);
        }
    }
}

std::vector<std::tuple<std::vector<GLint>, std::vector<GLsizei>, int>> Arena::getArrangement(){
    std::vector<std::tuple<std::vector<GLint>, std::vector<GLsizei>, int>> returnMe;
    std::vector<GLint>   offsets;
    std::vector<GLsizei> lengths;
    int playerSpecificOffset = 0;
    int count  = 0;
    int offset = 0;

    for (auto p: *players){
        offsets.clear();
        lengths.clear();
        playerSpecificOffset = offset;

        for (auto i: *(p.second.lines)){
            assert(offsets.size() == lengths.size());
            if (not i.isNull()){
                lengths.push_back(i.getDataLen());
                offsets.push_back(offset);
                ++count;
            } 
            // Add an extra offset if the line is erased
            offset += i.getDataLen();
        }

        // playerVertcies[p.second.name] = std::pair<int, int>(playerSpecificOffset, offset);
        returnMe.push_back(std::make_tuple(offsets, lengths, count));
    }
    return returnMe;
}

void Arena::clear(std::string name){
    if (name.empty()){
        vertices->clear();
        for (auto p: *players){
            p.second.lines->clear();
            p.second.vertices->clear();
        }
    }
    else{
        (*players)[name].lines->clear();
        (*players)[name].vertices->clear();
    }
}

/* 
std::vector<float>* Arena::getVertices(){
    std::vector<float>* sum = new std::vector<float>;
    for (auto p: *players){

    }
}
 */
Arena::~Arena() {}