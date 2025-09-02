#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        printf("Child (PID=%d) exiting now...\n", getpid());
        exit(0);  
    } else {
        printf("Parent (PID=%d) sleeping, child becomes zombie...\n", getpid());
        sleep(10);  
        printf("Parent now collecting child...\n");
        wait(NULL); 
    }

    return 0;
}
