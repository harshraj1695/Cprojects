#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("entering in the main process\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        printf("child process created with child id %d\n", getpid());

        char *args[] = {"./a.out", "hgskjg", "djg", "jgkjd", NULL};

        execve("./a.out", args, NULL);

        // If execve fails, this will run
        perror("execve failed");
        _exit(1);
    } else {
        int status;
        printf("parent process with pid %d\n", getpid());
        wait(&status);

        if (WIFEXITED(status)) {
            printf("child exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}

