#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Thread for sending messages
void* send_thread(void* arg) {
    int sock = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("You: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (send(sock, buffer, strlen(buffer), 0) <= 0) {
            printf("Failed to send. Server may have closed.\n");
            break;
        }
    }

    return NULL;
}

// Thread for receiving messages
void* recv_thread(void* arg) {
    int sock = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sock, buffer, sizeof(buffer), 0);
        if (n == 0) {
            printf("Server disconnected.\n");
            close(sock);
            exit(0);  // terminate client
        }
        printf("\nServer: %s", buffer);
        printf("You: "); // re-show prompt
        fflush(stdout);
    }

    return NULL;
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t tid_send, tid_recv;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Connect to localhost
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf(" Connected to server.\n");

    // Create sender and receiver threads
    pthread_create(&tid_send, NULL, send_thread, &sock);
    pthread_create(&tid_recv, NULL, recv_thread, &sock);

    // Wait for threads to finish
    pthread_join(tid_send, NULL);
    pthread_join(tid_recv, NULL);

    close(sock);
    return 0;
}
