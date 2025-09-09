#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void* fun(void* client)
{
    pthread_detach(pthread_self());
    char buffer[BUFFER_SIZE];
    int client_fd = *(int*)client;
    free(client);
    if(client_fd < 0) {
        perror("accept failed");
    }

    printf("Client connected.\n");

    // Chat loop with this client
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(client_fd, buffer, sizeof(buffer), 0);
        if(n == 0) {
            printf("Client disconnected.\n");
            close(client_fd);
            break; // go back to accept()
        }
        buffer[n]='\0';
        printf("Client: %s\n", buffer);

        //        printf("Server: ");
        //        fgets(buffer, sizeof(buffer), stdin);
        //        send(*client_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "server got the message\n");
        send(client_fd, buffer, strlen(buffer), 0);
    }
    return NULL;
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    // threading

    // pthread_t thread[8];
    //  Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("socket failed");
        exit(1);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen
    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Main loop: accept new clients
    // int i = 8;
    while(1) {
        pthread_t thread;
        int* new_sock = malloc(sizeof(int));
        int client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
        if(client_fd < 0) {
            perror("accept failed");
            free(new_sock);
            continue;
        }
        *new_sock= client_fd;
        pthread_create(&thread, NULL, fun, new_sock);
    }

    close(server_fd);
    return 0;
}
