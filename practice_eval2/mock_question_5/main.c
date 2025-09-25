#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int turn = 0;  
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

void *printodd(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 0 && !odd_done) {
            pthread_cond_wait(&cond, &lock);
        }
        if (odd > 100) {
            odd_done = 1;
            turn = 1; // pass to even
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            break;
        }
//         if(prime_done || odd_done){
//            turn=1;
//            pthread_cond_broadcast(&cond);
//            pthread_mutex_unlock(&lock);
//        }
        printf("Odd: %d\n", odd);
        odd += 2;
        turn = 1; // next even
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *printeven(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 1 && !even_done) {
            pthread_cond_wait(&cond, &lock);
        }
        if (even > 100) {
            even_done = 1;
            turn = 2; 
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            break;
        }
        if(prime_done || even_done){
            turn=0;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
        }
        printf("Even: %d\n", even);
        even += 2;
        turn = 2; 
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *printprime(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (turn != 2 && !prime_done) {
            pthread_cond_wait(&cond, &lock);
        }
        if (prime > 100) {
            prime_done = 1;
            turn = 0; // back to odd
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            break;
        }
        while (prime <= 100 && !isprime(prime)) {
            prime++;
        }
        if (prime > 100) {
            prime_done = 1;
            turn = 0;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&lock);
            break;
        }
        printf("Prime: %d\n", prime);
        prime++;
        turn = 0; // next odd
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, printodd, NULL);
    pthread_create(&t2, NULL, printeven, NULL);
    pthread_create(&t3, NULL, printprime, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
