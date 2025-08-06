#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Thread 1 acquired lock1\n");
    sleep(1); // Give thread 2 a chance

    printf("Thread 1 trying to acquire lock2\n");
    if (pthread_mutex_trylock(&lock2) == 0) {
        printf("Thread 1 acquired lock2\n");
        pthread_mutex_unlock(&lock2);
    } else {
        printf("Thread 1 couldn't acquire lock2 (avoiding deadlock)\n");
    }

    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Thread 2 acquired lock2\n");
    sleep(1); // Give thread 1 a chance

    printf("Thread 2 trying to acquire lock1\n");
    if (pthread_mutex_trylock(&lock1) == 0) {
        printf("Thread 2 acquired lock1\n");
        pthread_mutex_unlock(&lock1);
    } else {
        printf("Thread 2 couldn't acquire lock1 (avoiding deadlock)\n");
    }

    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    printf("Finished execution\n");
    return 0;
}
