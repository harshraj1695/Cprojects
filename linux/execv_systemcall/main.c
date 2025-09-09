#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char** argv)
{
//    char* arg[] = { "li", "-l", NULL };
    //    char* envp = NULL;
    

    printf("parent process before being replaced\n");
    int ar = execve("/home/harsh1695/office", arg, NULL);
    if(ar == -1) {
        printf("process creation failed\n");
    }
    printf("this will not print if process replace is succesfull");
    return 0;
}
