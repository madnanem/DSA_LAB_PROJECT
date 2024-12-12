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


void InitStack(Stack* stack) {
    stack->top = NULL;
}

void push(Stack* stack, Book book) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        printf(RED"   Memory allocation failed!\n"RESET);
        return;
    }
    newNode->book = book;
    newNode->next = stack->top;
    stack->top = newNode;
}

Book pop(Stack* stack) {
    if (stack->top == NULL) {
        printf(RED"   Stack is empty!\n"RESET);
        Book emptyBook = {0, "", "", false};
        return emptyBook;
    }
    StackNode* temp = stack->top;
    Book book = temp->book;
    stack->top = temp->next;
    free(temp);
    return book;
}

void displayStack(Stack* stack) {
    StackNode* current = stack->top;
    if (!current) {
        printf("   No recently returned books.\n");
        return;
    }
    printf(CYAN" Recently Returned Books:\n"RESET);
    while (current) {
        printf(GREEN"   Book ID: %d,\t Title: %s,\t Author: %s\n"RESET, 
                current->book.bookID, current->book.title, current->book.author);
        current = current->next;
    }
}