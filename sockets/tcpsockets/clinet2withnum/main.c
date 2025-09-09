#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080

int sock = -1; // make socket global so handler can access

// Ctrl+C handler
void handle_sigint(int sig)
{
    printf("\nCaught Ctrl+C, closing connection...\n");
    if(sock != -1) {
        close(sock);
    }
    exit(0);
}
int main()
{

    signal(SIGINT, handle_sigint);

//    int sock = 0;
    struct sockaddr_in serv_addr;

    // 1. Create socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // 2. Connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at 127.0.0.1:%d\n", PORT);
    printf("Enter integers (send 0 to get sum):\n");

    while(1) {
        int num;
        printf("Enter the no > ");
        scanf("%d", &num);

        int net_num = htonl(num);
        write(sock, &net_num, sizeof(net_num));

        if(num == 0) {
            int result_net, result;
            int valread = read(sock, &result_net, sizeof(result_net));
            if(valread == sizeof(result_net)) {
                result = ntohl(result_net);
                printf("Server Sum = %d\n", result);
            }
        } else if(num <= 1) {
            close(sock);
            printf("Exiting...\n");
            break;
        }
    }

    return 0;
}
