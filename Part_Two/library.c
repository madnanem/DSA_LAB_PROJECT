#include <stdio.h>
#include <stdlib.h>
#include "Stack.c"
#include "Queue.c"
#include "library.h"
#include <unistd.h> // for sleep() Function


// ANSI color codes
#define RESET "\033[0m"      // Reset to default color
#define RED "\033[31m"       // Red color
#define ORANGE "\033[38;5;214m"  // Orange color
#define GREEN "\033[32m"     // Green color
#define YELLOW "\033[33m"    // Yellow color
#define CYAN "\033[36m"      // Cyan color


void initInventory(Inventory* inventory) {
    inventory->head = NULL;
}

void addBook(Inventory* inventory, Book book) {
    NodeInventory* newNode = (NodeInventory*)malloc(sizeof(NodeInventory));
    if (!newNode) {
        printf(RED"   Memory allocation failed!\n"RESET);
        return;
    }
    newNode->book = book;
    newNode->next = inventory->head;
    inventory->head = newNode;
    printf(YELLOW"  Book added successfully: %s by %s\n"RESET, book.title, book.author);
}

void borrowBook(Inventory* inventory, Queue* queue, int bookID, User user) {
    NodeInventory* current = inventory->head;
    while (current) {
        if (current->book.bookID == bookID) {
            if (current->book.isAvailable) {
                current->book.isAvailable = false;
                printf(YELLOW"  Book borrowed successfully: %s by %s\n"RESET, current->book.title, current->book.author);
            } else {
                printf(RED"   Book is currently unavailable. Adding user to the borrow request queue.\n"RESET);
                user.requestedBookID = bookID;
                enqueue(queue, user);
            }
            return;
        }
        current = current->next;
    }
    printf(RED"   Book with ID %d not found in the inventory.\n"RESET, bookID);
}

void returnBook(Stack* stack, Inventory* inventory, int bookID) {
    NodeInventory* current = inventory->head;
    while (current) {
        if (current->book.bookID == bookID) {
            current->book.isAvailable = true;
            push(stack, current->book);
            printf(YELLOW"  Book returned successfully: %s by %s\n"RESET, current->book.title, current->book.author);
            return;
        }
        current = current->next;
    }
    printf(RED"   Book with ID %d not found in the inventory.\n"RESET, bookID);
}

void processRequests(Queue* queue, Stack* stack, Inventory* inventory) {
    if (!queue->head) {
        printf(RED"   No users in the borrow request queue.\n"RESET);
        return;
    }
    User user = dequeue(queue);
    NodeInventory* current = inventory->head;
    while (current) {
        if (current->book.bookID == user.requestedBookID && current->book.isAvailable) {
            current->book.isAvailable = false;
            printf(CYAN"  Processed request for User: %s (ID: %d). Book borrowed: %s by %s\n"RESET,
                   user.name, user.userID, current->book.title, current->book.author);
            return;
        }
        current = current->next;
    }
    printf(ORANGE"   Requested book (ID: %d) is still unavailable for User: %s (ID: %d).\n"RESET,
           user.requestedBookID, user.name, user.userID);
}

void searchBook(Inventory* inventory, int bookID) {
    NodeInventory* current = inventory->head;
    while (current) {
        if (current->book.bookID == bookID) {
            printf(CYAN"   Book found: %s by %s. %s\n"RESET,
                   current->book.title, current->book.author,
                   current->book.isAvailable ? "Available" : "Currently Borrowed");
            return;
        }
        current = current->next;
    }
    printf(RED"   Book with ID %d not found in the inventory.\n"RESET, bookID);
}

// Display inventory
void displayInventory(Inventory* inventory) {
    NodeInventory* current = inventory->head;
    if (!current) {
        printf(RED"   Inventory is empty.\n"RESET);
        return;
    }
    printf(CYAN" Library Inventory:\n"RESET);
    while (current) {
        printf(GREEN"   Book ID: %d,\t Title: %s,\t Author: %s,\t Status: %s\n"RESET,
               current->book.bookID, current->book.title, current->book.author,
               current->book.isAvailable ? "Available" : "Borrowed");
        current = current->next;
    }
}



int main() {
    Inventory InventoryLib;
    Stack recentlyReturned;
    Queue borrowRequests;

    initInventory(&InventoryLib);
    InitStack(&recentlyReturned);
    InitQueue(&borrowRequests);

    int choice;
    do {
        printf(CYAN " \n--- Library System Menu ---\n" RESET);
        printf("   1. Add New Book\n");
        printf("   2. Borrow Book\n");
        printf("   3. Return Book\n");
        printf("   4. Process Borrow Requests\n"); 
        printf("   5. View Recently Returned Books\n");
        printf("   6. Display Borrow Request Queue\n");
        printf("   7. Search for a Book\n");
        printf("   8. Display Inventory\n");  // hadi zyada just to see the Inventory 
        printf("   0. Exit\n");

        printf("\n "RED"  IMPORTANT:"RESET" Don't foget to exit the program in the end. \n");

        printf("\n   Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Book book;
            printf(" Input Book ID: ");
            scanf("%d", &book.bookID);
            printf(" Input Title: ");
            scanf(" %[^\n]", book.title);
            printf(" Input Author: ");
            scanf(" %[^\n]", book.author);
            book.isAvailable = true;
            addBook(&InventoryLib, book);
        } else if (choice == 2) {
            User user;
            int bookID;
            printf(" Input User ID: ");
            scanf("%d", &user.userID);
            printf(" Input Name: ");
            scanf(" %[^\n]", user.name);
            printf(" Input Requested Book ID: ");
            scanf("%d", &bookID);
            borrowBook(&InventoryLib, &borrowRequests, bookID, user);
        } else if (choice == 3) {
            int bookID;
            printf(" Input Book ID to return : ");
            scanf("%d", &bookID);
            returnBook(&recentlyReturned, &InventoryLib, bookID);
        } else if (choice == 4) {
            processRequests(&borrowRequests, &recentlyReturned, &InventoryLib);
        } else if (choice == 5) {
            displayStack(&recentlyReturned);
        } else if (choice == 6) {
            displayQueue(&borrowRequests);
        } else if (choice == 7) {
            int bookID;
            printf(" Input Book ID to search : ");
            scanf("%d", &bookID);
            searchBook(&InventoryLib, bookID);
        } else if (choice == 8) {
            displayInventory(&InventoryLib);
        } else break;
    } while (choice != 0 );

    printf(GREEN"   THANK YOU ! SEE NEXT TIME \n"RESET);
    sleep(1);
    printf(CYAN"   Exiting Library System... \n"RESET);
    return 0;
}
