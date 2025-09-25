#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>

pthread_cond_t cond;
pthread_mutex_t lock;
int try=0;
void* fun(void *arg){
    pthread_mutex_lock(&lock);
    printf("In fun1 \n");
   while(try!=0){
       pthread_cond_wait(&cond,&lock);
   }
   try=1;
   printf("fun2 release\n");
   pthread_mutex_unlock(&lock);
   
   return NULL;
}

void * fun2(void* arg){
    pthread_mutex_lock(&lock);
    printf("In thread2 \n");
    while(try!=1){
        pthread_cond_wait(&cond,&lock);
    }
    printf("Thread2 success\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}



int main(int argc, char **argv)
{
	pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
    
    pthread_t thread[2];
    pthread_create(&thread[0],NULL,fun,NULL);
    pthread_create(&thread[1],NULL,fun2,NULL);
    
    for(int i=0;i<2;i++){
        pthread_join(thread[i],NULL);
    }
	return 0;
}
