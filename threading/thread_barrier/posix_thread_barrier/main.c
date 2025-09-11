#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 4

pthread_barrier_t barrier;

void* worker(void* arg) {
    int id = *((int*)arg);
    printf("Thread %d: reached barrier\n", id);

    // Wait until all threads reach here
    pthread_barrier_wait(&barrier);

    printf("Thread %d: passed barrier\n", id);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT];

    // Initialize barrier for THREAD_COUNT threads
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}
