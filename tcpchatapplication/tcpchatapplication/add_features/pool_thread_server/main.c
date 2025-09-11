#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

// ---------------- Macros ----------------
#define PORT 2000
#define MAX_CLIENTS 10
#define BACKLOG (MAX_CLIENTS * 2)
#define BUF 256

#define NAME_SIZE 20
#define MSG_SIZE 235

// ---------------- Global ----------------
int server_fd;
struct sockaddr_in server_addr;

// client array to store active connections
int clients[MAX_CLIENTS];
int client_count = 0;

// queue for accepted sockets (for thread pool)
int Q[MAX_CLIENTS];
int rear = 0, front = 0, qs = 0;

// sync tools
pthread_mutex_t mutex;
pthread_cond_t cond;

// ---------------- Queue Functions ----------------
void init_queue() {
    for (int i = 0; i < MAX_CLIENTS; i++) Q[i] = -1;
}

void enqueue(int client_fd) {
    pthread_mutex_lock(&mutex);
    if (qs < MAX_CLIENTS) {
        Q[rear] = client_fd;
        rear = (rear + 1) % MAX_CLIENTS;
        qs++;
        pthread_cond_signal(&cond);
    } else {
        printf("Max clients reached. Closing new connection.\n");
        close(client_fd);
    }
    pthread_mutex_unlock(&mutex);
}

int dequeue() {
    pthread_mutex_lock(&mutex);
    while (qs == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    int client_fd = Q[front];
    Q[front] = -1;
    front = (front + 1) % MAX_CLIENTS;
    qs--;
    pthread_mutex_unlock(&mutex);
    return client_fd;
}

// ---------------- Client Management ----------------
void add_client(int fd) {
    pthread_mutex_lock(&mutex);
    if (client_count < MAX_CLIENTS) {
        clients[client_count++] = fd;
    }
    pthread_mutex_unlock(&mutex);
}

void remove_client(int fd) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == fd) {
            // shift remaining clients down
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

// broadcast msg to all connected clients except sender
void broadcast(int sender_fd, const char *msg, int len) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_fd) {
            send(clients[i], msg, len, 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

// ---------------- Thread Function ----------------
void* pool_of_threads(void* arg) {
    while (1) {
        int client_fd = dequeue();
        if (client_fd == -1) continue;

        add_client(client_fd);
        printf("Thread %lu handling client fd=%d\n", pthread_self(), client_fd);

        char buf[BUF];
        while (1) {
            memset(buf, 0, BUF);
            int bytes_read = recv(client_fd, buf, BUF, 0);

            if (bytes_read <= 0) {
                printf("Client fd=%d disconnected\n", client_fd);
                close(client_fd);
                remove_client(client_fd);
                break;
            }

            buf[bytes_read] = '\0';
            printf("Message from fd=%d: %s\n", client_fd, buf);

            if (strncmp(buf, "close", 5) == 0) {
                printf("Closing client fd=%d\n", client_fd);
                close(client_fd);
                remove_client(client_fd);
                break;
            }

            // broadcast to all other clients
            broadcast(client_fd, buf, bytes_read);
        }
    }
    return NULL;
}

// ---------------- Acceptor ----------------
void acceptor() {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd == -1) {
            if (errno == EINTR) break; // interrupted by signal
            perror("accept");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        printf("Accepted connection from %s:%d (fd=%d)\n",
               ip, ntohs(client_addr.sin_port), client_fd);

        enqueue(client_fd);
    }
}

// ---------------- Main ----------------
int main() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 2;
    }

    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        close(server_fd);
        return 3;
    }

    printf("Server listening on port %d...\n", PORT);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    init_queue();

    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, pool_of_threads, NULL);
    }

    acceptor();

    close(server_fd);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
