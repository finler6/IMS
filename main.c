#include <stdio.h>
#include "users.h"
#include "posts.h"
#include "messages.h"
#include "events.h"

#define MAX_USERS 100
#define MAX_POSTS 100
#define MAX_MESSAGES 100
#define MAX_EVENTS 100

int main() {
    User users[MAX_USERS];
    Post posts[MAX_POSTS];
    Message messages[MAX_MESSAGES];
    Event events[MAX_EVENTS];
    int user_count = 0, post_count = 0, message_count = 0, event_count = 0;

    for (int i = 0; i < 10; i++) {
        char name[50];
        snprintf(name, sizeof(name), "Uživatel_%d", i);
        add_user(users, &user_count, name);
    }

    // Создаем события
    add_event(events, &event_count, CREATE_POST, 0, 0, "Hello, it's my first post!", 1);
    add_event(events, &event_count, CREATE_POST, 1, 0, "Today is a good day!", 2);
    add_event(events, &event_count, SEND_MESSAGE, 0, 1, "Hi Bob, how are you?", 3);
    add_event(events, &event_count, LIKE_POST, 1, 0, NULL, 4);
    add_event(events, &event_count, COMMENT_POST, 2, 1, "I agree!", 5);

    // Обрабатываем события
    printf("Start processing events...\n");
    process_events(events, event_count, users, user_count, posts, &post_count, messages, &message_count);
    calculate_event_times(events, event_count);

    // Выводим результаты
    printf("\nList of users:\n");
    print_users(users, user_count);

    printf("\nList of posts:\n");
    print_posts(posts, post_count);

    printf("\nList of messages between users %s and %s:\n", users[0].name, users[1].name);
    print_messages_between_users(messages, message_count, 0, 1);

    printf("\nGenerating random events...\n");
    generate_random_events(events, &event_count, 10, user_count, post_count);

    // Обработка событий
    printf("\nStart processing events...\n");
    process_events(events, event_count, users, user_count, posts, &post_count, messages, &message_count);

    print_time_histogram(events, event_count, 5); // Группировка по 5 единиц времени

    print_statistics(events, event_count, users, user_count, posts, post_count, messages, message_count);

    return 0;
}
