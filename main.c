#include "game.h"
#include "menu.h"
#include "save.h"

User users[MAX_USERS];
int user_count = 0;
User current_user = {0};

int main() {
    init_ncurses();
    load_users(users, &user_count);

    int choice;
    do {
        choice = show_main_menu();
        switch (choice) {
            case 1: handle_new_user(users, &user_count); break;
            case 2: handle_login(users, user_count, &current_user); break;
            case 3: 
                if (current_user.username[0] != '\0') {
                    start_new_game(&current_user);
                } else {
                    attron(COLOR_PAIR(4));
                    printw("Please login first!\n");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    getch();
                }
                break;
            case 4: 
                if (current_user.username[0] != '\0') {
                    continue_saved_game(&current_user);
                } else {
                    attron(COLOR_PAIR(4));
                    printw("Please login first!\n");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    getch();
                }
                break;
            case 5: show_leaderboard(users, user_count); break;
            case 6: show_settings(); break;
            case 7: break;
            default:
                attron(COLOR_PAIR(4));
                printw("Invalid choice!\n");
                attroff(COLOR_PAIR(4));
                refresh();
                getch();
        }
    } while (choice != 7);

    save_users(users, user_count);
    cleanup_ncurses();
    return 0;
}
