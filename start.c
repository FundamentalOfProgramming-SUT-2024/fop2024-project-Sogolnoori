#include <ncurses.h>
#include <locale.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <time.h>
#include "game.c"

#define ll long long

struct user{
    int id, open_game;
    char user_name[100], email[100], password[100];
    ll birth;
    int total_score, golds, games_played;
    struct game *game;
};

void read_users();
void write_users();

void main_menu();
void menu_border();

void sign_up();
 int user_name_validation(char[]);
 int email_validation(char *);
 int password_validation(char[]);
 void new_user(char[], char[], char[]);

void sign_in();
void profile();
void scoreboard();
 void scoreboard_sort_users();

void game_menu();
 int game_play(struct game*);

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
    ////EMOJY
    setlocale(LC_ALL,"");
    ///MUSIC
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    srand(time(NULL));
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, 220, COLOR_BLACK); //Gold 220
    init_pair(2, 248, COLOR_BLACK); //Silver 248
    init_pair(3, 131, COLOR_BLACK); //Bronze! 131
    init_pair(4, 117, COLOR_BLACK); //Blue
    init_pair(5, COLOR_BLACK, COLOR_WHITE); //inwhite
    init_pair(6, COLOR_BLACK, COLOR_RED); //inred
    // for (int i = 0; i < 400; i ++){
    //     init_pair(i + 1, i, COLOR_BLACK);
    //     attron(COLOR_PAIR(i + 1));
    //     mvprintw(i / 25, i % 25 * 4, "%d", i);
    // }
    // getch();

    // while(1){
    //     for (int i = 0; i < 10; i ++){
    //         for (int j = 0; j < 10; j ++){
    //             mvprintw(i + 10, j + 10, ".");
    //         }
    //     }
    //     mvprintw(15, 15, "🗡𓌜");
    //     getch();
    // }

    ///// 🏆
    read_users();
    main_menu();
    endwin();
    write_users();
    return 0;
}

void read_game(struct user* user, FILE *fptr){
    user -> game = (struct game *) malloc(sizeof(struct game));
    struct game* game = user -> game;
    ////
    fscanf(fptr, "%d", &(game -> current_level));
    fscanf(fptr, "%d %d", &(game -> current_x), &(game -> current_y));
    fscanf(fptr, "%d", &(game -> Vis));
    fscanf(fptr, "%d", &(game -> golds));
    fscanf(fptr, "%d %d", &(game -> hunger), &(game -> health));
    fscanf(fptr, "%d", &(game -> food));
    fscanf(fptr, "%d", &(game -> current_w));
    fscanf(fptr, "%d %d", &(game -> mace), &(game -> sword));
    fscanf(fptr, "%d %d %d", &(game -> dagger), &(game -> wand), &(game -> arrow));
    fscanf(fptr, "%d %d %d", &(game -> health_s), &(game -> speed_s), &(game -> damage_s));
    return;
}

void write_game(struct user* user, FILE* fptr){
    user -> game = (struct game *) malloc(sizeof(struct game));
    struct game* game = user -> game;
    ////
    fprintf(fptr, "%d ", game -> current_level);
    fprintf(fptr, "%d %d ", game -> current_x, game -> current_y);
    fprintf(fptr, "%d ", game -> Vis);
    fprintf(fptr, "%d ", game -> golds);
    fprintf(fptr, "%d %d ", game -> hunger, game -> health);
    fprintf(fptr, "%d ", game -> food);
    fprintf(fptr, "%d ", game -> current_w);
    fprintf(fptr, "%d %d ", game -> mace, game -> sword);
    fprintf(fptr, "%d %d %d ", game -> dagger, game -> wand, game -> arrow);
    fprintf(fptr, "%d %d %d ", game -> health_s, game -> speed_s, game -> damage_s);
    return;
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
        fscanf(fptr, "%lld", &(users[i] -> birth));
        fscanf(fptr, "%d", &(users[i] -> total_score));
        fscanf(fptr, "%d", &(users[i] -> golds));
        fscanf(fptr, "%d", &(users[i] -> games_played));
        fscanf(fptr, "%d", &(users[i] -> open_game));
        if(users[i] -> open_game){
            read_game(users[i], fptr);
        }
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
        fprintf(fptr, "%d ", users[i] -> id);
        fprintf(fptr, "%lld\n", users[i] -> birth);
        fprintf(fptr, "%d ", users[i] -> total_score);
        fprintf(fptr, "%d ", users[i] -> golds);
        fprintf(fptr, "%d ", users[i] -> games_played);
        fprintf(fptr, "%d\n", users[i] -> open_game);
        if(users[i] -> open_game){
            write_game(users[i], fptr);
        }
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

int email_validation(char *email) {
    regex_t regex;
    int ans;
    const char *pattern = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$";
    regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
    ans = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    return !ans;
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
    users[N] -> id = N;
    strcpy(users[N] -> user_name, user_name);
    strcpy(users[N] -> email, email);
    strcpy(users[N] -> password, password);
    users[N] -> total_score = 0;
    users[N] -> open_game = 0;
    users[N] -> games_played = 0;
    users[N] -> birth = time(NULL);
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
    int loc[6] = {7, 16, 32, 48, 61, 78};
    int pg = 0;
    while(1){
        menu_border();
        for (int i = 0; i < 5; i ++) mvprintw(i + 2, 15, lines[i]);
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        if((N + 19) / 20 > 1){
            if(pg == 0) mvprintw(LINES - 2, 26, "/  N for next page");
            else mvprintw(LINES - 2, 26, "/  N for first page");
        }
        for (int i = 0; i < 6; i ++) mvprintw(9, loc[i], "%s", top[i]);
        scoreboard_sort_users();
        for (int i = 0; i < N - pg * 20 && i < 20; i ++){
            int id = pg * 20 + i;
            if(signed_in && users[id] == current_user) attron(A_BOLD);
            if(id < 3) attron(COLOR_PAIR(i + 1));
            mvprintw(11 + i, 8, "%d", id + 1);
            mvprintw(11 + i, (id < 3 ? 20 : 17), "%s", users[id] -> user_name);
            mvprintw(11 + i, 34, "%d", users[id] -> total_score);
            mvprintw(11 + i, 50, "%d", users[id] -> golds);
            mvprintw(11 + i, 66, "%d", users[id] -> games_played);
            mvprintw(11 + i, 82, "%d", (time(0) - users[id] -> birth) / 86400);
            if(id == 0){
                mvprintw(11 + 0, 5, Ugold_medal);
                mvprintw(11 + 0, 15, "GOAT-");
            }
            if(id == 1){
                mvprintw(11 + 1, 5, Usecond_medal);
                mvprintw(11 + 1, 13, "LEGEND-");
            }
            if(id == 2){
                mvprintw(11 + 2, 5, Uthird_medal);
                mvprintw(11 + 2, 13, "MASTER-");
            }
            if(id < 3) attroff(COLOR_PAIR(i + 1));
            if(signed_in && users[id] == current_user) attroff(A_BOLD);
        }
        move(0, 0);
        refresh();
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
    room -> x0 = rand() % (ROW - 10) + 1;
    room -> y0 = rand() % (COL - 10) + 1;
    room -> x1 = room -> x0 + rand() % 5 + 7; /// 6-10
    room -> y1 = room -> y0 + rand() % 5 + 7;
    return room;
}

int floor_check(struct floor* floor){
    int map[ROW + 10][COL + 10];
    for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) map[i][j] = 0;
    for (int i = 0; i < floor -> room_count; i ++){
        struct Room *room = floor -> rooms[i];
        if(room -> x1 >= ROW - 3 || room -> y1 >= COL - 3) return 0;
        for (int i = room -> x0; i <= room -> x1; i ++){
            if(room -> y0 && map[i][room -> y0 - 1]) return 0;
            if(room -> y1 + 1 < COL && map[i][room -> y1 + 1]) return 0;
        }
        for (int j = room -> y0; j <= room -> y1; j ++){
            if(room -> x0 && map[room -> x0 - 1][j]) return 0;
            if(room -> x1 + 1 < ROW && map[room -> x1 + 1][j]) return 0;
        }
        for (int i = room -> x0; i <= room -> x1; i ++){
            for (int j = room -> y0; j <= room -> y1; j ++){
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
    for (int i = 0; i < 100; i ++) for (int j = 0; j < 1000; j ++) mark[i][j] = seen[i][j] = 0;
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
        }///!
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

void goldize(struct floor* floor, int T){
    for (int r = 0; r < floor -> room_count; r ++){
        int x0 = floor -> rooms[r] -> x0, x1 = floor -> rooms[r] -> x1;
        int y0 = floor -> rooms[r] -> y0, y1 = floor -> rooms[r] -> y1;
        /////NORMAL GOLDS
        int rnd = rand() % 7;
        int gold_cnt = 3;
        if(rnd == 0) gold_cnt = 0;
        else if(rnd < 3) gold_cnt = 1;
        else if(rnd < 6) gold_cnt = 2;
        if(T == 1) gold_cnt = 20;
        int gx[50], gy[50];
        while(1){
            bool check = 1;
            for (int i = 0; i < gold_cnt; i ++){
                gx[i] = x0 + 1 + rand() % (x1 - x0 - 2);
                gy[i] = y0 + 1 + rand() % (y1 - y0 - 2);
            }
            for (int i = 0; i < gold_cnt; i ++){
                for (int j = 0; j < i; j ++){
                    if(gx[i] == gx[j] && gy[i] == gy[j]) check = 0;
                }
                if(floor -> map[gx[i]][gy[i]] != '.') check = 0;
            }
            if(check) break;
        }
        for (int i = 0; i < gold_cnt; i ++) floor -> map[gx[i]][gy[i]] = 'g';
        ///BLACK GOLD
        if(rand() % 4 == 0){
            while(1){
                int gx = x0 + 1 + rand() % (x1 - x0 - 2);
                int gy = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[gx][gy] != '.') continue;
                floor -> map[gx][gy] = 'G';
                break;
            }
        }
    }
    return;
}

void treasurize(struct floor * floor){
    int r = rand() % floor -> room_count;
    int x0 = floor -> rooms[r] -> x0, x1 = floor -> rooms[r] -> x1;
    int y0 = floor -> rooms[r] -> y0, y1 = floor -> rooms[r] -> y1;
    while(1){
        int x = x0 + 1 + rand() % (x1 - x0 - 2);
        int y = y0 + 1 + rand() % (y1 - y0 - 2);
        if(floor -> map[x][y] != '.') continue;
        if(floor -> map[x][y + 1] != '.') continue;
        floor -> map[x][y] = 'T';
        break;
    }
    return;
}

void trapize(struct floor* floor, int T){
    floor -> trap = (int **) malloc((ROW + 10) * sizeof(int *));
    for (int i = 0; i < ROW; i ++){
        floor -> trap[i] = (int *) malloc((COL + 10) * sizeof(int));
        for (int j = 0; j < COL; j ++) floor -> trap[i][j] = 0;
    }
    for (int i = 0; i < T; i ++){
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floor -> map[x][y] != '.') continue;
            floor -> trap[x][y] = 1;
            break;
        }
    }
    return;
}

void weaponize(struct game* game){
    game -> current_w = -1;
    game -> mace = 1;
    game -> sword = 0;
    game -> dagger = 0;
    game -> wand = 0;
    game -> arrow = 0;
    ///DAGER
    for (int i = 0; i < 10; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'd';
            break;
        }
    }
    ///WAND
    for (int i = 0; i < 10; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'w';
            break;
        }
    }
    ///ARROW
    for (int i = 0; i < 10; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'a';
            break;
        }
    }
    ///SWORD
    int s_floor = rand() % 5;
    struct floor *floors = game -> floors[s_floor];
    while(1){
        int x = rand() % (ROW - 1), y = rand() % (COL - 1);
        if(floors -> map[x][y] != '.') continue;
        if(floors -> trap[x][y] == 1) continue;
        floors -> map[x][y] = 's';
        break;
    }
    return;
}

void spellize(struct game* game){
    game -> health_s = 0;
    game -> speed_s = 0;
    game -> damage_s = 0;
    ///HEALTHSPELL
    for (int i = 0; i < 5; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'H';
            break;
        }
    }
    ///SPEEDSPELL
    for (int i = 0; i < 5; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'S';
            break;
        }
    }
    ///DAMAGESPELL
    for (int i = 0; i < 5; i ++){
        int s_floor = rand() % 6;
        struct floor *floors = game -> floors[s_floor];
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floors -> map[x][y] != '.') continue;
            if(floors -> trap[x][y] == 1) continue;
            floors -> map[x][y] = 'D';
            break;
        }
    }
    ///SPELLROOM
    for (int i = 0; i < 5; i ++){
        struct floor *floor = game -> floors[i];
        int x0 = floor -> rooms[2] -> x0, x1 = floor -> rooms[2] -> x1;
        int y0 = floor -> rooms[2] -> y0, y1 = floor -> rooms[2] -> y1;
        for (int i = 0; i < 2; i ++){
            while(1){
                int x = x0 + 1 + rand() % (x1 - x0 - 2);
                int y = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[x][y] != '.') continue;
                floor -> map[x][y] = 'H';
                break;
            }
        }
        for (int i = 0; i < 2; i ++){
            while(1){
                int x = x0 + 1 + rand() % (x1 - x0 - 2);
                int y = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[x][y] != '.') continue;
                floor -> map[x][y] = 'S';
                break;
            }
        }
        for (int i = 0; i < 2; i ++){
            while(1){
                int x = x0 + 1 + rand() % (x1 - x0 - 2);
                int y = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[x][y] != '.') continue;
                floor -> map[x][y] = 'D';
                break;
            }
        }
    }
    return;
}

void monsterize(struct floor* floor, int T){
    floor -> monster_count = T;
    floor -> monsters = (struct monster**) malloc(T * sizeof(struct monster*));
    for (int m = 0; m < T; m ++){
        floor -> monsters[m] = (struct monster*) malloc(sizeof(struct monster));
        struct monster* monster = floor -> monsters[m];
        monster -> type = rand() % 5;
        monster -> act = 0;
        monster -> health = 5 * (monster -> type + 1) + (monster -> type == 4 ? 5 : 0);
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floor -> map[x][y] != '.') continue;
            if(floor -> trap[x][y] == 1) continue;
            monster -> x = x;
            monster -> y = y;
            break;
        }
    }
}

struct floor* new_floor(){
    struct floor *floor = (struct floor *) malloc(sizeof(struct floor));
    ////ROOMS
    floor -> room_count = rand() % 2 + 6; /// 6-7 rooms
    floor -> rooms = (struct Room **) malloc(floor -> room_count * sizeof(struct Room *));
    while(1){
        for (int i = 0; i < floor -> room_count; i ++){
            floor -> rooms[i] = new_room();
        }
        if(floor_check(floor)) break;
    }
    ////MP
    floor -> mp = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        floor -> mp[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) floor -> mp[i][j] = ' ';
    }
    for (int i = 0; i < floor -> room_count; i ++){
        struct Room *room = floor -> rooms[i];
        for (int i = room -> x0; i <= room -> x1; i ++){
            for (int j = room -> y0; j <= room -> y1; j ++){
                floor -> mp[i][j] = '.';
            }
        }
        for (int i = room -> x0; i <= room -> x1; i ++){
            floor -> mp[i][room -> y0] = '|';
            floor -> mp[i][room -> y1] = '|';
        }
        for (int j = room -> y0; j <= room -> y1; j ++){
            floor -> mp[room -> x0][j] = '-';
            floor -> mp[room -> x1][j] = '-';
        }
    }
    ////CORRIDEOR
    draw_corridor(floor -> mp, floor -> rooms[0] -> x0 + 1, floor -> rooms[0] -> y0);
    ////MAP
    floor -> map = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        floor -> map[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) floor -> map[i][j] = floor -> mp[i][j];
    }
    /////VIS
    floor -> vis = (int **) malloc((ROW + 10) * sizeof(int *));
    for (int i = 0; i < ROW; i ++){
        floor -> vis[i] = (int *) malloc((COL + 10) * sizeof(int));
        for (int j = 0; j < COL; j ++) floor -> vis[i][j] = 0;
    }
    ////STAIRS
    floor -> stair_x = floor -> rooms[1] -> x0 + rand() % 6 + 1;
    floor -> stair_y = floor -> rooms[1] -> y0 + rand() % 6 + 1;
    floor -> map[floor -> stair_x][floor -> stair_y] = '<';
    ///GOLDIZE
    goldize(floor, 0);
    ////TRAPIZE
    trapize(floor, 10);
    ////FOODIZE
    foodize(floor, 10);
    ///MONSTERIZE
    monsterize(floor, 5);
    return floor;
}

struct floor* new_treasure_floor(){
    struct floor *floor = (struct floor *) malloc(sizeof(struct floor));
    ///ROOM
    floor -> room_count = 1;
    struct Room *room = (struct Room *) malloc(sizeof(struct Room));
    floor -> rooms = (struct Room **) malloc(sizeof(struct Room *));
    floor -> rooms[0] = room;
    room -> x0 = ROW / 2 - 8 - 2; room -> x1 = ROW / 2 + 8 - 2;
    room -> y0 = COL / 2 - 25; room -> y1 = COL / 2 + 25;
    ////MP
    floor -> mp = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        floor -> mp[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) floor -> mp[i][j] = ' ';
    }
    for (int i = room -> x0; i <= room -> x1; i ++){
        for (int j = room -> y0; j <= room -> y1; j ++){
            floor -> mp[i][j] = '.';
        }
    }
    for (int i = room -> x0; i <= room -> x1; i ++){
        floor -> mp[i][room -> y0] = '|';
        floor -> mp[i][room -> y1] = '|';
    }
    for (int j = room -> y0; j <= room -> y1; j ++){
        floor -> mp[room -> x0][j] = '-';
        floor -> mp[room -> x1][j] = '-';
    }
    ////MAP
    floor -> map = (char **) malloc((ROW + 10) * sizeof(char *));
    for (int i = 0; i < ROW; i ++){
        floor -> map[i] = (char *) malloc((COL + 10) * sizeof(char));
        for (int j = 0; j < COL; j ++) floor -> map[i][j] = floor -> mp[i][j];
    }
    /////VIS
    floor -> vis = (int **) malloc((ROW + 10) * sizeof(int *));
    for (int i = 0; i < ROW; i ++){
        floor -> vis[i] = (int *) malloc((COL + 10) * sizeof(int));
        for (int j = 0; j < COL; j ++) floor -> vis[i][j] = 1;
    }
    ///COINS
    goldize(floor, 1);
    ////TRAPIZE
    trapize(floor, 30);
    ////FOODIZE
    foodize(floor, 10);
    ///MONSTERIZE
    monsterize(floor, 8);
    return floor;
}

struct game* new_game(){
    struct game *game = (struct game*) malloc(sizeof(struct game));
    for (int f = 0; f < 5; f ++){
        game -> floors[f] = new_floor();
        //for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) game -> map[f][i][j] = game -> floors[f] -> map[i][j];
        //for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) game -> to[f][i][j] = ' ';
        //for (int i = 0; i < ROW; i ++) for (int j = 0; j < COL; j ++) game -> vis[f][i][j] = 0;
    }
    game -> floors[5] = new_treasure_floor();
    treasurize(game -> floors[4]);
    game -> Vis = 0;
    game -> current_level = 0;
    game -> current_x = game -> floors[0] -> rooms[0] -> x0 + rand() % 6 + 1;
    game -> current_y = game -> floors[0] -> rooms[0] -> y0 + rand() % 6 + 1;

    game -> health = 100;
    game -> hunger = 0;
    game -> food = 0;

    weaponize(game);
    spellize(game);
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
                int res = game_play(game);
                if(signed_in){
                    current_user -> game = game;
                    current_user -> open_game = 1;
                    if(res != -1){
                        current_user -> games_played ++;
                        current_user -> open_game = 0;
                    }
                    if(res == 1){
                        current_user -> golds += game -> golds;
                        current_user -> total_score += 0;//////
                    }
                }
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
                struct game *game = current_user -> game;
                int res = game_play(game);
                if(res != -1){
                    current_user -> games_played ++;
                    current_user -> open_game = 0;
                }
                if(res == 1){
                    current_user -> golds += game -> golds;
                    current_user -> total_score += 0;//////
                }
            }
            if(cr == 2){
                game_menu();
            }
        }
        else if(get == 27){
            main_menu();
            return;
        }
    }
    return;
}