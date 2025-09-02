#include <stdio.h>
#include <stdlib.h>
#define MAX 5   // capacity of queue

typedef struct Queue {
    int arr[MAX];
    int front, rear;
} Queue;

// Initialize queue
void init(Queue* q) {
    q->front = 0;
    q->rear = -1;
}

// Check if queue is full
int isFull(Queue* q) {
    return q->rear == MAX - 1;
}

// Check if queue is empty
int isEmpty(Queue* q) {
    return q->front > q->rear;
}

// Enqueue
void enqueue(Queue* q, int val) {
    if (isFull(q)) {
        printf("Queue Overflow!\n");
        return;
    }
    q->arr[++q->rear] = val;
    printf("%d enqueued\n", val);
}

// Dequeue
int dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue Underflow!\n");
        return -1;
    }
    return q->arr[q->front++];
}

// Peek (front element)
int peek(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }
    return q->arr[q->front];
}

// Display queue
void display(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Queue: ");
    for (int i = q->front; i <= q->rear; i++) {
        printf("%d ", q->arr[i]);
    }
    printf("\n");
}

int main() {
    Queue q;
    init(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);
    display(&q);

    printf("Dequeued: %d\n", dequeue(&q));
    printf("Front element: %d\n", peek(&q));
    display(&q);

    enqueue(&q, 50);
    enqueue(&q, 60); // Will overflow (rear reached MAX-1)
    display(&q);

    return 0;
}
