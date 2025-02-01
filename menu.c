#include "menu.h"
#include "save.h"
#include "player.h"
#include "map.h"
#include <ctype.h>
#include <string.h>

void handle_login(User *users, int user_count, User *current_user) {
    clear();
    printw("=== User login ===\n");

    char username[50], password[50];
    printw("Username: ");
    echo();
    getstr(username);
    noecho();

    printw("Password: ");
    getstr(password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *current_user = users[i];
            printw("Successful login!\n");
            refresh();
            getch();
            return;
        }
    }

    printw("Username or password is wrong!\n");
    refresh();
    getch();
}

bool is_valid_password(const char *password) {
    int len = strlen(password);
    if (len < 7) return false;

    bool has_upper = false, has_lower = false, has_digit = false;
    for (int i = 0; i < len; i++) {
        if (isupper(password[i])) has_upper = true;
        if (islower(password[i])) has_lower = true;
        if (isdigit(password[i])) has_digit = true;
    }
    return has_upper && has_lower && has_digit;
}

void handle_new_user(User *users, int *user_count) {
    clear();
    printw("=== Create New User ===\n");

    User new_user = {0};
    printw("Username: ");
    echo();
    getstr(new_user.username);
    noecho();

    for (int i = 0; i < *user_count; i++) {
        if (strcmp(users[i].username, new_user.username) == 0) {
            printw("Username already exists!\n");
            refresh();
            getch();
            return;
        }
    }

    do {
        printw("Password (min 7 chars, 1 uppercase, 1 lowercase, 1 digit): ");
        getstr(new_user.password);
    } while (!is_valid_password(new_user.password));

    do {
        printw("Email: ");
        getstr(new_user.email);
    } while (!is_valid_email(new_user.email));

    users[(*user_count)++] = new_user;
    save_users(users, *user_count);
    printw("User created successfully!\n");
    refresh();
    getch();
}

void start_new_game(User *user) {
    clear();
    printw("=== Start new game===\n");
    printw("User: %s\n", user->username);
    refresh();
    getch();

    Player player;
    GameMap map;
    init_player(&player);
    generate_map(&map, 1);

    timeout(100);
    int ch;
    while ((ch = getch()) != 'q') {
        clear();
        draw_map(&map);
        draw_player(&player);
        refresh();

        switch (ch) {
            case KEY_UP:    move_player(&player, &map, 0, -1); break;
            case KEY_DOWN:  move_player(&player, &map, 0, 1);  break;
            case KEY_LEFT:  move_player(&player, &map, -1, 0); break;
            case KEY_RIGHT: move_player(&player, &map, 1, 0);  break;
            case 's':       reveal_secret_doors(&map, player.x, player.y); break;
        }

        handle_trap(&player, &map);
    }
}

int show_main_menu() {
    clear();
    printw("=== Main Menu ===\n");
    printw("1. Create New User\n");
    printw("2. Login\n");
    printw("3. Start New Game\n");
    printw("4. Continue Saved Game\n");
    printw("5. Leaderboard\n");
    printw("6. Settings\n");
    printw("7. Exit\n");
    printw("Enter your choice: ");
    refresh();
    return getch() - '0';
}

void show_leaderboard(const User *users, int user_count) {
    clear();
    printw("=== Leaderboard ===\n");

    User sorted_users[MAX_USERS];
    memcpy(sorted_users, users, user_count * sizeof(User));
    for (int i = 0; i < user_count - 1; i++) {
        for (int j = i + 1; j < user_count; j++) {
            if (sorted_users[i].score < sorted_users[j].score) {
                User temp = sorted_users[i];
                sorted_users[i] = sorted_users[j];
                sorted_users[j] = temp;
            }
        }
    }

    for (int i = 0; i < user_count && i < 10; i++) {
        printw("%d. %s - Score: %d\n", i + 1, sorted_users[i].username, sorted_users[i].score);
    }

    refresh();
    getch();
}

void show_settings() {
    clear();
    printw("=== Settings ===\n");
    printw("1. Change Character Color\n");
    printw("2. Change Music\n");
    printw("3. Back\n");
    refresh();

    int choice = getch() - '0';
    if (choice == 1) {
        printw("Character color changed!\n");
    } else if (choice == 2) {
        printw("Music changed!\n");
    }
    refresh();
    getch();
}

void continue_saved_game(User *user) {
    clear();
    printw("Continue saved game for %s\n", user->username);

    Player player;
    GameMap map;
    load_game(&player, &map);

    if (player.health > 0) {
        printw("Game for user %s continues from the last saved point.\n", user->username);
        printw("Current floor: %d\n", player.current_floor);
        printw("Health: %d\n", player.health);
        printw("Gold: %d\n", player.gold);
    } else {
        printw("No saved game found!\n");
    }

    refresh();
    getch();
}