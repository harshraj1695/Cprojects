#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	pid_t id;
    id=fork();
    
    if(id<0){
        printf("Error ocured \n");
        exit(1);
    }else if(id==0){
        
//        printf("Child process entered with id: %d and paent id: %d\n",getpid(),getppid());
//        fflush(stdout);
//        printf("Entering sleep for 10 sec\n");
//        while(1){
        sleep(2);
        printf("Orphan Child process entered with id: %d and paent id: %d\n",getpid(),getppid());
                fflush(stdout);
//        }
        
    }else{
//        wait(NULL);
        printf("Parent process with id:%d and parent of parent is %d\n", getpid(), getppid());
                fflush(stdout);

        exit(0);
    }
	return 0;
}
