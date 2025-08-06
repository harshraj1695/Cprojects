#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    char write_msg[] = "Message without fork";
    char read_msg[100];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Write to the pipe
    write(fd[1], write_msg, strlen(write_msg) + 1);  // +1 to include null terminator
    close(fd[1]);  // Close write end

    // Read from the pipe
    read(fd[0], read_msg, sizeof(read_msg));
    printf("Read from pipe: %s\n", read_msg);
    close(fd[0]);  // Close read end

    return 0;
}
