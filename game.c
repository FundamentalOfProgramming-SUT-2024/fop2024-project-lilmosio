#include "game.h"
#include "map.h"
#include <string.h>

void init_ncurses() {
    initscr();
    start_color();
    use_default_colors();
    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK); 
    init_pair(5, COLOR_CYAN, COLOR_BLACK);  
    init_pair(6, COLOR_GREEN, COLOR_BLACK);   
    init_pair(7, COLOR_BLUE, COLOR_BLACK);   
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    
    bkgd(COLOR_PAIR(1));
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
}

void cleanup_ncurses() {
    endwin();
}

bool is_valid_email(const char *email) {
    return (strchr(email, '@') && strchr(email, '.'));
}

void start_game() {
    GameMap map;
    generate_map(&map, 1);

    int ch;
    while ((ch = getch()) != 'q') {
        clear();
        draw_map(&map);
        refresh();
    }
}
