#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


//__thread int global=0;
int global =0;
pthread_mutex_t lock;
void* updateglobal(void*args){
    pthread_mutex_lock(&lock);
    for(int i=0;i<10000000;i++){
        global++;
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}
int main(int argc, char **argv)
{
	pthread_t thread[2];
    pthread_mutex_init(&lock,NULL);
    printf("CReating first thread \n");
    
    pthread_create(&thread[0],NULL, updateglobal, NULL);
    
    printf("going in the 2 thread \n");
    pthread_create(&thread[1],NULL, updateglobal,NULL);
    
   pthread_join(thread[1],NULL);
   pthread_join(thread[0],NULL);
   
   pthread_mutex_destroy(&lock);
   printf("value of global varible is : %d\n",global);
   
	return 0;
}
