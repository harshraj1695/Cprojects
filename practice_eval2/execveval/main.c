#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int mastersock = socket(AF_INET, SOCK_STREAM, 0);
    if(mastersock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(mastersock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if(listen(mastersock, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while(1) {
        int acceptid = accept(mastersock, NULL, NULL);
        if(acceptid < 0) {
            perror("accept");
            continue;
        }

        char buffer[100];
        memset(buffer, 0, sizeof(buffer));

        int n = read(acceptid, buffer, sizeof(buffer) - 1);
        if(n <= 0) {
            close(acceptid);
            continue;
        }
        buffer[n] = '\0';

        if(strcmp(buffer, "exit") == 0) {
            close(acceptid);
            break;
        }

        pid_t pid = fork();
        if(pid == 0) {
            // Child: execute shell command using execv
//            int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//            if(fd < 0) {
//                perror("open");
//                exit(1);
//            }

            char cmd[256];
            snprintf(cmd, sizeof(cmd), "%s | cat >> file.txt", buffer); // buffer has command from client

            char* args[] = { "/bin/sh", "-c", cmd, NULL };
            execv("/bin/sh", args);

            perror("execv failed");
            exit(1);

        } else if(pid > 0) {
            wait(NULL); // wait for child to finish

            // Send file.txt back to client
            int fd = open("file.txt", O_RDONLY);
            if(fd < 0) {
                perror("open file.txt");
                close(acceptid);
                continue;
            }

            char buf[1024];
            ssize_t r;
            while((r = read(fd, buf, sizeof(buf))) > 0) {
                write(acceptid, buf, r);
            }
            close(fd);
        } else {
            perror("fork failed");
            close(acceptid);
        }
    }

    close(mastersock);
    return 0;
}
