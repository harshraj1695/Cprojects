#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barrier.h"


barrier b;

void* worker(void *arg){
    int num=*((int*)arg);
    free(arg);
    printf("Entering for thread id : %d\n",num);
    int a=barrier_wait(&b);
    if(a==1){
        printf("barrier pint break at %d thread id\n",num);
    }
    printf("completing execition of %d thread\n", num);
    return NULL;
}
int main(int argc, char **argv)
{
     init(&b,5);
     
     pthread_t thread[5];
     for(int i=0;i<5;i++){
         int *num;
         num=(int*)malloc(sizeof(int));
         *num=i;
         pthread_create(&thread[i], NULL,worker,num);
     }
     
     for(int i=0;i<5;i++){
         pthread_join(thread[i],NULL);
     }
     
     barrier_destroy(&b);
     	return 0;
}
