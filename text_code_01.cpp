#include <pthread.h>

int num_threads = 4;  // Number of threads
pthread_t threads[num_threads];

void* thread_function(void* args) {
    // Your thread logic here
    return NULL;
}

for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, thread_function, (void*)i);
}

for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
}

