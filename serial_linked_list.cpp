#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Initialize the head of the linked list
Node* head = NULL;

// Function to check if an element is present in the linked list (Member)
int Member(int value) {
    Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            return 1;  // Found the element
        }
        current = current->next;
    }
    return 0;  // Element not found
}

// Function to insert an element at the beginning of the linked list (Insert)
void Insert(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

// Function to delete an element from the linked list (Delete)
void Delete(int value) {
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
            return;  // Element deleted
        }
        prev = current;
        current = current->next;
    }
}

// Function to print the elements in the linked list
void PrintList() {
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    srand(time(NULL));  // Seed the random number generator with the current time

    int n = 100;  // Number of unique values to populate the linked list
    int m = 50;   // Total number of random Member, Insert, and Delete operations
    float mMember = 0.4;  // Fraction of Member operations
    float mInsert = 0.3;  // Fraction of Insert operations
    float mDelete = 0.3;  // Fraction of Delete operations

    // Populate the linked list with n random, unique values
    for (int i = 0; i < n; i++) {
        int randomValue;
        do {
            randomValue = rand() % (1 << 16);  // Generate a random value between 0 and 2^16 - 1
        } while (Member(randomValue));  // Ensure uniqueness
        Insert(randomValue);
    }

    // Perform random Member, Insert, and Delete operations
    for (int i = 0; i < m; i++) {
        float random = (float)rand() / RAND_MAX;  // Generate a random float between 0 and 1

        if (random < mMember) {
            // Perform Member operation
            int randomValue = rand() % (1 << 16);
            printf("Member(%d) = %d\n", randomValue, Member(randomValue));
        } else if (random < mMember + mInsert) {
            // Perform Insert operation
            int randomValue;
            do {
                randomValue = rand() % (1 << 16);
            } while (Member(randomValue));  // Ensure uniqueness
            Insert(randomValue);
            printf("Insert(%d)\n", randomValue);
        } else {
            // Perform Delete operation
            if (head != NULL) {
                int randomIndex = rand() % (1 << 16);
                Delete(head->data);  // Delete the first element for simplicity
                printf("Delete(%d)\n", randomIndex);
            }
        }
    }

    // Print the final state of the linked list
    printf("Final list: ");
    PrintList();

    return 0;
}