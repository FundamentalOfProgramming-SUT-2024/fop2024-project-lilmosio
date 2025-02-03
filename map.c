#include "map.h"
#include <stdlib.h>
#include <time.h>

bool is_room_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
}

void generate_random_room(GameMap *map, int *room_x, int *room_y, int *room_w, int *room_h) {
    for (int y = *room_y; y < *room_y + *room_h; y++) {
        for (int x = *room_x; x < *room_x + *room_w; x++) {
            map->tiles[y][x] = '.';
        }
    }

    for (int x = *room_x; x < *room_x + *room_w; x++) {
        map->tiles[*room_y][x] = '|';
        map->tiles[*room_y + *room_h - 1][x] = '|';
    }
    for (int y = *room_y; y < *room_y + *room_h; y++) {
        map->tiles[y][*room_x] = '|';
        map->tiles[y][*room_x + *room_w - 1] = '|';
    }

    int trap_count = rand() % 3 + 1;
    for (int i = 0; i < trap_count; i++) {
        int trap_x = *room_x + rand() % *room_w;
        int trap_y = *room_y + rand() % *room_h;
        map->tiles[trap_y][trap_x] = '^';
    }

    int pillar_count = rand() % 3 + 1;
    for (int i = 0; i < pillar_count; i++) {
        int pillar_x = *room_x + rand() % *room_w;
        int pillar_y = *room_y + rand() % *room_h;
        map->tiles[pillar_y][pillar_x] = 'O';
    }

    int window_count = rand() % 2 + 1;
    for (int i = 0; i < window_count; i++) {
        int window_x = *room_x + rand() % *room_w;
        int window_y = *room_y + rand() % *room_h;
        map->tiles[window_y][window_x] = '=';
    }
}

void generate_corridor(GameMap *map, int x1, int y1, int x2, int y2) {
    int x = x1, y = y1;
    bool horizontal_first = rand() % 2;

    if (horizontal_first) {
        while (x != x2) {
            if (map->tiles[y][x] == ' ') {
                map->tiles[y][x] = '#';
            }
            x += (x2 > x1) ? 1 : -1;
        }
        while (y != y2) {
            if (map->tiles[y][x] == ' ') {
                map->tiles[y][x] = '#';
            }
            y += (y2 > y1) ? 1 : -1;
        }
    } else {
        while (y != y2) {
            if (map->tiles[y][x] == ' ') {
                map->tiles[y][x] = '#';
            }
            y += (y2 > y1) ? 1 : -1;
        }
        while (x != x2) {
            if (map->tiles[y][x] == ' ') {
                map->tiles[y][x] = '#';
            }
            x += (x2 > x1) ? 1 : -1;
        }
    }

    if (map->tiles[y1][x1] == '|') {
        map->tiles[y1][x1] = '+';
    }
    if (map->tiles[y2][x2] == '|') {
        map->tiles[y2][x2] = '+';
    }
}

void generate_map(GameMap *map, int floor) {
    srand(time(NULL) + floor);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[y][x] = ' ';
            map->traps[y][x] = false;
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        map->tiles[y][0] = '|';
        map->tiles[y][MAP_WIDTH - 1] = '|';
    }
    for (int x = 0; x < MAP_WIDTH; x++) {
        map->tiles[0][x] = '|';
        map->tiles[MAP_HEIGHT - 1][x] = '|';
    }

    int num_rooms = 6;
    int room_x[num_rooms], room_y[num_rooms], room_w[num_rooms], room_h[num_rooms];

    for (int i = 0; i < num_rooms; i++) {
        do {
            room_w[i] = rand() % 6 + 4;
            room_h[i] = rand() % 6 + 4;
            room_x[i] = rand() % (MAP_WIDTH - room_w[i] - 1) + 1;
            room_y[i] = rand() % (MAP_HEIGHT - room_h[i] - 1) + 1;
        } while (i > 0 && is_room_collision(room_x[i], room_y[i], room_w[i], room_h[i],
                                            room_x[i-1], room_y[i-1], room_w[i-1], room_h[i-1]));

        generate_random_room(map, &room_x[i], &room_y[i], &room_w[i], &room_h[i]);
    }

    for (int i = 1; i < num_rooms; i++) {
        int center_x1 = room_x[i-1] + room_w[i-1] / 2;
        int center_y1 = room_y[i-1] + room_h[i-1] / 2;
        int center_x2 = room_x[i] + room_w[i] / 2;
        int center_y2 = room_y[i] + room_h[i] / 2;

        generate_corridor(map, center_x1, center_y1, center_x2, center_y2);
    }

    for (int i = 0; i < num_rooms; i++) {
        int door_x, door_y;
        do {
            door_x = room_x[i] + rand() % room_w[i];
            door_y = room_y[i] + rand() % room_h[i];
        } while (map->tiles[door_y][door_x] != '|');

        map->tiles[door_y][door_x] = '+';
    }

    for (int i = 0; i < num_rooms; i++) {
        for (int y = room_y[i] + 1; y < room_y[i] + room_h[i] - 1; y++) {
            for (int x = room_x[i] + 1; x < room_x[i] + room_w[i] - 1; x++) {
                if (rand() % 100 < 10) {
                    map->traps[y][x] = true;
                }
            }
        }
    }
}

void draw_map(const GameMap *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map->tiles[y][x] == '|') {
                attron(COLOR_PAIR(2));
            } else if (map->tiles[y][x] == '_') {
                attron(COLOR_PAIR(2));
            } else if (map->tiles[y][x] == '+') {
                attron(COLOR_PAIR(3));
            } else if (map->tiles[y][x] == '^') {
                attron(COLOR_PAIR(4));
            } else if (map->tiles[y][x] == 'O') {
                attron(COLOR_PAIR(5));
            } else if (map->tiles[y][x] == '=') {
                attron(COLOR_PAIR(6));
            } else if (map->tiles[y][x] == '#') {
                attron(COLOR_PAIR(7));
            } else {
                attron(COLOR_PAIR(1));
            }

            mvaddch(y, x, map->tiles[y][x]);
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(5) | COLOR_PAIR(6) | COLOR_PAIR(7) | COLOR_PAIR(8));
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

                attron(COLOR_PAIR(6) | A_BLINK);
                mvaddch(new_y, new_x, '+');
                attroff(COLOR_PAIR(6) | A_BLINK);
                refresh();
                napms(100);
            }
        }
    }
}
