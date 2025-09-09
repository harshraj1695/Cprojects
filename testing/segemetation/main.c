#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = malloc(100);
    p[25] = 10;    // okay (within 100 bytes / 25 ints if sizeof(int)=4)
    p[200] = 20;   // invalid memory write (buffer overflow)
    printf("Done\n");
    free(p);
    return 0;
}
