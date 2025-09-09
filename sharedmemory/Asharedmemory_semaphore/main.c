#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/myshm"
#define SIZE 1024

int main()
{
    int fd;
    char* ptr;

    // creating semaphore
    sem_t* sem;
    sem = sem_open("/sem1", O_RDWR | O_CREAT, 0666, 0);

    // cleanup old shared memory
    shm_unlink(SHM_NAME);

    // create shared memory object
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // set its size
    ftruncate(fd, SIZE);

    // map to memory
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    if(fork() == 0) {
        // Child: read from shared memory
        //        sleep(2);  //wait for parent to write
       
       
       
       // using semaphore 
       sem_wait(sem);
        printf("Child read: %s\n", ptr);
        munmap(ptr, SIZE);
        close(fd);
        exit(0);
    } else {
        // Parent: write into shared memory
        const char* msg = "Hello from parent!";
        sprintf(ptr, "%s", msg);
        printf("Parent wrote: %s\n", msg);
        sem_post(sem);
        wait(NULL); // wait for child
        munmap(ptr, SIZE);
        close(fd);
        shm_unlink(SHM_NAME); // cleanup
    }

    return 0;
}
