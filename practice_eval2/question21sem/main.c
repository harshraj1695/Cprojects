#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>

#define SHM_SIZE 1024
#define SHM1_NAME "/myshm1"
#define SHM2_NAME "/myshm2"
#define SEM1_NAME "/mysem1"
#define SEM2_NAME "/mysem2"

char *shm1;
char *shm2;

sem_t *sem1; 
sem_t *sem2; 

typedef struct {
    char *shm1;
    char *shm2;
} thread_args;

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
        sem_post(sem1); 
        usleep(1000);   
    }

    close(fd);
    strcpy(shm, "EOF");
    sem_post(sem1); 
    return NULL;
}

// Processor thread
void *processor(void *arg) {
    thread_args *args = (thread_args *)arg;
    while (1) {
        sem_wait(sem1); 

        if (strcmp(args->shm1, "EOF") == 0) break;

        if (strlen(args->shm1) == 0) {
            sem_post(sem1); 
            usleep(1000);
            continue;
        }

        char temp[SHM_SIZE];
        int j = 0;
        for (int i = 0; args->shm1[i]; i++)
            if (!is_vowel(args->shm1[i])) temp[j++] = args->shm1[i];
        temp[j] = '\0';

        strcpy(args->shm2, temp);
        sem_post(sem2); 
    }

    strcpy(args->shm2, "EOF");
    sem_post(sem2);
    return NULL;
}

// Writer thread
void *writer(void *arg) {
    char *shm = (char *)arg;
    int fd = open("b.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open b.txt"); return NULL; }

    while (1) {
        sem_wait(sem2);

        if (strcmp(shm, "EOF") == 0) break;

        if (strlen(shm) == 0) {
            sem_post(sem2);
            usleep(1000);
            continue;
        }

        write(fd, shm, strlen(shm));
        write(fd, "\n", 1);
    }

    close(fd);
    return NULL;
}

int main() {
    // Create named shared memory
    int fd1 = shm_open(SHM1_NAME, O_CREAT | O_RDWR, 0666);
    int fd2 = shm_open(SHM2_NAME, O_CREAT | O_RDWR, 0666);
    if (fd1 < 0 || fd2 < 0) { perror("shm_open"); exit(1); }

    ftruncate(fd1, SHM_SIZE);
    ftruncate(fd2, SHM_SIZE);

    shm1 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    shm2 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    sem1 = sem_open(SEM1_NAME, O_CREAT, 0666, 0);
    sem2 = sem_open(SEM2_NAME, O_CREAT, 0666, 0);

    thread_args args = {shm1, shm2};
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, reader, shm1);
    pthread_create(&t2, NULL, processor, &args);
    pthread_create(&t3, NULL, writer, shm2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    // Cleanup
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);

    munmap(shm1, SHM_SIZE);
    munmap(shm2, SHM_SIZE);
    close(fd1);
    close(fd2);
    shm_unlink(SHM1_NAME);
    shm_unlink(SHM2_NAME);

    printf("File processing done. Check b.txt\n");
    return 0;
}
