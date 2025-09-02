#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080

int sock = -1; // global for signal handler

// Ctrl+C handler
void handle_sigint(int sig)
{
    printf("\nCaught Ctrl+C, closing connection...\n");
    if(sock != -1) {
        close(sock);
    }
    exit(0);
}

int main()
{
    signal(SIGINT, handle_sigint);

    struct sockaddr_in serv_addr;

    // 1. Create socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // 2. Connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at 127.0.0.1:%d\n", PORT);

    while(1) {
        char str[256];
        printf("Enter message: ");
        fgets(str, sizeof(str), stdin);   // read string from user

        if(strncmp(str, "exit", 4) == 0) { // exit condition
            printf("Closing connection...\n");
            break;
        }

        // send string to server
        write(sock, str, strlen(str));
    }

    close(sock);
    return 0;
}
