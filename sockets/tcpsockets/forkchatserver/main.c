#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_fd, client_fds[MAX_CLIENTS];
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    fd_set readfds;
    int max_fd, activity;

    // Initialize client array
    for (int i = 0; i < MAX_CLIENTS; i++) client_fds[i] = -1;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Clear set
        FD_ZERO(&readfds);

        // Add server socket
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        // Add all clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] > 0)
                FD_SET(client_fds[i], &readfds);
            if (client_fds[i] > max_fd)
                max_fd = client_fds[i];
        }

        // Wait for activity
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            // New connection
            new_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_fd < 0) {
                perror("accept");
                continue;
            }
            printf("New client connected.\n");

            // Add to client array
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] == -1) {
                    client_fds[i] = new_fd;
                    break;
                }
            }
        }

        // Check all clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = client_fds[i];
            if (fd > 0 && FD_ISSET(fd, &readfds)) {
                memset(buffer, 0, sizeof(buffer));
                int n = recv(fd, buffer, sizeof(buffer), 0);
                if (n <= 0) {
                    printf("Client disconnected.\n");
                    close(fd);
                    client_fds[i] = -1;
                } else {
                    printf("Message: %s", buffer);

                    // Broadcast to all clients
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_fds[j] > 0 && client_fds[j] != fd) {
                            send(client_fds[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
