#include <stdio.h>
#include <stdlib.h>

#define SIZE 5   // max capacity

typedef struct {
    int arr[SIZE];
    int front, rear;
} CircularQueue;

// Initialize queue
void init(CircularQueue *q) {
    q->front = -1;
    q->rear = -1;
}

// Check if empty
int isEmpty(CircularQueue *q) {
    return (q->front == -1);
}

// Check if full
int isFull(CircularQueue *q) {
    return ((q->rear + 1) % SIZE == q->front);
}

// Enqueue
void enqueue(CircularQueue *q, int value) {
    if (isFull(q)) {
        printf("Queue Overflow!\n");
        return;
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % SIZE;
    q->arr[q->rear] = value;
    printf("Inserted %d\n", value);
}

// Dequeue
int dequeue(CircularQueue *q) {
    if (isEmpty(q)) {
        printf("Queue Underflow!\n");
        return -1;
    }
    int val = q->arr[q->front];
    if (q->front == q->rear) {
        // Queue becomes empty
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % SIZE;
    }
    return val;
}

// Display
void display(CircularQueue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Queue elements: ");
    int i = q->front;
    while (1) {
        printf("%d ", q->arr[i]);
        if (i == q->rear) break;
        i = (i + 1) % SIZE;
    }
    printf("\n");
}

int main() {
    CircularQueue q;
    init(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);
    enqueue(&q, 50);  // should overflow

    display(&q);

    printf("Dequeued: %d\n", dequeue(&q));
    printf("Dequeued: %d\n", dequeue(&q));

    enqueue(&q, 60);
    enqueue(&q, 70);

    display(&q);

    return 0;
}
