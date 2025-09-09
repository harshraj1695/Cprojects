#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define THREAD_POOL_SIZE 4

typedef struct {
    int fd;
    int action; // 0 = READ, 1 = WRITE
    char buffer[BUFFER_SIZE];
    int length;
} task_t;

task_t task_queue[MAX_CLIENTS];
int front = 0, rear = 0;
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void enqueue(task_t task)
{
    pthread_mutex_lock(&queue_lock);
    task_queue[rear] = task;
    rear = (rear + 1) % MAX_CLIENTS;
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_lock);
}

task_t dequeue()
{
    pthread_mutex_lock(&queue_lock);
    while(front == rear) { // queue empty
        pthread_cond_wait(&queue_cond, &queue_lock);
    }
    task_t task = task_queue[front];
    front = (front + 1) % MAX_CLIENTS;
    pthread_mutex_unlock(&queue_lock);
    return task;
}

void* worker_thread(void* arg)
{
    pthread_detach(pthread_self());

    while(1) {
        task_t task = dequeue();

        if(task.action == 0) { // READ
            int n = recv(task.fd, task.buffer, BUFFER_SIZE, 0);
            if(n <= 0) {
                printf("Client disconnected (fd=%d)\n", task.fd);
                close(task.fd);
            } else {
                task.buffer[n] = '\0';
                printf("Client(fd=%d) says: %s\n", task.fd, task.buffer);

                // echo back -> create new WRITE task
                task_t write_task;
                write_task.fd = task.fd;
                write_task.action = 1;
                strcpy(write_task.buffer, task.buffer);
                write_task.length = strlen(task.buffer);
                enqueue(write_task);
            }
        } else if(task.action == 1) { // WRITE
            send(task.fd, task.buffer, task.length, 0);
        }
    }
    return NULL;
}

void handler(int sig)
{
    printf("\nCTRL+C caught, shutting down server...\n");
    exit(0);
}

int main()
{
    signal(SIGINT, handler);

    int server_fd, max_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    int client_fds[MAX_CLIENTS];
    fd_set readfds;

    // Init client list
    for(int i = 0; i < MAX_CLIENTS; i++)
        client_fds[i] = -1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(1);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Start worker threads
    for(int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, worker_thread, NULL);
    }

    // Dispatcher loop with select()
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(client_fds[i] != -1) {
                FD_SET(client_fds[i], &readfds);
                if(client_fds[i] > max_fd)
                    max_fd = client_fds[i];
            }
        }

        if(select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select error");
            continue;
        }

        // New client connection
        if(FD_ISSET(server_fd, &readfds)) {
            int new_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
            if(new_fd < 0) {
                perror("accept failed");
                continue;
            }

            printf("New client connected (fd=%d)\n", new_fd);
            for(int i = 0; i < MAX_CLIENTS; i++) {
                if(client_fds[i] == -1) {
                    client_fds[i] = new_fd;
                    break;
                }
            }
        }

        // Check existing clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(client_fds[i] != -1 && FD_ISSET(client_fds[i], &readfds)) {
                task_t task;
                task.fd = client_fds[i];
                task.action = 0; // READ
                enqueue(task);
            }
        }
    }

    close(server_fd);
    return 0;
}
