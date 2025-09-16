#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Recursive mutex structure
typedef struct {
    pthread_mutex_t lock;   // protects internal fields
    pthread_cond_t cond;    // wait for release
    pthread_t owner;        // thread that owns this lock
    int count;              // recursive count
} recursive_mutex_t;

// Initialize recursive mutex
void recursive_mutex_init(recursive_mutex_t *rm) {
    pthread_mutex_init(&rm->lock, NULL);
    pthread_cond_init(&rm->cond, NULL);
    rm->owner = 0;
    rm->count = 0;
}

// Lock function
oid recursive_mutex_lock(recursive_mutex_t *rm) {
    pthread_t self = pthread_self();
    pthread_mutex_lock(&rm->lock);   //  protect shared data

    if (rm->count > 0 && pthread_equal(rm->owner, self)) {
        //  Case 1: I already own it → just increase my tally
        rm->count++;
    } else {
        // Someone else owns it → I must wait
        while (rm->count != 0) {
            pthread_cond_wait(&rm->cond, &rm->lock);
        }
        //  Now it's free → take ownership
        rm->owner = self;
        rm->count = 1;
    }

    pthread_mutex_unlock(&rm->lock); //  done updating
}

// Unlock function
void recursive_mutex_unlock(recursive_mutex_t *rm) {
    pthread_mutex_lock(&rm->lock);   //  protect shared data

    if (!pthread_equal(rm->owner, pthread_self())) {
        //  Error: only owner can unlock
        pthread_mutex_unlock(&rm->lock);
        return;
    }

    rm->count--;   //  remove one tally mark

    if (rm->count == 0) {
        //  fully released
        rm->owner = 0;  // (owner is meaningless now)
        pthread_cond_signal(&rm->cond); //  wake one waiting thread
    }

    pthread_mutex_unlock(&rm->lock); //  done updating
}


// Destroy function
void recursive_mutex_destroy(recursive_mutex_t *rm) {
    pthread_mutex_destroy(&rm->lock);
    pthread_cond_destroy(&rm->cond);
}
recursive_mutex_t rm;

void* worker(void *arg) {
    recursive_mutex_lock(&rm);
    printf("Thread %ld: first lock\n", pthread_self());

    recursive_mutex_lock(&rm);
    printf("Thread %ld: second lock (recursive)\n", pthread_self());

    sleep(1);

    recursive_mutex_unlock(&rm);
    printf("Thread %ld: unlock once\n", pthread_self());

    recursive_mutex_unlock(&rm);
    printf("Thread %ld: unlock twice (fully released)\n", pthread_self());

    return NULL;
}

int main() {
    pthread_t t1, t2;

    recursive_mutex_init(&rm);

    pthread_create(&t1, NULL, worker, NULL);
    sleep(0.1); // let t1 start first
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    recursive_mutex_destroy(&rm);
    return 0;
}
