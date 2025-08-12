#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 50  // maximum size of stack

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
void push(stack *s, char value) {
    if (isFull(s)) {
        printf("Stack Overflow! Cannot push %d\n", value);
        return;
    }
    s->items[++(s->top)] = value;
//    printf("%d pushed to stack\n", value);
}

// Pop an element from the stack
char pop(stack *s) {
    if (isEmpty(s)) {
        printf("Stack Underflow! Cannot pop\n");
        return -1;
    }
    return s->items[(s->top)--];
}

// Peek at the top element
char peek(stack *s) {
    if (isEmpty(s)) {
        printf("Stack is empty\n");
        return -1;
    }
    return s->items[s->top];
}

 bool validateparanthesis(stack *st, char *input){
      int i = 0;

    while (input[i] != '\0') {
        if (input[i] == '(' || input[i] == '{' || input[i] == '[') {
            push(st, input[i]);
        }
        else if (input[i] == ')' || input[i] == '}' || input[i] == ']') {
            if (isEmpty(st)) return false;

            char topchar = peek(st);
            if ((input[i] == ')' && topchar != '(') ||
                (input[i] == '}' && topchar != '{') ||
                (input[i] == ']' && topchar != '[')) {
                return false;
            }
            pop(st);
        }
        i++;
    }

    return isEmpty(st);
 }
int main(int argc, char **argv)
{
	stack s;
    init(&s);
    char input[50];
    printf("Enter the input expression \n");
    scanf("%s",input);
    if(validateparanthesis(&s,input)){
        printf("valid input expression \n");
    }else{
        printf("invalid expression \n");
    }
    
	return 0;
}
