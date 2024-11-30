#include "posts.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Функция для ретвита поста
void retweet_post(Post posts[], int *post_count, User users[], int user_count, int retweeter_id, const Post *original_post) {
    if (!original_post) return; // Если оригинального поста нет

    Post new_post = { *post_count, retweeter_id, "", 0, 0, 1, original_post->id, virtual_time };
    snprintf(new_post.content, sizeof(new_post.content), "RT: %.500s", original_post->content);
    posts[*post_count] = new_post;
    (*post_count)++;

    // Увеличиваем счетчик постов пользователя
    User *retweeter = find_user_by_id(users, user_count, retweeter_id);
    if (retweeter) {
        retweeter->posts_count++;
    }
}

// Функция для создания нового поста
void create_post(Post posts[], int *post_count, User users[], int user_count, int author_id, const char *content) {
    if (*post_count >= MAX_POSTS) {
        printf("Ошибка: превышено максимальное количество постов!\n");
        return;
    }
    Post new_post = { *post_count, author_id, "", 0, 0, 0, -1, virtual_time };
    strncpy(new_post.content, content, sizeof(new_post.content) - 1);
    posts[*post_count] = new_post;
    (*post_count)++;

    // Увеличиваем счетчик постов пользователя
    User *author = find_user_by_id(users, user_count, author_id);
    if (author) {
        author->posts_count++;
    }
}

// Функция для удаления поста
void delete_post(Post posts[], int *post_count, User users[], int user_count, int id) {
    for (int i = 0; i < *post_count; i++) {
        if (posts[i].id == id) {
            // Уменьшаем счетчик постов автора
            User *author = find_user_by_id(users, user_count, posts[i].author_id);
            if (author) {
                author->posts_count--;
            }

            // Сдвигаем оставшиеся посты
            for (int j = i; j < *post_count - 1; j++) {
                posts[j] = posts[j + 1];
            }
            (*post_count)--;
            printf("Post s ID %d byl odstraněn.\n", id);
            return;
        }
    }
    printf("Post s ID %d nebyl nalezen.\n", id);
}

// Функция для поиска поста по ID
Post* find_post_by_id(Post posts[], int post_count, int id) {
    for (int i = 0; i < post_count; i++) {
        if (posts[i].id == id) {
            return &posts[i];
        }
    }
    return NULL;
}

// Функция для добавления лайка к посту
void like_post(Post *post) {
    if (post) {
        post->likes++;
    }
}

// Функция для добавления комментария к посту
void comment_post(Post *post) {
    if (post) {
        post->comments++;
    }
}

// Функция для вывода всех постов
void print_posts(Post posts[], int post_count) {
    printf("+-------------------------------------------------------------------------------------------+\n");
    printf("| ID příspěvku | Autor   | Obsah                          | Lajky | Komentáře | Retweet |\n");
    printf("+-------------------------------------------------------------------------------------------+\n");
    for (int i = 0; i < post_count; i++) {
        printf("| %-12d | %-7d | %-30s | %-5d | %-10d | %-7s |\n",
               posts[i].id, posts[i].author_id, posts[i].content, posts[i].likes, posts[i].comments,
               posts[i].is_retweet ? "Ano" : "Ne");
    }
    printf("+-------------------------------------------------------------------------------------------+\n");
}

// Функция для подсчета постов за определенный временной интервал
int count_posts_in_time_range(Post posts[], int post_count, int user_id, int start_time, int end_time) {
    int count = 0;
    for (int i = 0; i < post_count; i++) {
        if (posts[i].author_id == user_id && posts[i].timestamp >= start_time && posts[i].timestamp <= end_time) {
            count++;
        }
    }
    return count;
}

// Функция для определения, насколько часто пользователь публикует посты
int is_user_frequent_poster(Post posts[], int post_count, int user_id, int time_window, int post_threshold) {
    time_t current_time = time(NULL);
    int recent_posts = count_posts_in_time_range(posts, post_count, user_id, current_time - time_window, current_time);
    return recent_posts >= post_threshold;
}

// Функция для вывода списка пользователей, которые часто публикуют посты
void print_frequent_posters(Post posts[], int post_count, User users[], int user_count, int time_window, int post_threshold) {
    printf("Пользователи, которые публикуют более %d постов за последние %d секунд:\n", post_threshold, time_window);
    printf("+----------------------------------------+\n");
    printf("| ID  | Jméno               | Пříspěвky |\n");
    printf("+----------------------------------------+\n");
    for (int i = 0; i < user_count; i++) {
        if (is_user_frequent_poster(posts, post_count, users[i].id, time_window, post_threshold)) {
            printf("| %-3d | %-19s | %-9d |\n",
                   users[i].id, users[i].name, users[i].posts_count);
        }
    }
    printf("+----------------------------------------+\n");
}
