#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
    int count;
    pthread_t owner;
    pthread_mutex_t lock;
    pthread_cond_t condi;
}rec_mutex;

//rec_mutex rec;
void init(rec_mutex *rec){
    rec->count=0;
    rec->owner=0;
    pthread_mutex_init(&rec->lock, NULL);
    pthread_cond_init(&rec->condi,NULL);
}

void rec_lock(rec_mutex *rec){
    pthread_t self =pthread_self();
    pthread_mutex_lock(&rec->lock);
    
    if(rec->count > 0 && pthread_equal(self, rec->owner)){  // for same thread
        rec->count++;
    }else{// for other thread
        while(rec->count!=0){ // if new arrive and one is already using 
            pthread_cond_wait(&rec->condi, &rec->lock);
        }
      // Now it's free → take ownership
        rec->owner = self;
        rec->count = 1;
    }
    pthread_mutex_unlock(&re
    c->lock);
}
void rec_unlock(rec_mutex *rec){
    pthread_t self=pthread_self();
    
    pthread_mutex_lock(&rec->lock);
    if(!pthread_equal(self, rec->owner)){   // only owner can unlock
        pthread_mutex_unlock(&rec->lock);
        return;
    }
    
    rec->count--;
    if(rec->count==0) {  // now owner can realse it
        rec->owner=0;
        pthread_cond_signal(&rec->condi);
    }
    pthread_mutex_unlock(&rec->lock);
    
}

rec_mutex rm;
void* worker(void *arg){
    rec_lock(&rm);
    printf("first lock is done \n");
    rec_lock(&rm);
    printf("second lock is applied \n");
    
    sleep(2);
    
    rec_unlock(&rm);
    printf("first unlock is done\n");
    
    rec_unlock(&rm);
    printf("second unlock is done\n");
    return NULL;
}

int main(int argc, char **argv)
{
	init(&rm);
    pthread_t thread;
    pthread_create(&thread, NULL, worker, NULL);
    pthread_join(thread, NULL);
	return 0;
}
