#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "locale.h"
#include <unistd.h>
#include <signal.h>

const int ROW = 30;
const int COL = 100;

int DEG = 10;
int C_COLOR = 0;

const char* Ucup = "\U0001F3C6";
const char* Ugold_medal = "\U0001F947";
const char* Usecond_medal = "\U0001F948";
const char* Uthird_medal = "\U0001F949";

const char* Usetting = "🔧";
const char* Udiamond = "\U0001F48E";
const char* Umoney = "\U0001F4B0";
const char* Ugold = "\u25c7";
const char* Ubgold = "\u25CB";

const char* Umorgh = "🍗";
const char* Uburger = "🍔";
const char* Umeat = "🍖";

const char* Umace = "🔨";
const char* Uwand = "𓐩";
const char* Usword = "🗡";
const char* Udagger = "𓌜";
const char* Uarrow = "\U000027B3";// ➳

////rooms:
//0: start
//1: stairs
//2: spellroom

pid_t pid;
int music = 0, spell_music = 0;
int message = -1;
int sp = 0, dm = 0;

char mes[20][300] = {
                    "You just killed a Deamon",
                    "You just killed a Fire Breathing Monster",
                    "You just killed a Giant",
                    "You just killed a Snake",
                    "You just killed an Undeed",

                    "You got the Sword",
                    "You got 10 Daggers",
                    "You got 8 Wands",
                    "You got 20 Arrows",

                    "You got 5 golds",
                    "You got 100 golds",

                    "You got a Health Spell",
                    "You got a Speed Spell",
                    "You got a Damage Spell",

                    "Oh, there was a trap",
                    "Welcome to Enchant Room",
                    "Press g for some help"
                    };

struct Room{
    int x0, y0, x1, y1;
};

struct monster{
    int x, y;
    int type, act, steps;
    int health;
};

struct floor{
    int room_count;
    int stair_x, stair_y;
    struct Room** rooms;
    char** mp, **map;
    int ** vis, **trap;
    int monster_count;
    struct monster** monsters;
};

/// > stair
/// g & G gold
/// f F Y food
/// T treasure
/// s d w a weapons
/// H S D spells
/// D F G S U  monsters// no need?

struct game{
    int current_level, current_x, current_y;
    struct floor* floors[6];
    int Vis;
    int golds, score;
    int hunger, health;
    int food, gfood, mfood;

    int current_w;
    int mace, sword;
    int dagger, wand, arrow;

    int health_s, speed_s, damage_s;
};