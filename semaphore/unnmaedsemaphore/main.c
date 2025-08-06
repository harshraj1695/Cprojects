#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
sem_t semaphore;
 
void * handlerFunc(void* args){
    int* num = (int*)args;
   
    sem_wait(&semaphore);
   
    printf("Hello I am thread no. %d and I accessed the critical sec now\n", *num);
    sleep(1); // system will sleep for 1 sec & then release or increment the semaphore for the other threads to get access.
   
    sem_post(&semaphore);
   
    free(num);
    return NULL;
}
 
int main(){
   
    pthread_t thread[5];
   
    //int sem_init(sem_t *sem, int pshared, unsigned int value);
    sem_init(&semaphore, 0, 1);
   
    for(int i = 0; i < 5; i++){        
        int* a = malloc(sizeof(int)); // here not declaring the vairiable like int a = I+1 bcoz at lst allthe threads will get the
        *a = i + 1;                   // same value i.e 5. as the same memory is present and the values just gets overwritten.
       
        pthread_create(&thread[i], NULL, handlerFunc,(void*)a);
    }
   
    // waiting for allthreads to occur and then finishing the main func.
       for(int i = 0; i < 5; i++){  
           pthread_join(thread[i], NULL);
       }
   
   
   
    sem_destroy(&semaphore);
    return 0;
   
}
 
 