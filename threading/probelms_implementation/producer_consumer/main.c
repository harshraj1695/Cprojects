#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t condi;
#define SIZE 10

int arr[SIZE];
int front = -1, rear = -1;

// Check if queue is full
int isFull() {
    return (rear == SIZE - 1);
}

// Check if queue is empty
int isEmpty() {
    return (front == -1 || front > rear);
}

// Enqueue (Insert element)
void enqueue(int value) {
    if (isFull()) {
        printf("Queue is full! Cannot insert %d\n", value);
        return;
    }
    if (front == -1) front = 0; // first element
    arr[++rear] = value;
    printf("Inserted %d\n", value);
}

// Dequeue (Remove element)
int dequeue() {
    if (isEmpty()) {
        printf("Queue is empty! Cannot dequeue\n");
        return -1;
    }
    int value = arr[front++];
    if (front > rear) { // Reset queue if all elements are dequeued
        front = rear = -1;
    }
    return value;
}

void* consume(void*arg){
    pthread_mutex_lock(&lock);
    if(isEmpty()){
        pthread_cond_wait(&condi,&lock);
    }
    dequeue();
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* produce(void* arg) {
    pthread_mutex_lock(&lock);

    // wait until buffer has space
    while (isFull()) {
        pthread_cond_wait(&condi, &lock);
    }

    enqueue(3);
    printf("Produced: 3\n");

    // signal that new item is available for consumers
    pthread_cond_signal(&condi);

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t producer, consumer;
	pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condi,NULL);
    
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume,NULL);
    
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
	return 0;
}