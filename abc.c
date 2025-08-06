#include <stdio.h>

int main() {
    int *ptr = NULL;  // pointer points to nothing

    *ptr = 10;        // 💥 trying to write to address 0 → segmentation fault

    printf("%d\n", *ptr);
    return 0;
}

