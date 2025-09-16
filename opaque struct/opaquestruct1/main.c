#include <stdio.h>
#include "opaque.h"

int main() {
    opaque *op = init();

    printf("a : %d\n", opaque_get(op));

    opaque_set(op, 5);
    printf("a : %d\n", opaque_get(op));
  
    destroy_opaque(op);
//    op->val=10;
    return 0;
}
