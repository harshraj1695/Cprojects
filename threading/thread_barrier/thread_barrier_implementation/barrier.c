#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "barrier.h"

int my_barrier_init(my_barrier_t* barrier, int count) {
    if (count <= 0) return -1;
    barrier->tripCount = count;
    barrier->count = 0;
    barrier->generation = 0;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    return 0;
}

int my_barrier_wait(my_barrier_t* barrier) {
    pthread_mutex_lock(&barrier->mutex);

    int gen = barrier->generation;

    barrier->count++;

    if (barrier->count == barrier->tripCount) {
        barrier->generation++;
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return 1; 
    }

    while (gen == barrier->generation) {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }

    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}

int my_barrier_destroy(my_barrier_t* barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}
