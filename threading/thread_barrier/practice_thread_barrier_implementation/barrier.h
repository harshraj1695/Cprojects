typedef struct barrier{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
    int limit;
    int cycle;
}barrier;

void init(barrier *b,int maxcount);
void barrier_destroy(barrier *b);
int barrier_wait(barrier *b);
