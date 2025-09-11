#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct sem{
    int count;
    pthread_mutex_t lock;
    pthread_cond_t condi;
}sem;
sem p;
void sem_init(int counti){
    p.count=counti;
    pthread_mutex_init(&(p.lock),NULL);
    pthread_cond_init(&(p.condi),NULL);
}
void sem_wait(){
    pthread_mutex_lock(&p.lock);
    while(p.count==0){
        pthread_cond_wait(&p.condi,&p.lock);
    }
    p.count--;
    pthread_mutex_unlock(&p.lock);
}
void sem_post(){
    pthread_mutex_lock(&p.lock);
    p.count++;
        pthread_cond_signal(&p.condi);
    pthread_mutex_unlock(&p.lock);
}
void sem_destoy(){
    p.count=0;
    pthread_mutex_destroy(&p.lock);
    pthread_cond_destroy(&p.condi);
}

void* worker(void* arg) {
    int id = (int)(long)arg;
    printf("Worker %d is waiting...\n", id);
    sem_wait();  // wait until resource available
    printf("Worker %d got the semaphore!\n", id);
    return NULL;
}
int main(int argc, char **argv)
{
	pthread_t threads[3];

    sem_init(0); // start with 0 resources → all workers will block

    // create 3 workers
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)(long)(i+1));
    }

    sleep(2); // give workers time to block
    printf("\nMain thread: posting 3 times...\n\n");

    for (int i = 0; i < 3; i++) {
        sem_post(); // wake one worker
        sleep(1);   // small delay so we see order clearly
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destoy();
	return 0;
}
