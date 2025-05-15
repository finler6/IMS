#ifndef EVENTS_H
#define EVENTS_H

#include "users.h"      // Для типа User
#include "posts.h"      // Для типа Post
#include "messages.h"   // Для типа Message
#include "config.h"

typedef enum {
    CREATE_POST,
    SEND_MESSAGE,
    LIKE_POST,
    COMMENT_POST,
    RETWEET, // Новый тип события
    FOLLOW   // Новый тип события
} EventType;


typedef struct {
    EventType type;     // Тип события
    int user_id;        // ID пользователя, который вызвал событие
    int target_id;      // ID целевого объекта (пост/пользователь)
    char content[256];  // Дополнительные данные (например, текст сообщения или комментарий)
    int timestamp;      // Временная метка события
} Event;

// Функции
void add_event(Event events[], int *event_count, EventType type, int user_id, int target_id, const char *content, int timestamp);
void process_events(Event events[], int event_count, User users[], int user_count, Post posts[], int *post_count, Message messages[], int *message_count);
void print_statistics(Event events[], int event_count, User users[], int user_count, Post posts[], int post_count, Message messages[], int message_count);
void generate_random_events(Event events[], int *event_count, int max_events, int user_count, int max_posts);
void generate_virtual_events(Event events[], int *event_count, int user_count, int max_posts);

#endif
