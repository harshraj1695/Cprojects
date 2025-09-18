#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char **argv)
{
	int fd1[2];
    int fd2[2];
    char buff1[100];
    char buff2[100];
    if(pipe(fd1)!=0){
        perror("pipe");
    }
    if(pipe(fd2)!=0){
        perror("pipe");
    }
    printf("In the main process\n");
    int pid=fork();
    
    if(pid<0){
        perror("fork");
    }else if(pid==0){
        printf("In the child process\n");
        close(fd1[0]);
        close(fd2[1]);
        sprintf(buff1,"hello from child\n");
        write(fd1[1],buff1,strlen(buff1));
        
        printf("reading from %d",fd2[0]);
        read(fd2[0],buff2,sizeof(buff2));
        printf("child read : %s\n",buff2);
    }else{
        printf("In the parent process \n");
        close(fd1[1]);
        close(fd2[0]);
        printf("read in parent from child\n");
        read(fd1[0], buff1,sizeof(buff1));
        printf("read from child : %s\n",buff1);
        printf("writing fro parent\n");  
        sprintf(buff2, "hello from parent\n");
        write(fd2[1],buff2,strlen(buff2));
        wait(NULL);
    }
	return 0;
}
