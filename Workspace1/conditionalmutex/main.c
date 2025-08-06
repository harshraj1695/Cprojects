#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int ready = 0; // Shared state

void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);

    while (!ready) {
        printf("Consumer: Waiting for condition...\n");
        pthread_cond_wait(&cond, &mutex); // Wait until signaled
    }
   
    printf("Consumer: Received signal, continuing work.\n");
         sleep(2); // not necessary

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* producer(void* arg) {
    sleep(2); // Simulate work not necessary

    pthread_mutex_lock(&mutex);
    ready = 1;
    printf("Producer: Setting ready = 1 and signaling condition.\n");
    sleep(2);  // not necessary 
    pthread_cond_signal(&cond); // Signal the waiting thread
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, consumer, NULL);
    pthread_create(&t2, NULL, producer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Main: Finished execution.\n");

    return 0;
}
