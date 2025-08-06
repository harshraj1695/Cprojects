#include <stdio.h>
#include <pthread.h>

int counter = 0;                  // Shared resource
pthread_mutex_t lock;            // Mutex declaration

void* increment(void* arg) {
    for (int i = 0; i < 100000; ++i) {
        pthread_mutex_lock(&lock);    // Lock the critical section
        counter++;                    // Critical section
        pthread_mutex_unlock(&lock);  // Unlock
    }
    printf("running thread %d\n",pthread_self());
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
  
    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    printf("Final counter value: %d\n", counter);

    return 0;
}
