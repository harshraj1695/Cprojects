#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SERVER_SOCKET "/tmp/unix_socket"

int main(void)
{
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Create unique client socket path
    struct sockaddr_un client_addr, server_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    memset(&server_addr, 0, sizeof(server_addr));

    client_addr.sun_family = AF_UNIX;
    snprintf(client_addr.sun_path, sizeof(client_addr.sun_path),
             "/tmp/unix_client_%d", getpid());

    unlink(client_addr.sun_path); // in case it exists
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    // Set up server address
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCKET, sizeof(server_addr.sun_path) - 1);

    // Send message to server
    const char *msg = "hello server!";
    if (sendto(sockfd, msg, strlen(msg), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sockfd);
        unlink(client_addr.sun_path);
        exit(1);
    }

    // Receive reply from server
    char buffer[100];
    socklen_t len = sizeof(server_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr*)&server_addr, &len);
    if (n < 0) {
        perror("recvfrom");
    } else {
        buffer[n] = '\0';
        printf("Server reply: %s\n", buffer);
    }

    close(sockfd);
    unlink(client_addr.sun_path); // cleanup client socket file
    return 0;
}
