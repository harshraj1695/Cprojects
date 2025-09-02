#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define QUEUE_PERMISSIONS 0660
#define MSQ "/testqueue"


int main(int argc, char **argv) {

    char buffer[MSG_BUFFER_SIZE];
    int msgq_fd = 0;

   
    /* To set msgQ attributes */
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((msgq_fd = mq_open(MSQ, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        printf("Client: mq_open failed, errno = %d\n", errno);
        exit(1);
    }

    while (1) {
        // Directly block on mq_receive without select
        memset(buffer, 0, MSG_BUFFER_SIZE);

        // Receive a message from the queue (blocks until a message is available)
        if (mq_receive(msgq_fd, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            printf("mq_receive error, errno = %d\n", errno);
            exit(1);
        }

        // Print received message
        printf("Message received from Queue = %s\n", buffer);
    }

    // Close the message queue (though this is never reached due to the infinite loop)
    mq_close(msgq_fd);
}