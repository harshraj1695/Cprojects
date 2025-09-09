#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{ 
    printf("system process id %d\n", getppid());
    printf("main parent (%d)\n", getpid());
    
    pid_t pid = fork();
    
    
    if(pid < 0) {
        printf("Fork creation failed \n");
        return 1;
    } 
    else if(pid == 0) {
        printf("child process created with child id %d, parent id is : (%d)\n", getpid(), getppid());
    } 
    else {
        wait(NULL);
        printf(" first parent with parent id (%d)\n", getpid());
        pid_t pid2= fork();
        if(pid2 < 0) {
            printf("Fork creation failed \n");
            return 2;
        } 
        else if(pid2 == 0) {
            printf("child = %d, and parent = (%d)\n", getpid(), getppid() );
        }
        else{
            wait(NULL);
            printf("main parent after fork (%d)\n", getpid());
        }
    }
    return 0;
}
