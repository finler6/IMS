#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Для usleep
#include "users.h"
#include "posts.h"
#include "messages.h"
#include "events.h"
#include "config.h"
#include <time.h>


// Глобальная переменная для отслеживания виртуального времени
int virtual_time = 0;

// Функция для инициализации пользователей
void initialize_users(User users[], int *user_count, int count) {
    for (int i = 0; i < count; i++) {
        char name[50];
        snprintf(name, sizeof(name), "Uživatel_%d", i);
        add_user(users, user_count, name);
    }
}

// Функция для создания начальных событий
void create_initial_events(Event events[], int *event_count) {
    add_event(events, event_count, CREATE_POST, 0, 0, "Hello, it's my first post!", 1);
    add_event(events, event_count, CREATE_POST, 1, 0, "Today is a good day!", 2);
    add_event(events, event_count, SEND_MESSAGE, 0, 1, "Hi Bob, how are you?", 3);
    add_event(events, event_count, LIKE_POST, 1, 0, NULL, 4);
    add_event(events, event_count, COMMENT_POST, 2, 1, "I agree!", 5);
}

// Функция симуляции одного виртуального дня
void simulate_day(Event events[], int *event_count, User users[], int user_count, Post posts[], int *post_count, Message messages[], int *message_count) {
    printf("Запуск симуляции дня...\n");

    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = (1000000 / TIME_SCALE) * 1000; // Перевод микросекунд в наносекунды

    for (virtual_time = 0; virtual_time < VIRTUAL_DAY_MINUTES; virtual_time++) {
        // Обработка событий текущего времени
        for (int i = 0; i < *event_count; i++) {
            if (events[i].timestamp == virtual_time) {
                process_events(&events[i], 1, users, user_count, posts, post_count, messages, message_count);
            }
        }

        // Ускорение времени
        nanosleep(&sleep_time, NULL); // Используем nanosleep вместо usleep
    }

    printf("Симуляция дня завершена.\n");
}

int main() {
    // Массивы для хранения пользователей, постов, сообщений и событий
    User *users = malloc(MAX_USERS * sizeof(User));
    if (!users) {
        perror("Ошибка выделения памяти для users");
        return 1;
    }

    Post *posts = malloc(MAX_POSTS * sizeof(Post));
    if (!posts) {
        perror("Ошибка выделения памяти для posts");
        free(users); // Освобождаем память перед выходом
        return 1;
    }

    Message *messages = malloc(MAX_MESSAGES * sizeof(Message));
    if (!messages) {
        perror("Ошибка выделения памяти для messages");
        free(users);
        free(posts);
        return 1;
    }

    Event *events = malloc(MAX_EVENTS * sizeof(Event));
    if (!events) {
        perror("Ошибка выделения памяти для events");
        free(users);
        free(posts);
        free(messages);
        return 1;
    }


    int user_count = 0, post_count = 0, message_count = 0, event_count = 0;

    // Инициализация пользователей
    initialize_users(users, &user_count, 10);

    // Динамическое добавление новых пользователей
    printf("\nДинамически добавляем новых пользователей...\n");
    for (int i = 10; i < 15; i++) {
        char name[50];
        snprintf(name, sizeof(name), "Nový uživatel_%d", i);
        add_user(users, &user_count, name);
    }
    printf("Список пользователей после добавления:\n");
    print_users(users, user_count);

    // Создание начальных событий
    create_initial_events(events, &event_count);

    // Обработка начальных событий
    printf("\nОбрабатываем начальные события...\n");
    process_events(events, event_count, users, user_count, posts, &post_count, messages, &message_count);

    // Симуляция виртуального дня
    printf("\nСимулируем один день...\n");
    generate_virtual_events(events, &event_count, user_count, post_count);
    simulate_day(events, &event_count, users, user_count, posts, &post_count, messages, &message_count);

    // Анализ активности пользователей
    printf("\nАнализируем активность пользователей...\n");
    int time_window = 1440; // Один виртуальный день
    int post_threshold = 5; // Минимум 5 постов
    print_frequent_posters(posts, post_count, users, user_count, time_window, post_threshold);

    // Вывод статистики
    printf("\nИтоговая статистика системы:\n");
    print_statistics(events, event_count, users, user_count, posts, post_count, messages, message_count);

    return 0;
}
