#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void second() {
    printf("In second(), jumping back to main\n");
    longjmp(env, 42); // jump back to setjmp
}

void first() {
    second();
    printf("This line will never be executed\n");
}

int main() {
    int val = setjmp(env);
    if (val == 0) {
        printf("setjmp returned 0, calling first()\n");
        first();
    } else {
        printf("Back in main, setjmp returned %d\n", val);
    }
    return 0;
}
