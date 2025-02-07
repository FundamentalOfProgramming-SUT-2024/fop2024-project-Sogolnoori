#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "knxjkal.c"

#include "locale.h"
#include <unistd.h>
#include <signal.h>

void update(struct game* game);

void music_border(){
    clear();
    for (int i = 0; i < COLS; i ++){
        mvprintw(0, i, "♫");
        mvprintw(LINES - 1, i, "♫");
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, "♫");
        mvprintw(i, COLS - 1, "♫");
    }
}

void music_menu(){
    char graph[24][1000] = {
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⢴⡖⠶⢦⣤⣤⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠚⠂⠑⢀⣤⣀⢀⣉⠉⠙⠛⠛⠻⠶⠶⣶⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠆⡄⢰⡞⠯⢭⣷⣚⣻⢿⣷⣶⣶⣴⣤⣤⣄⣉⡙⠛⠿⠿⣶⣶⣤⣤⣀⡀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠀⡇⡸⠀⠀⠀⠀⠈⠉⠉⠛⠛⠒⠿⠿⠯⣷⣖⣊⠭⢕⣒⡐⠠⠉⠙⠻⠩⠓⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⡇⡀⠙⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠒⠷⢦⣤⡘⢀⢤⡾⡀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠐⠠⠨⠟⠛⢶⣶⣤⣤⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣨⡾⡗⢰⠁",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⡿⡴⠠⢷⣶⡂⠢⠄⢀⣉⡈⠋⠛⠟⠳⡶⢦⣤⣄⣀⠀⠀⠀⠀⠀⠀⡸⢠⠇⡸⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡘⢳⠀⡞⠙⠓⠶⠯⣿⣓⣶⡮⢭⣔⣒⡠⠤⢁⣈⠈⠉⠛⠛⠶⠦⢤⣀⠀⣼⠀⡇⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠠⣞⣽⠯⢭⠓⢦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠌⢸⣦⠁⠀⠀⠀⠀⠀⠀⠈⠉⠙⠛⠾⠿⣿⣶⣶⠬⣭⣒⣂⠤⢀⣠⡈⢱⠏⢰⠁⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠐⠈⢠⠶⣤⡀⢁⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠃⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠻⠿⢧⣄⡘⢹⡛⠀⣼⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⢠⢛⡛⠤⠶⠆⠜⠉⡱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣄⣀⣀⣀⠀⠀⠘⠀⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣩⣾⡇⢠⠇⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠈⠻⢤⣠⡆⣀⣠⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡶⠟⠻⠿⠿⢧⣀⠉⠩⠉⠙⡃⢦⢈⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢁⢰⠃⣸⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣻⣤⠄⠠⡶⠿⣛⣛⣛⣻⣷⡍⠐⠒⢶⢽⣿⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡈⡼⠀⡇⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠙⡁⢀⡴⠚⠉⠉⠉⠉⠉⠉⠙⠳⣦⠋⢀⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢡⠇⢸⠁⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣕⠠⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢈⠁⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠨⠆⠺⠀⡏⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡤⣀⠀⠀⠀⠀⠀⠀⣜⠸⠣⠈⠠⣤⡀⠀⠀⠀⠀⠀⠀⣀⣤⢤⡆⠐⠁⣸⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣴⣶⣶⣿⣿⠛⠿⠛⠲⡦⣄⠀⣷⠸⠁⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠟⠁⠀⠀⠀⠀⠀⠀⠘⢦⡀⠘⠡⢦⠍⠛⠓⠒⢲⣿⡿⠏⢒⡜⠁⠀⡴⠃⠀⠀⠀⠀⠀⢀⢖⠟⣩⠉⣉⣩⣭⣤⣤⣴⣤⠐⠐⠃⢬⣺⣿⡄⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⢸⠀⠀⠀⠀⠀⠀⠀⠈⠙⢦⣄⡀⠈⢲⣶⣶⣶⡶⠶⠿⢛⣡⠴⠋⠁⠀⠀⠀⠀⠀⠀⠰⠿⠃⠀⢙⣫⡭⠵⠶⠖⠒⠒⠶⠦⣤⠀⢻⣿⢧⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⡀⠋⠀⢂⢀⣤⣤⣄⡀⠀⠀⠀⠀⠀⠈⠉⠛⠒⠶⠶⠶⠒⠚⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣄⠀⣄⠼⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠚⡹⠃⢸⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⠝⠛⠛⠀⢸⠤⠠⠄⠀⠠⠠⠥⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣆⠘⠃⠄⠐⠲⢤⣤⣤⣀⣠⣤⣤⠶⠋⠺⠇⠁⢀⡏⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢮⣁⣀⣀⣀⡀⠸⡇⢀⣤⠤⠤⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢦⡀⠀⠀⠠⢀⣀⠀⠀⢀⠙⣁⣠⣴⠂⢀⣠⠏⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠈⠃⠀⡄⢈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠳⠦⣤⣄⣛⣛⣛⣛⣛⣋⣩⡥⠖⠋⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠲⠴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"};

    clear();
    int cr = 0, op = 5;
    char options[10][50] = {"NONE", "Music 1", "Music 2", "Music 3", "Music 4"};
    while(1){
        noecho();
        clear();
        noecho();
        music_border();
        for (int i = 0; i < 24; i ++){
            mvprintw(i + 3 + (LINES - ROW) / 2, 22 + (COLS - COL) / 2, graph[i]);
        }
        mvprintw(4, 5, "CHOOSE A MUSIC:");
        mvprintw(LINES - 2, 4, "Press m to exit");
        for (int i = 0; i < op; i ++){
            mvprintw(6 + i, 7, options[i]);
        }
        for (int i = 0; i < op; i ++){
            if(cr == i){
                attron(A_REVERSE);
                mvprintw(6 + i, 7, options[i]);
                attroff(A_REVERSE);
            }
        }
        attron(COLOR_PAIR(7));
        mvprintw(6 + music, 15, "✔");
        attroff(COLOR_PAIR(7));
        refresh();
        int get = getch();
        if(get == KEY_UP) cr = (cr + op - 1) % op;
        else if(get == KEY_DOWN) cr = (cr + 1) % op;
        if(get == '\n'){
            if(music != 0) kill(pid, SIGTERM);
            music = cr;
            if(music != 0){
                pid = fork();
                char mus[100] = "music0.mp3";
                mus[5] = '0' + music;
                if(pid == 0){
                    execlp("mpg123", "mpg123", "-q", "--loop", "-1", mus, NULL);
                }
            }
        }
        if(get == 'Q' || get == 'm') return;
    }
    return;
}

int move_char(struct game* game, char e){
    int xp = 0, yp = 0;
    if(e == 'b' || e == 'k' || e == 'n') xp = 1;
    if(e == 'y' || e == 'j' || e == 'u') xp = -1;
    if(e == 'u' || e == 'l' || e == 'n') yp = 1;
    if(e == 'y' || e == 'h' || e == 'b') yp = -1;
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    x += xp;
    y += yp;
    if(x < 0 || x >= ROW) return 0;
    if(y < 0 || y >= COL) return 0;
    char val[10] = {'.', '#', '+', '<'};//< is not important
    bool can_go = 0;
    for (int i = 0; i < 4; i ++) if(floor -> mp[x][y] == val[i]) can_go = 1;
    if(!can_go) return 0;
    for (int m = 0; m < floor -> monster_count; m ++){
        if(floor -> monsters[m] -> act == 1 && floor -> monsters[m] -> x == x && floor -> monsters[m] -> y == y) return 0;
    }
    game -> current_x += xp;
    game -> current_y += yp;
    if(game -> hunger + 1 <= DEG * 40){
        game -> hunger ++;
    }
    else{
        game -> health --;
    }
    update(game);
    return 1;
}

void go_down(struct game* game){
    if(game -> current_x != game -> floors[game -> current_level] -> stair_x) return;
    if(game -> current_y != game -> floors[game -> current_level] -> stair_y) return;
    if(game -> current_level == 4){
        /////i dont knowww
        return;
    }
    game -> current_level ++;
    game -> current_x = game -> floors[game -> current_level] -> stair_x;
    game -> current_y = game -> floors[game -> current_level] -> stair_y;
    update(game);
    return;
}

void go_up(struct game* game){
    if(game -> current_x != game -> floors[game -> current_level] -> stair_x) return;
    if(game -> current_y != game -> floors[game -> current_level] -> stair_y) return;
    if(game -> current_level == 0){
        /////i dont knowww again!
        return;
    }
    game -> current_level --;
    game -> current_x = game -> floors[game -> current_level] -> stair_x;
    game -> current_y = game -> floors[game -> current_level] -> stair_y;
    update(game);
    return;
}

void foodize(struct floor* floor, int k){
    for (int i = 0; i < k; i ++){
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floor -> map[x][y] != '.') continue;
            if(floor -> trap[x][y] == 1) continue;
            if(floor -> map[x][y + 1] != '.') continue;
            if(floor -> map[x][y - 1] != '.') continue;
            int rnd = rand() % 6;
            if(rnd == 0){
                floor -> map[x][y] = 'F';
            }
            else if(rnd == 1){
                floor -> map[x][y] = 'Y';
            }
            else floor -> map[x][y] = 'f';
            break;
        }
    }
    return;
}

void food_menu(struct game* game){
    int cr = 0;
    while(1){
        clear();
        int lxmcjslj = 5, kdjlsj = COL / 2 - 20;
        int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
        mvprintw(lxmcjslj + kx - 3, kdjlsj - 1 + ky, "|");
        for (int i = 0; i < 40; i ++) mvprintw(lxmcjslj - 1 + kx - 3, kdjlsj + i + ky, "-");
        for (int i = 0; i < 40; i ++) mvprintw(lxmcjslj + 1 + kx - 3, kdjlsj + i + ky, "-");
        mvprintw(lxmcjslj + kx - 3, kdjlsj + 40 + ky, "|");

        attron(COLOR_PAIR(5));
        if(game -> hunger / DEG == 40) attron(COLOR_PAIR(6));
        for (int i = 0; i < game -> hunger / DEG; i ++){
            mvprintw(lxmcjslj + kx - 3, kdjlsj + i + ky, " ");
        }
        if(game -> hunger / DEG == 40) attron(COLOR_PAIR(6));
        attroff(COLOR_PAIR(5));

        mvprintw(lxmcjslj - 2 + kx - 3, kdjlsj + 17 + ky, "HUNGER:");
        ///normal
        mvprintw(7 + kx, kdjlsj + 14 + ky, "Normal Foods");
        mvprintw(7 + kx, kdjlsj + 26 + ky, Umorgh);
        mvprintw(9 + kx, kdjlsj + 15 + ky, "Quantity: %d", game -> food);
        if(cr == 0) attron(COLOR_PAIR(5));
        mvprintw(11 + kx, kdjlsj + 13 + ky, "EAT NORMAL FOOD");
        if(cr == 0) attroff(COLOR_PAIR(5));
        ///good
        mvprintw(14 + kx, kdjlsj + 12 + ky, "Exellent Foods");
        mvprintw(14 + kx, kdjlsj + 27 + ky, Uburger);
        mvprintw(16 + kx, kdjlsj + 15 + ky, "Quantity: %d", game -> gfood);
        if(cr == 1) attron(COLOR_PAIR(5));
        mvprintw(18 + kx, kdjlsj + 12 + ky, "EAT Exellent FOOD");
        if(cr == 1) attroff(COLOR_PAIR(5));
        ///magic
        mvprintw(21 + kx, kdjlsj + 14 + ky, "Magic Foods");
        mvprintw(21 + kx, kdjlsj + 25 + ky, Umeat);
        mvprintw(23 + kx, kdjlsj + 15 + ky, "Quantity: %d", game -> mfood);
        if(cr == 2) attron(COLOR_PAIR(5));
        mvprintw(25 + kx, kdjlsj + 13 + ky, "EAT Magic FOOD");
        if(cr == 2) attroff(COLOR_PAIR(5));

        mvprintw(LINES - 2, 4, "Press E to exit");
        refresh();
        ////KEYS
        int get = getch();
        if(get == 'Q' || get == 'E') break;
        if((get == 'e' || get == '\n')){
            if(cr == 0 && game -> food){
                game -> food --;
                if(rand() % 10 == 0){
                    if(game -> hunger >= DEG) game -> hunger += DEG;
                    game -> health -= DEG * 2;
                    if(game -> health > 100) game -> health = 100;
                    if(game -> health < 0) game -> health = 0;
                }
                else{
                    if(game -> hunger >= DEG) game -> hunger -= DEG;
                    game -> health += DEG * 2;
                    if(game -> health > 100) game -> health = 100;
                }
            }
            if(cr == 1 && game -> gfood){
                game -> gfood --;
                if(game -> hunger >= DEG) game -> hunger -= DEG;
                game -> health += DEG * 2;
                if(game -> health > 100) game -> health = 100;
                dm += 10;
            }
            if(cr == 2 && game -> mfood){
                game -> mfood --;
                if(game -> hunger >= DEG) game -> hunger -= DEG;
                game -> health += DEG * 2;
                if(game -> health > 100) game -> health = 100;
                sp += 10;
            }
        }
        if(get == KEY_UP) cr = (cr + 2) % 3;
        if(get == KEY_DOWN) cr = (cr + 1) % 3;
    }
    return;
}

int monster_move_check(struct game* game, int m, int d){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    struct monster *monster = floor -> monsters[m];
    if(d == 0 && monster -> x == x) return 0;
    if(d == 1 && monster -> y == y) return 0;
    int x2 = monster -> x, y2 = monster -> y;
    if(d == 0) x2 += monster -> x < x ? 1 : -1;
    if(d == 1) y2 += monster -> y < y ? 1 : -1;
    if(x2 == x && y2 == y) return 0;
    if(floor -> map[x2][y2] == '|' || floor -> map[x2][y2] == '-' || floor -> map[x2][y2] == '+') return 0;
    for (int mm = 0; mm < floor -> monster_count; mm ++){
        if(floor -> monsters[mm] -> act != 1) continue;
        if(mm != m && floor -> monsters[mm] -> x == x2 && floor -> monsters[mm] -> y == y2){
            return 0;
        }
    }
    return 1;
}

void update(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    message = -1;
    ////VISROOMS
    for (int r = 0; r < floor -> room_count; r ++){
        int x0 = floor -> rooms[r] -> x0;
        int x1 = floor -> rooms[r] -> x1;
        int y0 = floor -> rooms[r] -> y0;
        int y1 = floor -> rooms[r] -> y1;
        if(x0 <= x && x <= x1 && y0 <= y && y <= y1){
            for (int i = x0; i <= x1; i ++){
                for (int j = y0; j <= y1; j ++){
                    floor -> vis[i][j] = 1;
                }
            }
        }
    }
    ////VISCORRIDERS
    for (int i = 0; i <= 3; i ++){
        char ch = floor -> mp[x + i][y];
        if(0 <= x + i && x + i < ROW && (ch == '#' || ch == '+')) floor -> vis[x + i][y] = 1;
        else break;
    }
    for (int i = 0; i >= -3; i --){
        char ch = floor -> mp[x + i][y];
        if(0 <= x + i && x + i < ROW && (ch == '#' || ch == '+')) floor -> vis[x + i][y] = 1;
        else break;
    }
    for (int j = 0; j <= 3; j ++){
        char ch = floor -> mp[x][y + j];
        if(0 <= y + j && y + j < COL && (ch == '#' || ch == '+')) floor -> vis[x][y + j] = 1;
        else break;
    }
    for (int j = 0; j >= -3; j --){
        char ch = floor -> mp[x][y + j];
        if(0 <= y + j && y + j < COL && (ch == '#' || ch == '+')) floor -> vis[x][y + j] = 1;
        else break;
    }
    ////CHECKFOOD
    if(floor -> map[x][y] == 'f' && game -> food < 5){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> food ++;
        foodize(floor, 1);
    }
    if(floor -> map[x][y - 1] == 'f' && game -> food < 5){
        floor -> map[x][y - 1] = floor -> mp[x][y - 1];
        game -> food ++;
        foodize(floor, 1);
    }
    if(floor -> map[x][y] == 'F' && game -> gfood < 5){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> gfood ++;
        foodize(floor, 1);
    }
    if(floor -> map[x][y - 1] == 'F' && game -> gfood < 5){
        floor -> map[x][y - 1] = floor -> mp[x][y - 1];
        game -> gfood ++;
        foodize(floor, 1);
    }
    if(floor -> map[x][y] == 'Y' && game -> mfood < 5){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> mfood ++;
        foodize(floor, 1);
    }
    if(floor -> map[x][y - 1] == 'Y' && game -> mfood < 5){
        floor -> map[x][y - 1] = floor -> mp[x][y - 1];
        game -> mfood ++;
        foodize(floor, 1);
    }
    ///CHECKSPELLROOM
    if(level != 5){
        int x0 = floor -> rooms[2] -> x0, x1 = floor -> rooms[2] -> x1;
        int y0 = floor -> rooms[2] -> y0, y1 = floor -> rooms[2] -> y1;
        if(x0 <= x && x1 && y0 <= y && y <= y1){
            game -> health -= 1;
        }
        if(x0 <= x && x <= x1 && y0 <= y && y <= y1){
            message = 15;
        }
    }
    ////CHECKGOLD
    if(floor -> map[x][y] == 'g'){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> golds += 5;
        message = 9;
    }
    if(floor -> map[x][y] == 'G'){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> golds += 100;
        message = 10;
    }
    ////CHECKTRAP
    if(floor -> trap[x][y] == 1){
        floor -> map[x][y] = '^';
        game -> health -= 5;
        message = 14;
    }
    ///MONSTER MOVES
    for (int m = 0; m < floor -> monster_count; m ++){
        struct monster* monster = floor -> monsters[m];
        if(monster -> act == -1) continue;
        monster -> act = 0;
        int same_room = 0;
        for (int r = 0; r < floor -> room_count; r ++){
            struct Room* room = floor -> rooms[r];
            int x0 = room -> x0, x1 = room -> x1;
            int y0 = room -> y0, y1 = room -> y1;
            if(x0 <= game -> current_x && game -> current_x <= x1 && y0 <= game -> current_y && game -> current_y <= y1){
                if(x0 <= monster -> x && monster -> x <= x1 && y0 <= monster -> y && monster -> y <= y1){
                    same_room = 1;
                }
                break;
            }
        }
        if(same_room){
            monster -> act = 1;
        }
        if(monster -> act == 1 && monster -> steps > 0){            
            if(rand() % 2 && monster_move_check(game, m, 0)){
                monster -> x += monster -> x < x ? 1 : -1;
                monster -> steps --;
            }
            else if(monster_move_check(game, m, 1)){
                monster -> y += monster -> y < y ? 1 : -1;
                monster -> steps --;
            }
            else if(monster_move_check(game, m, 0)){
                monster -> x += monster -> x < x ? 1 : -1;
                monster -> steps --;
            }
        }
    }
    ///MONSTER DAMAGE
    for (int m = 0; m < floor -> monster_count; m ++){
        struct monster* monster = floor -> monsters[m];
        int xm = monster -> x, ym = monster -> y;
        if(monster -> act != 1) continue;
        if(abs(xm - x) <= 1 && abs(ym - y) <= 1){
            game -> health -= monster -> type + 1;
        }
    }
    return;
}

char ** draw(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    char **drw = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        drw[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) drw[i][j] = ' ';
    }
    for (int i = 0; i < ROW; i ++){
        for (int j = 0; j < COL; j ++){
            drw[i][j] = ' ';
            if(floor -> vis[i][j] || game -> Vis) drw[i][j] = floor -> map[i][j];
        }
    }
    /////moondeeee
    drw[x][y] = '@';
    return drw;
}

void get_weapon(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    if(floor -> map[x][y] == 's'){
        game -> sword = 1;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 5;
    }
    if(floor -> map[x][y - 1] == 's'){
        game -> sword = 1;
        floor -> map[x][y - 1] = floor -> mp[x][y - 1];
        message = 5;
    }
    if(floor -> map[x][y] == 'd'){
        game -> dagger += 10;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 6;
    }
    if(floor -> map[x][y] == 'w'){
        game -> wand += 8;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 7;

    }
    if(floor -> map[x][y] == 'a'){
        game -> arrow += 20;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 8;

    }
    return;
}

void get_spell(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    if(floor -> map[x][y] == 'H'){
        game -> health_s ++;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 11;
    }
    if(floor -> map[x][y] == 'S'){
        game -> speed_s ++;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 12;
    }
    if(floor -> map[x][y] == 'D'){
        game -> damage_s ++;
        floor -> map[x][y] = floor -> mp[x][y];
        message = 13;
    }
    return;
}

void end_spell_music(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    int x0 = floor -> rooms[2] -> x0, x1 = floor -> rooms[2] -> x1;
    int y0 = floor -> rooms[2] -> y0, y1 = floor -> rooms[2] -> y1;
    if(spell_music && !(x0 <= x && x <= x1 && y0 <= y && y <= y1)){
        kill(pid, SIGTERM);
        if(music != 0){
            pid = fork();
            char mus[100] = "music0.mp3";
            mus[5] = '0' + music;
            if(pid == 0){
                execlp("mpg123", "mpg123", "-q", "--loop", "-1", mus, NULL);
            }
        }
        spell_music = 0;
    }
    return;
}

void spellroom(struct game* game, char** to){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    int x0 = floor -> rooms[2] -> x0, x1 = floor -> rooms[2] -> x1;
    int y0 = floor -> rooms[2] -> y0, y1 = floor -> rooms[2] -> y1;
    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    if(x0 <= x && x <= x1 && y0 <= y && y <= y1 && !spell_music){
        message = 15;
    }
    for (int x = x0; x <= x1; x ++){
        for (int y = y0; y <= y1; y ++){
            if(to[x][y] != '.' && to[x][y] != '-' && to[x][y] != '|' && to[x][y] != '+') continue;
            int cl = 8;
            if(to[x][y] == '.') cl = 9 + rand() % 2;
            attron(COLOR_PAIR(cl));
            mvprintw(x + kx, y + ky, "%c", to[x][y]);
            attroff(COLOR_PAIR(cl));
        }
    }
    if(music && !spell_music && x0 <= x && x <= x1 && y0 <= y && y <= y1){
        kill(pid, SIGTERM);
        pid = fork();
        char mus[100] = "spellroom.mp3";
        if(pid == 0){
            execlp("mpg123", "mpg123", "-q", "--loop", "-1", mus, NULL);
        }
        spell_music = 1;
    }
    end_spell_music(game);
    return;
}

void weapon_menu(struct game* game){
    char select[4][100] = {
        "  __  ___ _   ___ ________  ",
        "/' _/| __| | | __/ _/_   _| ",
        "`._`.| _|| |_| _| \\__ | |   ",
        "|___/|___|___|___\\__/ |_|   "};
    char mace[6][100] = {
        "                            Ʌ___",
        "                         .'  v   `.",
        "/|====================== | ^   v  |>",
        "\\|====================== |   v   ^|>",
        "                          `.____.'",
        "                              V"};

    char sword[4][100] = {
        "              /\\                              ______,....------,",
        "/VVVVVVVVVVVVVV|===================\"\"\"\"\"\"\"\"\"\"\"\"       ___,..-'",
        "`^^^^^^^^^^^^^^|======================----------\"\"\"\"\"\"",
        "              \\/"};

    char dagger[5][100] = {
        "          |ˉ|",
        " _________| |------------------\\",
        "| | | | |   |-------------------〉",
        " `^^^^^^^^| |------------------⁄",
        "          |ˍ|"};

    char wand[10][100] = {
        "         ☆",
        "       ☆ ★ ☆",
        "      ˍ  ☆",
        "     //",
        "    //",
        "   //",
        "  //",
        " //",
        "//",
        "ˉ"};

    char arrow[7][100] = {
        "        (",
        "         \\",
        "          )",
        ">>>---------------->",
        "          )",
        "         /",
        "        ("};
    
    int damage[5] = {5, 10, 12, 15, 5}; 
    int page = 0;
    while(1){
        clear();
        if(page == 0){
            for (int i = 0; i < 6; i ++){
                mvprintw(3 + i, COLS / 2 - 17, mace[i]);
            }
            mvprintw(LINES / 2 - 3, COLS / 2 - 4, "Mace ");
            mvprintw(LINES / 2 - 3, COLS / 2 + 1, Umace);
            mvprintw(LINES / 2 + 3, COLS / 2 - 7, "Ready to use!");
            mvprintw(LINES / 2 + 1, COLS / 2 - 6, "Short-Range");
        }
        if(page == 1){
            for (int i = 0; i < 4; i ++){
                mvprintw(3 + i, COLS / 2 - 32, sword[i]);
            }
            mvprintw(LINES / 2 - 3, COLS / 2 - 4, "Sword ");
            mvprintw(LINES / 2 - 3, COLS / 2 + 2, Usword);
            mvprintw(LINES / 2 + 3, COLS / 2 - 6, "Quantity: %d", game -> sword);
            mvprintw(LINES / 2 + 1, COLS / 2 - 6, "Short-Range");
        }
        if(page == 2){
            for (int i = 0; i < 5; i ++){
                mvprintw(3 + i, COLS / 2 - 16, dagger[i]);
            }
            mvprintw(LINES / 2 - 3, COLS / 2 - 4, "Dagger ");
            attron(COLOR_PAIR(2));
            mvprintw(LINES / 2 - 3, COLS / 2 + 3, Udagger);
            attroff(COLOR_PAIR(2));
            mvprintw(LINES / 2 + 3, COLS / 2 - 6, "Quantity: %d", game -> dagger);
            mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Effective Range: %d", 5);
        }
        if(page == 3){
            for (int i = 0; i < 10; i ++){
                mvprintw(1 + i, COLS / 2 - 6, wand[i]);
            }
            mvprintw(LINES / 2 - 3, COLS / 2 - 3, "Wand ");
            attron(COLOR_PAIR(4));
            mvprintw(LINES / 2 - 3, COLS / 2 + 2, Uwand);
            attroff(COLOR_PAIR(4));
            mvprintw(LINES / 2 + 3, COLS / 2 - 6, "Quantity: %d", game -> wand);
            mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Effective Range: %d", 10);
        }
        if(page == 4){
            for (int i = 0; i < 7; i ++){
                mvprintw(3 + i, COLS / 2 - 9, arrow[i]);
            }
            mvprintw(LINES / 2 - 3, COLS / 2 - 4, "Arrow");
            attron(COLOR_PAIR(3));
            mvprintw(LINES / 2 - 3, COLS / 2 + 2, Uarrow);
            attroff(COLOR_PAIR(3));
            mvprintw(LINES / 2 + 3, COLS / 2 - 6, "Quantity: %d", game -> arrow);
            mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Effective Range: %d", 5);
        }
        mvprintw(LINES / 2 - 1, COLS / 2 - 5, "Damage: %d", damage[page]);
        if(page == game -> current_w){
            attron(COLOR_PAIR(7));
        }
        for (int i = 0; i < 4; i ++){
            mvprintw(LINES - 7 + i, COLS / 2 - 14, select[i]);
        }
        if(page == game -> current_w){
            attroff(COLOR_PAIR(7));
        }
        mvprintw(LINES - 2, 4, "Press i to exit");
        if(page != 0){
            mvprintw(LINES / 2 - 2, 0, "/-");
            mvprintw(LINES / 2 - 1, 0, "\\-");
        }
        if(page != 4){
            mvprintw(LINES / 2 - 2, COLS - 3, "-\\");
            mvprintw(LINES / 2 - 1, COLS - 3, "-/");
        }
        refresh();
        ////KEYS
        int get = getch();
        if(get == 'Q' || get == 'i') return;
        if(get == KEY_RIGHT && page != 4) page ++;
        if(get == KEY_LEFT && page != 0) page --;
        if(get == '\n'){
            int t = 1;
            if(page == 1) t = game -> sword > 0;
            if(page == 2) t = game -> dagger > 0;
            if(page == 3) t = game ->  wand > 0;
            if(page == 4) t = game -> arrow > 0;
            if(!t) continue;
            game -> current_w = page;
        }
    }
    return;
}

void spell_menu(struct game* game){
    char select[4][100] = {
        "  __  ___ _   ___ ________  ",
        "/' _/| __| | | __/ _/_   _| ",
        "`._`.| _|| |_| _| \\__ | |   ",
        "|___/|___|___|___\\__/ |_|   "};

    char wis[14][100] = {
        "                   .",
        "         /^\\     .",
        "    /\\   \"V\"",
        "   /__\\   I      O  o",
        "  //..\\\\  I     .",
        "  \\].`[/  I",
        "  /l\\/j\\  (]    .  O",
        " /. ~~ ,\\/I          .",
        " \\\\L__j^\\/I       o",
        "  \\/--v}  I     o   .",
        "  |    |  I   _________",
        "  |    |  I c(`       ')o",
        "  |    l  I   \\.     ,/",
        "_/j  L l\\_!  _//^---^\\\\_  "};

    int cr = 0;
    while(1){
        clear();
        for (int i = 0; i < 14; i ++){
            mvprintw(LINES / 2 - 7 + i, 10, wis[i]);
        }

        mvprintw(LINES / 2 - 10, COLS / 3, "Health Spell:");
        mvprintw(LINES / 2, COLS / 3, "Speed Spell:");
        mvprintw(LINES / 2 + 10, COLS / 3, "Damage Spell:");

        mvprintw(LINES / 2 - 10 + 2, COLS / 3 + 2, "Quantity: %d", game -> health_s);
        mvprintw(LINES / 2 + 2, COLS / 3 + 2, "Quantity: %d", game -> speed_s);
        mvprintw(LINES / 2 + 10 + 2, COLS / 3 + 2, "Quantity: %d", game -> damage_s);

        attron(COLOR_PAIR(12));
        mvprintw(LINES / 2 - 10, COLS / 3 + 14, "✩");
        attroff(COLOR_PAIR(12));
        attron(COLOR_PAIR(13));
        mvprintw(LINES / 2, COLS / 3 + 14, "✩");
        attroff(COLOR_PAIR(13));
        attron(COLOR_PAIR(14));
        mvprintw(LINES / 2 + 10, COLS / 3 + 14, "✩");

        attroff(COLOR_PAIR(14));
        
        for (int l = 0; l < 3; l ++){
            if(cr == l){
                attron(COLOR_PAIR(22));
            }
            for (int i = 0; i < 4; i ++){
                mvprintw(LINES / 2 + 10 * (l - 1) + i - 2, COLS / 2 + 24, select[i]);
            }
            if(cr == l){
                attroff(COLOR_PAIR(22));
            }
        }
        mvprintw(LINES - 2, 4, "Press s to exit");
        refresh();
        ////KEYS
        int get = getch();
        if(get == 'Q' || get == 's') return;
        if(get == KEY_UP) cr = (cr + 2) % 3;
        if(get == KEY_DOWN) cr = (cr + 1) % 3;
        if(get == '\n'){
            if(cr == 0){
                if(game -> health_s > 0){
                    game -> health = 100;
                    game -> health_s --;
                }
            }
            if(cr == 1){
                if(game -> speed_s > 0){
                    sp = 10;
                    game -> speed_s --;
                }
            }
            if(cr == 2){
                if(game -> damage_s > 0){
                    dm = 10;
                    game -> damage_s --;
                }
            }
        }
    }
    return;
}

void guide_border(){
    clear();
    for (int i = 0; i < COLS; i ++){
        mvprintw(0, i, "?");
        mvprintw(LINES - 1, i, "?");
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, "?");
        mvprintw(i, COLS - 1, "?");
    }
}

void guide_menu(){
    char guide[35][200] = {
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡴⣾⢻⣻⠭⠓⠻⠀⠀⠀⠀⠀⠈⠉⠉⠛⠛⠒⠦⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⠖⠛⣿⣾⣓⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⡾⠿⠒⠚⠋⠉⠉⠉⠉⠉⠉⠉⠉⠛⠒⠶⠦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠳⣤⡀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⢀⣠⠾⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠶⣄⡀⠀⠀⠀⣤⣀⣄⣰⣄⡿⢸⠏⡿⣆⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⢀⣴⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⢦⣼⣿⢏⡼⢻⠏⡼⢣⡿⣾⣱⠉⣷⠀⠀⠀⠀",
        "⠀⠀⠀⠀⣰⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣄⣜⣷⣟⣨⠏⣴⣃⡞⣹⣇⡟⣴⢋⣧⠀⠀⠀",
        "⠀⠀⠀⣼⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡽⢻⣍⠑⣿⣏⣴⢁⡞⢱⠏⡾⣱⢏⡾⣸⡇⠀⠀",
        "⠀⠀⣼⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣭⡾⣋⡽⡃⠈⠫⣿⠞⢰⡇⡼⢡⡿⣿⣡⢯⣷⠀⠀",
        "⠀⢰⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⡟⢉⡶⢛⣀⠀⠺⣿⠋⡜⢃⣞⣿⣻⢋⣾⣽⠀⠀",
        "⢀⣸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢴⣾⣯⣞⣯⡧⠀⠀⢸⡾⢡⣾⡿⣿⠃⡾⣽⡟⠀⠀",
        "⢸⡿⢀⣇⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣻⢻⠟⣷⡶⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠋⠙⢻⣿⡇⠀⠀⠸⣧⣞⡽⣷⢋⡾⣹⣻⠃⠀⠀",
        "⢸⡇⢸⡁⣿⣷⢶⣶⣤⣀⡀⠀⠀⠀⣼⣿⣯⣿⣿⠞⠉⠀⠈⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠃⠀⠀⢠⣿⡞⣳⣯⠞⣠⣿⠃⠀⠀⠀",
        "⠀⠻⠾⣧⣿⣿⣼⣼⣿⣥⣷⣀⣀⣀⣿⣿⠞⠁⠀⠀⠀⠀⠀⢸⡇⢠⣿⣶⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠏⣰⣧⡟⣴⡿⠃⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠷⠚⣿⡞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⡼⣣⣏⡴⠋⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠾⠃⠀⠀⢹⣥⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⠞⣱⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠴⠚⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡾⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣴⣾⢛⡟⡽⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⢾⡟⣿⣻⢃⡞⣹⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⢿⢧⡎⣴⣷⠃⡞⣹⢻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣱⣷⢯⡏⣰⣷⠏⡾⣰⢷⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⣿⣿⢷⡏⣰⣿⣏⡾⣡⣿⠿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⢦⣀⠀⠀⠀⠀⠀⠀⠀⠀⡠⢿⣿⣿⡟⣰⢿⡏⣿⣷⣟⣁⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣾⣼⣿⣓⠂⠀⠀⠀⠀⠀⢠⣽⣭⣽⣿⣿⣻⣿⣿⢿⣿⣿⡿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⣿⣿⣿⣷⣿⣷⣶⣾⣷⣶⣼⣿⣿⣿⣾⣿⣿⠛⣿⣸⢻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⡾⠿⠽⢿⣉⣷⣿⣉⣵⣖⣫⣿⣻⡿⠗⣫⡿⢻⣇⡿⣴⢣⠇⣸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣷⠀⠀⠀⠀⠀⠀⠈⠉⠙⢿⣿⣿⣩⣿⡽⢁⡞⡽⣳⠋⡟⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣗⣺⣿⠃⡼⣻⢧⡟⣼⠁⣏⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣶⣿⣸⢣⡇⡞⢱⠃⣾⡼⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢸⢣⡞⢸⢣⡏⡼⣿⣵⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡇⢰⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡼⢡⣏⡾⣶⣵⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⢸⢿⣦⣦⣤⢀⠀⠀⠀⠀⠀⠀⠀⢸⢃⡞⣼⣵⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠻⣿⣍⣹⣹⣙⣟⡝⣇⠧⠀⠀⠀⠀⠘⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠓⠒⠒⠒⠒⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"};


    while(1){
        clear();
        guide_border();
        for (int i = 0; i < 35; i ++){
            mvprintw(LINES / 2 - 18 + i, COLS / 2 - 10, guide[i]);
        }
        mvprintw(LINES / 2 - 8, 5, "t -> To take weapon/spell");
        mvprintw(LINES / 2 - 6, 5, "Q -> To Quit");
        mvprintw(LINES / 2 - 4, 5, "E -> Food Menu");
        mvprintw(LINES / 2 - 2, 5, "i -> Weapon Menu");
        mvprintw(LINES / 2, 5, "s -> Spell Menu");
        mvprintw(LINES / 2 + 2, 5, "m -> Music Menu");
        mvprintw(LINES / 2 + 4, 5, "g -> Guide Menu :)");
        refresh();
        int get = getch();
        if(get == 'Q') return;
    }
    return;
}

void draw_monsters(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    char mons[10] = {'D', 'F', 'G', 'S', 'U'};
    for (int m = 0; m < floor -> monster_count; m ++){
        struct monster* monster = floor -> monsters[m];
        int x = monster -> x, y = monster -> y;
        if(!floor -> vis[x][y] || monster -> act == -1) continue;
        mvprintw(x + kx, y + ky, "%c", mons[monster -> type]);
    }
    return;
}

void end_lose_border(){
    clear();
    for (int i = 0; i < COLS; i ++){
        mvprintw(0, i, "♱");
        mvprintw(LINES - 1, i, "♱");
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, "♱");
        mvprintw(i, COLS - 1, "♱");
    }
    return;
}

void end_win_border(){
    clear();
    for (int i = 0; i < COLS; i += 2){
        mvprintw(0, i, Ucup);
        mvprintw(LINES - 1, i, Ucup);
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, Ucup);
        mvprintw(i, COLS - 2, Ucup);
    }
    return;
}

void end_game_lose(struct game* game){
    end_spell_music(game);
    ////s p l i f f
    char lines[4][100] = { "___ ___ _____  __ __  _  _____  _____    _____  _____  _____  _____ ",
                           "\\  |  //  _  \\/  |  \\< >/  _  \\/   __\\  |  _  \\/   __\\/  _  \\|  _  \\",
                           " |   | |  |  ||  |  ||/ |  _  <|   __|  |  |  ||   __||  _  ||  |  |",
                           " \\___/ \\_____/\\_____/   \\__|\\_/\\_____/  |_____/\\_____/\\__|__/|_____/"};
    while(1){
        clear();
        end_lose_border();
        for (int i = 0; i < 4; i ++){
            mvprintw(4 + i, COLS / 2 - 34, "%s", lines[i]);
        }
        mvprintw(12, COLS / 2 - 8, "but don't worry!");
        mvprintw(13, COLS / 2 - 12, "You'll get them next time ̆̈");
        mvprintw(LINES - 2, 4, "Press any key to exit");
        refresh();
        ////KEYS
        int get = getch();
        return;
    }
    return;
}

void end_game_win(struct game *game){
    end_spell_music(game);
    ////s p l i f f
    char lines[4][100] = { "___ ___ _____  __ __    __  __  ___  _____ ",
                           "\\  |  //  _  \\/  |  \\  /   /  \\/___\\/  _  \\",
                           " |   | |  |  ||  |  |  |  /\\  ||   ||  |  |",
                           " \\___/ \\_____/\\_____/  \\__/\\__/\\___/\\__|__/"};
    while(1){
        clear();
        end_win_border();
        for (int i = 0; i < 4; i ++){
            mvprintw(4 + i, COLS / 2 - 22, "%s", lines[i]);
        }
        mvprintw(12, COLS / 2 - 13, "Congratulations, YOU WIN 🥂");
        mvprintw(14, COLS / 2 - 7, "Final Score: %d", game -> score);
        mvprintw(15, COLS / 2 - 8, "Gold Collected: %d", game -> golds);
        mvprintw(LINES - 3, 4, "Press any key to exit");
        refresh();
        ////KEYS
        int get = getch();
        return;
    }
    return;
}

void shoot(struct game* game){
    if(game -> current_w == -1){
        getch();
        return;
    }
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    for (int m = 0; m < floor -> monster_count; m ++){
        struct monster* monster = floor -> monsters[m];
        int x = monster -> x, y = monster -> y;
    }
    if(game -> current_w == 0){ // mace
        for (int m = 0; m < floor -> monster_count; m ++){
            struct monster* monster = floor -> monsters[m];
            if(monster -> act != 1) continue;
            if(abs(monster -> x - x) > 1 || abs(monster -> y - y) > 1) continue;
            monster -> health -= 5;
            if(dm){
                dm --;
                monster -> health -= 5;
            }
            if(monster -> health <= 0 && monster -> act != -1){
                game -> score += (monster -> type + 1) * 100;
                monster -> act = -1;
                message = monster -> type;
            }
        }
        return;
    }
    if(game -> current_w == 1){// sword
        for (int m = 0; m < floor -> monster_count; m ++){
            struct monster* monster = floor -> monsters[m];
            if(monster -> act != 1) continue;
            if(abs(monster -> x - x) > 1 || abs(monster -> y - y) > 1) continue;
            monster -> health -= 10;
            if(dm){
                dm --;
                monster -> health -= 10;
            }
            if(monster -> health <= 0 && monster -> act != -1){
                game -> score += (monster -> type + 1) * 100;
                monster -> act = -1;
                message = monster -> type;
            }
        }
        return;
    }
    if(game -> current_w == 2){// dagger
        int dir = getch();
        int xp = 0, yp = 0;
        if(dir == KEY_UP) xp = -1;
        if(dir == KEY_DOWN) xp = 1;
        if(dir == KEY_RIGHT) yp = 1;
        if(dir == KEY_LEFT) yp = -1;
        int x = game -> current_x, y = game -> current_y;
        int att = 0;
        for (int i = 0; i < 5; i ++){
            x += xp; y += yp;
            if(floor -> map[x][y] == '|' || floor -> map[x][y] == '-' || floor -> map[x][y] == '+'){
                x -= xp; y -= yp;
                break;
            }
            for (int m = 0; m < floor -> monster_count; m ++){
                struct monster* monster = floor -> monsters[m];
                if(monster -> act != 1) continue;
                if(monster -> x != x || monster -> y != y) continue;
                monster -> health -= 12;
                if(dm){
                    dm --;
                    monster -> health -= 12;
                }
                if(monster -> health <= 0 && monster -> act != -1){
                    game -> score += (monster -> type + 1) * 100;
                    monster -> act = -1;
                    message = monster -> type;
                }
                att = 1;
                break;
            }
            if(att) break;
        }
        if(!att){
            floor -> map[x][y] = 'd';
        }
        return;
    }
    if(game -> current_w == 3){ // wand
        int dir = getch();
        int xp = 0, yp = 0;
        if(dir == KEY_UP) xp = -1;
        if(dir == KEY_DOWN) xp = 1;
        if(dir == KEY_RIGHT) yp = 1;
        if(dir == KEY_LEFT) yp = -1;
        int x = game -> current_x, y = game -> current_y;
        int att = 0;
        for (int i = 0; i < 10; i ++){
            x += xp; y += yp;
            if(floor -> map[x][y] == '|' || floor -> map[x][y] == '-' || floor -> map[x][y] == '+'){
                x -= xp; y -= yp;
                break;
            }
            for (int m = 0; m < floor -> monster_count; m ++){
                struct monster* monster = floor -> monsters[m];
                if(monster -> act != 1) continue;
                if(monster -> x != x || monster -> y != y) continue;
                monster -> health -= 15;
                if(dm){
                    dm --;
                    monster -> health -= 15;
                }
                if(monster -> health <= 0 && monster -> act != -1){
                    game -> score += (monster -> type + 1) * 100;
                    monster -> act = -1;
                    message = monster -> type;
                }
                att = 1;
                break;
            }
            if(att) break;
        }
        if(!att){
            floor -> map[x][y] = 'w';
        }
        return;
    }
    if(game -> current_w == 4){ // arrow
        int dir = getch();
        int xp = 0, yp = 0;
        if(dir == KEY_UP) xp = -1;
        if(dir == KEY_DOWN) xp = 1;
        if(dir == KEY_RIGHT) yp = 1;
        if(dir == KEY_LEFT) yp = -1;
        int x = game -> current_x, y = game -> current_y;
        int att = 0;
        for (int i = 0; i < 5; i ++){
            x += xp; y += yp;
            if(floor -> map[x][y] == '|' || floor -> map[x][y] == '-' || floor -> map[x][y] == '+'){
                x -= xp; y -= yp;
                break;
            }
            for (int m = 0; m < floor -> monster_count; m ++){
                struct monster* monster = floor -> monsters[m];
                if(monster -> act != 1) continue;
                if(monster -> x != x || monster -> y != y) continue;
                monster -> health -= 5;
                if(dm){
                    dm --;
                    monster -> health -= 5;
                }
                if(monster -> health <= 0 && monster -> act != -1){
                    game -> score += (monster -> type + 1) * 100;
                    monster -> act = -1;
                    message = monster -> type;
                }
                att = 1;
                break;
            }
            if(att) break;
        }
        if(!att){
            floor -> map[x][y] = 'a';
        }
        return;
    }
    return;
}

int all_monsters_killed(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
    int cnt = 0;
    for (int m = 0; m < floor -> monster_count; m ++){
        if(floor -> monsters[m] -> act == -1) cnt ++;
    }
    return cnt;
}

int game_play_t(struct game* game){
    char blabla[3][100] = {
        "       ,,,",
        "      (o o)",
        "--oOO--( )--OOo--"};

    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    game -> current_level = 5;
    game -> current_x = ROW / 2;
    game -> current_y = COL / 2;
    update(game);
    while(1){
        if(game -> health <= 0){
            end_game_lose(game);
            return 0;
        }
        if(all_monsters_killed(game)){
            end_game_win(game);
            return 1;
        }
        clear();
        noecho();
        char **to = draw(game);
        if(message != -1){
            mvprintw(LINES - 4, 12, mes[message]);
            for (int i = 0; i < 3; i ++){
                mvprintw(LINES - 3 + i, 0, blabla[i]);
            }
        }
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                ///golds
                if(to[i][j] == 'g'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i + kx, j + ky, Ugold);
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == 'G'){
                    attron(COLOR_PAIR(11));
                    mvprintw(i + kx, j + ky, Ubgold);
                    attroff(COLOR_PAIR(11));
                }
                ///weapons
                else if(to[i][j] == 'd'){
                    attron(COLOR_PAIR(2));
                    mvprintw(i + kx, j + ky, Udagger);
                    attroff(COLOR_PAIR(2));
                }
                else if(to[i][j] == 'w'){
                    attron(COLOR_PAIR(4));
                    mvprintw(i + kx, j + ky, Uwand);
                    attroff(COLOR_PAIR(4));
                }
                else if(to[i][j] == 'a'){
                    attron(COLOR_PAIR(3));
                    mvprintw(i + kx, j + ky, Uarrow);
                    attroff(COLOR_PAIR(3));
                }
                ///spells
                else if(to[i][j] == 'H'){
                    attron(COLOR_PAIR(12));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(12));
                }
                else if(to[i][j] == 'S'){
                    attron(COLOR_PAIR(13));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(13));
                }
                else if(to[i][j] == 'D'){
                    attron(COLOR_PAIR(14));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(14));
                }
                ///user
                else if(to[i][j] == '@'){
                    attron(COLOR_PAIR(20 + C_COLOR));
                    mvprintw(i + kx, j + ky, "@");
                    attroff(COLOR_PAIR(20 + C_COLOR));
                }
                else mvprintw(i + kx, j + ky, "%c", to[i][j]);
            }
        }
        draw_monsters(game);
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'f'){
                    mvprintw(i + kx, j + ky, Umorgh);
                }
                if(to[i][j] == 'F'){
                    mvprintw(i + kx, j + ky, Uburger);
                }
                if(to[i][j] == 'Y'){
                    mvprintw(i + kx, j + ky, Umeat);
                }
            }
        }
        /////INFO
        mvprintw(1, 3, "LEVEL: FINAL");
        mvprintw(1, 21, "GOLDS: %d", game -> golds);
        mvprintw(1, 36, "HEALTH: %d", game -> health);

        mvprintw(2 + kx, COL / 2 - 8 + ky, "💎 TREASURE ROOM 💎");
        refresh();
        ////KEYS
        int e = getch();
        if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
            move_char(game, e);
            if(sp){
                sp --;
                move_char(game, e);
            }
        }
        if(e == 'f'){
            e = getch();
            if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
                while(move_char(game, e));
            }
        }
        if(e == 'g'){
            guide_menu();
        }
        if(e == 'E'){
            food_menu(game);
        }
        if(e == 's'){
            spell_menu(game);
        }
        if(e == 'i'){
            weapon_menu(game);
        }
        if(e == 'm'){
            music_menu();
        }
        if(e == 't'){
            get_weapon(game);
            get_spell(game);
        }
        if(e == ' '){
            shoot(game);
        }
        if(e == 'Q'){
            return -1;
        }
    }
}

int game_play(struct game* game){
    char blabla[3][100] = {
        "       ,,,",
        "      (o o)",
        "--oOO--( )--OOo--"};

    if(game -> current_level == 5) return game_play_t(game);
    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    update(game);
    message = 16;
    while (1){
        if(game -> health <= 0){
            end_game_lose(game);
            return 0;
        }
        clear();
        noecho();
        char **to = draw(game);
        if(message != -1){
            mvprintw(LINES - 4, 12, mes[message]);
            for (int i = 0; i < 3; i ++){
                mvprintw(LINES - 3 + i, 0, blabla[i]);
            }
        }
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                mvprintw(i + kx, j + ky, "%c", to[i][j]);
                ///golds
                if(to[i][j] == 'g'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i + kx, j + ky, Ugold);
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == 'G'){
                    attron(COLOR_PAIR(11));
                    mvprintw(i + kx, j + ky, Ubgold);
                    attroff(COLOR_PAIR(11));
                }
                ////weapons
                else if(to[i][j] == 'd'){
                    attron(COLOR_PAIR(2));
                    mvprintw(i + kx, j + ky, Udagger);
                    attroff(COLOR_PAIR(2));
                }
                else if(to[i][j] == 'w'){
                    attron(COLOR_PAIR(4));
                    mvprintw(i + kx, j + ky, Uwand);
                    attroff(COLOR_PAIR(4));
                }
                else if(to[i][j] == 'a'){
                    attron(COLOR_PAIR(3));
                    mvprintw(i + kx, j + ky, Uarrow);
                    attroff(COLOR_PAIR(3));
                }
                ///spells
                else if(to[i][j] == 'H'){
                    attron(COLOR_PAIR(12));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(12));
                }
                else if(to[i][j] == 'S'){
                    attron(COLOR_PAIR(13));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(13));
                }
                else if(to[i][j] == 'D'){
                    attron(COLOR_PAIR(14));
                    mvprintw(i + kx, j + ky, "✩");
                    attroff(COLOR_PAIR(14));
                }
                ////user
                else if(to[i][j] == '@'){
                    attron(COLOR_PAIR(20 + C_COLOR));
                    mvprintw(i + kx, j + ky, "@");
                    attroff(COLOR_PAIR(20 + C_COLOR));
                }
            }
        }
        spellroom(game, to);
        draw_monsters(game);
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'f'){
                    mvprintw(i + kx, j + ky, Umorgh);
                }
                if(to[i][j] == 'F'){
                    mvprintw(i + kx, j + ky, Uburger);
                }
                if(to[i][j] == 'Y'){
                    mvprintw(i + kx, j + ky, Umeat);
                }
                if(to[i][j] == 'T'){
                    mvprintw(i + kx, j + ky, Udiamond);
                }
                else if(to[i][j] == 's'){
                    mvprintw(i + kx, j + ky, Usword);
                }
            }
        }
        ////// information:
        mvprintw(1, 13 + (COLS - COL) / 2, "LEVEL: %d", game -> current_level + 1);
        mvprintw(1, 28 + (COLS - COL) / 2, "Score: %d", game -> score);
        mvprintw(1, 43 + (COLS - COL) / 2, "GOLDS: %d", game -> golds);
        mvprintw(1, 58 + (COLS - COL) / 2, "HEALTH:");
        for (int i = 0; i < game -> health / 5; i ++){
            //attron(COLOR_PAIR(5));
            mvprintw(1, 66 + i + (COLS - COL) / 2, "▋");
            //attroff(COLOR_PAIR(5));
        }
        refresh();
        ////LOKED KEYS
        int e = getch();
        if(e == 'M'){
            game -> Vis ^= 1;
        }
        if(game -> Vis) continue;
        //// KEYS
        if(e == '~'){
            return game_play_t(game);
        }
        if(e == 'g'){
            guide_menu();
        }
        if(e == 'E'){
            food_menu(game);
        }
        if(e == 's'){
            spell_menu(game);
        }
        if(e == 'i'){
            weapon_menu(game);
        }
        if(e == 'm'){
            music_menu();
        }
        //weapons
        if(e == 'w'){
            game -> current_w = -1;
        }
        if(e == 'T' && game -> floors[game -> current_level] -> map[game -> current_x][game -> current_y] == 'T'){
            return game_play_t(game);
        }
        if(e == 't'){
            get_weapon(game);
            get_spell(game);
        }
        if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
            move_char(game, e);
            if(sp){
                sp --;
                move_char(game, e);
            }
        }
        if(e == 'f'){
            e = getch();
            if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
                while(move_char(game, e));
            }
        }
        if(e == KEY_RIGHT) go_down(game);
        if(e == KEY_LEFT) go_up(game);
        if(e == 'Q') return -1;
        if(e == ' '){
            shoot(game);
        }
    }
    return 0;
}