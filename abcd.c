#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int ROW = 30;
const int COL = 100;
const int DEG = 10;
const char* Ucup = "\U0001F3C6";
const char* Udiamond = "\U0001F48E";
const char* Ugold_medal = "\U0001F947";
const char* Usecond_medal = "\U0001F948";
const char* Uthird_medal = "\U0001F949";
const char* Umoney = "\U0001F4B0";
const char* Ugold = "\u25c7";
const char* Ubgold = "\u25CB";
const char* Umorgh = "🍗";
const char* Usword = "🗡";
const char* Udagger = "𓌜";
///\U000027B3 ➳

////rooms:
//0: start
//1: stairs
//2: spellroom


struct Room{
    int x0, y0, x1, y1;
};

struct monster{
    int x, y;
    int act, type;
    int health;
};

struct floor{
    int room_count;
    struct Room** rooms;
    char** mp, **map;
    int ** vis, **trap;
    int stair_x, stair_y;
    int monster_count;
    struct monster** monsters;
};

/// > stair
/// g & G gold
/// f food
/// T treasure
/// d w a s weapons
/// H S D spells
/// D F G S U  monsters// no need?

struct game{
    int current_level, current_x, current_y;
    struct floor* floors[6];
    int Vis;
    int golds;
    int hunger, health;
    int food;

    int current_w;
    int mace, sword;
    int dagger, wand, arrow;

    int health_s, speed_s, damage_s;
};