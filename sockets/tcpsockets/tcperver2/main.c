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

    int new_socket,client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    int max_sd;
    int client_sum[MAX_CLIENTS] = { 0 };

    // Initialize client sockets
    for(int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    // 1. Create socket
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if(listen(master_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for(int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];
            if(sd > 0)
                FD_SET(sd, &readfds);
            if(sd > max_sd)
                max_sd = sd;
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // New connection
        if(FD_ISSET(master_socket, &readfds)) {
            if((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("New client connected: fd %d, ip %s, port %d\n", new_socket, inet_ntoa(address.sin_addr),
                   ntohs(address.sin_port));

            for(int i = 0; i < MAX_CLIENTS; i++) {
                if(client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    client_sum[i] = 0;
                    break;
                }
            }
        }

        // Handle client messages
        for(int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];
            if(FD_ISSET(sd, &readfds)) {
                int num_net, valread;
                valread = read(sd, &num_net, sizeof(int));

                if(valread <= 0) {
                    printf("Client %d disconnected\n", i);
                    close(sd);
                    client_socket[i] = 0;
                    client_sum[i] = 0;
                } else {
                    int num = ntohl(num_net); // convert to host order
                    printf("Client %d sent: %d\n", i, num);

                    if(num == 0) {
                        int sum_net = htonl(client_sum[i]);
                        send(sd, &sum_net, sizeof(sum_net), 0);
                        printf("Sent sum = %d to client %d\n", client_sum[i], i);
                        client_sum[i] = 0; // reset sum
                    } else {
                        client_sum[i] += num;
                        printf("Client %d running sum = %d\n", i, client_sum[i]);
                    }
                }
            }
        }
    }

    return 0;
}
