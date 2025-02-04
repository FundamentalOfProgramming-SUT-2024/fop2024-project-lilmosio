// save.h
#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include "map.h"

void save_users(const User *users, int count);
void load_users(User *users, int *count);
void save_game(const Player *player, const GameMap *map);
void load_game(Player *player, GameMap *map);

#endif
