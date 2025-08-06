// writer.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    key_t key = ftok("file.txt", 'A');
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *data = (char *)shmat(shmid, NULL, 0);

    strcpy(data, "Hello from writer!");
    printf("Writer wrote:");

    // keep process alive for reader to read
    sleep(15);

    shmdt(data);
//    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
