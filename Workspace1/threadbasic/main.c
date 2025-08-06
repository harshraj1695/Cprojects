#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Thread function
void* myThreadFunction(void* arg) {
    printf("Hello from thread!\n");
    pthread_exit(NULL);  // or just return NULL;
}

int main() {
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, myThreadFunction, NULL);

    pthread_join(thread_id, NULL);
//    pthread_exit(NULL);
    printf("👋 Back to main thread\n");

    return 0;
}
