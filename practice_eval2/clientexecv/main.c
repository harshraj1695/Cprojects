#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    while(1) {
        char command[100];
        printf("Enter command (or 'exit' to quit): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // remove newline

        write(sock, command, strlen(command));

        if(strcmp(command, "exit") != 0) {
            char buffer[1024];
            ssize_t n;
            while((n = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[n] = '\0';
                 if(strstr(buffer, "<END>\n")) break;
                printf("%s", buffer);
            }
        }else{
            close(sock);
            break;
        }
    }
    close(sock);
    return 0;
}
