#include "menu.h"
#include "save.h"
#include "player.h"
#include "map.h"
#include <ctype.h>
#include <string.h>

void handle_login(User *users, int user_count, User *current_user) {
    clear();
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("        LOGIN           \n");
    attroff(COLOR_PAIR(3) | A_BOLD);

    char username[50], password[50];
    attron(COLOR_PAIR(1));
    printw(" Username: ");
    echo();
    getstr(username);
    noecho();

    printw(" Password: ");
    getstr(password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *current_user = users[i];
            attron(COLOR_PAIR(6));
            printw("Login successful!\n");
            attroff(COLOR_PAIR(6));
            refresh();
            getch();
            return;
        }
    }

    attron(COLOR_PAIR(4));
    printw("Username or password is wrong!\n");
    attroff(COLOR_PAIR(4));
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
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("     CREATE USER        \n");
    attroff(COLOR_PAIR(3) | A_BOLD);

    User new_user = {0};
    attron(COLOR_PAIR(1));
    printw(" Username: ");
    echo();
    getstr(new_user.username);
    noecho();

    for (int i = 0; i < *user_count; i++) {
        if (strcmp(users[i].username, new_user.username) == 0) {
            attron(COLOR_PAIR(4));
            printw("Username already exists!\n");
            attroff(COLOR_PAIR(4));
            refresh();
            getch();
            return;
        }
    }

    do {
        printw(" Password (min 7 chars, 1 uppercase, 1 lowercase, 1 digit): ");
        getstr(new_user.password);
    } while (!is_valid_password(new_user.password));

    do {
        printw(" Email: ");
        echo();
        getstr(new_user.email);
        noecho();
    } while (!is_valid_email(new_user.email));

    users[(*user_count)++] = new_user;
    save_users(users, *user_count);
    attron(COLOR_PAIR(6));
    printw("User created successfully!\n");
    attroff(COLOR_PAIR(6));
    refresh();
    getch();
}

void start_new_game(User *user) {
    clear();
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("      NEW GAME          \n");
    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(1));
    printw(" User: %s\n", user->username);
    refresh();
    getch();

    Player player;
    GameMap map;
    init_player(&player);
    generate_map(&map, 1);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map.tiles[y][x] == '.') {
                player.x = x;
                player.y = y;
                break;
            }
        }
        if (player.x != MAP_WIDTH / 2 || player.y != MAP_HEIGHT / 2) {
            break;
        }
    }

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
    int row, col;
    getmaxyx(stdscr, row, col);

    bkgd(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    for (int i = 0; i < col; i++) {
        mvaddch(row / 2 - 7, i, '=');
        mvaddch(row / 2 + 8, i, '=');
    }
    for (int i = row / 2 - 7; i <= row / 2 + 8; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, col - 1, '|');
    }
    attroff(COLOR_PAIR(2));

    // عنوان منو
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(row / 2 - 4, (col - strlen("      MAIN MENU         ")) / 2, "      MAIN MENU         ");
    attroff(COLOR_PAIR(3) | A_BOLD);

    // گزینه‌های منو
    attron(COLOR_PAIR(1));
    mvprintw(row / 2 - 2, (col - strlen("1. Create New User")) / 2, "1. Create New User");
    mvprintw(row / 2 - 1, (col - strlen("2. Login")) / 2, "2. Login");
    mvprintw(row / 2, (col - strlen("3. Start New Game")) / 2, "3. Start New Game");
    mvprintw(row / 2 + 1, (col - strlen("4. Continue Saved Game")) / 2, "4. Continue Saved Game");
    mvprintw(row / 2 + 2, (col - strlen("5. Leaderboard")) / 2, "5. Leaderboard");
    mvprintw(row / 2 + 3, (col - strlen("6. Settings")) / 2, "6. Settings");
    mvprintw(row / 2 + 4, (col - strlen("7. Exit")) / 2, "7. Exit");
    attroff(COLOR_PAIR(1));

    attron(A_BOLD);
    mvprintw(row / 2 + 6, (col - strlen("Enter your choice: ")) / 2, "Enter your choice: ");
    attroff(A_BOLD);

    refresh();
    return getch() - '0';
}

void show_leaderboard(const User *users, int user_count) {
    clear();
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("        LEADERBOARD          \n");
    printw("   PLAYER         SCORE      \n");
    
    for (int i = 0; i < user_count && i < 10; i++) {
        if (i == 0) {
            printw(" %-12s  %-13d  (GOAT)\n", users[i].username, users[i].score);
        } else {
            printw(" %-12s  %-13d \n", users[i].username, users[i].score);
        }
    }
    
    attroff(COLOR_PAIR(3) | A_BOLD);
    refresh();
    getch();
}

void show_settings() {
    clear();
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("       SETTINGS         \n");
    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(1));
    printw(" 1. Change Character Color\n");
    printw(" 2. Change Music\n");
    printw(" 3. Back\n\n");
    
    attron(COLOR_PAIR(5));
    printw(" Select: ");
    attroff(COLOR_PAIR(5));
    refresh();
    int choice = getch() - '0';
    if (choice == 1) {
        attron(COLOR_PAIR(6));
        printw("Character color changed!\n");
        attroff(COLOR_PAIR(6));
    } else if (choice == 2) {
        attron(COLOR_PAIR(6));
        printw("Music changed!\n");
        attroff(COLOR_PAIR(6));
    }
    refresh();
    getch();
}

void continue_saved_game(User *user) {
    clear();
    attron(COLOR_PAIR(3) | A_BOLD);
    printw("   CONTINUE GAME        \n");
    attroff(COLOR_PAIR(3) | A_BOLD);

    Player player;
    GameMap map;
    load_game(&player, &map);

    if (player.health > 0) {
        attron(COLOR_PAIR(1));
        printw(" Game for user %s continues from the last saved point.\n", user->username);
        printw(" Current floor: %d\n", player.current_floor);
        printw(" Health: %d\n", player.health);
        printw(" Gold: %d\n", player.gold);
        attroff(COLOR_PAIR(1));
    } else {
        attron(COLOR_PAIR(4));
        printw("No saved game found!\n");
        attroff(COLOR_PAIR(4));
    }

    refresh();
    getch();
}
