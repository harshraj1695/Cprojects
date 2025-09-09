#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_NAME "/mysem"

int main() {
    sem_t *sem;

    // create semaphore with initial value = 0
    sem_unlink(SEM_NAME); // remove old one if exists
    sem = sem_open(SEM_NAME, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    if (fork() == 0) {
        // Child process
        printf("Child: waiting...\n");
        sem_wait(sem);   // wait until parent signals
        printf("Child: got signal!\n");
        sem_close(sem);
        exit(0);
    } else {
        // Parent process
        sleep(2);
        printf("Parent: posting signal\n");
        sem_post(sem);   // signal child
        sem_close(sem);
        sem_unlink(SEM_NAME); // cleanup
    }

    return 0;
}
