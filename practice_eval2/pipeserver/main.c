#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int mastersock = socket(AF_INET, SOCK_STREAM, 0);
    if (mastersock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(mastersock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(mastersock, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int acceptid = accept(mastersock, NULL, NULL);
        if (acceptid < 0) {
            perror("accept");
            continue;
        }

        char buffer[100];
        memset(buffer, 0, sizeof(buffer));

        int n = read(acceptid, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            close(acceptid);
            continue;
        }
        buffer[n] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            close(acceptid);
            break;
        }

        int fd[2];
        if (pipe(fd) < 0) {
            perror("pipe");
            close(acceptid);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child
            close(fd[0]); // close read end
            dup2(fd[1], STDOUT_FILENO); // redirect stdout to pipe
            dup2(fd[1], STDERR_FILENO); // redirect stderr too
            close(fd[1]);

            char* args[] = { "/bin/sh", "-c", buffer, NULL };
            execv("/bin/sh", args);

            perror("execv failed");
            exit(1);

        } else if (pid > 0) {
            // Parent
            close(fd[1]); // close write end
            char buf[1024];
            ssize_t r;
            while ((r = read(fd[0], buf, sizeof(buf))) > 0) {
                write(acceptid, buf, r);
            }
            close(fd[0]);
            wait(NULL);

            // Mark end of output for client
            write(acceptid, "<END>\n", 6);
        } else {
            perror("fork failed");
            close(acceptid);
        }

        close(acceptid);
    }

    close(mastersock);
    return 0;
}
