#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
int paused[N] = {0}; // paused flags for each thread

void* worker(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // check if paused
        pthread_mutex_lock(&lock);
        while (paused[id]) {
            printf("Thread %d paused\n", id);
            pthread_cond_wait(&cond, &lock); // pause point
        }
        pthread_mutex_unlock(&lock);

        // do some work
        printf("Thread %d working...\n", id);
        sleep(1);  // simulate work
    }
    return NULL;
}

// pause a specific thread
void pause_thread(int id) {
    pthread_mutex_lock(&lock);
    paused[id] = 1;
    pthread_mutex_unlock(&lock);
}

// resume a specific thread
void resume_thread(int id) {
    pthread_mutex_lock(&lock);
    paused[id] = 0;
    pthread_cond_broadcast(&cond); // wake all threads (each checks its own flag)
    pthread_mutex_unlock(&lock);
}

int main() {
    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    sleep(3);
    pause_thread(2);    // pause thread 2
    sleep(5);
    resume_thread(2);   // resume thread 2

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
