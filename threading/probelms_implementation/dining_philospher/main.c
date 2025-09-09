#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 5

pthread_mutex_t lock;
pthread_cond_t condi;
int spoon[SIZE]; // -1 = free, otherwise philosopher id

void* solv(void *arg) {
    int ind = *((int*)arg);
    free(arg);

    int left = (ind - 1 + SIZE) % SIZE;
    int right = ind;

    while (1) {
        printf("Philosopher %d is thinking\n", ind);
        sleep(1);

        pthread_mutex_lock(&lock);

        // Wait until both spoons are free
        while (!(spoon[left] == -1 && spoon[right] == -1)) {
            pthread_cond_wait(&condi, &lock);
        }
        printf("\n");
        // Pick up both spoons
        spoon[left] = ind;
        spoon[right] = ind;
        printf("Philosopher %d picked up spoons %d and %d\n", ind, left, right);

        pthread_mutex_unlock(&lock);

        // Eating
        printf("Philosopher %d is eating\n", ind);
        sleep(2);

        pthread_mutex_lock(&lock);
        // Release spoons
        spoon[left] = -1;
        spoon[right] = -1;
        printf("Philosopher %d released spoons %d and %d\n", ind, left, right);
        
        

        // Signal other waiting philosophers
        pthread_cond_broadcast(&condi);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t thread[SIZE];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condi, NULL);

    // Initialize all spoons as free
    for (int i = 0; i < SIZE; i++) {
        spoon[i] = -1;
    }

    for (int i = 0; i < SIZE; i++) {
        int* phil = malloc(sizeof(int));
        *phil = i;
        pthread_create(&thread[i], NULL, solv, phil);
    }

    for (int i = 0; i < SIZE; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condi);
    return 0;
}