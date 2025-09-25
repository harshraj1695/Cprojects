//#include <stdio.h>
//#include <stdlib.h>
//#include "queue.h"
//
//void init(queue *q, int cap) {
//    q->size = cap;
//    q->arr = (int *)calloc(q->size, sizeof(int));
//    q->front = -1;
//    q->rear = -1;
//}
//
//int isfull(queue *q) {
//    return q->rear == q->size - 1;
//}
//
//int isempty(queue *q) {
//    return (q->front == -1 || q->front > q->rear);
//}
//
//void push(queue *q, int val) {
//    if (isfull(q)) {
//        printf("Overflow\n");
//        return;
//    }
//    if (q->front == -1) {
//        q->front = 0;
//    }
//    q->rear++;
//    q->arr[q->rear] = val;
//}
//
//int pop(queue *q) {
//    if (isempty(q)) {
//        printf("Underflow\n");
//        return -1;
//    }
//    int temp = q->arr[q->front];
//    q->front++;
//    return temp;
//}
//
//void display(queue *q) {
//    if (isempty(q)) {
//        printf("Queue is empty\n");
//        return;
//    }
//    for (int i = q->front; i <= q->rear; i++) {
//        printf("%d ", q->arr[i]);
//    }
//    printf("\n");
//}
//
