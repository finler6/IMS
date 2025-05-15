#include "users.h"
#include "posts.h" // Для работы с постами
#include <stdio.h>
#include <string.h>
#include <time.h>

void add_user(User users[], int *user_count, const char *name) {
    User new_user = { *user_count, "", 0, 0, 0, {0}, {0}, 0, 0 };
    strncpy(new_user.name, name, sizeof(new_user.name) - 1);
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

void follow_user(User *user, User *target_user) {
    if (user->following_count >= MAX_USERS) {
        printf("Ошибка: превышено максимальное количество подписок!\n");
        return;
    }
    if (target_user->followers_count >= MAX_USERS) {
        printf("Ошибка: превышено максимальное количество подписчиков!\n");
        return;
    }

    for (int i = 0; i < user->following_count; i++) {
        if (user->following[i] == target_user->id) {
            printf("Пользователь уже подписан на данного пользователя.\n");
            return;
        }
    }

    user->following[user->following_count++] = target_user->id;
    target_user->followers[target_user->followers_count++] = user->id;
}

void unfollow_user(User *user, User *target_user) {
    int found = 0;

    for (int i = 0; i < user->following_count; i++) {
        if (user->following[i] == target_user->id) {
            for (int j = i; j < user->following_count - 1; j++) {
                user->following[j] = user->following[j + 1];
            }
            user->following_count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Ошибка: пользователь не подписан на данного пользователя.\n");
        return;
    }

    found = 0;
    for (int i = 0; i < target_user->followers_count; i++) {
        if (target_user->followers[i] == user->id) {
            for (int j = i; j < target_user->followers_count - 1; j++) {
                target_user->followers[j] = target_user->followers[j + 1];
            }
            target_user->followers_count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Ошибка: у пользователя нет такого подписчика.\n");
    }
}

void print_following(const User *user) {
    printf("Following (%d): ", user->following_count);
    for (int i = 0; i < user->following_count; i++) {
        printf("%d ", user->following[i]);
    }
    printf("\n");
}

void print_followers(const User *user) {
    printf("Followers (%d): ", user->followers_count);
    for (int i = 0; i < user->followers_count; i++) {
        printf("%d ", user->followers[i]);
    }
    printf("\n");
}

void print_users(User users[], int user_count) {
    printf("+------------------------------------------------------------------------+\n");
    printf("| ID  | Jméno               | Příspěvky | Зprávy | Reakce    | Sledující  | Sleduje  |\n");
    printf("+------------------------------------------------------------------------+\n");
    for (int i = 0; i < user_count; i++) {
        printf("| %-3d | %-19s | %-9d | %-6d | %-9d | %-12d | %-9d |\n",
               users[i].id, users[i].name, users[i].posts_count,
               users[i].messages_count, users[i].reactions_count,
               users[i].followers_count, users[i].following_count);
    }
    printf("+------------------------------------------------------------------------+\n");
}

int count_recent_posts(Post posts[], int post_count, int user_id, int current_time, int time_window) {
    int recent_posts = 0;
    for (int i = 0; i < post_count; i++) {
        if (posts[i].author_id == user_id && (current_time - posts[i].timestamp) <= time_window) {
            recent_posts++;
        }
    }
    return recent_posts;
}

int is_user_recently_active(Post posts[], int post_count, int user_id, int current_time, int time_window, int post_threshold) {
    int recent_posts = count_recent_posts(posts, post_count, user_id, current_time, time_window);
    return recent_posts >= post_threshold;
}

void print_recently_active_users(User users[], int user_count, Post posts[], int post_count, int current_time, int time_window, int post_threshold) {
    printf("Недавние активные пользователи (более %d постов за последние %d секунд):\n", post_threshold, time_window);
    printf("+----------------------------------------+\n");
    printf("| ID  | Jméno               | Пříspěвky |\n");
    printf("+----------------------------------------+\n");
    for (int i = 0; i < user_count; i++) {
        if (is_user_recently_active(posts, post_count, users[i].id, current_time, time_window, post_threshold)) {
            printf("| %-3d | %-19s | %-9d |\n",
                   users[i].id, users[i].name, users[i].posts_count);
        }
    }
    printf("+----------------------------------------+\n");
}
