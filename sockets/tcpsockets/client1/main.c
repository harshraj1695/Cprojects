#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // 1. Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // 2. Set server info
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form (localhost = 127.0.0.1)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at 127.0.0.1:%d\n", PORT);
    printf("Enter integers (send 0 to get sum):\n");

    while (1) {
        int num;
        printf("Enter the no > ");
        scanf("%d", &num);

        // Send number to server
        char msg[50];
        snprintf(msg, sizeof(msg), "%d", num);
        send(sock, msg, strlen(msg), 0);

        // If number is 0, wait for reply (sum)
        if (num == 0) {
            int valread = read(sock, buffer, BUFFER_SIZE - 1);
            if (valread > 0) {
                buffer[valread] = '\0';
                printf("Server: %s", buffer);
            }
        }
    }

    close(sock);
    return 0;
}
