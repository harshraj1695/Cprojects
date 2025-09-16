#ifndef OPAQUE_H
#define OPAQUE_H

// Forward declaration: user knows it exists, but not fields
typedef struct opaque opaque;

// API functions
opaque* init(void);
int opaque_get(opaque* op);
void opaque_set(opaque* op, int val);
void destroy_opaque(opaque* op);

#endif
