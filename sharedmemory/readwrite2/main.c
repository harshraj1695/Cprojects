// writer.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1024
#define MAX_MSG 100

int main() {
    key_t key = ftok("file.txt", 'A');
    int shmid = shmget(key, SIZE, 0666 | IPC_CREAT);
    char *shm = (char *)shmat(shmid, NULL, 0);

    int *index_ptr = (int *)shm;
    char *buffer = shm + sizeof(int);

    if (*index_ptr == 0)
        *index_ptr = 0;  // initialize write index if first time

    while (1) {
          printf("\n--- Chat log ---\n%s", buffer);

        char msg[MAX_MSG];
        printf("writer: Enter message: ");
        fgets(msg, MAX_MSG, stdin);
        msg[strcspn(msg, "\n")] = '\0';

        int offset = *index_ptr;
        int written = snprintf(buffer + offset, SIZE - sizeof(int) - offset, "Reader: %s\n", msg);
        *index_ptr += written;

        char choice;
        printf("Send another? (y/n): ");
        scanf(" %c", &choice);
        getchar();
        if (choice != 'y') break;
    }
//       shmctl(shmid, IPC_RMID, NULL);

    shmdt(shm);
    return 0;
}
