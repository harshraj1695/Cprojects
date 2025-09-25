#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SHM_SIZE 1024

// Global shared memory
char *shm1;
char *shm2;

// Global semaphores
sem_t sem1; // reader -> processor
sem_t sem2; // processor -> writer

typedef struct {
    char *shm1;
    char *shm2;
} thread_args;

// Check if character is a vowel
int is_vowel(char c) {
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
           c=='A'||c=='E'||c=='I'||c=='O'||c=='U';
}

// Reader thread
void *reader(void *arg) {
    char *shm = (char *)arg;

    int fd = open("a.txt", O_RDONLY);
    if (fd < 0) { perror("open a.txt"); return NULL; }

    ssize_t n;
    char buffer[SHM_SIZE];
    while ((n = read(fd, buffer, SHM_SIZE-1)) > 0) {
        buffer[n] = '\0';
        strcpy(shm, buffer);
        sem_post(&sem1); // signal processor
    }

    close(fd);

    strcpy(shm, "EOF");  // mark end
    sem_post(&sem1);
    return NULL;
}

// Processor thread
void *processor(void *arg) {
    thread_args *args = (thread_args *)arg;

    while (1) {
        sem_wait(&sem1);       // wait for reader
        if (strcmp(args->shm1, "EOF") == 0) break;

        char temp[SHM_SIZE];
        int j = 0;
        for (int i = 0; args->shm1[i]; i++) {
            if (!is_vowel(args->shm1[i])) {
                temp[j++] = args->shm1[i];
            }
        }
        temp[j] = '\0';

        strcpy(args->shm2, temp); // write to shm2
        sem_post(&sem2);          // signal writer
    }

    strcpy(args->shm2, "EOF");
    sem_post(&sem2);
    return NULL;
}

// Writer thread
void *writer(void *arg) {
    char *shm = (char *)arg;

    int fd = open("b.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open b.txt"); return NULL; }

    while (1) {
        sem_wait(&sem2);       // wait for processor
        if (strcmp(shm, "EOF") == 0) break;

        write(fd, shm, strlen(shm));
        write(fd, "\n", 1);
    }

    close(fd);
    return NULL;
}

int main() {
    // Create shared memory
    shm1 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shm2 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Initialize semaphores
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    thread_args args = { shm1, shm2 };

    pthread_t t1, t2, t3;

    // Create threads
    pthread_create(&t1, NULL, reader, shm1);  // pass shm1 directly
    pthread_create(&t2, NULL, processor, &args); // pass struct
    pthread_create(&t3, NULL, writer, shm2);  // pass shm2 directly

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Destroy semaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    // Free shared memory
    munmap(shm1, SHM_SIZE);
    munmap(shm2, SHM_SIZE);

    printf("File processing done. Check b.txt\n");
    return 0;
}
