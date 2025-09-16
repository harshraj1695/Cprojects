
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "barrier.h"


void init(barrier *b,int maxcount){
    pthread_mutex_init(&b->lock,NULL);
    pthread_cond_init(&b->cond,NULL);
    b->count=0;
    b->limit=maxcount;
    b->cycle=0;
}
void barrier_destroy(barrier *b){
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->cond);

}

int barrier_wait(barrier *b){
    pthread_mutex_lock(&b->lock);
    b->count++;
    int gen=b->cycle;
    if(b->count==b->limit){
        b->count=0;
        b->cycle++;
        pthread_cond_broadcast(&b->cond);
        pthread_mutex_unlock(&b->lock);
        return 1;
    }
    
    while(gen==b->cycle){
        pthread_cond_wait(&b->cond,&b->lock);
    }
    
    pthread_mutex_unlock(&b->lock);
    return 0;
}