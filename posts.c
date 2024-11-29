#include "posts.h"
#include <stdio.h>
#include <string.h>

void create_post(Post posts[], int *post_count, int author_id, const char *content) {
    Post new_post = { *post_count, author_id, "", 0, 0 };
    strncpy(new_post.content, content, sizeof(new_post.content) - 1);
    posts[*post_count] = new_post;
    (*post_count)++;
}

Post* find_post_by_id(Post posts[], int post_count, int id) {
    for (int i = 0; i < post_count; i++) {
        if (posts[i].id == id) {
            return &posts[i];
        }
    }
    return NULL;
}

void like_post(Post *post) {
    if (post) {
        post->likes++;
    }
}

void comment_post(Post *post) {
    if (post) {
        post->comments++;
    }
}

void print_posts(Post posts[], int post_count) {
    printf("+--------------------------------------------------------------------------+\n");
    printf("| ID příspěvku | Autor   | Obsah                          | Lajky | Komentáře |\n");
    printf("+--------------------------------------------------------------------------+\n");
    for (int i = 0; i < post_count; i++) {
        printf("| %-12d | %-7d | %-30s | %-5d | %-10d |\n",
               posts[i].id, posts[i].author_id, posts[i].content, posts[i].likes, posts[i].comments);
    }
    printf("+--------------------------------------------------------------------------+\n");
}
