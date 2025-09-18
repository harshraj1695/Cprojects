#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char** argv)
{
    // signal(SIGPIPE,handler);
    pid_t pid;
    int fd[2];
    char buff[100] = "hello from child\n";
    if(pipe(fd) == -1) {
        printf("Error in pipe creation");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if(pid < 0) {
        printf("Error in pipe creation");
        exit(EXIT_FAILURE);
    } else if(pid == 0) { // child reades
        close(fd[1]);
        char str[200];
        int ret;
        while((ret = read(fd[0], str, sizeof(str) - 1)) > 0) {
            str[ret] = '\0';
            printf("Child got: %s\n", str);
        }
//     read(fd[0],str, sizeof(str)-1);
//     str[199]='\0';
//     printf("%s",str);
 
        close(fd[0]);
    } else { // parent writes
        close(fd[0]);
        write(fd[1], buff, strlen(buff)+1);
//        sleep(1);
        write(fd[1], buff, strlen(buff)+1);
//        sleep(1);
        write(fd[1], buff, strlen(buff)+1);
        // printf("ret:%d",ret);
        wait(NULL);
        close(fd[1]);
    }
    return 0;
}