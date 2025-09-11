#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "barrier.h"

#define THREADS 4

my_barrier_t barrier;

void* worker(void* arg) {
    int id = *((int*)arg);

    printf("Thread %d: before barrier\n", id);

    int res = my_barrier_wait(&barrier);

    if (res == 1) {
        // This is the "serial thread"
        printf("Thread %d: I am the last to arrive!\n", id);
    }
    sleep(2);
    printf("Thread %d: after barrier\n", id);
    return NULL;
}

int main() {
    pthread_t th[THREADS];
    int ids[THREADS];

    my_barrier_init(&barrier, THREADS);

    for (int i = 0; i < THREADS; i++) {
        ids[i] = i;
        pthread_create(&th[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(th[i], NULL);
    }

    my_barrier_destroy(&barrier);
    return 0;
}
