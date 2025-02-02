#ifndef MAP_H
#define MAP_H

#include "game.h"

typedef struct {
    char tiles[MAP_HEIGHT][MAP_WIDTH];
    bool walls[MAP_HEIGHT][MAP_WIDTH];
    bool doors[MAP_HEIGHT][MAP_WIDTH];
    bool corridors[MAP_HEIGHT][MAP_WIDTH];
    bool pillars[MAP_HEIGHT][MAP_WIDTH];
    bool windows[MAP_HEIGHT][MAP_WIDTH];
    bool traps[MAP_HEIGHT][MAP_WIDTH];
    bool secret_doors[MAP_HEIGHT][MAP_WIDTH];
} GameMap;

void generate_map(GameMap *map, int floor);
void draw_map(const GameMap *map);
void reveal_secret_doors(GameMap *map, int x, int y);
void generate_random_room(GameMap *map, int *room_x, int *room_y, int *room_w, int *room_h);
bool is_room_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void generate_corridor(GameMap *map, int x1, int y1, int x2, int y2);

#endif
