#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/demosocket"
#define BUFFER_SIZE 128

int server_busy = 0; // global busy flag

// recv int safely
int recv_int(int sock, int* value) {
    char* ptr = (char*)value;
    int bytes_read = 0;

    while (bytes_read < sizeof(int)) {
        int ret = read(sock, ptr + bytes_read, sizeof(int) - bytes_read);
        if (ret <= 0)
            return -1; // error or client closed
        bytes_read += ret;
    }
    return 0;
}

// cleanup child processes
void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // free the server
        server_busy = 0;
        printf("Child %d finished. Server free now.\n", pid);
    }
}

int main() {
    struct sockaddr_un addr;
    int ret, socket_conn, data_socket;
    char buffer[BUFFER_SIZE];

    signal(SIGCHLD, sigchld_handler); // reap children

    unlink(SOCKET_NAME); // cleanup

    socket_conn = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_conn == -1) {
        perror("socket");
        exit(1);
    }
    printf("socket() success\n");

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = bind(socket_conn, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    if (ret == -1) {
        perror("bind");
        exit(1);
    }
    printf("bind() success\n");

    ret = listen(socket_conn, 5);
    if (ret == -1) {
        perror("listen");
        exit(1);
    }
    printf("listen() success\n");

    for (;;) {
        printf("Waiting for accept...\n");
        data_socket = accept(socket_conn, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            continue;
        }
        printf("Client connected.\n");

        if (server_busy) {
            strcpy(buffer, "BUSY");
            write(data_socket, buffer, strlen(buffer) + 1);
            close(data_socket);
            printf("Rejected client: server busy.\n");
            continue;
        }

        // mark busy
        server_busy = 1;

        pid_t pid = fork();
        if (pid == 0) {
            // child handles client
            int result = 0;

            strcpy(buffer, "READY");
            write(data_socket, buffer, strlen(buffer) + 1);

            while (1) {
                int data;
                if (recv_int(data_socket, &data) == -1) {
                    break;
                }
                printf("data received = %d\n", data);

                if (data == 0)
                    break;

                result += data;
            }

            // send result
            sprintf(buffer, "result is %d\n", result);
            write(data_socket, buffer, strlen(buffer) + 1);

            close(data_socket);
            printf("Client done. Child exiting.\n");
            exit(0); // child exits
        } else if (pid > 0) {
            // parent closes copy
            close(data_socket);
        } else {
            perror("fork");
            close(data_socket);
            server_busy = 0;
        }
    }

    close(socket_conn);
    unlink(SOCKET_NAME);
    return 0;
}
