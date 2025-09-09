#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define MSQ "/testqueue"

int main(int argc, char** argv)
{
    char buffer[MSG_BUFFER_SIZE];
    mqd_t recvr_msgq_fd;

    // Open queue (server must have created it)
    recvr_msgq_fd = mq_open(MSQ, O_WRONLY);
    if (recvr_msgq_fd == (mqd_t)-1) {
        perror("Client: mq_open failed");
        exit(1);
    }

    while (1) {
        memset(buffer, 0, MSG_BUFFER_SIZE);

        printf("Enter msg to be sent to receiver (type 'close' to quit): ");
        if (fgets(buffer, MSG_BUFFER_SIZE, stdin) == NULL)
            continue;

        // Remove newline at the end
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "close") == 0) {
            mq_close(recvr_msgq_fd);
            printf("Client: Connection closed.\n");
            return 0;
        }

        if (mq_send(recvr_msgq_fd, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Client: Not able to send message to server");
            exit(1);
        }
    }

    mq_close(recvr_msgq_fd);
    return 0;
}
