#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    const char *fifo_path = "myfifo";
    mkfifo(fifo_path, 0666); // Create FIFO (in case it doesn't exist)

    char buffer[100];
    int fd = open(fifo_path, O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("Reader got: %s\n", buffer);
    close(fd);

    return 0;
}
