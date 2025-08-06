// client.c

#include <stdio.h>              // For printf
#include <string.h>             // For strlen
#include <unistd.h>             // For close()
#include <arpa/inet.h>          // For socket functions and sockaddr_in

#define SERVER_IP "127.0.0.1"   // Server IP address (localhost)
#define PORT 8080               // Port number to connect to
#define BUFFER_SIZE 1024        // Buffer size for receiving data

int main() {
    int sock;                          // Socket file descriptor
    struct sockaddr_in server_addr;   // Structure to hold server's address info
    char buffer[BUFFER_SIZE];         // Buffer to store data received from server

    // 1. Create a TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET: IPv4
    // SOCK_STREAM: TCP
    // 0: Let OS choose protocol (default TCP)

    // 2. Set server address fields
    server_addr.sin_family = AF_INET;                // IPv4
    server_addr.sin_port = htons(PORT);              // Convert port to network byte order
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    // inet_pton converts SERVER_IP from string ("127.0.0.1") to binary form

    // 3. Connect to the server using socket and address
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // 4. Prepare message to send
    char *message = "Hello from client!";

    // 5. Send the message to the server
    send(sock, message, strlen(message), 0);
    // send(sock, buffer, length_of_data, flags=0)

    // 6. Receive response from server
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    // recv(sock, buffer, max_length, flags=0)

    buffer[bytes_received] = '\0'; // Null-terminate received data
    printf("Server replied: %s\n", buffer);

    // 7. Close the connection
    close(sock);

    return 0;
}
