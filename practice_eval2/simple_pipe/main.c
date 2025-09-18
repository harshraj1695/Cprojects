#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char** argv)
{
    int fd[2];
    int pp = pipe(fd);
    char buffer[100];
    if(pp != 0) {
        perror("pipe");
    }
    int pid = fork();
    if(pid < 0) {
        perror("fork");

    } else if(pid == 0) {
        // chiled process
        printf("In Child process\n");
        close(fd[1]);
        
        
        // one large wole read at a time
        read(fd[0], buffer, sizeof(buffer)-1);
        buffer[99]='\0';
     printf(" string read at child is %s\n", buffer);


// here we need to match the size for each read in buff
//read(fd[0], buffer, 18);   
//buffer[19]='\0';
//printf(" string read at child is %s\n", buffer);
//memset(buffer, 0, sizeof(buffer));
//read(fd[0],buffer,20);
//buffer[21]='\0';
//        printf(" string read at child is %s\n", buffer);
    } else {
        // parent process
        printf("In parent process\n");
        close(fd[0]);
        sprintf(buffer, "parent is writing\n");
        write(fd[1], buffer, strlen(buffer));
        sprintf(buffer, "hii harsh\n");
        write(fd[1], buffer, strlen(buffer));

        wait(NULL);
    }
    return 0;
}
