#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_PRIMES 10000
int primes[MAX_PRIMES];
int pos = 0;   // next index to store
sem_t *sem;    // semaphore for protecting pos

typedef struct Range {
    int start;
    int end;
    int threadid;
} Range;

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* find_primes(void* arg) {
    Range* range = (Range*)arg;
    for (int i = range->start; i <= range->end; i++) {
        if (is_prime(i)) {
            sem_wait(sem);  // lock before writing
            if (pos < MAX_PRIMES) {
                primes[pos++] = i;
            }
            sem_post(sem);  // unlock
        }
    }
    free(range);
    return NULL;
}

int main() {
    int num, size_thread;
    printf("Enter the range from 2 to end point: ");
    scanf("%d", &num);
    printf("Enter the number of threads: ");
    scanf("%d", &size_thread);

    // Create a named semaphore (works across processes too)
    sem = sem_open("/sem1", O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        sem_unlink("/sem1");
        exit(1);
    }

    pthread_t thread[size_thread];
    int chunk = num / size_thread;
    for (int i = 0; i < size_thread; i++) {
        Range* r = (Range*)malloc(sizeof(Range));
        r->start = i * chunk + 1;
        r->end = (i == size_thread - 1) ? num : (i + 1) * chunk;
        r->threadid = i + 1;
        pthread_create(&thread[i], NULL, find_primes, r);
    }

    for (int i = 0; i < size_thread; i++) {
        pthread_join(thread[i], NULL);
    }

    // Print primes
    printf("\nPrimes found:\n");
    for (int i = 0; i < pos; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    // Cleanup
    sem_close(sem);
    sem_unlink("/sem1");
    return 0;
}
