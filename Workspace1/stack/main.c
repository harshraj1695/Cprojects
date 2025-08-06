#include <stdio.h>
#include <stdlib.h>

// Define Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Push a value onto the stack
Node* push(Node* top, int value) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    if (newnode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newnode->data = value;
    newnode->next = top;
    top = newnode;
    return top;
}

// Pop the top value from the stack
Node* pop(Node* top) {
    if (top == NULL) {
        printf("Stack Underflow\n");
        return NULL;
    }
    Node* temp = top;
    printf("Popped: %d\n", temp->data);
    top = top->next;
    free(temp);
    return top;
}

// Peek at the top value of the stack
int peek(Node* top) {
    if (top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    return top->data;
}

// Check if stack is empty
int isEmpty(Node* top) {
    return top == NULL;
}

// Print the stack
void printStack(Node* top) {
    printf("Stack: ");
    while (top != NULL) {
        printf("%d ", top->data);
        top = top->next;
    }
    printf("\n");
}

int main() {
    Node* top = NULL;

    top = push(top, 10);
    top = push(top, 20);
    top = push(top, 30);

    printStack(top);

    printf("Top element: %d\n", peek(top));

    top = pop(top);
    top = pop(top);

    printStack(top);

    if (isEmpty(top))
        printf("Stack is empty\n");
    else
        printf("Stack is not empty\n");

    top = pop(top);  // last pop
    top = pop(top);  // underflow check

    return 0;
}
