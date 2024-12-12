#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdbool.h>


// Book structure
typedef struct {
    int bookID;
    char title[50];
    char author[50];
    bool isAvailable;
} Book;

// User structure
typedef struct {
    int userID;
    char name[50];
    int requestedBookID;
} User;

typedef struct StackNode {
    Book book;
    struct StackNode *next;  
} StackNode;

typedef struct QueueNode {
    User user;
    struct QueueNode *next;  
} QueueNode;

typedef struct NodeInventory {
    Book book;
    struct NodeInventory *next;
} NodeInventory;

// Stack structure
typedef struct {
    StackNode *top;
} Stack;

// Queue structure
typedef struct {
    QueueNode *head;
    QueueNode *tail;
} Queue;

// Inventory structure  
typedef struct {
    NodeInventory *head;
} Inventory;

// Function prototypes
// Stack :
void InitStack(Stack* stack);
void push(Stack* stack, Book book);
Book pop(Stack* stack);
void displayStack(Stack* stack);

// Queue :
void InitQueue(Queue* queue);
void enqueue(Queue* queue, User user);
User dequeue(Queue* queue);
void displayQueue(Queue* queue);

void initInventory(Inventory* inventory);
void addBook(Inventory* inventory, Book book);
void borrowBook(Inventory* inventory, Queue* queue, int bookID, User user);
void returnBook(Stack* stack, Inventory* inventory, int bookID);
void processRequests(Queue* queue, Stack* stack, Inventory* inventory);
void searchBook(Inventory* inventory, int bookID);
void displayInventory(Inventory* inventory);

#endif 