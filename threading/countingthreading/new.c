#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int global; // definition (storage allocated here)
pthread_mutex_t lock; // definition

void* updateglobal(void* args) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 10000000; i++) {
        global++;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}
