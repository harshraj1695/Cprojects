#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t reader;
    pthread_cond_t writer;
    int no_reader;
    int no_waiting;
    int no_writer; // 1 if writer is called else 0
} rdwr_lock;

void init(rdwr_lock* lc)
{
    lc->no_reader = 0;
    lc->no_waiting = 0;
    lc->no_writer = 0;
    pthread_mutex_init(&lc->lock, NULL);
    pthread_cond_init(&lc->reader, NULL);
    pthread_cond_init(&lc->writer, NULL);
}
void rdwr_destroy(rdwr_lock* lc)
{
    lc->no_reader = 0;
    lc->no_waiting = 0;
    lc->no_writer = 0;
    pthread_mutex_destroy(&lc->lock);
    pthread_cond_destroy(&lc->reader);
    pthread_cond_destroy(&lc->writer);
}

void readlock(rdwr_lock* lc)
{
    pthread_mutex_lock(&lc->lock);

    while(lc->no_writer != 0 || lc->no_waiting > 0) {
        pthread_cond_wait(&lc->reader, &lc->lock);
    }
    lc->no_reader++;

    pthread_mutex_unlock(&lc->lock);
}

void writelock(rdwr_lock* lc)
{
    pthread_mutex_lock(&lc->lock);
     lc->no_waiting++;
    while(lc->no_reader > 0 || lc->no_writer) {
        pthread_cond_wait(&lc->writer, &lc->lock);
    }
    lc->no_waiting--;
    lc->no_writer = 1;
    pthread_mutex_unlock(&lc->lock);
}

void readunlock(rdwr_lock* lc)
{
    pthread_mutex_lock(&lc->lock);
    lc->no_reader--;
    if(lc->no_reader == 0) {
        pthread_cond_signal(&lc->writer);
    }
    pthread_mutex_unlock(&lc->lock);
}
void writeunlock(rdwr_lock* lc)
{
    pthread_mutex_lock(&lc->lock);
    lc->no_writer = 0;
    if(lc->no_waiting > 0) {
        pthread_cond_signal(&lc->writer);
    } else {
        pthread_cond_broadcast(&lc->reader);
    }
    pthread_mutex_unlock(&lc->lock);
}
int count = 2;
rdwr_lock rea;
void* reader(void* arg)
{
    readlock(&rea);

    for(int i = 0; i < 5; i++) {
        printf("reader thread count %d and adding %d to it reasult %d\n", count, i, count + i);
    }
    readunlock(&rea);
    return NULL;
}

void* writer(void* arg)
{
    writelock(&rea);
    for(int i = 0; i < 5; i++) {
        count += i;
    }
    printf("count value after write operation %d\n", count);
    writeunlock(&rea);
    return NULL;
}
int main(int argc, char** argv)
{
    
    init(&rea);

    pthread_t thread[3];
    pthread_create(&thread[0], NULL, reader, NULL);
    pthread_create(&thread[1], NULL, writer, NULL);
    pthread_create(&thread[2], NULL, reader, NULL);

    for(int i = 0; i < 3; i++) {
        pthread_join(thread[i], NULL);
    }
    rdwr_destroy(&rea);
    return 0;
}
