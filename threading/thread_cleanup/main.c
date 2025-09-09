#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void cleanup(void *arg) {
    pthread_mutex_unlock((pthread_mutex_t*)arg);
    printf("Cleanup: mutex unlocked\n");
}

void* worker(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Thread: holding lock\n");

    // Register cleanup handler
    pthread_cleanup_push(cleanup, &lock);

    // Simulate long work
    for (int i = 0; i < 5; i++) {
        printf("Thread: working %d\n", i);
        sleep(1);
        // Cancellation point inside sleep()
    }

    pthread_cleanup_pop(1); // execute cleanup if normal exit

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    sleep(2);
    printf("Main: canceling thread\n");
    pthread_cancel(tid);

    pthread_join(tid, NULL);
    printf("Main: done\n");
    return 0;
}
