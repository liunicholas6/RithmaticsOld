#pragma once

#include "Player.hpp"
#include "Globals.hpp"

#include <GL/gl.h>
#include <map>

// using std::vector;

class Arena{
private:    
public:
    Color background;
    std::map<std::string, Player>* players;
    std::vector<float>* vertices;

    Arena(): 
        background(0.2f, 0.2f, 0.2f, 0.7f),     // nice gray "breeze dark" background color 
        players(new std::map<std::string, Player>),
        vertices(new std::vector<float>)
        { (*players)["user"] = Player("user"); };
    Arena(Player player): 
        background(0.2f, 0.2f, 0.2f, 0.7f),     // nice gray "breeze dark" background color 
        players(new std::map<std::string, Player>),
        vertices(new std::vector<float>)
        { (*players)[player.name] = player; };
    ~Arena();

    void clear(std::string name = "");
    void update();
    std::vector<float>* getVertices();
    std::vector<std::tuple<std::vector<GLint>, std::vector<GLsizei>, int>> getArrangement(); //! Depricated
};