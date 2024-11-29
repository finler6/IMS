#include "users.h"
#include <stdio.h>
#include <string.h>

void add_user(User users[], int *user_count, const char *name) {
    User new_user = { *user_count, "", 0, 0, 0 };
    strcpy(new_user.name, name);
    users[*user_count] = new_user;
    (*user_count)++;
}

User* find_user_by_id(User users[], int user_count, int id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == id) {
            return &users[i];
        }
    }
    return NULL;
}

void print_users(User users[], int user_count) {
    printf("+------------------------------------------------------------+\n");
    printf("| ID  | Jméno               | Příspěvky | Zprávy | Reakce    |\n");
    printf("+------------------------------------------------------------+\n");
    for (int i = 0; i < user_count; i++) {
        printf("| %-3d | %-19s | %-9d | %-6d | %-9d |\n",
               users[i].id, users[i].name, users[i].posts_count,
               users[i].messages_count, users[i].reactions_count);
    }
    printf("+------------------------------------------------------------+\n");
}
