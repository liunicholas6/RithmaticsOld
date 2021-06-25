#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>

//  Regular
//! Warning
//? Blue
//* Bold
//TODO Remember...
//todo remember...
/** @param parameter*/
//// Nevermind

#define RAD2DEG 56.54866776;
#ifndef PI
#define PI 3.14159265358979323846
#endif
#define verbose 4
#define TOO_MUCH 50 * 6
#define FILE_NAME_LENGTH 45

// #define __CLASS__                  \
//     std::string file = __FILE__;   \
//     file.substr(FILE_NAME_LENGTH);

#define appendVector(main, append)                                        \
    switch(append.size()){                                                \
        case 0:                                                           \
            break;                                                        \
        case 1:                                                           \
            main.push_back(append[0]); break;                             \
        default:                                                          \
            main.insert(main.end(), append.begin(), append.end()); break; \
    }

#define appendHeapVector(main, append)                                        \
    switch(append->size()){                                                   \
        case 0:                                                               \
            break;                                                            \
        case 1:                                                               \
            main->push_back((*append)[0]); break;                             \
        default:                                                              \
            main->insert(main->end(), append->begin(), append->end()); break; \
    }

#define logVal(var) std::cout << g::getDebugCount() << ": " << #var << " = " << std::fixed << std::setprecision(16) << var << std::endl;
#define logNamedVal(var, name) std::cout << g::getDebugCount() << ": " << #name << " = " << std::fixed << std::setprecision(16) << var << std::endl;

#define _print(message) std::cout << g::getDebugCount() << ": " << #message;
#define _endl std::cout << std::endl;
#define _rawprint(message) std::cout << ": " << #message;
#define _log g::log

#define START_TIME time_t _start, _finish; \
                   time(&_start);

#define LOG_TIME time(&_finish); \
                 std::string file = __FILE__; \
                 std::cout << g::getDebugCount() << "\b\b\b\b: " << file.substr(FILE_NAME_LENGTH) << "::" << __func__ << " took " \
                 << difftime(_finish, _start) << " seconds to run.\n";

#define START_CLOCK clock_t _how_long; \
                    _how_long = clock();


#define LOG_CLOCK _how_long = clock() - _how_long; \
                  std::string file = __FILE__; \
                  std::cout << g::getDebugCount() << "\b\b\b\b: " << file.substr(FILE_NAME_LENGTH) << "::" << __func__ << " took " \
                  << float(_how_long) << " cycles to run.\n"; // this is not necisarriy "cycles"


/*
0: No debug info
5: All debug info
*/

// This is to be able to have a reversed for-based range loop
// template <typename T>
// struct reversion_wrapper { T& iterable; };
// template <typename T>
// auto begin (reversion_wrapper<T> w) { return std::rbegin(w.iterable); }
// template <typename T>
// auto end (reversion_wrapper<T> w) { return std::rend(w.iterable); }
// template <typename T>
// reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }

namespace g{

unsigned long getDebugCount();

void log(std::string message = "", unsigned int verbosity = verbose);

void printVector(const std::vector<float>& v, int itemsPerLine = -1, int spacing = 11);

void printVector(const std::vector<int>& v, int itemsPerLine = -1, int spacing = 11);

float round(float var, int digits = 1);


// static int set_at(SDL_Surface *surf, int x, int y, Uint32 color, int *drawn_area);

// void printVector(const std::vector<Point>& v, int itemsPerLine = -1, int spacing = 11);
} // namespace g


/*////////////////////////////////////////////////TODO://////////////////////////////////////////////////////
* Finish making lines eraseable
* Line::IdentifyLine() function
//? Hook up GUI for a menu (imgui, nuklear)
** Add options to menu
*** Chalk color
* Add modes
** Tutorial
** Example duels
** Show named defenses
** Practice mode
*** Duel against a stationary defense
** Analyze mode
*** Draw without your lines doing anything, but have it analyze them and print your accuracy
** Helper mode
*** Whenever you draw a circle, lines appear subdividing it for you so you don't have to intuit it 
*** (maybe change how many lines with the scrollwheel?)
** Survival Mode!! 
*** How long can YOU survive against a horde of wild chalklings?
** Autocorrect Mode
*** Maybe consider folding this into another mode, but having the program, when you finish drawing a line,
***    delete it and replace it with the optimal version of that line.
* Add an AI to play against
* Add a starting screen
* Add networking
* Make lines of vigor shoot out
* Allow lines to get broken parts
* Allow lines to get pushed back
* Make it so when you draw, instead of regular lines, it looks like you're drawing chalk
* Add a beginner option, for people who just learned, are drawing with a mouse, or both
*/ 