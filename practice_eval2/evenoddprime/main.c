#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int turn = 1;   // 1=odd, 2=even, 3=prime
int odd = 1, even = 2, prime = 2;
int odd_done = 0, even_done = 0, prime_done = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

int isprime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Helper: check if all sequences are done
int all_done() {
    return odd_done && even_done && prime_done;
}

void *print_odd(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 1 && !all_done()) {
            pthread_cond_wait(&cond, &lock);
        }
        if (all_done()) {
            pthread_mutex_unlock(&lock);
            break;
        }
        if (odd > 100) {
            odd_done = 1;
            turn = 2;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            continue;
        }
        printf("Odd: %d\n", odd);
        odd += 2;
        turn = 2;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *print_even(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 2 && !all_done()) {
            pthread_cond_wait(&cond, &lock);
        }
        if (all_done()) {
            pthread_mutex_unlock(&lock);
            break;
        }
        if (even > 100) {
            even_done = 1;
            turn = 3;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            continue;
        }
        printf("Even: %d\n", even);
        even += 2;
        turn = 3;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *print_prime(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 3 && !all_done()) {
            pthread_cond_wait(&cond, &lock);
        }
        if (all_done()) {
            pthread_mutex_unlock(&lock);
            break;
        }
        while (prime <= 100 && !isprime(prime)) {
            prime++;
        }
        if (prime > 100) {
            prime_done = 1;
            turn = 1;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            continue;
        }
        printf("Prime: %d\n", prime);
        prime++;
        turn = 1;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, print_odd, NULL);
    pthread_create(&t2, NULL, print_even, NULL);
    pthread_create(&t3, NULL, print_prime, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
