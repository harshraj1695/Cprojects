#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/demosocket"
#define BUFFER_SIZE 128

int recv_int(int sock, int* value)
{
    char* ptr = (char*)value;
    int bytes_read = 0;

    while(bytes_read < sizeof(int)) {
        int ret = read(sock, ptr + bytes_read, sizeof(int) - bytes_read);
        if(ret <= 0)
            return -1; // error or client closed
        bytes_read += ret;
    }
    return 0;
}

int main()
{
    struct sockaddr_un addr;
    int ret, socket_conn, data_socket;
    char buffer[BUFFER_SIZE];
    int server_busy = 0; // flag for busy state

    unlink(SOCKET_NAME); // cleanup

    socket_conn = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socket_conn == -1) {
        perror("socket");
        exit(1);
    }
    printf("socket() success\n");

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = bind(socket_conn, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    if(ret == -1) {
        perror("bind");
        exit(1);
    }
    printf("bind() success\n");

    ret = listen(socket_conn, 5);
    if(ret == -1) {
        perror("listen");
        exit(1);
    }
    printf("listen() success\n");

    for(;;) {
        printf("Waiting for accept...\n");
        // inside the accept loop
        data_socket = accept(socket_conn, NULL, NULL);
        if(data_socket == -1) {
            perror("accept");
            exit(1);
        }
        //  Tell client it's ready to accept data
        strcpy(buffer, "READY");
        write(data_socket, buffer, strlen(buffer) + 1);

        int result = 0;
        int bt=0;
        while(1) {
            int data;
            if(recv_int(data_socket, &data) == -1) {
                perror("read");
                break;
            }
            printf("data received = %d\n", data);

            if(data == 0){
                bt=1;
                break;
            }
            

            result += data;
        }

        // Send result back
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "result is %d\n", result);
        printf("sending result back to client\n");
        ret = write(data_socket, buffer, strlen(buffer) + 1);
        if(ret == -1) {
            perror("write");
        }
        if(bt==1)
        close(data_socket);
        server_busy = 0; // free server for next client
        printf("Client done. Server is now free.\n");
    }

    close(socket_conn);
    unlink(SOCKET_NAME);
    return 0;
}
