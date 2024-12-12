#include <stdio.h>
#include <stdlib.h>
#include "library.h"


// ANSI color codes
#define RESET "\033[0m"      // Reset to default color
#define RED "\033[31m"       // Red color
#define ORANGE "\033[38;5;214m"  // Orange color
#define GREEN "\033[32m"     // Green color
#define YELLOW "\033[33m"    // Yellow color
#define CYAN "\033[36m"      // Cyan color


void InitQueue(Queue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void enqueue(Queue* queue, User user) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        printf(RED"   Memory allocation failed!\n"RESET);
        return;
    }
    newNode->user = user;
    newNode->next = NULL;
    if (queue->tail) {
        queue->tail->next = newNode;
    }
    queue->tail = newNode;
    if (!queue->head) {
        queue->head = newNode;
    }
}

User dequeue(Queue* queue) {
    if (!queue->head) {
        printf(RED"   Queue is empty!\n"RESET);
        User emptyUser = {0, "", 0};
        return emptyUser;
    }
    QueueNode* temp = queue->head;
    User user = temp->user;
    queue->head = temp->next;
    if (!queue->head) {
        queue->tail = NULL;
    }
    free(temp);
    return user;
}

void displayQueue(Queue* queue) {
    QueueNode* current = queue->head;
    if (!current) {
        printf("   No users in the borrow request queue.\n");
        return;
    }
    printf(CYAN" Borrow Request Queue:\n"RESET);
    while (current) {
        printf(GREEN"   User ID: %d,\t Name: %s,\t Requested Book ID: %d\n"RESET, 
                current->user.userID, current->user.name, current->user.requestedBookID);
        current = current->next;
    }
}