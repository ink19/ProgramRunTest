#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

int main() {
    initscr();
    mvprintw(5, 5, "Hello world!");
    int scrh, scrw;
    for (int loop_i = 100; loop_i > 0; --loop_i) {
        //clear();
        getmaxyx(stdscr, scrh, scrw);
        
        move(0, 0);
        for (int loop_j = 0; loop_j < loop_i; ++loop_j) {
            printw("#");
        }
        mvprintw(0, scrw - 10, "Test");
        printw("\n");
        refresh();
        sleep(1);
    }
    getch();
    endwin();
    return 0;
}