#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"

#define BUFFER_SIZE 128

#define MAX_CLIENT_SUPPORTED 32

/* Array to store monitored file descriptors */

int monitored_fd_set[MAX_CLIENT_SUPPORTED];

/* Initialize the monitored file descriptor set */

static void initialize_monitor_fd_set()
{

    for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {

        monitored_fd_set[i] = -1;
    }
}

/* Add a new file descriptor to the monitored set */

static void add_to_monitored_fd_set(int skt_fd)
{

    for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {

        if(monitored_fd_set[i] == -1) {

            monitored_fd_set[i] = skt_fd;

            break;
        }
    }
}

/* Remove a file descriptor from the monitored set */

static void remove_from_monitored_fd_set(int skt_fd)
{

    for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {

        if(monitored_fd_set[i] == skt_fd) {

            monitored_fd_set[i] = -1;

            break;
        }
    }
}

/* Refresh the fd_set with current monitored file descriptors */

static void refresh_fd_set(fd_set* fd_set_ptr)
{

    FD_ZERO(fd_set_ptr);

    for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {

        if(monitored_fd_set[i] != -1) {

            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

/* Get the highest file descriptor number */

static int get_max_fd()
{

    int max_fd = -1;

    for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {

        if(monitored_fd_set[i] > max_fd) {

            max_fd = monitored_fd_set[i];
        }
    }

    return max_fd;
}

int main()
{

    struct sockaddr_un name;

    int connection_socket, data_socket, ret;

    char buffer[BUFFER_SIZE];

    fd_set readfds;
    unlink(SOCKET_NAME);
    initialize_monitor_fd_set(); // Initialize the monitored FD set

    // Create the master socket (server socket)

    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(connection_socket == -1) {

        perror("socket");

        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(struct sockaddr_un));

    name.sun_family = AF_UNIX;

    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    // Bind the socket to the specified address

    ret = bind(connection_socket, (const struct sockaddr*)&name, sizeof(struct sockaddr_un));

    if(ret == -1) {

        perror("bind");

        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections

    ret = listen(connection_socket, 20);

    if(ret == -1) {

        perror("listen");

        exit(EXIT_FAILURE);
    }

    add_to_monitored_fd_set(connection_socket); // Add master socket to monitored set

    for(;;) {

        refresh_fd_set(&readfds); // Refresh the fd_set with current monitored FDs

        printf("Waiting on select()...\n");

        // Call select() to monitor the file descriptors

        ret = select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);

        if(ret == -1) {

            perror("select");

            exit(EXIT_FAILURE);
        }

        // Check if there is data on the master socket (new client connection)

        if(FD_ISSET(connection_socket, &readfds)) {

            data_socket = accept(connection_socket, NULL, NULL);

            if(data_socket == -1) {

                perror("accept");

                exit(EXIT_FAILURE);
            }

            printf("New client connected\n");

            add_to_monitored_fd_set(data_socket); // Add client socket to monitored set
        }

        // Handle data from existing clients

        for(int i = 0; i < MAX_CLIENT_SUPPORTED; i++) {
            int client_fd = monitored_fd_set[i];

            // Skip the listening socket (only accept() works there)
            if(client_fd != -1 && client_fd != connection_socket && FD_ISSET(client_fd, &readfds)) {
                // Read data from the client
                memset(buffer, 0, BUFFER_SIZE);
                ret = read(client_fd, buffer, BUFFER_SIZE);
                if(ret == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                if(ret == 0) {
                    // Client disconnected
                    printf("Client disconnected\n");
                    close(client_fd);
                    remove_from_monitored_fd_set(client_fd);
                } else {
                    // Print the received string
                    printf("Received message: %s\n", buffer);

                    // Optional: echo back to client
                    write(client_fd, buffer, strlen(buffer));
                }
            }
        }
    }

    close(connection_socket); // Close the master socket when done

    remove_from_monitored_fd_set(connection_socket);

    unlink(SOCKET_NAME); // Clean up the socket file

    return 0;
}
