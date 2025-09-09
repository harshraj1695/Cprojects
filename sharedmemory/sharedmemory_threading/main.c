#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
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
void* find_primes(void* arg)
{
    Range* range = (Range*)arg;
    for(int i = range->start; i <= range->end; i++) {
        if(is_prime(i)) {
            printf("Thread %d: %d is prime\n", range->threadid, i);
        }
    }

    free(range);
    return NULL;
}

int main(int argc, char** argv)
{
    int num, size_thread;
    printf("Enter the rane from 2 to end point till where you want to find the prime no\n");
    scanf("%d", &num);
    printf("enter the no of thread \n");
    scanf("%d", &size_thread);
    pthread_t thread[size_thread];
    int chunk = num / size_thread;
    for(int i = 0; i < size_thread; i++) {
        Range* r = (Range*)malloc(sizeof(Range));
        r->start = i * chunk + 1;
        r->end = (i == size_thread - 1) ? num : (i + 1) * chunk;
        r->threadid = i + 1;
        pthread_create(&thread[i], NULL, find_primes, r);
    }
    for(int i = 0; i < size_thread; i++) {
        pthread_join(thread[i], NULL);
    }
    return 0;
}
