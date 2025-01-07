#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int ROW = 25;
const int COL = 60;


struct Room{
    int x0, y0, x1, y1;
};

struct floor{
    int room_count;
    struct Room** rooms;
    char** map;
};

struct game{
    int current_level, golds;
    struct floor* floors[5];
    char map[5][ROW + 10][COL + 10];
    int hide[5][ROW + 10][COL + 10];
};

struct user{
    int id, open_game;
    char user_name[100], password[100], email[100];
    int total_score, games_played, golds, experience;
    struct game *game;
};

void read_users();
void write_users();

void main_menu();
void menu_border();

void sign_up();
 int user_name_validation(char[]);
 int email_validation(char[]);
 int password_validation(char[]);
 void new_user(char[], char[], char[]);

void sign_in();
void profile();
void scoreboard();
 void scoreboard_sort_users();

void game_menu();
 void game_play(struct game*);

 struct game* new_game();
  struct floor* new_floor();
   struct Room* new_room();
  int floor_check(struct floor*);
  void draw_corridor(char **, int, int);

/////

int N = 0;
int signed_in = 0;
struct user* users[2000], *current_user;

int main(){
    srand(time(0));
    initscr();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, 220, COLOR_BLACK); //Gold 220
    init_pair(2, 248, COLOR_BLACK); //Silver 248
    init_pair(3, 131, COLOR_BLACK); //Bronze! 131
    // for (int i = 0; i < 170; i ++){
    //     init_pair(i + 1, i, COLOR_BLACK);
    //     attron(COLOR_PAIR(i + 1));
    //     mvprintw(i / 100, i % 100, "s");
    // }
    // getch();
    read_users();
    main_menu();
    endwin();
    write_users();
    return 0;
}

void read_users(){
    FILE *fptr;
    fptr = fopen("users.txt", "r");
    fscanf(fptr, "%d", &N);
    for (int i = 0; i < N; i ++){
        users[i] = (struct user *) malloc(sizeof(struct user));
        fscanf(fptr, "%s", users[i] -> user_name);
        fscanf(fptr, "%s", users[i] -> email);
        fscanf(fptr, "%s", users[i] -> password);
        fscanf(fptr, "%d", &(users[i] -> id));
    }
    return;
}

void write_users(){
    FILE *fptr;
    fptr = fopen("users.txt", "w");
    fprintf(fptr, "%d\n", N);
    for (int i = 0; i < N; i ++){
        fprintf(fptr, "%s ", users[i] -> user_name);
        fprintf(fptr, "%s ", users[i] -> email);
        fprintf(fptr, "%s ", users[i] -> password);
        fprintf(fptr, "%d\n", users[i] -> id);
    }
    fclose(fptr);
    return;
}

void menu_border(){
    clear();
    for (int i = 0; i < COLS; i ++){
        mvprintw(0, i, "*");
        mvprintw(LINES - 1, i, "*");
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, "*");
        mvprintw(i, COLS - 1, "*");
    }
    return;
}

void main_menu(){
    char options[6][20] = {"SIGN UP", "SIGN IN", "PROFILE", "SCOREBOARD", "LETS PLAY!!!"};
    int cr = 0;
    while(1){
        menu_border();
        mvprintw(4, 5, "MAIN MENU:");
        for (int i = 0; i < 5; i ++){
            mvprintw(5 + i, 7, options[i]);
        }
        for (int i = 0; i < 5; i ++){
            if(cr == i){
                attron(A_REVERSE);
                mvprintw(5 + i, 7, options[i]);
                attroff(A_REVERSE);
            }
        }
        move(0, 0);
        int get = getch();
        if(get == KEY_UP) cr = (cr + 4) % 5;
        else if(get == KEY_DOWN) cr = (cr + 1) % 5;
        else if(get == '\n'){
            if(cr == 0) sign_up();
            if(cr == 1) sign_in();
            if(cr == 2) profile();
            if(cr == 3) scoreboard();
            if(cr == 4) game_menu();
            return;
        }
        else if(get == 27) break;
        refresh();
    }
    return;
}

int user_name_validation(char user_name[]){
    for (int i = 0; i < N; i ++){
        if(strcmp(users[i] -> user_name, user_name) == 0) return 0;
    }        
    return 1;
}

int email_validation(char email[]){
    return 1;
}

int password_validation(char password[]){
    if(strlen(password) < 7) return 1;
    int number = 0, lower = 0, upper = 0;
    for (int i = 0; i < strlen(password); i ++){
        if('0' <= password[i] && password[i] <= '9') number = 1;
        if('a' <= password[i] && password[i] <= 'z') lower = 1;
        if('A' <= password[i] && password[i] <= 'Z') upper = 1;
    }
    if(!number) return 2;
    if(!lower) return 3;
    if(!upper) return 4;
    return 0;
}

void new_user(char user_name[], char email[], char password[]){
    users[N] = (struct user *) malloc(sizeof(struct user));
    strcpy(users[N] -> user_name, user_name);
    strcpy(users[N] -> email, email);
    strcpy(users[N] -> password, password);
    users[N] -> total_score = 0;
    users[N] -> open_game = 0;
    users[N] -> games_played = 0;
    users[N] -> experience = 0;
    N ++;
    return;
}

void sign_up(){
    clear();
    refresh();
    while(1){
        clear();
        menu_border();
        refresh();
        char user_name[100], email[100], password[100];

        mvprintw(5, 5, "Enter your username");
        move(6, 6);
        getstr(user_name);
        if(!user_name_validation(user_name)){
            mvprintw(8, 6, "This Username is already used!!");
            mvprintw(LINES - 2, 6, "Press nay key to exit");
            getch();
            main_menu();
            return;
        }

        mvprintw(8, 5, "Enter your email");
        move(9, 6);
        getstr(email);
        if(!email_validation(email)){
            mvprintw(11, 6, "email is not valid!!");
            mvprintw(LINES - 2, 6, "Press any key to exit");
            getch();
            main_menu();
            return;
        }

        mvprintw(11, 5, "Enter your password");
        move(12, 6);
        getstr(password);
        int password_valid = password_validation(password);
        if(password_valid){
            if(password_valid == 1) mvprintw(14, 6, "password must contain at least 7 characters!!");
            if(password_valid == 2) mvprintw(14, 6, "password must contain a number!!");
            if(password_valid == 3) mvprintw(14, 6, "password must contain a lower!!");
            if(password_valid == 4) mvprintw(14, 6, "password must contain an upper!!");
            mvprintw(LINES - 2, 4, "Press any key to exit");
            getch();
            main_menu();
            return;
        }

        new_user(user_name, email, password);
        refresh();
        clear();
        menu_border();
        mvprintw(5, 10, "Hello %s :)", user_name);
        getch();
        main_menu();
        break;
    }
    return;
}

void sign_in(){
    clear();
    refresh();
    if(signed_in == 1){
        menu_border();
        mvprintw(5, 5, "You are already signed in!");
        mvprintw(LINES - 2, 4, "Press any key to exit");
        getch();
        main_menu();
        return;
    }
    while(1){
        clear();
        menu_border();
        refresh();
        char user_name[100], password[100];
        mvprintw(5, 5, "Enter your username");
        move(6, 6);
        getstr(user_name);
        int id = -1;
        for (int i = 0; i < N; i ++){
            if(strcmp(user_name, users[i] -> user_name) == 0) id = i;
        }
        if(id == -1){
            mvprintw(8, 5, "Username not found!");
            mvprintw(LINES - 2, 4, "Press any key to exit");
            getch();
            main_menu();
            return;
        }
        mvprintw(8, 5, "Enter your password %s:)", user_name);
        move(9, 6);
        getstr(password);
        if(strcmp(users[id] -> password, password) != 0){
            mvprintw(11, 5, "Incorrect password!");
            mvprintw(LINES - 2, 4, "Press any key to exit");
            getch();
            main_menu();
            return;
        }
        signed_in = 1;
        current_user = users[id];
        clear();
        menu_border();
        mvprintw(5, 5, "Yeaaaayyyy");
        mvprintw(6, 5, "You are signed in %s :)", current_user -> user_name);
        mvprintw(LINES - 2, 4, "Press any key to exit");
        refresh();
        getch();
        main_menu();
        return;
    }
    return;
}

void profile(){
    clear();
    refresh();
    while(1){
        menu_border();
        if(signed_in == 0){
            mvprintw(4, 5, "Ooops!");
            mvprintw(5, 5, "You are not signed in :(");
            mvprintw(LINES - 2, 4, "Press Enter to exit");
            getch();
            main_menu();
            return;
        }
        mvprintw(5, 5, "ME:");
        mvprintw(6, 5, "My email:");
        mvprintw(7, 5, "My password:");
        mvprintw(8, 5, "My score:");

        mvprintw(5, 22, "%s", current_user -> user_name);
        mvprintw(6, 22, "%s", current_user -> email);
        mvprintw(7, 22, "%s", current_user -> password);
        mvprintw(8, 22, "%s", current_user -> total_score);
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        refresh();
        if(getch() == '\n'){
            main_menu();
            return;
        }
    }
    return;
}

void scoreboard_sort_users(){
    for (int i = 0; i < N; i ++){
        for (int j = 0; j + 1 < N; j ++){
            if(users[j] -> total_score > users[j + 1] -> total_score){
                struct user* tr = users[j];
                users[j] = users[j + 1];
                users[j + 1] = tr;
            }
        }
    }
    return;
}

void scoreboard(){
    clear();
    refresh();
    char lines[5][100] = {"***   ***    ***   ***   ****  ****    ***      *     ***   ***",
                          "*    *   *  *   *  *  *  *     *   *  *   *    * *    *  *  *  *",
                          "***  *      *   *  ***   ****  ****   *   *   * * *   ***   *   *", 
                          "  *  *   *  *   *  * *   *     *   *  *   *  *     *  * *   *  *", 
                          "***   ***    ***   *  *  ****  ****    ***   *     *  *  *  ***"     };
    char top[6][20] = {"Rank", "Username", "Score", "Golds", "games_played", "experience"};
    int loc[6] = {7, 14, 32, 48, 61, 78};
    int pg = 0;
    while(1){
        menu_border();
        for (int i = 0; i < 5; i ++) mvprintw(i + 2, 8, lines[i]);
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        if((N + 19) / 20 > 1){
            if(pg == 0) mvprintw(LINES - 2, 26, "/  N for next page");
            else mvprintw(LINES - 2, 26, "/  N for first page");
        }
        for (int i = 0; i < 6; i ++) mvprintw(9, loc[i], "%s", top[i]);
        refresh();
        scoreboard_sort_users();
        for (int i = 0; i < N - pg * 20 && i < 20; i ++){
            int id = pg * 20 + i;
            if(signed_in && users[id] == current_user) attron(A_BOLD);
            if(i < 3 && pg == 0) attron(COLOR_PAIR(i + 1));
            mvprintw(11 + i, 8, "%d", id + 1);
            mvprintw(11 + i, 14, "%s", users[id] -> user_name);
            mvprintw(11 + i, 34, "%d", users[id] -> total_score);
            mvprintw(11 + i, 50, "%d", users[id] -> golds);
            mvprintw(11 + i, 66, "%d", users[id] -> games_played);
            mvprintw(11 + i, 82, "%d", users[id] -> experience);
            if(i < 3 && pg == 0) attroff(COLOR_PAIR(i + 1));
            if(signed_in && users[id] == current_user) attroff(A_BOLD);
        }
        int get = getch();
        if(get == 'n' || get == 'N') pg = (pg + 1) % ((N + 19) / 20);
        if(get == 27 || get == '\n'){
            main_menu();
            return;
        }
    }
    return;
}


/*
"    ****     *       *   *    ****        *
"   *        * *     * * * *   *        *   *
"   *   **  * * *   *   *   *  ****         *
"   *   *  *     *  *       *  *        *   *
"    ****  *     *  *       *  ****        * 
*/


struct Room* new_room(){
    struct Room *room = (struct Room *) malloc(sizeof(struct Room));
    room -> x0 = rand() % ROW;
    room -> y0 = rand() % COL;
    room -> x1 = room -> x0 + rand() % 5 + 7;
    room -> y1 = room -> y0 + rand() % 5 + 7;
    return room;
}

int floor_check(struct floor* floor){
    int map[ROW + 10][COL + 10];
    for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) map[i][j] = 0;
    for (int i = 0; i < floor -> room_count; i ++){
        struct Room *room = floor -> rooms[i];
        if(room -> x1 > ROW || room -> y1 > COL) return 0;
        for (int i = room -> x0; i < room -> x1; i ++){
            if(room -> y0 && map[i][room -> y0 - 1]) return 0;
            if(room -> y1 + 1 < COL && map[i][room -> y1 + 1]) return 0;
        }
        for (int j = room -> y0; j < room -> y1; j ++){
            if(room -> x0 && map[room -> x0 - 1][j]) return 0;
            if(room -> x1 + 1 < ROW && map[room -> x1 + 1][j]) return 0;
        }
        for (int i = room -> x0; i < room -> x1; i ++){
            for (int j = room -> y0; j < room -> y1; j ++){
                if(map[i][j]) return 0;
                map[i][j] ++;
            }
        }
    }
    return 1;
}

void draw_corridor(char** map, int sx, int sy){
    int opt[100000], par[100][1000], mark[100][1000], seen[100][1000];
    int dx[4] = {0, 0, 1, -1}, dy[4] = {1, -1, 0, 0};
    for (int i = 0; i < 100; i ++) for (int j = 0; j < 1000; j ++) mark[i][j] = seen[i][j];
    int q = 1, f = 0;
    mark[sx][sy] = 1;
    opt[0] = sx * COL + sy; 
    while(q != f){
        int x = opt[f] / COL, y = opt[f] % COL;
        f ++;
        ///randomize!!
        for (int i = 0; i < 3; i ++){
            int rr = rand(), tr;
            if(rr % 200 != 0) continue;
            tr = dx[i];
            dx[i] = dx[i + 1]; dx[i + 1] = tr;
            tr = dy[i];
            dy[i] = dy[i + 1]; dy[i + 1] = tr;
        }
        for (int d = 0; d < 4; d ++){
            int xx = x + dx[d], yy = y + dy[d];
            if(xx < 0 || yy < 0 || xx >= ROW || yy >= COL || mark[xx][yy]) continue;
            if(map[xx][yy] == '-' && (yy == 0 || yy == COL - 1 || map[xx][yy - 1] == ' ' || map[xx][yy + 1] == ' ')) continue;
            if(map[xx][yy] == '|' && (xx == 0 || xx == ROW - 1 || map[xx - 1][yy] == ' ' || map[xx + 1][yy] == ' ')) continue;
            opt[q ++] = xx * COL + yy;
            mark[xx][yy] = 1;
            par[xx][yy] = x * COL + y;
        }
        if(map[x][y] != ' ' && !seen[x][y]){
            int x0 = x, x1 = x, y0 = y, y1 = y;
            while(x0 > 0 && map[x0 - 1][y] != ' ') x0 --;
            while(x1 + 1 < ROW && map[x1 + 1][y] != ' ') x1 ++;
            while(y0 > 0 && map[x][y0 - 1] != ' ') y0 --;
            while(y1 + 1 < COL && map[x][y1 + 1] != ' ') y1 ++;
            for (int i = x0; i <= x1; i ++) for (int j = y0; j <= y1; j ++) seen[i][j] = 1;
            if(x == sx && y == sy) continue;
            map[x][y] = '+';
            int nd = par[x][y];
            x = nd / COL;
            y = nd % COL;
            while(!seen[x][y]){
                map[x][y] = '#';
                nd = par[x][y];
                x = nd / COL;
                y = nd % COL;
            }
            map[x][y] = '+';
        }
    }
    return;
}

struct floor* new_floor(){
    struct floor *floor = (struct floor *) malloc(sizeof(struct floor));
    srand(time(NULL));
    ////ROOMS
    floor -> room_count = rand() % 2 + 6;
    floor -> rooms = (struct Room **) malloc(floor -> room_count * sizeof(struct Room *));
    while(1){
        for (int i = 0; i < floor -> room_count; i ++){
            floor -> rooms[i] = new_room();
        }
        if(floor_check(floor)) break;
    }
    ////MAP
    floor -> map = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        floor -> map[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) floor -> map[i][j] = ' ';
    }
    for (int i = 0; i < floor -> room_count; i ++){
        struct Room *room = floor -> rooms[i];
        for (int i = room -> x0; i < room -> x1; i ++){
            for (int j = room -> y0; j < room -> y1; j ++){
                floor -> map[i][j] = '.';
            }
        }
        for (int i = room -> x0; i < room -> x1; i ++){
            floor -> map[i][room -> y0] = '|';
            floor -> map[i][room -> y1 - 1] = '|';
        }
        for (int j = room -> y0; j < room -> y1; j ++){
            floor -> map[room -> x0][j] = '-';
            floor -> map[room -> x1 - 1][j] = '-';
        }
    }
    ////CORRIDEOR
    draw_corridor(floor -> map, floor -> rooms[0] -> x0 + 1, floor -> rooms[0] -> y0);
    return floor;
}

struct game* new_game(){
    struct game *game = (struct game*) malloc(sizeof(struct game));
    for (int f = 0; f < 5; f ++){
        game -> floors[f] = new_floor();
        for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) game -> map[f][i][j] = game -> floors[f] -> map[i][j];
        for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) game -> hide[f][i][j] = 0;
    }
    game -> current_level = 0;
    return game;
}

void game_menu(){
    clear();
    int cr = 0, op = 3;
    char options[10][50] = {"New game", "Continue previous game", "Settings"};
    while(1){
        clear();
        menu_border();
        mvprintw(4, 5, "GAME MENU:");
        mvprintw(LINES - 2, 4, "Press Esc to exit");
        for (int i = 0; i < op; i ++){
            mvprintw(5 + i, 7, options[i]);
        }
        for (int i = 0; i < op; i ++){
            if(cr == i){
                attron(A_REVERSE);
                mvprintw(5 + i, 7, options[i]);
                attroff(A_REVERSE);
            }
        }
        move(0, 0);
        int get = getch();
        if(get == KEY_UP) cr = (cr + op - 1) % op;
        else if(get == KEY_DOWN) cr = (cr + 1) % op;
        else if(get == '\n'){
            if(cr == 0){
                struct game *game = new_game();
                if(signed_in){
                    current_user -> game = game;
                    current_user -> open_game = 1;
                }
                game_play(game);
                ////////moonde hanooz
                return;
            } 
            if(cr == 1){
                if(signed_in == 0 || current_user -> open_game == 0){
                    clear();
                    menu_border();
                    mvprintw(5, 5, "No saved game!");
                    mvprintw(6, 5, "please start a new game");
                    mvprintw(LINES - 2, 4, "Press any key to exit");
                    getch();
                    game_menu();
                    return;
                }
                game_play(current_user -> game);
                /////////moondeeee
            }
            if(cr == 2){
                game_menu();
            }
            return;
        }
        else if(get == 27){
            main_menu();
            return;

        }
    }
    return;
}

void game_play(struct game* game){
    clear();
    int kx = (LINES - ROW) / 2, ky = (COLS - COL) / 2;
    while (1){
        clear();
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                mvprintw(i + kx, j + ky, "%c", game -> map[game-> current_level][i][j]);
            }
        }
        mvprintw(LINES - 1, 3, "LEVEL: %d", game -> current_level + 1);
        mvprintw(LINES - 1, 18, "GOLDS: %d", game -> golds);
        refresh();
        getch();
    }
    return;
}