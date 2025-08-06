#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Thread function with argument
void* threadFunc(void* arg) {
    int num = *(int*)arg;
    printf(" Hello from thread! Received: %d\n", num);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int value = 42;

    // Create thread and pass address of value
    pthread_create(&tid, NULL, threadFunc, &value);

    // Wait for the thread to complete
    pthread_join(tid, NULL);

    printf("Back in main thread\n");
    exit(0);
}
