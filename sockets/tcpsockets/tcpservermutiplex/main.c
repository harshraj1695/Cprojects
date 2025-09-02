#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENT 8

int MONITORED_FD[MAX_CLIENT];

int master_socket=-1;

// Ctrl+C handler
void handle_sigint(int sig)
{
    printf("\nCaught Ctrl+C, closing connection...\n");
    if(master_socket != -1) {
        close(master_socket);
    }
    exit(0);
}


static void initialise()
{
    for(int i = 0; i < MAX_CLIENT; i++) {
        MONITORED_FD[i] = -1;
    }
}

static void add_to_monitorfd(int fd)
{
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(MONITORED_FD[i] == -1) {
            MONITORED_FD[i] = fd;
            break;
        }
    }
}

static void re_init(fd_set* readfds)
{
    FD_ZERO(readfds);
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(MONITORED_FD[i] != -1) {
            FD_SET(MONITORED_FD[i], readfds);
        }
    }
}

static int get_maxfd()
{
    int maxi = -1;
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(MONITORED_FD[i] > maxi) {
            maxi = MONITORED_FD[i];
        }
    }
    return maxi;
}

void remove_fd(int fd)
{
    for(int i = 0; i < MAX_CLIENT; i++) {
        if(MONITORED_FD[i] == fd) {
            MONITORED_FD[i] = -1;
        }
    }
}

int main()
{
    
    signal(SIGINT,handle_sigint);
    int comm_socket;
    fd_set readfd;
    struct sockaddr_in server, client;
    char buffer[108];
    socklen_t client_len;
    int ret;

    initialise();

    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(master_socket == -1) {
        perror("socket");
        exit(1);
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    ret = bind(master_socket, (struct sockaddr*)&server, sizeof(server));
    if(ret == -1) {
        perror("bind");
        exit(1);
    }

    ret = listen(master_socket, MAX_CLIENT);
    if(ret == -1) {
        perror("listen");
        exit(1);
    }

    add_to_monitorfd(master_socket);

    while(1) {
        FD_ZERO(&readfd);
        re_init(&readfd);

        printf("blocking at select...\n");
        ret = select(get_maxfd() + 1, &readfd, NULL, NULL, NULL);
        if(ret == -1) {
            perror("select");
            exit(1);
        }

        // Check master socket for new connection
        if(FD_ISSET(master_socket, &readfd)) {
            printf("new connection\n");
            client_len = sizeof(client);
            comm_socket = accept(master_socket, (struct sockaddr*)&client, &client_len);
            if(comm_socket == -1) {
                perror("accept");
                exit(1);
            }
            add_to_monitorfd(comm_socket);
            printf("connection established (fd=%d)\n", comm_socket);
        } else {
            // Check all client sockets
            for(int i = 0; i < MAX_CLIENT; i++) {
                if(MONITORED_FD[i] != -1 && FD_ISSET(MONITORED_FD[i], &readfd)) {
                    comm_socket = MONITORED_FD[i];
                    memset(buffer, 0, sizeof(buffer));
                    int n = read(comm_socket, buffer, sizeof(buffer) - 1);
                    if(n <= 0) {
                        printf("Client (fd=%d) disconnected\n", comm_socket);
                        close(comm_socket);
                        remove_fd(comm_socket);
                    } else {
                        buffer[n] = '\0';
                        printf("server read: %s\n", buffer);
                        if(strcmp(buffer, "EXIT") == 0) {
                            printf("Closing client %d\n", comm_socket);
                            close(comm_socket);
                            remove_fd(comm_socket);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
