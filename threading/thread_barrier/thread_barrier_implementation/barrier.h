typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;        // number of threads that have reached the barrier
    int tripCount;    // number of threads required to pass
    int generation;   // used to distinguish between different rounds
} my_barrier_t;

int my_barrier_init(my_barrier_t* barrier, int count);

int my_barrier_wait(my_barrier_t* barrier);

int my_barrier_destroy(my_barrier_t* barrier);