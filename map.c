#include "map.h"
#include <stdlib.h>
#include <time.h>

bool is_room_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
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

    for (int x = *room_x; x < *room_x + *room_w; x++) {
        map->tiles[*room_y][x] = '_';
        map->tiles[*room_y + *room_h - 1][x] = '_';
    }
    for (int y = *room_y; y < *room_y + *room_h; y++) {
        map->tiles[y][*room_x] = '|';
        map->tiles[y][*room_x + *room_w - 1] = '|';
    }
}

void generate_corridor(GameMap *map, int x1, int y1, int x2, int y2) {
    int x = x1, y = y1;

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
}

void generate_map(GameMap *map, int floor) {
    srand(time(NULL));
    (void)floor;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[y][x] = ' ';
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        map->tiles[y][0] = '|';
        map->tiles[y][MAP_WIDTH - 1] = '|';
    }
    for (int x = 0; x < MAP_WIDTH; x++) {
        map->tiles[0][x] = '_';
        map->tiles[MAP_HEIGHT - 1][x] = '_';
    }

    int num_rooms = 5;
    int room_x[num_rooms], room_y[num_rooms], room_w[num_rooms], room_h[num_rooms];

    for (int i = 0; i < num_rooms; i++) {
        do {
            generate_random_room(map, &room_x[i], &room_y[i], &room_w[i], &room_h[i]);
        } while (i > 0 && is_room_collision(room_x[i], room_y[i], room_w[i], room_h[i],
                                            room_x[i-1], room_y[i-1], room_w[i-1], room_h[i-1]));
    }

    for (int i = 1; i < num_rooms; i++) {
        int center_x1 = room_x[i-1] + room_w[i-1] / 2;
        int center_y1 = room_y[i-1] + room_h[i-1] / 2;
        int center_x2 = room_x[i] + room_w[i] / 2;
        int center_y2 = room_y[i] + room_h[i] / 2;

        generate_corridor(map, center_x1, center_y1, center_x2, center_y2);
    }

    for (int i = 0; i < num_rooms; i++) {
        int door_x = room_x[i] + rand() % room_w[i];
        int door_y = room_y[i] + rand() % room_h[i];

        if (map->tiles[door_y][door_x] == '|' || map->tiles[door_y][door_x] == '_') {
            map->tiles[door_y][door_x] = '+';  // در
        }
    }
}

void draw_map(const GameMap *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mvaddch(y, x, map->tiles[y][x]);
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
            }
        }
    }
}
