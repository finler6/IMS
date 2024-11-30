#ifndef POSTS_H
#define POSTS_H

#include "users.h"
#include "config.h"

typedef struct {
    int id;               // ID příspěvku
    int author_id;        // ID autora
    char content[512];    // Obsah příspěvku
    int likes;            // Počet lajků
    int comments;         // Počet komentářů
    int is_retweet;       // Indikace, zda je příspěvek retweet
    int original_post_id; // ID původního příspěvku (pro retweet)
    int timestamp;        // Временная метка публикации
} Post;


extern int virtual_time;
// Основные функции
void retweet_post(Post posts[], int *post_count, User users[], int user_count, int retweeter_id, const Post *original_post);
void create_post(Post posts[], int *post_count, User users[], int user_count, int author_id, const char *content);
void delete_post(Post posts[], int *post_count, User users[], int user_count, int id);
Post* find_post_by_id(Post posts[], int post_count, int id);
void like_post(Post *post);
void comment_post(Post *post);
void print_posts(Post posts[], int post_count);

// Временной анализ
int count_posts_in_time_range(Post posts[], int post_count, int user_id, int start_time, int end_time);
int is_user_frequent_poster(Post posts[], int post_count, int user_id, int time_window, int post_threshold);
void print_frequent_posters(Post posts[], int post_count, User users[], int user_count, int time_window, int post_threshold);

#endif // POSTS_H
