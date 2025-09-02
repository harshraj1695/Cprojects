#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define SOCKET_NAME "/tmp/demosocket"
#define BUFFER_SIZE 128

int try_connect() {
    int sock;
    struct sockaddr_un addr;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    if (connect(sock, (const struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sock);
        return -1;
    }
    return sock;
}

int main() {
    int data_socket, ret, i;
    char buffer[BUFFER_SIZE];

    data_socket = try_connect();
    if (data_socket == -1) {
        fprintf(stderr, "Server is down, retry later.\n");
        exit(1);
    }

    // Read server response
    memset(buffer, 0, BUFFER_SIZE);
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret <= 0) {
        perror("read");
        close(data_socket);
        exit(1);
    }

    if (strcmp(buffer, "BUSY") == 0) {
        printf("Server says: BUSY. Exiting.\n");
        close(data_socket);
        return 0;
    }

    if (strcmp(buffer, "READY") != 0) {
        printf("Unexpected server message: %s\n", buffer);
        close(data_socket);
        return 0;
    }

    printf("Server is READY. Start sending numbers.\n");

    do {
        printf("Enter number (0 to quit): ");
        scanf("%d", &i);
        ret = write(data_socket, &i, sizeof(int));
        if (ret == -1) {
            perror("write");
            break;
        }
        printf("Sent %d\n", i);
    } while (i);

    // receive result
    memset(buffer, 0, BUFFER_SIZE);
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret > 0) {
        printf("Server replied: %s\n", buffer);
    }

    close(data_socket);
    return 0;
}
