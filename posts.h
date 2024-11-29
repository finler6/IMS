#ifndef POSTS_H
#define POSTS_H

typedef struct {
    int id;             // Уникальный идентификатор поста
    int author_id;      // ID автора поста
    char content[512];  // Текст поста
    int likes;          // Количество лайков
    int comments;       // Количество комментариев
} Post;

// Функции
void create_post(Post posts[], int *post_count, int author_id, const char *content);
Post* find_post_by_id(Post posts[], int post_count, int id);
void like_post(Post *post);
void comment_post(Post *post);
void print_posts(Post posts[], int post_count);

#endif
