#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 24
#define MAX_USERS 100
#define MAX_PASSWORD_LEN 50
#define MAX_EMAIL_LEN 100

typedef struct {
    char username[50];
    char password[MAX_PASSWORD_LEN];
    char email[MAX_EMAIL_LEN];
    int score;
    int gold;
    int games_played;
    time_t join_date;
} User;

typedef struct {
    int x;
    int y;
    int health;
    int gold;
    int keys;
    int current_floor;
    bool has_master_key;
} Player;

typedef struct {
    char tiles[MAP_HEIGHT][MAP_WIDTH];
    bool traps[MAP_HEIGHT][MAP_WIDTH];
    bool stairs[MAP_HEIGHT][MAP_WIDTH];
    bool secret_doors[MAP_HEIGHT][MAP_WIDTH];
} GameMap;

void init_ncurses();
void cleanup_ncurses();
bool is_valid_email(const char *email);

#endif