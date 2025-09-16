#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
    int a;
    int b;
}info;
int global=5;
pthread_mutex_t lock;
void* worker(void* arg){
   info *in = (info*)arg;       
    info *out = malloc(sizeof(info));  

    pthread_mutex_lock(&lock);
    out->a = 10 * in->a;
    out->b = 20 * in->b;
    global++;
    pthread_mutex_unlock(&lock);

    free(in);  
    return out;
   
}

int main(int argc, char **argv)
{
	pthread_mutex_init(&lock,NULL);
    pthread_t thread[5];   
    
    for(int i=0; i<5;i++){
      info *p = malloc(sizeof(info));  
        p->a = i + 1;
        p->b = i + 2;
        pthread_create(&thread[i], NULL, worker, p);
    }
    
    for(int i=0;i<5;i++){
        
        void *ret;
        pthread_join(thread[i], &ret);
        info *p = (info*)ret;
        printf("Thread %d result: %d %d\n", i, p->a, p->b);
        free(p); 
        
    }
    
    
    
    
	return 0;
}
