#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MQ "/mq"
int main(int argc, char** argv)
{
    mqd_t fd;
    unlink(MQ);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;
 
    fd = mq_open(MQ, O_CREAT | O_RDWR, 0666, &attr);
    if(fd == -1) {
        perror("mq_open");
    }
            char* st=NULL;
        size_t size;
    while(1) {
        getline(&st, &size, stdin);
        int a = mq_send(fd, st, strlen(st), 0);
        if(a == -1) {
            perror("mq_send");
        }
        printf("send success\n");
        struct mq_attr attr2;
        if(mq_getattr(fd, &attr2) == -1) {
            perror("mq_getattr");
        } else {
            printf("Current capacity (messages in queue): %ld\n", attr2.mq_curmsgs);
        }
         
    }
    free(st);
    mq_close(fd);
    mq_unlink(MQ);
    return 0;
}
