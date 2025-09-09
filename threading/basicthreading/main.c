#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_fun(void *arg) {
    char *buff = (char*)arg;
    printf("Entering into thread with process id %d and thread id %ld\n",getpid(), pthread_self());
    printf("Thread says: %s\n", buff);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t pt;
    char msg[] = "Hello from thread!";
//  int a=10;
    printf("Main thread: creating a new thread... with process id %d\n",getpid());
//        int id=pthread_create(&pt, NULL, thread_fun, (void*)&a);


    int id=pthread_create(&pt, NULL, thread_fun, (void*)msg);
    if(id!=0){
        printf("thread creation failed\n");
    }

    pthread_join(pt, NULL);  // Wait for thread to finish

    printf("Main thread: thread finished execution\n");
    return 0;
}
