#include "events.h"
#include "users.h"
#include "posts.h"
#include "messages.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void add_event(Event events[], int *event_count, EventType type, int user_id, int target_id, const char *content, int timestamp) {
    Event new_event = { type, user_id, target_id, "", timestamp };
    if (content) {
        strncpy(new_event.content, content, sizeof(new_event.content) - 1);
    }
    events[*event_count] = new_event;
    (*event_count)++;
}

void process_events(Event events[], int event_count, User users[], int user_count, Post posts[], int *post_count, Message messages[], int *message_count) {
    for (int i = 0; i < event_count; i++) {
        Event event = events[i];
        printf("Zpracovávám událost: %d (Uživatel: %d, Cíl: %d, Čas: %d)\n",
               event.type, event.user_id, event.target_id, event.timestamp);

        switch (event.type) {
            case CREATE_POST:
                create_post(posts, post_count, event.user_id, event.content);
                break;

            case SEND_MESSAGE:
                send_message(messages, message_count, event.user_id, event.target_id, event.content, event.timestamp);
                break;

            case LIKE_POST: {
                Post *post = find_post_by_id(posts, *post_count, event.target_id);
                if (post) like_post(post);
                break;
            }

            case COMMENT_POST: {
                Post *post = find_post_by_id(posts, *post_count, event.target_id);
                if (post) comment_post(post);
                break;
            }
        }
    }
}

void print_statistics(Event events[], int event_count, User users[], int user_count, Post posts[], int post_count, Message messages[], int message_count) {
    int total_likes = 0, total_comments = 0;

    // Подсчет лайков и комментариев
    for (int i = 0; i < post_count; i++) {
        total_likes += posts[i].likes;
        total_comments += posts[i].comments;
    }

    printf("+--------------------------------------------------+\n");
    printf("| Statistika systému                               |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Celkový počet událostí: %-25d |\n", event_count);
    printf("| Počet uživatelů: %-30d |\n", user_count);
    printf("| Počet příspěvků: %-30d |\n", post_count);
    printf("| Počet zpráv: %-33d |\n", message_count);
    printf("| Celkový počet lajků: %-27d |\n", total_likes);
    printf("| Celkový počet komentářů: %-23d |\n", total_comments);
    printf("+--------------------------------------------------+\n");
}

void generate_random_events(Event events[], int *event_count, int max_events, int user_count, int max_posts) {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    for (int i = 0; i < max_events; i++) {
        EventType type = rand() % 4; // Случайный тип события (0-3)
        int user_id = rand() % user_count;
        int target_id = (type == CREATE_POST) ? 0 : rand() % max_posts;
        char content[256];

        if (type == CREATE_POST) {
            snprintf(content, sizeof(content), "Automatický příspěvek uživatele %d", user_id);
        } else if (type == SEND_MESSAGE) {
            snprintf(content, sizeof(content), "Automatická zpráva od %d k %d", user_id, target_id);
        } else if (type == COMMENT_POST) {
            snprintf(content, sizeof(content), "Komentář uživatele %d", user_id);
        } else {
            content[0] = '\0'; // Пустое содержимое для лайков
        }

        add_event(events, event_count, type, user_id, target_id, content, i + 1);
    }
}

void calculate_event_times(Event events[], int event_count) {
    int min_time = INT_MAX, max_time = 0, total_time = 0;

    for (int i = 0; i < event_count; i++) {
        int time = events[i].timestamp;
        if (time < min_time) min_time = time;
        if (time > max_time) max_time = time;
        total_time += time;
    }

    printf("\n+-----------------------------------------------+\n");
    printf("| Časová statistika                             |\n");
    printf("+-----------------------------------------------+\n");
    printf("| Minimální čas: %d                             |\n", min_time);
    printf("| Maximální čas: %d                             |\n", max_time);
    printf("| Průměrný čas: %.2f                           |\n", (double)total_time / event_count);
    printf("+-----------------------------------------------+\n");
}

void print_time_histogram(Event events[], int event_count, int bin_size) {
    int max_time = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].timestamp > max_time) max_time = events[i].timestamp;
    }

    int bins = (max_time / bin_size) + 1;
    int histogram[bins];
    memset(histogram, 0, sizeof(histogram));

    for (int i = 0; i < event_count; i++) {
        int bin_index = events[i].timestamp / bin_size;
        histogram[bin_index]++;
    }

    printf("\n+---------------------------------------+\n");
    printf("| Histogram časových intervalů          |\n");
    printf("+---------------------------------------+\n");
    printf("| Interval  | Počet událostí            |\n");
    printf("+---------------------------------------+\n");
    for (int i = 0; i < bins; i++) {
        printf("| %3d - %-3d | %-25d |\n", i * bin_size, (i + 1) * bin_size - 1, histogram[i]);
    }
    printf("+---------------------------------------+\n");
}
