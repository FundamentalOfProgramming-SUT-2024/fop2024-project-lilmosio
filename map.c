#include "map.h"
#include <stdlib.h>
#include <time.h>

void generate_map(GameMap *map, int floor) {
    srand(time(NULL) + floor);
    memset(map, 0, sizeof(GameMap));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == 0 || y == MAP_HEIGHT-1 || x == 0 || x == MAP_WIDTH-1) {
                map->tiles[y][x] = '#';
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        int x, y, w, h;
        generate_random_room(map, &x, &y, &w, &h);
        
        if (rand() % 2 == 0) {
            map->tiles[y][x + w/2] = '+';
        }
    }

    for (int y = 1; y < MAP_HEIGHT-1; y++) {
        for (int x = 1; x < MAP_WIDTH-1; x++) {
            if (map->tiles[y][x] == '.' && rand() % 100 < 15) {
                map->traps[y][x] = true;
            }
        }
    }
}

void draw_map(const GameMap *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map->tiles[y][x] == '#') {
                attron(COLOR_PAIR(1));
                mvaddch(y, x, '#');
            } else if (map->tiles[y][x] == '.') {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, '.');
            }
        }
    }
}

void draw_player(const Player *player) {
    attron(COLOR_PAIR(3));
    mvaddch(player->y, player->x, '@');
}

void generate_random_room(GameMap *map, int *room_x, int *room_y, int *room_w, int *room_h) {
    *room_w = rand() % 10 + 5;
    *room_h = rand() % 10 + 5;
    *room_x = rand() % (MAP_WIDTH - *room_w - 2) + 1;
    *room_y = rand() % (MAP_HEIGHT - *room_h - 2) + 1;

    for (int y = *room_y; y < *room_y + *room_h; y++) {
        for (int x = *room_x; x < *room_x + *room_w; x++) {
            map->tiles[y][x] = '.';
        }
    }
}

void reveal_secret_doors(GameMap *map, int x, int y) {
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];

        if (new_x >= 0 && new_x < MAP_WIDTH && new_y >= 0 && new_y < MAP_HEIGHT) {
            if (map->secret_doors[new_y][new_x]) {
                map->tiles[new_y][new_x] = '+';
                map->secret_doors[new_y][new_x] = false;
                printw("یک در مخفی آشکار شد!\n");
                refresh();
                getch();
            }
        }
    }
}