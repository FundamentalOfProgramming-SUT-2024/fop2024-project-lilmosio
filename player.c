#include "player.h"

void init_player(Player *player) {
    player->x = MAP_WIDTH / 2;
    player->y = MAP_HEIGHT / 2;
    player->health = 100;
    player->gold = 0;
    player->keys = 1;
    player->current_floor = 1;
    player->has_master_key = false;
}

void move_player(Player *player, const GameMap *map, int dx, int dy) {
    int new_x = player->x + dx;
    int new_y = player->y + dy;

    if (new_x >= 0 && new_x < MAP_WIDTH && 
        new_y >= 0 && new_y < MAP_HEIGHT && 
        map->tiles[new_y][new_x] != '#') {
        player->x = new_x;
        player->y = new_y;
    }
}

void handle_trap(Player *player, const GameMap *map) {
    if (map->traps[player->y][player->x]) {
        player->health -= 10;
        printw("You walked on a trap and lost 10 health points\n");
        refresh();
        getch();

        if (player->health <= 0) {
            printw("The player has died!\n");
            refresh();
            getch();
            exit(0);
        }
    }
}