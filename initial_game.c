//// 403102882

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ll long long

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
    int current_level;
    char map[5][ROW + 10][COL + 10];
    struct floor* floors[5];
};

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
    }
    game -> current_level = 0;
    return game;
}
 
int main(){
    struct game* game = new_game();
    srand(time(0));
    char ** map = game -> floors[0] -> map;
    for(int i = 0; i < ROW; i ++){
       printf("%s\n", map[i]);
    }
    return 0;
}