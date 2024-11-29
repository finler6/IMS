#include "messages.h"
#include <stdio.h>
#include <string.h>

void send_message(Message messages[], int *message_count, int sender_id, int receiver_id, const char *content, int timestamp) {
    Message new_message = { *message_count, sender_id, receiver_id, "", timestamp };
    strncpy(new_message.content, content, sizeof(new_message.content) - 1);
    messages[*message_count] = new_message;
    (*message_count)++;
}

void print_messages_between_users(Message messages[], int message_count, int user1_id, int user2_id) {
    printf("+------------------------------------------------------------+\n");
    printf("| ID zprávy | Odesílatel | Příjemce | Obsah                  |\n");
    printf("+------------------------------------------------------------+\n");
    for (int i = 0; i < message_count; i++) {
        if ((messages[i].sender_id == user1_id && messages[i].receiver_id == user2_id) ||
            (messages[i].sender_id == user2_id && messages[i].receiver_id == user1_id)) {
            printf("| %-9d | %-10d | %-8d | %-23s |\n",
                   messages[i].id, messages[i].sender_id, messages[i].receiver_id, messages[i].content);
            }
    }
    printf("+------------------------------------------------------------+\n");
}
