#pragma once

#include "Line.hpp"
#include "Globals.hpp"

#define DRAW_COLOR 0.989f, 0.989f, 0.989f, 0.989f // just a distinct almost white for debugging purposes

class Player{
private:
    unsigned long numLines;

public:
    bool breached;
    Color drawColor;
    std::vector<Line>* lines;
    std::vector<float>* vertices;
    std::string name;

    Player(Color color = Color(DRAW_COLOR)): 
        drawColor(color),
        breached(false), 
        name("___"), 
        numLines(0),
        vertices(new std::vector<float>),
        lines(new std::vector<Line>)
        { init(); };
    Player(std::string name, Color color = Color(DRAW_COLOR)): 
        drawColor(color), 
        breached(false), 
        name(name), 
        numLines(0),
        vertices(new std::vector<float>),
        lines(new std::vector<Line>)
        { init(); };
    ~Player();

    std::vector<float>* update();
    void init();
};