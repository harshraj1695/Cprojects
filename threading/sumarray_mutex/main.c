#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int globalsum = 0;
typedef struct args {
    int n;
    int* Arr;
    int start;
    int end;
} args;

pthread_mutex_t lock;
void* calculatesum(void* arg)
{
    args* ne = (args*)arg;
    pthread_mutex_lock(&lock);
    for(int i = ne->start; i < ne->end; i++) {
        globalsum += ne->Arr[i];
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}
int main(int argc, char** argv)
{
    int n;
    pthread_mutex_init(&lock, NULL);
    printf("Enter the size of array\n");
    scanf("%d", &n);
    int* Arr;
    printf("Enter element in array of size %d \n", n);
    Arr = (int*)malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) {
        scanf("%d", &Arr[i]);
    }

    pthread_t thread[2];

    for(int i = 0; i < 2; i++) {
        args* obj = malloc(sizeof(args));
        obj->n = n;
        obj->Arr = Arr;
        obj->start = (n / 2) * i;
        obj->end = (i == 1) ? n : (n / 2) * (i + 1);
        pthread_create(&thread[i], NULL, calculatesum, obj);
    }
    for(int i = 0; i < 2; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("galobal sum is %d\n", globalsum);
    pthread_mutex_destroy(&lock);
    return 0;
}
