#include <stdio.h>
#include <pthread.h>

int counter = 0;                  // Shared resource
pthread_mutex_t lock;            // Mutex declaration
#define THREAD_SIZE 10
void* increment(void* arg) {
        printf("running thread %lu\n",pthread_self());

    for (int i = 0; i < 100000; ++i) {
        pthread_mutex_lock(&lock);    // Lock the critical section
        counter++;                    // Critical section
        pthread_mutex_unlock(&lock);  // Unlock
    }
    return NULL;
}

int main() {
       pthread_t t[THREAD_SIZE];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    // Create all threads
    for (int i = 0; i < THREAD_SIZE; i++) {
        if (pthread_create(&t[i], NULL, increment, NULL) != 0) {
            printf("Failed to create thread %d\n", i);
        }
        pthread_join(t[i],NULL);
            printf("counter value: %d\n", counter);

    }
//    pthread_create(&t1, NULL, increment, NULL);
//    pthread_create(&t2, NULL, increment, NULL);
  
    // Wait for both threads to finish
//    pthread_join(t1, NULL);
//    pthread_join(t2, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    printf("Final counter value: %d\n", counter);

    return 0;
}
