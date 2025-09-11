#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include "config.h"


#define PORT 8080
#define BUFFER_SIZE 256
#define NAME_LEN 32

struct sockaddr_in serv_addr;
char ip[256];
int main()
{
    int sock;
    char buffer[BUFFER_SIZE];
    char name[NAME_LEN];
    fd_set readfds;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }
//    //
//        serv_addr.sin_family = AF_INET;
//        serv_addr.sin_port = htons(PORT);
//    
//        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//            perror("inet_pton");
//            exit(1);
//        }
////
    // loading config data of structure 
    load_config();

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to chat server at 127.0.0.1:%d\n", PORT);
    
    // ask for username
    printf("Enter your name: ");
    fflush(stdout);
    if(fgets(name, NAME_LEN, stdin) == NULL) {
        printf("Input error.\n");
        exit(1);
    }
    name[strcspn(name, "\n")] = '\0';
    send(sock, name, strlen(name), 0);

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);    // stdin
        FD_SET(sock, &readfds); // server
        int max_fd = sock;

        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0) {
            perror("select");
            break;
        }

        // user input
        if(FD_ISSET(0, &readfds)) {
            if(fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                break;
            }
            send(sock, buffer, strlen(buffer), 0);

            if(strncmp(buffer, "exit", 4) == 0) {
                printf("Disconnected.\n");
                break;
            }
        }

        // server messages (chat + realtime status)
        if(FD_ISSET(sock, &readfds)) {
            int valread = read(sock, buffer, BUFFER_SIZE - 1);
            if(valread <= 0) {
                printf("Server disconnected.\n");
                break;
            }
            buffer[valread] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }
    }

    close(sock);
    return 0;
}
