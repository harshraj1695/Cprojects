#include <stdio.h>
#include <stdlib.h>

#define MAX 5  // maximum size of stack

typedef struct {
    int items[MAX];
    int top;
} stack;

// Initialize stack
void init(stack *s) {
    s->top = -1;
}

// Check if stack is full
int isFull(stack *s) {
    return s->top == MAX - 1;
}

// Check if stack is empty
int isEmpty(stack *s) {
    return s->top == -1;
}

// Push an element onto the stack
void push(stack *s, int value) {
    if (isFull(s)) {
        printf("Stack Overflow! Cannot push %d\n", value);
        return;
    }
    s->items[++(s->top)] = value;
    printf("%d pushed to stack\n", value);
}

// Pop an element from the stack
int pop(stack *s) {
    if (isEmpty(s)) {
        printf("Stack Underflow! Cannot pop\n");
        return -1;
    }
    return s->items[(s->top)--];
}

// Peek at the top element
int peek(stack *s) {
    if (isEmpty(s)) {
        printf("Stack is empty\n");
        return -1;
    }
    return s->items[s->top];
}

int main() {
    stack s;
    init(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Top element is %d\n", peek(&s));

    printf("Popped: %d\n", pop(&s));
    printf("Popped: %d\n", pop(&s));

    return 0;
}
