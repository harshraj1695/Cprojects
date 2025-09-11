#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t sem;  // semaphore

void* worker(void* arg) {
    long id = (long)arg;

    sem_wait(&sem);  // acquire semaphore once
    printf("Thread %ld entered critical section\n", id);
    sleep(1);        // simulate work

    // Increment semaphore 10 times
    for (int i = 0; i < 10; i++) {
        sem_post(&sem);
        printf("Thread %ld posted semaphore (%d/10)\n", id, i + 1);
        usleep(100000); // small delay to observe the effect
    }

    printf("Thread %ld leaving critical section\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize semaphore with value 1 (binary)
    sem_init(&sem, 0, 1);
    printf("Semaphore initialized with value 1\n");

    // Create threads
    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, (void*)i);

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&sem);
    return 0;
}
