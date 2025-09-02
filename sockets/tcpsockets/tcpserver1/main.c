#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int master_socket=-1; // make socket global so handler can access

// Ctrl+C handler
void handle_sigint(int sig)
{
    printf("\nCaught Ctrl+C, closing connection...\n");
    if(master_socket != -1) {
        close(master_socket);
    }
    exit(0);
}
int main()
{

    signal(SIGINT, handle_sigint);

    int new_socket, client_socket[MAX_CLIENTS];
    int client_sum[MAX_CLIENTS]; // store running sum per client
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    fd_set readfds;
    char buffer[BUFFER_SIZE];

    // Initialize all client sockets and sums
    for(int i = 0; i < MAX_CLIENTS; i++) {
        client_socket[i] = 0;
        client_sum[i] = 0;
    }

    // 1. Create master socket
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Prepare server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. Bind socket
    if(bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    // 4. Listen
    if(listen(master_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Clear and set descriptors
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        int max_sd = master_socket;

        // Add child sockets
        for(int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];
            if(sd > 0)
                FD_SET(sd, &readfds);
            if(sd > max_sd)
                max_sd = sd;
        }

        // Wait for activity
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }

        // New connection
        if(FD_ISSET(master_socket, &readfds)) {
            if((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New client connected: fd %d, ip %s, port %d\n", new_socket, inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));

            // Add new socket
            for(int i = 0; i < MAX_CLIENTS; i++) {
                if(client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    client_sum[i] = 0; // reset sum for new client
                    break;
                }
            }
        }

        // Check IO for existing clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];

            if(FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, BUFFER_SIZE);
                if(valread <= 0) {
                    // Client disconnected
                    close(sd);
                    client_socket[i] = 0;
                    client_sum[i] = 0;
                    printf("Client %d disconnected\n", i);
                } else {
                    buffer[valread] = '\0';
                    int num = atoi(buffer); // convert received string to int

                    if(num == 0) {
                        // Send sum and reset
                        char result[50];
                        snprintf(result, sizeof(result), "Sum = %d\n", client_sum[i]);
                        send(sd, result, strlen(result), 0);
                        printf("Client %d requested sum, sent: %s", i, result);
                        client_sum[i] = 0;
                    } else {
                        client_sum[i] += num;
                        printf("Client %d sent %d, current sum = %d\n", i, num, client_sum[i]);
                    }
                }
            }
        }
    }

    return 0;
}
