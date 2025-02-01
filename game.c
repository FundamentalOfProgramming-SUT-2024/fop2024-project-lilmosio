#include "game.h"

// در game.c
void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}

void cleanup_ncurses() {
    endwin();
}

bool is_valid_email(const char *email) {
    return (strchr(email, '@') && strchr(email, '.'));
}