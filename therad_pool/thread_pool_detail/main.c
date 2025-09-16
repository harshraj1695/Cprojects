#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// ---------------------- TASK STRUCTURE ---------------------- //
// Each task has:
// 1. A function pointer (the work to do)
// 2. A void* argument (data for the function)
// 3. A pointer to the next task (for the queue)
typedef struct task {
    void (*function)(void *arg);
    void *arg;
    struct task *next;
} task_t;

// ---------------------- THREADPOOL STRUCTURE ---------------------- //
// The thread pool manages:
// 1. A queue of tasks
// 2. An array of worker threads
// 3. Synchronization (mutex + condition variables)
// 4. Shutdown flag
typedef struct {
    pthread_mutex_t lock;      // Protects shared task queue
    pthread_cond_t notify;     // Signals workers that a task is available
    pthread_t *threads;        // Array of worker threads
    task_t *head;              // Task queue head (FIFO)
    task_t *tail;              // Task queue tail
    int thread_count;          // Number of threads in pool
    int shutdown;              // Flag: 1 if shutting down
} threadpool_t;

// ---------------------- WORKER THREAD FUNCTION ---------------------- //
// Each thread runs this function in an infinite loop:
// 1. Lock queue
// 2. Wait for task or shutdown signal
// 3. Pop task from queue
// 4. Unlock
// 5. Run the task
void *worker_thread(void *pool) {
    threadpool_t *tp = (threadpool_t *)pool;

    while (1) {
        pthread_mutex_lock(&tp->lock);

        // If no task is available and not shutting down → wait
        while (tp->head == NULL && !tp->shutdown) {
            pthread_cond_wait(&tp->notify, &tp->lock);
        }

        // If shutdown flag is set → exit thread
        if (tp->shutdown) {
            pthread_mutex_unlock(&tp->lock);
            pthread_exit(NULL);
        }

        // Get task from queue (FIFO)
        task_t *task = tp->head;
        if (task != NULL) {
            tp->head = task->next;
            if (tp->head == NULL) { // if queue becomes empty
                tp->tail = NULL;
            }
        }

        pthread_mutex_unlock(&tp->lock);

        // Execute the task
        if (task != NULL) {
            (task->function)(task->arg);
            free(task);
        }
    }
    return NULL;
}

// ---------------------- CREATE THREADPOOL ---------------------- //
// 1. Allocate threadpool struct
// 2. Initialize mutex, condition variable
// 3. Create N worker threads
threadpool_t *threadpool_create(int num_threads) {
    threadpool_t *tp = malloc(sizeof(threadpool_t));
    if (!tp) return NULL;

    tp->thread_count = num_threads;
    tp->shutdown = 0;
    tp->head = tp->tail = NULL;

    pthread_mutex_init(&tp->lock, NULL);
    pthread_cond_init(&tp->notify, NULL);

    tp->threads = malloc(sizeof(pthread_t) * num_threads);

    // Create all worker threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&tp->threads[i], NULL, worker_thread, tp);
    }

    return tp;
}

// ---------------------- ADD TASK TO POOL ---------------------- //
// 1. Allocate a new task
// 2. Lock queue
// 3. Append task at the tail of queue (FIFO)
// 4. Signal one worker that a task is available
// 5. Unlock
void threadpool_add(threadpool_t *tp, void (*function)(void *), void *arg) {
    task_t *task = malloc(sizeof(task_t));
    if (!task) return;

    task->function = function;
    task->arg = arg;
    task->next = NULL;

    pthread_mutex_lock(&tp->lock);

    if (tp->tail == NULL) {
        tp->head = tp->tail = task;
    } else {
        tp->tail->next = task;
        tp->tail = task;
    }

    // Wake up ONE worker thread waiting on condition variable
    pthread_cond_signal(&tp->notify);
    pthread_mutex_unlock(&tp->lock);
}

// ---------------------- DESTROY THREADPOOL ---------------------- //
// 1. Lock and set shutdown flag
// 2. Wake up ALL threads so they can exit
// 3. Join all threads
// 4. Cleanup memory
void threadpool_destroy(threadpool_t *tp) {
    pthread_mutex_lock(&tp->lock);
    tp->shutdown = 1;
    pthread_cond_broadcast(&tp->notify); // wake up ALL workers
    pthread_mutex_unlock(&tp->lock);

    // Wait for all threads to exit
    for (int i = 0; i < tp->thread_count; i++) {
        pthread_join(tp->threads[i], NULL);
    }

    // Free remaining tasks in queue (if any)
    task_t *current = tp->head;
    while (current != NULL) {
        task_t *tmp = current;
        current = current->next;
        free(tmp);
    }

    pthread_mutex_destroy(&tp->lock);
    pthread_cond_destroy(&tp->notify);

    free(tp->threads);
    free(tp);
}

// ---------------------- EXAMPLE TASK ---------------------- //
// Just prints an integer (simulating work)
void example_task(void *arg) {
    int num = *(int *)arg;
    printf("Thread %ld processing task %d\n", pthread_self(), num);
    sleep(1); // simulate some work
    free(arg); // free memory passed as argument
}

// ---------------------- MAIN FUNCTION ---------------------- //
int main() {
    // Create pool with 4 worker threads
    threadpool_t *pool = threadpool_create(4);

    // Submit 10 tasks
    for (int i = 0; i < 10; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        threadpool_add(pool, example_task, arg);
    }

    // Wait for some time so tasks finish
    sleep(5);

    // Destroy pool (waits for threads to exit)
    threadpool_destroy(pool);

    return 0;
}
