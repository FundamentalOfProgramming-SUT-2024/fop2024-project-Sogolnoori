#include <ncurses.h>

int main(){
    initscr();
    refresh();
    getchar();
    endwin();
    return 0;
}
