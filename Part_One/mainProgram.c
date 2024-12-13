#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep() Function


// ANSI color codes
#define RESET "\033[0m"      // Reset to default color
#define RED "\033[31m"       // Red color
#define ORANGE "\033[38;5;214m"  // Orange color
#define GREEN "\033[32m"     // Green color
#define YELLOW "\033[33m"    // Yellow color
#define CYAN "\033[36m"      // Cyan color 

// Task's structure 
typedef struct Task {
    int id;                          
    char description[100];           
    int priority;                   // priority (1-5)
    char status;                    // status (P: Pending, I: In Progress, C: Completed)
    struct Task* next;      
} Task;


// Function to create a new task
Task* createTask(int id, const char* description, int priority, char status){
    Task* NewTask = (Task*)malloc(sizeof(Task));
    if(!NewTask){
        printf(RED "   Memory Allocation failed.\n" RESET);
        exit(1);
    }
    NewTask->id = id;
    strcpy(NewTask->description, description);
    NewTask->priority = priority;
    NewTask->status = status;
    NewTask->next = NULL;

    return NewTask;
}

void insertTask(Task** task, int id, const char* description, int priority, char status){
    if( status != 'P' && status != 'I' && status != 'C' &&
        status != 'p' && status != 'i' && status != 'c'){
        printf(RED "   Invalid status entered. USE JUST : P, I, or C.\n" RESET);
        return;
    }

    Task* temp = *task;
    while (temp != NULL) {
        if (temp->id == id) {
            printf(RED "   Task with ID %d already exists. Please use ANOTHER ID (Unique).\n" RESET);
            return;
        }
        temp = temp->next;
    }

    Task* TaskInserted = createTask(id, description, priority, status);

    if(*task == NULL || (*task)->priority >= priority){  
        TaskInserted->next = *task;
        *task = TaskInserted;
        return;
    }

    Task* current = *task;
    while(current->next != NULL && current->next->priority <= priority){ 
        current = current->next;
    }
    TaskInserted->next = current->next;
    current->next = TaskInserted;
}

void deleteTask(Task** task, int id){
    if (*task == NULL){
        printf(RED "   The list is empty.\n" RESET);
        return;
    }

    Task* temp = *task;
    if (temp->id == id) {
        *task = temp->next;
        free(temp);
        printf(YELLOW "  Task with ID %d deleted successfully.\n" RESET, id);
        return;
    }

    Task* prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf(RED "   Task with ID %d not found.\n" RESET, id);
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf(YELLOW "  Task with ID %d deleted successfully.\n" RESET, id);
}

void updateStatus(Task* task, int id, char newStatus){
    if( newStatus != 'P' && newStatus != 'I' && newStatus != 'C' &&
        newStatus != 'p' && newStatus != 'i' && newStatus != 'c'){
        printf(RED "   Invalid status entered. USE JUST :  P, I, or C.\n" RESET);
        return;
    }

    Task* current = task;
    while (current != NULL){
        if (current->id == id){
            current->status = newStatus;
            printf(YELLOW "  Task with ID %d updated successfully.\n" RESET, id);
            return;
        }
        current = current->next;
    }
    printf(RED "   Task with ID %d not found.\n" RESET, id);
}

void displayTasks(Task* task){
    Task* current;

    // Pending tasks
    printf(CYAN "\nPending tasks:\n" RESET);
    current = task;
    int Pending = 0;
    while(current != NULL){
        if (current->status == 'P' || current->status == 'p'){
            printf(ORANGE "   ID: %d,\t Desc: %s,\t Priority: %d,\t Status: %c\n" RESET,
                   current->id, current->description, current->priority, current->status);
            Pending = 1;
        }
        current = current->next;
    }
    if(!Pending){
        printf(RED "   No Pending tasks found.\n" RESET);
    }

    // In Progress tasks
    printf(CYAN "\nIn Progress tasks:\n" RESET);
    current = task;
    int InProgress = 0;
    while(current != NULL){
        if (current->status == 'I' || current->status == 'i'){
            printf(YELLOW "   ID: %d,\t Desc: %s,\t Priority: %d,\t Status: %c\n" RESET,
                   current->id, current->description, current->priority, current->status);
            InProgress = 1;
        }
        current = current->next;
    }
    if(!InProgress){
        printf(RED "   No In Progress tasks found.\n" RESET);
    }

    // Completed tasks
    printf(CYAN "\nCompleted tasks:\n" RESET);
    current = task;
    int Completed = 0;
    while(current != NULL){
        if (current->status == 'C' || current->status == 'c'){
            printf(GREEN "   ID: %d,\t Desc: %s,\t Priority: %d,\t Status: %c\n" RESET,
                   current->id, current->description, current->priority, current->status);
            Completed = 1;
        }
        current = current->next;
    }
    if(!Completed){
        printf(RED "   No Completed tasks found.\n" RESET);
    }
}

void searchByPriority(Task* task, int priority){
    if (priority < 1 || priority > 5){
        printf(RED "   Priority must be between 1 and 5.\n" RESET);
        return;
    }

    Task* current = task;
    int found = 0;
    while(current != NULL){
        if (current->priority == priority){
            printf("   ID: "GREEN"%d"RESET",\t Desc: "GREEN"%s"RESET",\t Priority: "GREEN"%d"RESET",\t Status: "GREEN"%c"RESET"\n",
                   current->id, current->description, current->priority, current->status);
            found = 1;
        }
        current = current->next;
    }

    if(!found){
        printf(RED "   No tasks found with priority %d.\n" RESET, priority);
    }
}

void freeTask(Task* task){
    Task* temp;
    while(task != NULL){
        temp = task;
        task = task->next;
        free(temp);
    }
}

 


int main(){
    Task* taskList = NULL;
    int choice, id, priority;
    char description[50], status;

    do {
        printf(CYAN "\n--- Task Management System ---\n" RESET);
        printf("  1. Add a Task\n");
        printf("  2. Delete a Task\n");
        printf("  3. Update Task Status\n");
        printf("  4. Display Tasks\n");
        printf("  5. Search Tasks by Priority\n");
        printf("  0. Exit\n");

        printf("\n "RED"   IMPORTANT:"RESET" Don't foget to exit the program in the end. \n");

        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                printf("Input Task ID: ");
                scanf("%d", &id);
                
                printf("Input Task Description: ");
                scanf(" %[^\n]", description);  

                do {
                    printf("Input Task Priority (1-5): ");
                    scanf("%d", &priority);
                    if (priority < 1 || priority > 5) {
                        printf(RED "   Invalid priority.  Try with a number between 1 and 5.\n" RESET);
                    }
                }while(priority < 1 || priority > 5);

                do {
                    printf("Input task status (" ORANGE "P:" RESET " Pending, " YELLOW "I:" RESET " In Progress, " GREEN "C:" RESET " Completed): ");
                    scanf(" %c", &status);

                    if(status != 'P' && status != 'p' && status != 'I' && status != 'i' && status != 'C' && status != 'c'){
                        printf(RED "  Invalid status . USE JUST :  P, I, or C.\n" RESET);
                    }
                }while(status != 'P' && status != 'p' && status != 'I' && status != 'i' && status != 'C' && status != 'c');

                insertTask(&taskList, id, description, priority, status);
                break;
            case 2:
                printf("Input Task ID to delete: ");
                scanf("%d", &id);
                deleteTask(&taskList, id);
                break;
            case 3:
                printf("Input Task ID to update: ");
                scanf("%d", &id);
                printf("Input new status (P: Pending, I: In Progress, C: Completed): ");
                scanf(" %c", &status);
                updateStatus(taskList, id, status);
                break;
            case 4:
                displayTasks(taskList);
                break;
            case 5:
                printf("Input Priority to search (1-5) : ");
                scanf("%d", &priority);
                searchByPriority(taskList, priority);
                break;
            case 0:
                freeTask(taskList);
                printf(GREEN "   THANK YOU ! \n" RESET);
                sleep(1);
                printf(GREEN "   Exiting the Program... \n"RESET);
                break;
            default:
                printf(RED "   Invalid choice!  Try again (1-6) - 0 to Exit the program.\n" RESET);
            
        }
    }while( choice != 0 );

    return 0;
}
