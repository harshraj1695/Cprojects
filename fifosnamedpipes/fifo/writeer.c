#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    const char *fifo_path = "myfifo";
    mkfifo(fifo_path, 0666); // Create FIFO

    char message[] = "Hello from writer!";
    int fd = open(fifo_path, O_WRONLY);
    write(fd, message, strlen(message) + 1);
    close(fd);

    return 0;
}

