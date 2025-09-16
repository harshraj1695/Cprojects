#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rea;


int count=2;
void * reader(void *arg){
    pthread_rwlock_rdlock(&rea);
    
    for(int i=0;i<5;i++){
        printf("reader thread count %d and adding %d to it reasult %d\n",count, i, count+i);
    }
    pthread_rwlock_unlock(&rea);
   return NULL;
}

void * writer(void *arg){
    pthread_rwlock_wrlock(&rea);
    for(int i=0;i<5;i++){
        count+=i;
    }
    printf("count value after write operation %d\n", count);
    pthread_rwlock_unlock(&rea);
   return NULL;
}

int main(int argc, char **argv)
{
	pthread_rwlock_init(&rea, NULL);
    
    pthread_t thread[3];
    pthread_create(&thread[0], NULL,reader,NULL);
    pthread_create(&thread[1],NULL, writer,NULL);
    pthread_create(&thread[2],NULL,reader,NULL);
    
    for(int i=0;i<3;i++){
        pthread_join(thread[i],NULL);
    }
    pthread_rwlock_destroy(&rea);

	return 0;
}
