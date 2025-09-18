#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>

#define MQ "/mq"
int main(int argc, char **argv)
{
	mqd_t fd;
    char buffer[100];
    fd=mq_open(MQ, O_RDWR);
    if(fd==-1){
        perror("fq_open");
    }
    while(1){
        memset(buffer,0,sizeof(buffer));
    int n=mq_receive(fd,buffer, sizeof(buffer),0);
    if(n<=0){
        printf("message not received\n");
    }
    printf("messgae : %s\n",buffer);
    
    }
	return 0;
}
