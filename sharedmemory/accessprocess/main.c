// reader.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    key_t key = ftok("file.txt", 'A');
    int shmid = shmget(key, 1024, 0666);
    char *data = (char *)shmat(shmid, NULL, 0);

    printf("Reader read: %s\n", data);

    shmdt(data);
    return 0;
}
