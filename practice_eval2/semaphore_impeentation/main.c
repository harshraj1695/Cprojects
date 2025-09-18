#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global = 1;

typedef struct semaphore {
    int counter;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} semaphore;

semaphore* sem;
void sem_init(int count)
{
    pthread_mutex_init(&sem->lock, NULL);
    pthread_cond_init(&sem->cond, NULL);
    sem->counter = count;
}

void se_wait()
{
    pthread_mutex_lock(&sem->lock);

    while(sem->counter == 0) {
        pthread_cond_wait(&sem->cond, &sem->lock);
    }
    sem->counter--;
    pthread_mutex_unlock(&sem->lock);
}
void se_post()
{
    pthread_mutex_lock(&sem->lock);
    sem->counter++;
    if(sem->counter > 0) {
        pthread_cond_signal(&sem->cond);
    }
    pthread_mutex_unlock(&sem->lock);
}

void* worker(void* arg)
{
    se_wait();
    printf("Thread %lu entered, global = %d\n", pthread_self(), global);
    global++;
    sleep(2); 
    printf("Thread %lu leaving, global = %d\n", pthread_self(), global);
    se_post();

    return NULL;
}
int main(int argc, char** argv)
{
    sem = (semaphore*)malloc(sizeof(semaphore));
    sem_init(2);

    pthread_t thread[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&thread[i], NULL, worker, NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(thread[i], NULL);
    }
    return 0;
}
