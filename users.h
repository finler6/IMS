#ifndef USERS_H
#define USERS_H

typedef struct {
    int id;
    char name[50];
    int posts_count;
    int messages_count;
    int reactions_count;
} User;

// Функции
void add_user(User users[], int *user_count, const char *name);
User* find_user_by_id(User users[], int user_count, int id);
void print_users(User users[], int user_count);

#endif
