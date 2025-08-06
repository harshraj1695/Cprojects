#include <stdio.h>

void hello() {
    printf("Hello, World!\n");
}

int main() {
    void (*func_ptr)() = hello;       // function pointer
    void (**ptr_to_func_ptr)() = &func_ptr;  // pointer to function pointer

    (*ptr_to_func_ptr)();  // calls hello
    return 0;
}
