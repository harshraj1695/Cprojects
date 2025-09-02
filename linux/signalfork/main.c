#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void sig_handler(int signum) {
    printf("Caught signal %d\n", signum);
}

int main(int argc, char **argv) {
    printf("currently working in main function \n");
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        printf("Child process (pid=%d)\n", getpid());
        signal(SIGUSR1, sig_handler);
        // wait for a signal
        pause();
        printf("Child exiting\n");
    } else {
        sleep(2);  // give child time to set handler
        printf("Parent sending SIGUSR1 to child (pid=%d)\n", pid);
        kill(pid, SIGUSR1);
        wait(NULL);
        sleep(2);
        printf("Parent exiting\n");
    }
    return 0;
}
