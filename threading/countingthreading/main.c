#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

extern int global;               // declaration only
//static int global;
extern pthread_mutex_t lock;     // declaration only
void* updateglobal(void* args);  // declaration only

int main(int argc, char **argv) {
    pthread_t thread[2];
    pthread_mutex_init(&lock, NULL);

    printf("Creating first thread\n");
    pthread_create(&thread[0], NULL, updateglobal, NULL);

    printf("Creating second thread\n");
    pthread_create(&thread[1], NULL, updateglobal, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    pthread_mutex_destroy(&lock);
    printf("Value of global variable is : %d\n", global);

    return 0;
}
