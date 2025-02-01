// save.c
#include "save.h"
#include <stdio.h>

void save_users(const User *users, int count) {
    FILE *file = fopen("data/users.dat", "wb");
    if (file) {
        fwrite(&count, sizeof(int), 1, file);
        fwrite(users, sizeof(User), count, file);
        fclose(file);
    }
}

void load_users(User *users, int *count) {
    FILE *file = fopen("data/users.dat", "rb");
    if (file) {
        fread(count, sizeof(int), 1, file);
        fread(users, sizeof(User), *count, file);
        fclose(file);
    }
}

void save_game(const Player *player, const GameMap *map) {
    FILE *file = fopen("data/save.dat", "wb");
    if (file) {
        fwrite(player, sizeof(Player), 1, file);
        fwrite(map, sizeof(GameMap), 1, file);
        fclose(file);
    }
}

void load_game(Player *player, GameMap *map) {
    FILE *file = fopen("data/save.dat", "rb");
    if (file) {
        fread(player, sizeof(Player), 1, file);
        fread(map, sizeof(GameMap), 1, file);
        fclose(file);
    }
}