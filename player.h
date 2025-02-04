// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "map.h"

void init_player(Player *player);
void move_player(Player *player, const GameMap *map, int dx, int dy);
void draw_player(const Player *player);
void handle_trap(Player *player, const GameMap *map);

#endif
