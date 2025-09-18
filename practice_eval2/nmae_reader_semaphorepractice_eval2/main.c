#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define SEM_NAME "/sem"
#define SHM_NAME "/shared"
#define SHM_SIZE sizeof(int)

int main() {
    // Open existing semaphore
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Open existing shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Map shared memory
    int *shared = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Wait for writer
    printf("Reader: waiting...\n");
    sem_wait(sem);

    // Read data
    printf("Reader: read %d\n", *shared);

    // Cleanup (reader does unlinking at the end)
    munmap(shared, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);

    // Remove shared objects from system
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);

    return 0;
}
