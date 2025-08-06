// mmap_example.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define FILEPATH "mappedfile.txt"
#define SIZE 1024

int main() {
    int fd;
    char *map;

    // Step 1: Open or create the file
    fd = open(FILEPATH, O_RDWR | O_CREAT, 0666);

    // Step 2: Resize the file
    ftruncate(fd, SIZE);

    // Step 3: Map the file to memory
    map = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Step 4: Write to memory-mapped file
    sprintf(map, "Hello from mmap!\n");
    printf("Data written using mmap.\n");

    // Step 5: Read back the data
    printf("Reading from mmap: %s", map);

    // Step 6: Unmap and close
    munmap(map, SIZE);
    close(fd);

    return 0;
}
