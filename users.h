#ifndef USERS_H
#define USERS_H

#include "config.h"

typedef struct {
    int id;
    char name[50];
    int posts_count;
    int messages_count;
    int reactions_count;
    int following[MAX_USERS]; // Список ID пользователей, на которых подписан
    int followers[MAX_USERS]; // Список ID подписчиков
    int following_count;      // Количество подписок
    int followers_count;      // Количество подписчиков
} User;

// Функции
void add_user(User users[], int *user_count, const char *name);
User* find_user_by_id(User users[], int user_count, int id);
void print_users(User users[], int user_count);
void follow_user(User *user, User *target_user);
void unfollow_user(User *user, User *target_user);
void print_following(const User *user);
void print_followers(const User *user);

#endif
