#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_CREAT, O_EXCL
#include <sys/stat.h>   // For mode constants
#include <semaphore.h>
#include <unistd.h>     // fork, sleep, getpid
#include <sys/wait.h>   // wait

#define SEM_NAME "/my_named_semaphore"

int main() {
    sem_t *sem;

    // Create the named semaphore with initial value 1 because firt child have to finish exxecution and than 
    // parent after incresing the semaphore value
    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open (creating)");
        sem = sem_open(SEM_NAME, 0);  // Try opening if already created
        if (sem == SEM_FAILED) {
            perror("sem_open (opening existing)");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        sem_wait(sem);  // Lock (enter critical section)

        printf("Process %d: In critical section (iteration %d)\n", getpid(), i+1);
        sleep(1);  // Simulate work
        printf("Process %d: Leaving critical section (iteration %d)\n", getpid(), i+1);

        sem_post(sem);  // Unlock (exit critical section)

        sleep(1);  // Simulate non-critical work
    }

    if (pid > 0) {
        wait(NULL);         // Wait for child to finish
        sem_close(sem);     // Close in parent
        sem_unlink(SEM_NAME);  // Unlink semaphore (cleanup)
    } else {
        sem_close(sem);     // Close in child
    }

    return 0;
}
