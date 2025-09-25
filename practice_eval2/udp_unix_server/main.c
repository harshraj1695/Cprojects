#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX "/tmp/unix_socket"

int main(void)
{
    int master_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (master_socket < 0) {
        perror("socket");
        exit(1);
    }
    printf("Socket created successfully\n");

    struct sockaddr_un addr, client;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, UNIX, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';

    unlink(UNIX); // remove any old socket file

    if (bind(master_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(master_socket);
        exit(1);
    }

    printf("Bind successful, waiting for clients...\n");

    char buffer[100];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        socklen_t length = sizeof(client);   // reset before each recvfrom()

        int n = recvfrom(master_socket, buffer, sizeof(buffer)-1, 0,
                         (struct sockaddr*)&client, &length);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';

        printf("Received: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested disconnect, shutting down.\n");
            break;
        }

        const char *reply = "server has received the data";
        int nn = sendto(master_socket, reply, strlen(reply), 0,
                        (struct sockaddr*)&client, length);
        if (nn < 0) {
            perror("sendto");
        }
    }

    close(master_socket);
    unlink(UNIX); // clean up socket file
    return 0;
}
