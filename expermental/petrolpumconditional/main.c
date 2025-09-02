#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_NOZZLES 2     // number of petrol pumps
#define NUM_CARS 5        // number of cars arriving

pthread_mutex_t lock;
pthread_cond_t cond;

int available_nozzles = NUM_NOZZLES;

void* car(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&lock);

    // Wait until nozzle available
    while (available_nozzles == 0) {
        printf("Car %d waiting... no nozzle free.\n", id);
        pthread_cond_wait(&cond, &lock);
    }

    // Occupy a nozzle
    available_nozzles--;
    printf("Car %d is filling petrol. Nozzles left = %d\n", id, available_nozzles);

    pthread_mutex_unlock(&lock);

    // Simulate filling time
    sleep(2);

    pthread_mutex_lock(&lock);

    // Release nozzle
    available_nozzles++;
    printf("Car %d finished. Nozzles left = %d\n", id, available_nozzles);

    // Signal waiting cars
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t cars[NUM_CARS];
    int ids[NUM_CARS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Create car threads
    for (int i = 0; i < NUM_CARS; i++) {
        ids[i] = i + 1;
        pthread_create(&cars[i], NULL, car, &ids[i]);
        sleep(1);  // cars arrive at different times
    }

    // Join all threads
    for (int i = 0; i < NUM_CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
