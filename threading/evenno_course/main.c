#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem, sem1;

void* oddhandleer(void* arg)
{
    char* threadi = (char*)arg;
    for(int i = 1; i < 16; i++) {
        if(i % 2 != 0)
            printf("%s prints %d \n", threadi, i);
        sem_post(&sem1);
        sem_wait(&sem);
        
    }
    return NULL;
}
void* evenhandleer(void* arg)
{
    char* threadi = (char*)arg;
    for(int i = 2; i < 16; i++) {
        sem_wait(&sem1);
        
        if(i % 2 == 0)
            printf("%s prints %d \n", threadi, i);
        sem_post(&sem);
    }
    return NULL;
}
int main(int argc, char** argv)
{
    pthread_t thread[2];
    sem_init(&sem, 0, 0);
    sem_init(&sem1, 0, 0);
    char* arg = "Thread 1";
    char* arg2 = "Thread 2";
    pthread_create(&thread[0], NULL, oddhandleer, arg);
    pthread_create(&thread[0], NULL, evenhandleer, arg2);

    for(int i = 0; i < 2; i++) {
        pthread_join(thread[i], NULL);
    }
    sem_destroy(&sem);
    return 0;
}
