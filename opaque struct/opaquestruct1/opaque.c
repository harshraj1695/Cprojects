#include <stdlib.h>
#include "opaque.h"

// Real struct definition (hidden from user)
struct opaque {
    int val;
};

opaque* init(void) {
    opaque *n = malloc(sizeof(opaque));
    if (!n) return NULL;
    n->val = 0;
    return n;
}

int opaque_get(opaque* op) {
    return op->val;
}

void opaque_set(opaque* op, int val) {
    op->val = val;
}

void destroy_opaque(opaque* op) {
    free(op);
}
