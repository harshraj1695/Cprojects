#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>




void* worker(void* arg) {
    while (1) {
        printf("Working...\n");
        sleep(1);   // cancellation point
        printf("this point will not print print as cancel is called imedeately
        \n");
    }
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

//    sleep(3);
    pthread_cancel(tid);   // request cancellation
    pthread_join(tid, NULL);
}
