#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"   // change if server runs elsewhere
#define SERVER_PORT 2000
#define BUF 256

int sock;

// Thread to continuously receive messages from server
void* receive_handler(void* arg) {
    char buf[BUF];
    while (1) {
        memset(buf, 0, BUF);
        int bytes_read = recv(sock, buf, BUF, 0);

        if (bytes_read <= 0) {
            printf("Disconnected from server.\n");
            close(sock);
            exit(0);
        }

        buf[bytes_read] = '\0';
        printf("\n[Server Broadcast] %s\n", buf);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    // setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 2;
    }

    // connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 3;
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // start thread to listen for server broadcasts
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);
    pthread_detach(recv_thread);

    // main loop for sending messages
    char msg[BUF];
    while (1) {
        memset(msg, 0, BUF);
        printf("You: ");
        fflush(stdout);

        if (fgets(msg, BUF, stdin) == NULL) break;

        // strip newline
        msg[strcspn(msg, "\n")] = '\0';

        // send to server
        if (send(sock, msg, strlen(msg), 0) == -1) {
            perror("send");
            break;
        }

        if (strcmp(msg, "close") == 0) {
            printf("Closing connection.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
