// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Step 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Step 3: Connect to server
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Step 4: Loop to send multiple messages from user
int a = 0;
while (a != 1) {
    printf("Enter message to send to server: ");
    fgets(buffer, BUFFER_SIZE, stdin);  // Get message from user

    send(sock, buffer, strlen(buffer), 0);  // Send to server

//    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
//    buffer[bytes_received] = '\0';
//    printf("Server replied: %s\n", buffer);  // Show server reply

    int b;
    printf("Do you want to send another message? (Press 1 to exit, 0 to continue): ");
    scanf("%d", &b);
    getchar(); // Consume leftover newline after scanf

    if (b == 1) a = 1;
}

    
//     Step 5: Send the message
    send(sock, buffer, strlen(buffer), 0);
//
//    // Step 6: Receive and print reply
//    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
//    buffer[bytes_received] = '\0';
//    printf("Server replied: %s\n", buffer);

    // Step 7: Close socket
    close(sock);

    return 0;
}
