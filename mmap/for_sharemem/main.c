#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_* constants
#include <sys/mman.h>
#include <sys/stat.h>   // For mode constants
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 4096
#define SHM_NAME "/my_shared_mem"

int main() {
    // 1. Create shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. Set size
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    // 3. Map shared memory
    char *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        sleep(1); // Ensure parent writes first
        printf("Child reads: %s", ptr);

        munmap(ptr, SIZE);
        close(fd);
        return 0;
    } else {
        // Parent process
        const char *msg = "Hello from parent via shared memory!\n";
        snprintf(ptr, SIZE, "%s", msg);
        printf("Parent wrote message.\n");

        // Wait for child
        wait(NULL);

        // Cleanup
        munmap(ptr, SIZE);
        close(fd);
        shm_unlink(SHM_NAME); // Remove shared memory object
    }

    return 0;
}
