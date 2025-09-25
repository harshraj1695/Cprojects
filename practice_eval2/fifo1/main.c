#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>   // for mkfifo()


#define FIFO "/tmp/fifo"

int main(int argc, char **argv)
{
	mkfifo(FIFO,0666);
    
int fd = open(FIFO, O_RDWR);
if(fd < 0) {
    perror("open fifo");
    exit(1);
}
    char buffer[100];
    strcpy(buffer,"hii i ma harsh ");
    
    write(fd,buffer,strlen(buffer)+1);
    char buff[100];
    read(fd,buff,sizeof(buff)-1);
    buff[99]='\0';
    printf("%s ",buff);
    
    close (fd);
	return 0;
}
