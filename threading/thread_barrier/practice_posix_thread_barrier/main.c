#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_barrier_t barrier;

void* worker(void* arg){
    int num=*((int*)arg);
    free(arg);
    printf("ENtering thread %d \n",num);
    int a=pthread_barrier_wait(&barrier);
    printf("%d thread execution\n ",num);
    return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread[5];
    pthread_barrier_init(&barrier,NULL,5);
    
    for(int i=0;i<5;i++){
        int *num=(int*)malloc(sizeof(int));
        *num=i;
        pthread_create(&thread[i],NULL, worker,num);
    }
    
    for(int i=0;i<5;i++){
        pthread_join(thread[i],NULL);
    }
    pthread_barrier_destroy(&barrier);
	return 0;
}
