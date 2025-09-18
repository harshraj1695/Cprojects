#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

sem_t sem;
int global = 5;
void* worker(void* arg)
{
     sem_wait(&sem);
    int num = *((int*)arg);
    free(arg);

    printf("Thread is printed with id : %d and value: %d\n", num + 1, global + (num + 1));
    global++;
    sem_post(&sem);
    return NULL;
}

int main(int argc, char** argv)
{
    sem_init(&sem,0,1);
    pthread_t thread[5];
    for(int i=0;i<5;i++){
        int *ptr;
        ptr=(int*)malloc(sizeof(int));
        *ptr=i;
        pthread_create(&thread[i],NULL,worker,ptr);
        
    }
    for(int i=0;i<5;i++){
        pthread_join(thread[i],NULL);
    }
    sem_destroy(&sem);
    return 0;
}
