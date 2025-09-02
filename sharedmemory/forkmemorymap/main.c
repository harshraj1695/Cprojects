// mmap_fork_example.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>

#define FILEPATH "mappedfile.txt"
#define SIZE 1024

int main() {
    int fd;
    char *map;

    // Step 1: Open or create the file
    fd = open(FILEPATH, O_RDWR | O_CREAT, 0666);

    // Step 2: Resize the file
    ftruncate(fd, SIZE);

    // Step 3: Memory map the file
    map = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid = fork();

    if (pid == 0) {
                sprintf(map, "Hello from child via mmap!\n");

        sleep(1);  // Ensure parent writes first
        printf("Child reading from mmap: %s", map);
    } else {
        sprintf(map, "Hello from parent via mmap!\n");
        printf("Parent wrote to mmap.\n");
        sleep(1);
        printf("paret reading from map %s\n",map);
        wait(NULL); // waithing for the child to end
         munmap(map, SIZE);
        close(fd);
        shm_unlink(); // cleanup
    }

    munmap(map, SIZE);
    close(fd);

    return 0;
}
