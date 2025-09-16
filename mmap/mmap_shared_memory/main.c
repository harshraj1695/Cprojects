#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_* constants
#include <sys/mman.h>
#include <sys/stat.h>   // For mode constants
#include <unistd.h>
#include <string.h>

#define SIZE 4096
#define SHM_NAME "/my_shared_mem"

int main() {
    // 1. Create or open shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. Resize shared memory object
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    // 3. Map shared memory into process address space
    char *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // 4. Write into shared memory
    const char *msg = "Hello from POSIX shared memory!\n";
    snprintf(ptr, SIZE, "%s", msg);

    printf("Message written to shared memory: %s", ptr);

    // 5. Optional: keep memory mapped, or unmap
    if (munmap(ptr, SIZE) == -1) {
        perror("munmap");
    }

    // 6. Close shared memory file descriptor
    close(fd);

    // 7. Optional: remove shared memory object from system
    // shm_unlink(SHM_NAME);

    return 0;
}
