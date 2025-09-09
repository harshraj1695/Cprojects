#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/testqueue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    struct mq_attr attr;

    // cleanup old queue
//    mq_unlink(QUEUE_NAME);

    // set attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    char buffer[MAX_SIZE];
    printf("Enter message: ");
    fgets(buffer, MAX_SIZE, stdin);

    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    printf("Message sent: %s\n", buffer);

    mq_close(mq);
    return 0;
}
