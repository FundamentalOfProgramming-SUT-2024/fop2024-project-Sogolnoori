#include <ncurses.h>
#include <locale.h>
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

void define_colors();
void read_users();
void write_users();
void read_game(struct user*, FILE *);
void write_game(struct user*, FILE*);


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


int N = 0;
int signed_in = 0;
struct user* users[2000], *current_user;


int main(){
    setlocale(LC_ALL,"");
    srand(time(NULL));
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    define_colors();

    ///// 🏆  🏹
    
    read_users();
    main_menu();
    endwin();
    write_users();
    return 0;
}

void define_colors(){
    init_pair(1, 220, COLOR_BLACK); //Gold 220
    init_pair(2, 248, COLOR_BLACK); //Silver 248
    init_pair(3, 131, COLOR_BLACK); //Bronze! 131
    init_pair(4, 147, COLOR_BLACK); //light purple
    init_pair(5, COLOR_BLACK, COLOR_WHITE); //inwhite
    init_pair(6, COLOR_BLACK, COLOR_RED); //inred
    init_pair(7, COLOR_GREEN, COLOR_BLACK);//green
    init_pair(8, 128, COLOR_BLACK); //purple
    init_pair(9, 227, COLOR_BLACK); //yellow
    init_pair(10, 229, COLOR_BLACK); //melo yellow
    init_pair(11, 52, COLOR_BLACK); //black gold

    init_pair(12, 39, COLOR_BLACK); //Blue
    init_pair(13, 23, COLOR_BLACK); //green
    init_pair(14, 99, COLOR_BLACK); //purple

    init_pair(20, 117, COLOR_BLACK); //Blue
    init_pair(21, 161, COLOR_BLACK); //red
    init_pair(22, 49, COLOR_BLACK); //green

    return;
}

void read_game(struct user* user, FILE *fptr){
    user -> game = (struct game *) malloc(sizeof(struct game));
    struct game* game = user -> game;
    ////
    fscanf(fptr, "%d", &(game -> current_level));
    fscanf(fptr, "%d %d", &(game -> current_x), &(game -> current_y));
    fscanf(fptr, "%d", &(game -> Vis));
    fscanf(fptr, "%d %d", &(game -> golds), &(game -> score));
    fscanf(fptr, "%d %d", &(game -> hunger), &(game -> health));
    fscanf(fptr, "%d %d %d", &(game -> food), &(game -> gfood), &(game -> mfood));
    fscanf(fptr, "%d", &(game -> current_w));
    fscanf(fptr, "%d %d", &(game -> mace), &(game -> sword));
    fscanf(fptr, "%d %d %d", &(game -> dagger), &(game -> wand), &(game -> arrow));
    fscanf(fptr, "%d %d %d", &(game -> health_s), &(game -> speed_s), &(game -> damage_s));
    for (int i = 0; i < 6; i ++){
        game -> floors[i] = (struct floor *) malloc(sizeof(struct floor));
        struct floor *floor = game -> floors[i];
        fscanf(fptr, "%d", &(floor -> room_count));
        fscanf(fptr, "%d %d", &(floor -> stair_x), &(floor -> stair_y));
        ///rooms
        floor -> rooms = (struct Room **) malloc(floor -> room_count * sizeof(struct Room *));
        for (int r = 0; r < floor -> room_count; r ++){
            floor -> rooms[r] = (struct Room *) malloc(sizeof(struct Room));
            struct Room* room = floor -> rooms[r];
            fscanf(fptr, "%d %d %d %d", &(room -> x0), &(room -> y0), &(room -> x1), &(room -> y1));
        }
        ///mp
        floor -> mp = (char **) malloc((ROW + 10) * sizeof(char *));
        char trash[10000];
        fgets(trash, 2000, fptr);
        for (int i = 0; i < ROW; i ++){
            floor -> mp[i] = (char *) malloc((COL + 10) * sizeof(char));
                fgets(floor -> mp[i], 2000, fptr);
        }
        ///map
        floor -> map = (char **) malloc((ROW + 10) * sizeof(char *));
        for (int i = 0; i < ROW; i ++){
            floor -> map[i] = (char *) malloc((COL + 10) * sizeof(char));
            fgets(floor -> map[i], 2000, fptr);
        }
        ///vis
        floor -> vis = (int **) malloc((ROW + 10) * sizeof(int *));
        for (int i = 0; i < ROW; i ++){
            floor -> vis[i] = (int *) malloc((COL + 10) * sizeof(int));
            for (int j = 0; j < COL; j ++){
                fscanf(fptr, "%d", &(floor -> vis[i][j]));
            }
        }
        ///trap
        floor -> trap = (int **) malloc((ROW + 10) * sizeof(int *));
        for (int i = 0; i < ROW; i ++){
            floor -> trap[i] = (int *) malloc((COL + 10) * sizeof(int));
            for (int j = 0; j < COL; j ++){
                fscanf(fptr, "%d", &(floor -> trap[i][j]));
            }
        }
        ///monst
        fscanf(fptr, "%d", &(floor -> monster_count));
        floor -> monsters = (struct monster **) malloc(floor -> monster_count * sizeof(struct monster *));
        for (int m = 0; m < floor -> monster_count; m ++){
            floor -> monsters[m] = (struct monster *) malloc(sizeof(struct monster));
            struct monster* monster = floor -> monsters[m];
            fscanf(fptr, "%d %d", &(monster -> x), &(monster -> y));
            fscanf(fptr, "%d %d %d", &(monster -> type), &(monster -> act), &(monster -> steps));
            fscanf(fptr, "%d", &(monster -> health));

        }
    }
    return;
}

void write_game(struct user* user, FILE* fptr){
    struct game* game = user -> game;
    ////
    fprintf(fptr, "%d ", game -> current_level);
    fprintf(fptr, "%d %d ", game -> current_x, game -> current_y);
    fprintf(fptr, "%d ", game -> Vis);
    fprintf(fptr, "%d %d ", game -> golds, game -> score);
    fprintf(fptr, "%d %d ", game -> hunger, game -> health);
    fprintf(fptr, "%d %d %d ", game -> food, game -> gfood, game -> mfood);
    fprintf(fptr, "%d ", game -> current_w);
    fprintf(fptr, "%d %d ", game -> mace, game -> sword);
    fprintf(fptr, "%d %d %d ", game -> dagger, game -> wand, game -> arrow);
    fprintf(fptr, "%d %d %d ", game -> health_s, game -> speed_s, game -> damage_s);
    for (int i = 0; i < 6; i ++){
        struct floor *floor = game -> floors[i];
        fprintf(fptr, "%d ", floor -> room_count);
        fprintf(fptr, "%d %d\n", floor -> stair_x, floor -> stair_y);
        ///rooms
        for (int r = 0; r < floor -> room_count; r ++){
            struct Room* room = floor -> rooms[r];
            fprintf(fptr, "%d %d %d %d\n", room -> x0, room -> y0, room -> x1, room -> y1);
        }
        ///mp
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                fprintf(fptr, "%c", floor -> mp[i][j]);
            }
            fprintf(fptr, "\n");
        }
        ///map
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                fprintf(fptr, "%c", floor -> map[i][j]);
            }
            fprintf(fptr, "\n");
        }
        ///vis
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                fprintf(fptr, "%d ", floor -> vis[i][j]);
            }
            fprintf(fptr, "\n");
        }
        ///trap
        for (int i = 0; i < ROW; i ++){
            for (int j = 0; j < COL; j ++){
                fprintf(fptr, "%d ", floor -> trap[i][j]);
            }
            fprintf(fptr, "\n");
        }
        ///monst
        fprintf(fptr, "%d\n", floor -> monster_count);
        for (int m = 0; m < floor -> monster_count; m ++){
            struct monster* monster = floor -> monsters[m];
            fprintf(fptr, "%d %d ", monster -> x, monster -> y);
            fprintf(fptr, "%d %d %d ", monster -> type, monster -> act, monster -> steps);
            fprintf(fptr, "%d ", monster -> health);

        }
    }
    fprintf(fptr, "\n\n");
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
    char options[6][20] = {"SIGN UP", "SIGN IN", "PROFILE", "SCOREBOARD", "MUSIC MENU", "PLAY"};
    int cr = 0;
    noecho();
    while(1){
        clear();
        menu_border();
        mvprintw(4, 5, "MAIN MENU:");
        for (int i = 0; i < 6; i ++){
            mvprintw(5 + i, 7, options[i]);
        }
        for (int i = 0; i < 6; i ++){
            if(cr == i){
                attron(A_REVERSE);
                mvprintw(5 + i, 7, options[i]);
                attroff(A_REVERSE);
            }
        }
        move(0, 0);
        int get = getch();
        if(get == KEY_UP) cr = (cr + 5) % 6;
        else if(get == KEY_DOWN) cr = (cr + 1) % 6;
        else if(get == '\n'){
            if(cr == 0) sign_up();
            if(cr == 1) sign_in();
            if(cr == 2) profile();
            if(cr == 3) scoreboard();
            if(cr == 4) music_menu();
            if(cr == 5) game_menu();
        }
        else if(get == 'Q') break;
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
    users[N] -> golds = 0;
    users[N] -> open_game = 0;
    users[N] -> games_played = 0;
    users[N] -> birth = time(NULL);
    N ++;
    return;
}

void sign_up(){
    echo();
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
            return;
        }

        mvprintw(8, 5, "Enter your email");
        move(9, 6);
        getstr(email);
        if(!email_validation(email)){
            mvprintw(11, 6, "email is not valid!!");
            mvprintw(LINES - 2, 6, "Press any key to exit");
            getch();
            return;
        }

        mvprintw(11, 5, "Enter your password or type \"random\" to get a random password");
        move(12, 6);
        getstr(password);
        int password_valid = password_validation(password);
        if(strcmp(password, "random") == 0){
            password[0] = 'A' + rand() % 26;
            password[1] = '0' + rand() % 10;
            password[2] = 'a' + rand() % 26;
            password[3] = 'a' + rand() % 26;
            password[4] = 'A' + rand() % 26;
            password[5] = '0' + rand() % 10;
            password[6] = 'A' + rand() % 26;
            password[7] = '\0';
        }
        else if(password_valid){
            if(password_valid == 1) mvprintw(14, 6, "password must contain at least 7 characters!!");
            if(password_valid == 2) mvprintw(14, 6, "password must contain a number!!");
            if(password_valid == 3) mvprintw(14, 6, "password must contain a lower!!");
            if(password_valid == 4) mvprintw(14, 6, "password must contain an upper!!");
            mvprintw(LINES - 2, 4, "Press any key to exit");
            getch();
            return;
        }
        new_user(user_name, email, password);
        refresh();
        clear();
        menu_border();
        mvprintw(5, 10, "Hello %s :)", user_name);
        mvprintw(7, 10, "Your password: %s", password);
        getch();
        break;
    }
    return;
}

void password_recovery_menu(){
    while(1){
        clear();
        menu_border();
        char user_name[100], email[100], password[100];
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
            return;
        }
        mvprintw(8, 5, "Enter your email");
        move(9, 6);
        getstr(email);
        if(strcmp(users[id] -> email, email) != 0){
            mvprintw(11, 6, "incorrect email!!");
            mvprintw(LINES - 2, 6, "Press any key to exit");
            getch();
            return;
        }
        clear();
        menu_border();
        mvprintw(5, 5, "Here is your password:");
        mvprintw(7, 8, "%s", users[id] -> password);
        mvprintw(LINES - 2, 6, "Press any key to exit");
        getch();
        return;
    }
    return;
}

void sign_in(){
    echo();
    clear();
    refresh();
    if(signed_in == 1){
        menu_border();
        mvprintw(5, 5, "You are already signed in!");
        mvprintw(LINES - 2, 4, "Press any key to exit");
        getch();
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
            return;
        }
        mvprintw(8, 5, "Enter your password %s:)", user_name);
        move(9, 6);
        getstr(password);
        if(strcmp(users[id] -> password, password) != 0){
            mvprintw(11, 5, "Incorrect password!");
            mvprintw(12, 5, "Forgot your password?  Press f to recover it");
            mvprintw(LINES - 2, 4, "Press Enter to exit");
            int get = getch();
            if(get == 'f'){
                password_recovery_menu();
            }
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
            return;
        }
        mvprintw(5, 5, "ME:");
        mvprintw(6, 5, "My email:");
        mvprintw(7, 5, "My password:");
        mvprintw(8, 5, "My score:");
        mvprintw(9, 5, "My golds:");
        mvprintw(10, 5, "My experience:");

        mvprintw(5, 22, "%s", current_user -> user_name);
        mvprintw(6, 22, "%s", current_user -> email);
        mvprintw(7, 22, "%s", current_user -> password);
        mvprintw(8, 22, "%d", current_user -> total_score);
        mvprintw(9, 22, "%d", current_user -> golds);
        mvprintw(10, 22, "%d m", (time(0) - current_user -> birth) / 60);
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        refresh();
        if(getch() == '\n'){
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
    int kx = (LINES - ROW) / 2 - 5, ky = (COLS - COL) / 2;
    char lines[5][100] = {"***   ***    ***   ***   ****  ****    ***     **    ***   ****",
                          "*    *   *  *   *  *  *  *     *   *  *   *   *  *   *  *  *   *",
                          "***  *      *   *  ***   ****  ****   *   *  ******  ***   *   *", 
                          "  *  *   *  *   *  * *   *     *   *  *   *  *    *  * *   *   *", 
                          "***   ***    ***   *  *  ****  ****    ***   *    *  *  *  ****"     };

    char top[6][20] = {"Rank", "Username", "Score", "Golds", "games_played", "experience"};
    int loc[6] = {7, 16, 32, 48, 61, 78};
    int pg = 0;
    while(1){
        menu_border();
        for (int i = 0; i < 5; i ++) mvprintw(i + 2 + kx, 15 + ky, lines[i]);
        for (int i = 0; i < 6; i ++) mvprintw(9 + kx, loc[i] + ky, "%s", top[i]);
        scoreboard_sort_users();
        for (int i = 0; i < N - pg * 20 && i < 20; i ++){
            int id = pg * 20 + i;
            if(signed_in && users[id] == current_user) attron(A_BOLD);
            if(id < 3) attron(COLOR_PAIR(i + 1));
            mvprintw(11 + i + kx, 8 + ky, "%d", id + 1);
            mvprintw(11 + i + kx, (id < 3 ? 20 : 17) + ky, "%s", users[id] -> user_name);
            mvprintw(11 + i + kx, 34 + ky, "%d", users[id] -> total_score);
            mvprintw(11 + i + kx, 50 + ky, "%d", users[id] -> golds);
            mvprintw(11 + i + kx, 66 + ky, "%d", users[id] -> games_played);
            mvprintw(11 + i + kx, 82 + ky, "%d", (time(0) - users[id] -> birth) / 86400);
            if(id == 0){
                mvprintw(11 + 0 + kx, 5 + ky, Ugold_medal);
                mvprintw(11 + 0 + kx, 15 + ky, "GOAT-");
            }
            if(id == 1){
                mvprintw(11 + 1 + kx, 5 + ky, Usecond_medal);
                mvprintw(11 + 1 + kx, 13 + ky, "LEGEND-");
            }
            if(id == 2){
                mvprintw(11 + 2 + kx, 5 + ky, Uthird_medal);
                mvprintw(11 + 2 + kx, 13 + ky, "MASTER-");
            }
            if(id < 3) attroff(COLOR_PAIR(i + 1));
            if(signed_in && users[id] == current_user) attroff(A_BOLD);
        }
        /////Messeges
        mvprintw(LINES - 2, 4, "Press Enter to exit");
        if((N + 19) / 20 > 1){
            if(pg == 0) mvprintw(LINES - 2, 26, "/  N for next page");
            else mvprintw(LINES - 2, 26, "/  N for first page");
        }
        refresh();
        int get = getch();
        if(get == 'n' || get == 'N') pg = (pg + 1) % ((N + 19) / 20);
        if(get == 27 || get == '\n'){
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
        if(T == 1){
            for (int i = 0; i < 10; i ++){
                while(1){
                int gx = x0 + 1 + rand() % (x1 - x0 - 2);
                int gy = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[gx][gy] != '.') continue;
                floor -> map[gx][gy] = 'G';
                break;
            }
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
        if(floor -> map[x][y - 1] != '.') continue;
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
        if(floors -> map[x][y + 1] != '.') continue;
        if(floors -> map[x][y - 1] != '.') continue;
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
        for (int i = 0; i < 3; i ++){
            while(1){
                int x = x0 + 1 + rand() % (x1 - x0 - 2);
                int y = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[x][y] != '.') continue;
                floor -> map[x][y] = 'H';
                break;
            }
        }
        for (int i = 0; i < 3; i ++){
            while(1){
                int x = x0 + 1 + rand() % (x1 - x0 - 2);
                int y = y0 + 1 + rand() % (y1 - y0 - 2);
                if(floor -> map[x][y] != '.') continue;
                floor -> map[x][y] = 'S';
                break;
            }
        }
        for (int i = 0; i < 3; i ++){
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
        monster -> steps = 5;
        if(monster -> type == 3) monster -> steps = 10000;
        monster -> health = 5 * (monster -> type + 1) + (monster -> type == 4 ? 5 : 0);
        while(1){
            int x = rand() % (ROW - 1), y = rand() % (COL - 1);
            if(floor -> map[x][y] != '.') continue;
            if(floor -> trap[x][y] == 1) continue;
            if(floor -> room_count > 1 && floor -> rooms[2] -> x0 <= x && x <= floor -> rooms[2] -> x1 && floor -> rooms[2] -> y0 <= y && y <= floor -> rooms[2] -> y1) continue;
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

    game -> golds = 0;
    game -> score = 0;

    game -> health = 100;
    game -> hunger = 0;
    game -> food = 0;
    game -> gfood = 0;
    game -> mfood = 0;

    weaponize(game);
    game -> health_s = 0;
    game -> speed_s = 0;
    game -> damage_s = 0;
    spellize(game);
    return game;
}

void setting_border(){
    clear();
    for (int i = 0; i < COLS; i += 2){
        mvprintw(0, i, Usetting);
        mvprintw(LINES - 1, i, Usetting);
    }
    for (int i = 0; i < LINES; i ++){
        mvprintw(i, 0, Usetting);
        mvprintw(i, COLS - 2, Usetting);
    }
    return;
}

void setting(){
    int cr = 0, cl = 0, op = 3;
    char titles[2][50] = {"DIFFICULTY:", "CHARACTER'S COLOR:"};
    char options[2][10][50] ={ {"EASY", "MEDIUM", "HARD"},
                               {"BLUE", "RED", "GREEN"} };
    int deg[3] = {100, 10, 5}, d[2];
    if(DEG == 100) d[0] = 0;
    if(DEG == 10) d[0] = 1;
    if(DEG == 5) d[0] = 2;
    d[1] = C_COLOR;
    while(1){
        noecho();
        clear();
        setting_border();
        int cnt = 30;
        for (int c = 0; c < 2; c ++){
            mvprintw(4, 5 + cnt * c, titles[c]);
            mvprintw(LINES - 2, 4, "Press Q to exit");
            for (int i = 0; i < op; i ++){
                mvprintw(6 + i, 7 + cnt * c, options[c][i]);
            }
            attron(COLOR_PAIR(7));
            mvprintw(6 + d[c], 15 + cnt * c, "✔");
            attroff(COLOR_PAIR(7));
        }
        for (int i = 0; i < op; i ++){
            if(cr == i){
                attron(A_REVERSE);
                mvprintw(6 + i, 7 + cnt * cl, options[cl][i]);
                attroff(A_REVERSE);
            }
        }
        refresh();
        int get = getch();
        if(get == KEY_UP) cr = (cr + op - 1) % op;
        else if(get == KEY_DOWN) cr = (cr + 1) % op;
        if(get == '\n'){
            d[cl] = cr;
        }
        if(get == KEY_RIGHT|| get == KEY_LEFT) cl = (cl + 1) % 2;
        DEG = deg[d[0]];
        C_COLOR = d[1];
        if(get == 'Q') return;
    }
}

void game_menu(){
     char blabla[3][100] = {
        "       ,,,",
        "      (o o)",
        "--oOO--( )--OOo--"};
    clear();
    int cr = 0, op = 3;
    char options[10][50] = {"New game", "Continue previous game", "Settings"};
    while(1){
        clear();
        //menu_border();
        for (int i = 0; i < 3; i ++){
            mvprintw(LINES - 3 + i, COLS - 20, blabla[i]);
        }
        mvprintw(LINES - 6, COLS - 30, "Hi, i'm Tuby");
        mvprintw(LINES - 5, COLS - 35, "i'm here to help you :)");
        mvprintw(4, 5, "GAME MENU:");
        mvprintw(LINES - 2, 4, "Press Q to exit");
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
                        current_user -> total_score += game -> score;
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
                    current_user -> total_score += game -> score;
                }
            }
            if(cr == 2){
                setting();
            }
        }
        else if(get == 'Q'){
            return;
        }
    }
    return;
}