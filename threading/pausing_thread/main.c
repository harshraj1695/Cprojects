#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
int paused = 0;

void* worker(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (paused) {
            printf("Thread paused...\n");
            pthread_cond_wait(&cond, &lock);
        }
        pthread_mutex_unlock(&lock);

        // Simulated work
        printf("Thread working...\n");
        sleep(1);
    }
    return NULL;
}

void pause_thread() {
    pthread_mutex_lock(&lock);
    paused = 1;
    pthread_mutex_unlock(&lock);
}

void resume_thread() {
    pthread_mutex_lock(&lock);
    paused = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    sleep(3);
    pause_thread();   // pause after 3 sec

    sleep(5);
    resume_thread();  // resume after 5 sec

    pthread_join(tid, NULL);
    return 0;
}
