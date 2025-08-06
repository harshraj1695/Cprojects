#include <stdio.h>
#include <pthread.h>

void* threadFunction(void* arg) {
    printf("Thread ID: %lu\n", pthread_self()); // Print the thread ID
    return NULL;
}

int main() {
    pthread_t thread;

    // Create a thread
    if (pthread_create(&thread, NULL, threadFunction, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    // Print main thread ID
    printf("Main thread ID: %lu\n", pthread_self());

    return 0;
}
