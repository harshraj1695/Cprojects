#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2];  // pipe1: parent to child
    int pipe2[2];  // pipe2: child to parent

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        return 1;
    }

    printf("Pipe1 (Parent → Child): READ FD = %d, WRITE FD = %d\n", pipe1[0], pipe1[1]);
    printf("Pipe2 (Child → Parent): READ FD = %d, WRITE FD = %d\n", pipe2[0], pipe2[1]);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child Process
        printf("[Child PID: %d] started.\n", getpid());
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2

        char msg_from_parent[100];
        read(pipe1[0], msg_from_parent, sizeof(msg_from_parent));
        printf("[Child] Received from parent (via pipe1[0] = %d): %s\n", pipe1[0], msg_from_parent);

        char response[] = "Hello Parent, from Child!";
        write(pipe2[1], response, strlen(response) + 1);
        printf("[Child] Sent response to parent (via pipe2[1] = %d)\n", pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {
        // Parent Process as pid>0
        printf("[Parent PID: %d] started.\n", getpid());
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2

        char msg[] = "Hello Child, from Parent!";
        write(pipe1[1], msg, strlen(msg) + 1);
        printf("[Parent] Sent message to child (via pipe1[1] = %d)\n", pipe1[1]);

        char msg_from_child[100];
        read(pipe2[0], msg_from_child, sizeof(msg_from_child));
        printf("[Parent] Received from child (via pipe2[0] = %d): %s\n", pipe2[0], msg_from_child);

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);  // Wait for child to finish
    }

    return 0;
}
