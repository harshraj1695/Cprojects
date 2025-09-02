#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock;
int balance = 1000;

void* deposite(void* args)
{
    for(int i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock);
        balance += 100;

        printf("Deposit: +100 | Balance = %d\n", balance);
        pthread_mutex_unlock(&lock);
        usleep(100000); 
    }
    return NULL;
}

void* withdraw(void* args)
{
    for(int i = 0; i < 20; i++) {
        pthread_mutex_lock(&lock);
        if(balance > 50) {
            balance -= 50;
            printf("Withdraw: -50 | Balance = %d\n", balance);
        }
        pthread_mutex_unlock(&lock);
        usleep(80000); 
    }
    return NULL;
}
int main(int argc, char** argv)
{
    pthread_mutex_init(&lock, NULL);

    pthread_t thread[2];

    pthread_create(&thread[0], NULL, deposite, NULL);

    pthread_create(&thread[1], NULL, withdraw, NULL);

    for(int i = 0; i < 2; i++) {
        pthread_join(thread[i], NULL);
    }
    printf("%d \n", balance);
    return 0;
}
