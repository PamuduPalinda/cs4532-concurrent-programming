#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#define MAX_THREADS 2 // Maximum number of threads you want to support

// Define the Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Initialize the head of the linked list
Node* head = NULL;
pthread_mutex_t mutex;

// Function to check if an element is present in the linked list (Member)
int Member(int value) {
    pthread_mutex_lock(&mutex);
    Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            pthread_mutex_unlock(&mutex);
            return 1;  // Found the element
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
    return 0;  // Element not found
}

// Function to insert an element at the beginning of the linked list (Insert)
void Insert(int value) {
    pthread_mutex_lock(&mutex);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = head;
    head = newNode;
    pthread_mutex_unlock(&mutex);
}

// Function to delete an element from the linked list (Delete)
void Delete(int value) {
    pthread_mutex_lock(&mutex);
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
            pthread_mutex_unlock(&mutex);
            return;  // Element deleted
        }
        prev = current;
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
}

// Function to print the elements in the linked list
void PrintList() {
    pthread_mutex_lock(&mutex);
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    pthread_mutex_unlock(&mutex);
}

unsigned long mutex_run(int case_num) {
    int n = 1000;  // Number of unique values to populate the linked list
    int m = 10;   // Total number of random Member, Insert, and Delete operations

    float mMember;
    float mInsert;
    float mDelete;
    switch (case_num)
    {
        case 1:{
            mMember = 0.99;
            mInsert = 0.005;
            mDelete = 0.005;
            break;
        }

        case 2:{
            mMember = 0.9;
            mInsert = 0.05;
            mDelete = 0.05;
            break;
        }

        case 3:{
            mMember = 0.5;
            mInsert = 0.25;
            mDelete = 0.25;
            break;
        }
        
        default:{
            mMember = 0.99;
            mInsert = 0.005;
            mDelete = 0.005;
            break;
        }
    }

    // Fractions of each operation
    int Mem = (int) (m * mMember);
    int Ins = (int)(m * mInsert);
    int Del= (int)( m * mDelete);




    // Populate the linked list with n random, unique values
    for (int i = 0; i < n; i++) {
        int randomValue;
        do {
            randomValue = rand() % (1 << 16);  // Generate a random value between 0 and 2^16 - 1
        } while (Member(randomValue));  // Ensure uniqueness
        Insert(randomValue);
    }

    struct timeval stop;
    struct timeval start;
    gettimeofday(&start, NULL); 

    // Perform random Member, Insert, and Delete operations
    for (int i = 0; i < m; i++) {
        float random = (float)rand() / RAND_MAX;  // Generate a random float between 0 and 1
        int op = rand() % 3;
        if (op==2 && random < Mem) {
            // Perform Member operation
            int randomValue = rand() % (1 << 16);
            printf("Member(%d) = %d\n", randomValue, Member(randomValue));
        } else if (op==0 && random < Ins) {
            // Perform Insert operation
            int randomValue;
            do {
                randomValue = rand() % (1 << 16);
            } while (Member(randomValue));  // Ensure uniqueness
            Insert(randomValue);
            printf("Insert(%d)\n", randomValue);
        } else if (op==1 && random < Del) {
            // Perform Delete operation
            if (head != NULL) {
                int randomIndex = rand() % (1 << 16);
                Delete(head->data);  // Delete the first element for simplicity
                printf("Delete(%d)\n", randomIndex);
            }
        }
    }
    // Calculate the elapsed time in milliseconds and store it in the array
    gettimeofday(&stop, NULL);
    unsigned long time;
    time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    return time;
}

void* thread_function(void* arg) {
    int case_num = *((int*)arg);
    mutex_run(case_num);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    if (numThreads < 1 || numThreads > MAX_THREADS) {
        printf("Number of threads should be between 1 and %d\n", MAX_THREADS);
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));  // Seed the random number generator with the current time

    int numRuns = 385;  // Number of times to run the program
    // Array to store operation times
    unsigned long operationTimes[numRuns];
    unsigned long sum = 0;
    pthread_t threads[MAX_THREADS];

    for (int run = 0; run < numRuns; run++) {
        int case_num = 1; // You can change this to the desired case number
        operationTimes[run] = 0;

        for (int i = 0; i < numThreads; i++) {
            pthread_create(&threads[i], NULL, thread_function, &case_num);
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }
        printf("Iteration number %d finished\n", run + 1);
    }

    unsigned long mean = sum / numRuns;
    unsigned long sum_squared_diff = 0;
    for (int i = 0; i < numRuns; i++) {
        unsigned long diff = operationTimes[i] - mean;
        sum_squared_diff += diff * diff;
    }

    unsigned long std_deviation = sqrt(sum_squared_diff / numRuns);

    // Print the mean and standard deviation
    printf("Mean time: %lu\n", mean);
    printf("Standard Deviation: %lu\n", std_deviation);

    pthread_mutex_destroy(&mutex);
    return 0;
}
