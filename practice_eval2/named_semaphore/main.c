#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define SEM_NAME "/sem"
#define SHM_NAME "/shared"
#define SHM_SIZE sizeof(int)

int main() {
    // Open or create semaphore (initial value = 0)
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Open or create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Resize shared memory
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Map shared memory
    int *shared = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Write data
    *shared = 1234;
    printf("Writer: wrote %d\n", *shared);

    // Signal reader
    sem_post(sem);

    // Cleanup 
    munmap(shared, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);

    return 0;
}
