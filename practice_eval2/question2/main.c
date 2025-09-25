#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SHM_SIZE 1024

typedef struct {
    char *shm1;
    char *shm2;
    sem_t *sem1; // reader -> processor
    sem_t *sem2; // processor -> writer
} thread_args;

void *reader(void *arg) {
    thread_args *args = (thread_args *)arg;
    FILE *fp = fopen("a.txt", "r");
    if (!fp) { perror("fopen a.txt"); return NULL; }

    char buffer[SHM_SIZE];
    while (fgets(buffer, sizeof(buffer), fp)) {
        // remove newline if exists
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(args->shm1, buffer);  // write to shm1
        sem_post(args->sem1);        // signal processor
    }

    fclose(fp);
    strcpy(args->shm1, "EOF"); // mark end
    sem_post(args->sem1);
    return NULL;
}

int is_vowel(char c) {
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
           c=='A'||c=='E'||c=='I'||c=='O'||c=='U';
}

void *processor(void *arg) {
    thread_args *args = (thread_args *)arg;
    while (1) {
        sem_wait(args->sem1);       // wait for reader
        if (strcmp(args->shm1, "EOF") == 0) break;

        char temp[SHM_SIZE];
        int j=0;
        for (int i=0; args->shm1[i]!='\0'; i++) {
            if (!is_vowel(args->shm1[i])) {
                temp[j++] = args->shm1[i];
            }
        }
        temp[j] = '\0';

        strcpy(args->shm2, temp);  // write to shm2
        sem_post(args->sem2);      // signal writer
    }

    strcpy(args->shm2, "EOF");
    sem_post(args->sem2);
    return NULL;
}

void *writer(void *arg) {
    thread_args *args = (thread_args *)arg;
    FILE *fp = fopen("b.txt", "w");
    if (!fp) { perror("fopen b.txt"); return NULL; }

    while (1) {
        sem_wait(args->sem2);       // wait for processor
        if (strcmp(args->shm2, "EOF") == 0) break;

        fprintf(fp, "%s\n", args->shm2);
    }

    fclose(fp);
    return NULL;
}

int main() {
    // create shared memory
    char *shm1 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char *shm2 = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_t sem1, sem2;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    thread_args args = { shm1, shm2, &sem1, &sem2 };

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, reader, &args);
    pthread_create(&t2, NULL, processor, &args);
    pthread_create(&t3, NULL, writer, &args);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    munmap(shm1, SHM_SIZE);
    munmap(shm2, SHM_SIZE);

    printf("File processing done. Check b.txt\n");
    return 0;
}
