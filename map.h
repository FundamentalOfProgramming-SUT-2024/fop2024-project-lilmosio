#ifndef MAP_H
#define MAP_H

#include "game.h"

void generate_map(GameMap *map, int floor);
void draw_map(const GameMap *map);
void reveal_secret_doors(GameMap *map, int x, int y);
void generate_random_room(GameMap *map, int *room_x, int *room_y, int *room_w, int *room_h);

#endif