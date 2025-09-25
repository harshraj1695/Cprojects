#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#define PORT 8080
#define BUF_SIZE 1024

// Avoid zombie children
void sigchld_handler(int signo) {
    (void)signo; // unused
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int listenfd, connfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[BUF_SIZE];

    // Create TCP socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow reuse of address
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(listenfd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Handle SIGCHLD to avoid zombie children
    signal(SIGCHLD, sigchld_handler);

    printf("TCP server listening on port %d\n", PORT);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (connfd < 0) {
            perror("accept failed");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            close(connfd);
            continue;
        }

        if (pid == 0) {
            // Child process
            close(listenfd); // close listening socket in child
            printf("Client connected: %s:%d\n", 
                    inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            // Echo server logic (can be replaced with YCP protocol handling)
            int n;
            while ((n = read(connfd, buffer, BUF_SIZE-1)) > 0) {
                buffer[n] = '\0';
                printf("Received from client: %s\n", buffer);
                write(connfd, buffer, n); // echo back
            }

            printf("Client disconnected\n");
            close(connfd);
            exit(0); // child exits
        } else {
            // Parent process
            close(connfd); // parent closes connected socket
        }
    }

    close(listenfd);
    return 0;
}
