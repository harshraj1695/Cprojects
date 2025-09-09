#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = (int*)malloc(sizeof(int));
    *p = 42;

    free(p);   // memory released

    printf("%d\n", *p);  //  dangling pointer (use-after-free)

    return 0;
}
