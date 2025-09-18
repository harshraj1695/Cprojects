#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>



int global=1;
pthread_rwlock_t lock;
void *reader(void* arg)
{
    pthread_rwlock_rdlock(&lock);
    int num=*((int*)arg);
    free(arg);
    printf("Value of lock with arg %d is %d\n",num,global+num);
    
   pthread_rwlock_unlock(&lock);
    return NULL;
}
void *writer(void *arg){
    pthread_rwlock_wrlock(&lock);
    global++;
    printf("Global Updated value id %d\n", global);
    pthread_rwlock_unlock(&lock);
    return NULL;
}

int main(int argc, char **argv)
{
	pthread_rwlock_init(&lock,NULL);
    pthread_t thread[10];
    for(int i=0;i<10;i++){
        if(i%2==0){
            int *ptr;
            ptr=(int*)malloc(sizeof(int));
            *ptr=i;
            pthread_create(&thread[i],NULL,reader,ptr);
        }else{
            pthread_create(&thread[i],NULL,writer,NULL);
        }
    }
    for(int i=0;i<10;i++)
    {
        pthread_join(thread[i],NULL);
    }
	return 0;
}
