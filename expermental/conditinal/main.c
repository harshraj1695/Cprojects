#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

int ready = 0;  // shared condition flag

void* waiter(void* arg) {
    pthread_mutex_lock(&lock);

    while (ready == 0) {
        printf("Waiter: Waiting for signal...\n");
        pthread_cond_wait(&cond, &lock);  // releases lock & waits
    }

    printf("Waiter: Got the signal! Proceeding...\n");

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* signaler(void* arg) {
    sleep(2);  // simulate some work
    pthread_mutex_lock(&lock);

    ready = 1;  // set condition
    printf("Signaler: Sending signal...\n");
    pthread_cond_signal(&cond);  // wake up waiter
    sleep(1);
    printf("signaler finish.....\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, waiter, NULL);
    pthread_create(&t2, NULL, signaler, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
