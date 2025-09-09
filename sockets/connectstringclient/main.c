#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main() {
    int sock;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create client socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type messages (type 'exit' to quit).\n");

    while (1) {
        printf("Enter message: ");
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Send message to server
        if (write(sock, buffer, strlen(buffer)) == -1) {
            perror("write");
            break;
        }

        // Receive reply (if server sends any)
        memset(buffer, 0, BUFFER_SIZE);
        int ret = read(sock, buffer, BUFFER_SIZE - 1);
        if (ret > 0) {
            printf("Server reply: %s\n", buffer);
        } else if (ret == 0) {
            printf("Server closed connection.\n");
            break;
        } else {
            perror("read");
            break;
        }
    }

    close(sock);
    return 0;
}
