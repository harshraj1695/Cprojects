#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t notEmpty, notFull;

#define SIZE 10

int arr[SIZE];
int front = 0, rear = 0, count = 0; // circular queue

int isFull() {
    return count == SIZE;
}

int isEmpty() {
    return count == 0;
}

void enqueue(int value) {
    arr[rear] = value;
    rear = (rear + 1) % SIZE;
    count++;
    printf("Produced %d\n", value);
}

int dequeue() {
    int value = arr[front];
    front = (front + 1) % SIZE;
    count--;
    printf("Consumed %d\n", value);
    return value;
}

void* produce(void* arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&lock);
        while (isFull()) {
            pthread_cond_wait(&notFull, &lock);
        }
        enqueue(i);
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&lock);
        usleep(100000); // slow down producer
    }
    return NULL;
}

void* consume(void* arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&lock);
        while (isEmpty()) {
            pthread_cond_wait(&notEmpty, &lock);
        }
        dequeue();
        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&lock);
        usleep(200000); // slow down consumer
    }
    return NULL;
}

int main() {
    pthread_t producer, consumer;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&notEmpty, NULL);
    pthread_cond_init(&notFull, NULL);

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notEmpty);
    pthread_cond_destroy(&notFull);

    return 0;
}