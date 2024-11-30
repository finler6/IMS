#ifndef MESSAGES_H
#define MESSAGES_H

#include "config.h"
typedef struct {
    int id;              // Уникальный идентификатор сообщения
    int sender_id;       // ID отправителя
    int receiver_id;     // ID получателя
    char content[256];   // Текст сообщения
    int timestamp;       // Временная метка (для упрощения используем int)
} Message;

// Функции
void send_message(Message messages[], int *message_count, int sender_id, int receiver_id, const char *content, int timestamp);
void print_messages_between_users(Message messages[], int message_count, int user1_id, int user2_id);
void print_messages_in_time_range(Message messages[], int message_count, int start_time, int end_time);

#endif
