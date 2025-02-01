#ifndef MENU_H
#define MENU_H

#include "game.h"

int show_main_menu();
void handle_new_user(User *users, int *user_count);
void handle_login(User *users, int user_count, User *current_user);
void show_leaderboard(const User *users, int user_count);
void show_settings();
void start_new_game(User *user);
void continue_saved_game(User *user);

bool is_valid_password(const char *password);

#endif