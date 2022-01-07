#ifndef VAR_H
#define VAR_H

#include <iostream>
using std::string;

#define RESET   "\033[0m"
#define RED     "\u001b[38;5;9m"
#define AQUA   "\033[32m"
#define GREEN   "\u001b[38;5;46m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define PINK    "\u001b[38;5;24526m"
#define GREY    "\u001b[38;5;244m"
#define ORANGE  "\u001b[38;5;3m"
#define MAGENTA   "\u001b[38;5;165m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDGREEN   "\u001b[1m\u001b[38;5;46m"
#define BOLDAQUA   "\033[1m\033[32m"
#define BOLDWHITE   "\033[1m\033[37m"
#define BOLDRED     "\033[1m\033[31m"
#define UNDERLINED_RED "\033[4m\033[1m\033[31m"
#define UNDERLINED_GREEN "\u001b[4m\u001b[38;5;46m"
#define UNDERLINED_YELLOW "\033[4m\033[33m"
#define UNDERLINED_MAGENTA "\033[4m\033[35m"

const string WARNING = UNDERLINED_RED;
const string STATS_UPDATE = RED;
const string SOMETHING_WRONG = BOLDRED;
const string STATS = ORANGE;
const string OBJECT_TAKEN = BOLDGREEN;
const string FIGHT_UPDATE = GREEN;

// #define keyW 72
// #define keyS 80
#define keyW 119
#define keyS 115
#define MaxRoom 15
#define arrowPointer 62
#define keyENTER 10 // 13 on windows
#define OPTIONS 3

#define n 110
#define s 115
#define e 101
#define w 119
// // #define o 111
#define q 113   //quit
#define h 104   //help
#define m 109   //map / eat
#define x 120   // explore
#define t 116   // torch
#define i 105   // inventory
#define c 99    // knife
#define p 112   // punchDamage
#define l 108   // throw knife
#define myFILE "Saves.dat"

#define PUNCH "punch"
#define KNIFE "knife"

const u_int8_t MAIN_BEDROOM = 0;
const u_int8_t SEC_BEDROOM = 1;
const u_int8_t STAIRS_TO_DOWN = 2;
const u_int8_t KITCHEN = 3;
const u_int8_t BATHROOM = 4;
const u_int8_t LUNCHROOM = 5;
const u_int8_t LIVING_ROOM = 6;
const u_int8_t BALCONY = 7;
const u_int8_t STAIRS_TO_UP = 8;
const u_int8_t ARMORY = 9;
const u_int8_t FOOD_STORAGE = 10;
const u_int8_t PRISON = 11;
const u_int8_t CORRIDOR_BALCONY = 12;
const u_int8_t CORRIDOR_SEC_BEDROOM = 13;
const u_int8_t CORRIDOR_KITCHEN = 14;


#endif