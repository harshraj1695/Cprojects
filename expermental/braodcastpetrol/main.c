#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NOZZLES 2   // how many cars can fill petrol at once
#define NUM_CARS 5

pthread_mutex_t lock;
pthread_cond_t cond;

int available_nozzles = 0;  // initially empty → no petrol

void* car(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&lock);
    while (available_nozzles == 0) {
        printf(" Car %d waiting for petrol...\n", id);
        pthread_cond_wait(&cond, &lock);
    }
    available_nozzles--;   // take nozzle
    printf(" Car %d is filling petrol... (nozzles left=%d)\n", id, available_nozzles);
    pthread_mutex_unlock(&lock);

    sleep(2); // simulate filling

    pthread_mutex_lock(&lock);
    available_nozzles++;   // release nozzle
    printf(" Car %d finished filling (nozzles left=%d)\n", id, available_nozzles);
    pthread_cond_signal(&cond); // wake another car if waiting
    pthread_mutex_unlock(&lock);

    return NULL;
}

void* tanker(void* arg) {
    sleep(3); // tanker arrives late
    pthread_mutex_lock(&lock);
    available_nozzles = MAX_NOZZLES; // refill all nozzles
    printf(" Tanker refilled! %d nozzles available\n", available_nozzles);
    pthread_cond_broadcast(&cond);   // wake *all* waiting cars
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t cars[NUM_CARS], t;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    int ids[NUM_CARS];
    for (int i = 0; i < NUM_CARS; i++) {
        ids[i] = i + 1;
        pthread_create(&cars[i], NULL, car, &ids[i]);
    }

    pthread_create(&t, NULL, tanker, NULL);

    for (int i = 0; i < NUM_CARS; i++)
        pthread_join(cars[i], NULL);
    pthread_join(t, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
