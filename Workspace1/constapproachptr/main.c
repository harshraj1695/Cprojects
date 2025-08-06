#include <stdio.h>

int main() {
    int a = 10, b = 20;

    //const int *ptr = pointer to const int
    const int *ptr1 = &a;
    // *ptr1 = 15;       // cannot modify value
    ptr1 = &b;           // can change pointer
    printf("ptr1 points to value: %d\n", *ptr1);

    //int * const ptr= const pointer to int
    int * const ptr2 = &a;
    *ptr2 = 30;          // can change value
    // ptr2 = &b;        // cannot change pointer
    printf("ptr2 points to value: %d\n", *ptr2);

    //const int * const ptr =const pointer to const int
    const int * const ptr3 = &a;
    // *ptr3 = 40;       //cannot change value
    // ptr3 = &b;        //cannot change pointer
    printf("ptr3 points to value: %d\n", *ptr3);

    return 0;
}
