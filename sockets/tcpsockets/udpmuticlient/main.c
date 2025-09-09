#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 108
#define MAX_CLIENTS 8

struct client_info {
    struct sockaddr_in addr;
    int active;
};

struct client_info clients[MAX_CLIENTS];

int find_or_add_client(struct sockaddr_in *client) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&
            clients[i].addr.sin_addr.s_addr == client->sin_addr.s_addr &&
            clients[i].addr.sin_port == client->sin_port) {
            return i; // existing client
        }
    }
    // add new client if space available
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].addr = *client;
            clients[i].active = 1;
            return i;
        }
    }
    return -1; // no space
}

int main() {
    int sockfd, ret;
    fd_set readfds;
    struct sockaddr_in server, client;
    socklen_t client_len;
    char buffer[MAX_BUFFER];

    // create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind socket
    ret = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0) {
        perror("bind");
        exit(1);
    }

    printf("UDP server listening on port %d\n", PORT);

    // initialize client list
    memset(clients, 0, sizeof(clients));

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        ret = select(sockfd + 1, &readfds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(sockfd, &readfds)) {
            client_len = sizeof(client);
            memset(buffer, 0, sizeof(buffer));

            int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr *)&client, &client_len);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            buffer[n] = '\0';

            int idx = find_or_add_client(&client);
            if (idx == -1) {
                printf("Max clients reached, ignoring packet from %s:%d\n",
                       inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                continue;
            }

            printf("Client[%d] %s:%d says: %s\n",
                   idx,
                   inet_ntoa(client.sin_addr),
                   ntohs(client.sin_port),
                   buffer);

            // Echo back
            sendto(sockfd, buffer, n, 0,
                   (struct sockaddr *)&client, client_len);

            // Handle exit
            if (strcmp(buffer, "EXIT") == 0) {
                printf("Client[%d] disconnected (%s:%d)\n", idx,
                       inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                clients[idx].active = 0;
            }
        }
    }

    close(sockfd);
    return 0;
}
