#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char** argv)
{
//    char* arg[] = { "ls", "-l", NULL };
    //    char* envp = NULL;
//   char *command[6];
//            command[0]="./main";
//            command[1]="|";
//            command[2]="cat";
//            command[3]=">>";
//            command[4]="file.txt";
//            command[5]=NULL;

//char* arg[3];
//arg[0]="ls";
//arg[1]="-l";
//arg[2]=NULL;
int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
char temp[100];
strcpy(temp,"ls -l ");
strcat(temp,"| cat>>file.txt");
char* arg[] = { "/bin/sh", "-c", temp, NULL };

    printf("parent process before being replaced\n");
//int ar = execv("/home/harsh1695/office/linux/signalfork/main", args);
int ar = execv("/bin/sh", arg);

    if(ar == -1) {
        printf("process creation failed\n");
    }
    printf("this will not print if process replace is succesfull");
    return 0;
}
