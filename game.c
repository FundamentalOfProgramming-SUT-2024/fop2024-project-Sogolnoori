#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "abcd.c"

int move_char(struct game* game, char e);
void update(struct game* game);
char ** draw(struct game* game);

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
            if(y && floor -> map[x][y - 1] == 'f') continue;
            floor -> map[x][y] = 'f';
            break;
        }
    }
    return;
}

void food_menu(struct game* game){
    while(1){
        clear();
        int lxmcjslj = 5, kdjlsj = COL / 2 - 20;
        mvprintw(lxmcjslj, kdjlsj - 1, "|");
        for (int i = 0; i < 40; i ++) mvprintw(lxmcjslj - 1, kdjlsj + i, "-");
        for (int i = 0; i < 40; i ++) mvprintw(lxmcjslj + 1, kdjlsj + i, "-");
        mvprintw(lxmcjslj, kdjlsj + 40, "|");

        attron(COLOR_PAIR(5));
        if(game -> hunger / DEG == 40) attron(COLOR_PAIR(6));
        for (int i = 0; i < game -> hunger / DEG; i ++){
            mvprintw(lxmcjslj, kdjlsj + i, " ");
        }
        if(game -> hunger / DEG == 40) attron(COLOR_PAIR(6));
        attroff(COLOR_PAIR(5));

        mvprintw(lxmcjslj - 2, kdjlsj + 17, "HUNGER:");
        mvprintw(12, kdjlsj + 12, "Foods Collected: %d", game -> food);
        attron(COLOR_PAIR(5));
        mvprintw(14, kdjlsj + 17, "EAT FOOD");
        attroff(COLOR_PAIR(5));
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        refresh();
        ////KEYS
        int get = getch();
        if(get == '\n') break;
        if(get == 'e' && game -> food){
            game -> food --;
            if(game -> hunger >= DEG) game -> hunger -= DEG;
            game -> health += DEG * 2;
            if(game -> health > 100) game -> health = 100;
        }
    }
    return;
}

void update(struct game* game){
    int level = game -> current_level, x = game -> current_x, y = game -> current_y;
    struct floor *floor = game -> floors[level];
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
    ////CHECKGOLD
    if(floor -> map[x][y] == 'g'){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> golds += 5;
    }
    if(floor -> map[x][y] == 'G'){
        floor -> map[x][y] = floor -> mp[x][y];
        game -> golds += 100;
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
    ////CHECKTRAP
    if(floor -> trap[x][y] == 1){
        floor -> map[x][y] = '^';
        game -> health -= 5;
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
    if(floor -> map[x][y] == 'd'){
        game -> dagger += 10;
        floor -> map[x][y] = floor -> mp[x][y];
    }
    if(floor -> map[x][y] == 'w'){
        game -> wand += 8;
        floor -> map[x][y] = floor -> mp[x][y];
    }
    if(floor -> map[x][y] == 'a'){
        game -> arrow += 20;
        floor -> map[x][y] = floor -> mp[x][y];
    }
    if(floor -> map[x][y] == 's'){
        game -> sword = 1;
        floor -> map[x][y] = floor -> mp[x][y];
    }
    return;
}

void weapon_menu(struct game* game){
    while(1){
        clear();
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        refresh();
        ////KEYS
        int get = getch();
        if(get == '\n') break;
    }
}

int treasure_room(struct game* game){
    game -> current_level = 5;
    game -> current_x = ROW / 2;
    game -> current_y = COL / 2;
    while(1){
        clear();
        noecho();
        char **to = draw(game);
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'g'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i, j, "\u25c7");
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == 'G'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i, j, "\u25CB");
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == '@'){
                    attron(COLOR_PAIR(4));
                    mvprintw(i, j, "@");
                    attroff(COLOR_PAIR(4));
                }
                else mvprintw(i, j, "%c", to[i][j]);
            }
        }
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'f'){
                    mvprintw(i, j, Umorgh);
                }
            }
        }
        /////INFO
        mvprintw(LINES - 1, 3, "LEVEL: FINAL");
        mvprintw(LINES - 1, 21, "GOLDS: %d", game -> golds);
        mvprintw(LINES - 1, 36, "HEALTH: %d", game -> health);
        mvprintw(2, COL / 2 - 8, "🔥TREASURE ROOM🔥");
        refresh();
        ////KEYS
        int e = getch();
        if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
            move_char(game, e);
        }
        if(e == 'f'){
            e = getch();
            if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
                while(move_char(game, e));
            }
        }
        if(e == 'E'){
            food_menu(game);
        }
        if(e == 'i'){
            weapon_menu(game);
        }
        if(e == 't'){
            get_weapon(game);
        }
    }
    return 0;
}

int game_play(struct game* game){
    //////hal nadaram
    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    /////]
    update(game);
    while (1){
        clear();
        noecho();
        char **to = draw(game);
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'g'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i, j, "\u25c7");
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == 'G'){
                    attron(COLOR_PAIR(1));
                    mvprintw(i, j, "\u25CB");
                    attroff(COLOR_PAIR(1));
                }
                else if(to[i][j] == '@'){
                    attron(COLOR_PAIR(4));
                    mvprintw(i, j, "@");
                    attroff(COLOR_PAIR(4));
                }
                else mvprintw(i, j, "%c", to[i][j]);
            }
        }
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                if(to[i][j] == 'f'){
                    mvprintw(i, j, Umorgh);
                }
                if(to[i][j] == 'T'){
                    mvprintw(i, j, Udiamond);
                }
                if(to[i][j] == 's'){
                    mvprintw(i, j, "S");
                }
            }
        }
        ////// information:
        mvprintw(LINES - 1, 3, "LEVEL: %d", game -> current_level + 1);
        mvprintw(LINES - 1, 18, "GOLDS: %d", game -> golds);
        mvprintw(LINES - 1, 33, "HEALTH: %d", game -> health);
        refresh();
        ////LOKED KEYS
        int e = getch();
        if(e == 'M'){
            game -> Vis ^= 1;
        }
        if(game -> Vis) continue;
        //// KEYS
        if(e == '~'){
            return treasure_room(game);
        }
        if(e == 'E'){
            food_menu(game);
        }
        if(e == 'i'){
            weapon_menu(game);
        }
        //weapons
        if(e == 'w'){
            game -> current_w = -1;
        }
        if(e == 'T' && game -> floors[game -> current_level] -> map[game -> current_x][game -> current_y] == 'T'){
            return treasure_room(game);
        }
        if(e == 't'){
            get_weapon(game);
        }
        if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
            move_char(game, e);
        }
        if(e == 'f'){
            e = getch();
            if(e == 'y' || e == 'u' || e == 'h' || e == 'j' || e == 'k' || e == 'l' || e == 'b' || e == 'n'){
                while(move_char(game, e));
            }
        }
        if(e == KEY_RIGHT) go_down(game);
        if(e == KEY_LEFT) go_up(game);
        if(e == 27) return -1;
    }
    return 0;
}