#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int global=5;
pthread_mutex_t lock;
void* worker(void* arg){
    int num=*((int*)arg);
    free(arg);
    
    pthread_mutex_lock(&lock);
    
    int *res=NULL;
    res=(int*)malloc(sizeof(int));
    *res=num*global;
    global++;
    
    pthread_mutex_unlock(&lock);
    
    return res;
}

int main(int argc, char **argv)
{
	pthread_mutex_init(&lock,NULL);
    pthread_t thread[5];
    
    for(int i=0;i<5;i++){
        int*p=NULL;
        p=(int*)malloc(sizeof(int));
        *p=i;
        pthread_create(&thread[i],NULL,worker,p);
    }
    for(int i=0;i<5;i++){
         void *pp;
        pthread_join(thread[i], &pp);  
        int *res = (int*)pp;
        printf("%d from thread %d\n", *res, i);
        free(res);  
    }
	return 0;
}
