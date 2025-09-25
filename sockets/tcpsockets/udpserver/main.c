#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080
#define MAX_BUFFER 108

int sockfd = -1;
void handle_sigint(int sig)
{
    printf("\nCaught Ctrl+C, closing connection...\n");
    if(sockfd!= -1) {
        close(sockfd);
    }
    exit(0);
}
int main()
{

    signal(SIGINT, handle_sigint);

    struct sockaddr_in server, client;
    char buffer[MAX_BUFFER];
    socklen_t client_len = sizeof(client);

    // create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // setup server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind
    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while(1) {
        memset(buffer, 0, sizeof(buffer));

        // receive message
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client, &client_len);
        if(n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';

        printf("Received from %s:%d -> %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

        // check EXIT
        if(strcmp(buffer, "EXIT") == 0) {
            printf("Client %s:%d disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            
            continue; // server keeps running
        }

        // echo back
        if(sendto(sockfd, buffer, n, 0, (struct sockaddr*)&client, client_len) < 0) {
            perror("sendto");
        }
    }

    close(sockfd);
    return 0;
}
