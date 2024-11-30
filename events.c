#include "events.h"
#include "users.h"
#include "posts.h"
#include "messages.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MORNING_PEAK_START 8 * 60     // Утро: 8:00
#define MORNING_PEAK_END 11 * 60      // Утро: 11:00
#define EVENING_PEAK_START 19 * 60    // Вечер: 19:00
#define EVENING_PEAK_END 22 * 60      // Вечер: 22:00
#define NIGHT_START 0 * 60            // Ночь: 00:00
#define NIGHT_END 6 * 60              // Ночь: 06:00


void generate_virtual_events(Event events[], int *event_count, int user_count, int max_posts) {
    srand(time(NULL)); // Инициализация генератора случайных чисел
    int current_time = 0; // Начало дня в минутах с 00:00

    while (current_time < 24 * 60) { // Пока не пройден один виртуальный день
        int user_id = rand() % user_count;
        EventType type = rand() % 6; // Случайный тип события
        int target_id = 0;
        char content[256] = "";

        // Определение активности в зависимости от времени суток
        double activity_multiplier = 1.0;
        if (current_time >= MORNING_PEAK_START && current_time <= MORNING_PEAK_END) {
            activity_multiplier = 1.5;
        } else if (current_time >= EVENING_PEAK_START && current_time <= EVENING_PEAK_END) {
            activity_multiplier = 2.0;
        } else if (current_time >= NIGHT_START && current_time <= NIGHT_END) {
            activity_multiplier = 0.5;
        }

        double event_probability = ((double)rand() / RAND_MAX) * activity_multiplier;

        // Генерация события с учетом вероятностей
        if (event_probability < 0.3 && type == CREATE_POST) {
            snprintf(content, sizeof(content), "Virtual post by user %d", user_id);
            add_event(events, event_count, CREATE_POST, user_id, 0, content, current_time);
        } else if (event_probability < 0.5 && type == SEND_MESSAGE) {
            int receiver_id = rand() % user_count;
            while (receiver_id == user_id) {
                receiver_id = rand() % user_count;
            }
            snprintf(content, sizeof(content), "Message from %d to %d", user_id, receiver_id);
            add_event(events, event_count, SEND_MESSAGE, user_id, receiver_id, content, current_time);
        } else if (event_probability < 0.7 && type == LIKE_POST && max_posts > 0) {
            target_id = rand() % max_posts;
            add_event(events, event_count, LIKE_POST, user_id, target_id, NULL, current_time);
        } else if (event_probability < 0.85 && type == COMMENT_POST && max_posts > 0) {
            target_id = rand() % max_posts;
            snprintf(content, sizeof(content), "Comment from user %d", user_id);
            add_event(events, event_count, COMMENT_POST, user_id, target_id, content, current_time);
        } else if (event_probability < 0.95 && type == RETWEET && max_posts > 0) {
            target_id = rand() % max_posts;
            add_event(events, event_count, RETWEET, user_id, target_id, NULL, current_time);
        } else if (event_probability <= 1.0 && type == FOLLOW) {
            int target_user_id = rand() % user_count;
            while (target_user_id == user_id) {
                target_user_id = rand() % user_count;
            }
            add_event(events, event_count, FOLLOW, user_id, target_user_id, NULL, current_time);
        }

        // Инкремент времени на случайный интервал
        current_time += rand() % 10 + 1; // Следующее событие через 1-10 минут
    }
}

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
        printf("Processing event: %d (User: %d, Target: %d, Time: %d)\n",
               event.type, event.user_id, event.target_id, event.timestamp);

        switch (event.type) {
            case CREATE_POST:
                create_post(posts, post_count, users, user_count, event.user_id, event.content);
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

            case RETWEET: {
                Post *post = find_post_by_id(posts, *post_count, event.target_id);
                if (post) retweet_post(posts, post_count, users, user_count, event.user_id, post);
                break;
            }

            case FOLLOW: {
                User *user = find_user_by_id(users, user_count, event.user_id);
                User *target_user = find_user_by_id(users, user_count, event.target_id);
                if (user && target_user) follow_user(user, target_user);
                break;
            }
        }
    }
}

void generate_random_events(Event events[], int *event_count, int max_events, int user_count, int max_posts) {
    srand(time(NULL));
    int current_time = 0;

    for (int i = 0; i < max_events; i++) {
        EventType type = rand() % 6;
        int user_id = rand() % user_count;
        int target_id = 0;
        char content[256] = "";

        // Определяем вероятности для разного времени суток
        double activity_multiplier = 1.0;
        if (current_time >= MORNING_PEAK_START && current_time <= MORNING_PEAK_END) {
            activity_multiplier = 1.5; // Утро: повышенная активность
        } else if (current_time >= EVENING_PEAK_START && current_time <= EVENING_PEAK_END) {
            activity_multiplier = 2.0; // Вечер: самая высокая активность
        } else if (current_time >= NIGHT_START && current_time <= NIGHT_END) {
            activity_multiplier = 0.5; // Ночь: минимальная активность
        }

        // Генерация события с учетом времени суток
        double event_probability = ((double)rand() / RAND_MAX) * activity_multiplier;
        if (event_probability < 0.3 && type == CREATE_POST) {
            snprintf(content, sizeof(content), "Post by user %d", user_id);
            add_event(events, event_count, CREATE_POST, user_id, 0, content, current_time);
        } else if (event_probability < 0.5 && type == SEND_MESSAGE) {
            int receiver_id = rand() % user_count;
            while (receiver_id == user_id) {
                receiver_id = rand() % user_count;
            }
            snprintf(content, sizeof(content), "Message from %d to %d", user_id, receiver_id);
            add_event(events, event_count, SEND_MESSAGE, user_id, receiver_id, content, current_time);
        } else if (event_probability < 0.7 && type == LIKE_POST && max_posts > 0) {
            target_id = rand() % max_posts;
            add_event(events, event_count, LIKE_POST, user_id, target_id, NULL, current_time);
        } else if (event_probability < 0.85 && type == COMMENT_POST && max_posts > 0) {
            target_id = rand() % max_posts;
            snprintf(content, sizeof(content), "Comment from user %d", user_id);
            add_event(events, event_count, COMMENT_POST, user_id, target_id, content, current_time);
        } else if (event_probability < 0.95 && type == RETWEET && max_posts > 0) {
            target_id = rand() % max_posts;
            add_event(events, event_count, RETWEET, user_id, target_id, NULL, current_time);
        } else if (event_probability <= 1.0 && type == FOLLOW) {
            int target_user_id = rand() % user_count;
            while (target_user_id == user_id) {
                target_user_id = rand() % user_count;
            }
            add_event(events, event_count, FOLLOW, user_id, target_user_id, NULL, current_time);
        }

        // Переход к следующему времени
        current_time += rand() % 15 + 1; // Следующее событие через 1-15 минут
    }
}

void print_statistics(Event events[], int event_count, User users[], int user_count, Post posts[], int post_count, Message messages[], int message_count) {
    int total_likes = 0, total_comments = 0;
    int total_retweets = 0, total_follows = 0;
    int active_users = 0;
    int event_types[6] = {0}; // Counters for each event type
    (void)messages; // Подавляем предупреждение о неиспользуемом параметре

    // Count likes, comments, and retweets from posts
    for (int i = 0; i < post_count; i++) {
        total_likes += posts[i].likes;
        total_comments += posts[i].comments;
        if (posts[i].is_retweet) {
            total_retweets++;
        }
    }

    // Count total follows from users and active users
    for (int i = 0; i < user_count; i++) {
        if (users[i].posts_count > 0 || users[i].messages_count > 0 || users[i].reactions_count > 0 || users[i].followers_count > 0 || users[i].following_count > 0) {
            active_users++;
        }
        total_follows += users[i].followers_count;
    }

    // Count event types
    for (int i = 0; i < event_count; i++) {
        event_types[events[i].type]++;
    }

    // Output overall statistics
    printf("+--------------------------------------------------+\n");
    printf("| System Statistics                                |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Total events: %-34d |\n", event_count);
    printf("| Number of users: %-31d |\n", user_count);
    printf("| Number of posts: %-31d |\n", post_count);
    printf("| Number of messages: %-28d |\n", message_count);
    printf("| Total likes: %-34d |\n", total_likes);
    printf("| Total comments: %-31d |\n", total_comments);
    printf("| Total retweets: %-31d |\n", total_retweets);
    printf("| Total followers: %-30d |\n", total_follows);
    printf("| Active users: %-33d |\n", active_users);
    printf("+--------------------------------------------------+\n");

    // Output statistics by event types
    printf("+--------------------------------------------------+\n");
    printf("| Event Types                                      |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Create posts: %-33d |\n", event_types[CREATE_POST]);
    printf("| Send messages: %-31d |\n", event_types[SEND_MESSAGE]);
    printf("| Likes: %-39d |\n", event_types[LIKE_POST]);
    printf("| Comments: %-36d |\n", event_types[COMMENT_POST]);
    printf("| Retweets: %-36d |\n", event_types[RETWEET]);
    printf("| Follows: %-37d |\n", event_types[FOLLOW]);
    printf("+--------------------------------------------------+\n");
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
    printf("| Time Statistics                               |\n");
    printf("+-----------------------------------------------+\n");
    printf("| Minimum time: %d                              |\n", min_time);
    printf("| Maximum time: %d                              |\n", max_time);
    printf("| Average time: %.2f                           |\n", (double)total_time / event_count);
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
    printf("| Time Interval Histogram               |\n");
    printf("+---------------------------------------+\n");
    printf("| Interval  | Number of events          |\n");
    printf("+---------------------------------------+\n");
    for (int i = 0; i < bins; i++) {
        printf("| %3d - %-3d | %-25d |\n", i * bin_size, (i + 1) * bin_size - 1, histogram[i]);
    }
    printf("+---------------------------------------+\n");
}
