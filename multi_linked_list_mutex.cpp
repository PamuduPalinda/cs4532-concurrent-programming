#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Define the Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Initialize the head of the linked list
Node* head = NULL;

// Mutex for the entire linked list
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to check if an element is present in the linked list (Member)
int Member(int value) {
    pthread_mutex_lock(&list_mutex);  // Lock the entire list
    Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            pthread_mutex_unlock(&list_mutex);  // Unlock the list
            return 1;  // Found the element
        }
        current = current->next;
    }
    pthread_mutex_unlock(&list_mutex);  // Unlock the list
    return 0;  // Element not found
}

// Function to insert an element at the beginning of the linked list (Insert)
void Insert(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;

    pthread_mutex_lock(&list_mutex);  // Lock the entire list
    newNode->next = head;
    head = newNode;
    pthread_mutex_unlock(&list_mutex);  // Unlock the list
}

// Function to delete an element from the linked list (Delete)
void Delete(int value) {
    pthread_mutex_lock(&list_mutex);  // Lock the entire list
    Node* current = head;
    Node* prev = NULL;
    while (current != NULL) {
        if (current->data == value) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            free(current);
            pthread_mutex_unlock(&list_mutex);  // Unlock the list
            return;  // Element deleted
        }
        prev = current;
        current = current->next;
    }
    pthread_mutex_unlock(&list_mutex);  // Unlock the list
}

// Function to print the elements in the linked list
void PrintList() {
    pthread_mutex_lock(&list_mutex);  // Lock the entire list
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    pthread_mutex_unlock(&list_mutex);  // Unlock the list
}

void* thread_function(void* arg) {
    int thread_id = *((int*)arg);

    int operations = 100;  // Number of operations per thread

    for (int i = 0; i < operations; i++) {
        // Generate random value for operation
        int random_value = rand() % 100;  // Assuming values are between 0 and 99

        if (random_value < 40) {  // mMember = 0.4
            int search_value = rand() % 100;  // Assuming values are between 0 and 99
            Member(search_value);
        } else if (random_value < 70) {  // mInsert = 0.3
            int insert_value = rand() % 100;  // Assuming values are between 0 and 99
            Insert(insert_value);
        } else {  // mDelete = 0.3
            int delete_value = rand() % 100;  // Assuming values are between 0 and 99
            Delete(delete_value);
        }
    }

    return NULL;
}

int main() {
    srand(time(NULL));  // Seed the random number generator with the current time

    int num_threads = 4;  // Specify the number of threads
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Create threads and assign tasks to each thread
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final state of the linked list
    printf("Final list: ");
    PrintList();
    system("pause");

    return 0;
}