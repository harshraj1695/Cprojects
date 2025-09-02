#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	printf("entering in the main process\n");
	pid_t pid=fork();
	if(pid<0){
		printf("error occured\n");
	}else if(pid==0){
		printf("child prcess created with child id %d\n",getpid());
	  char *args[] = {"./prog1.out", "hgskjg", "djg", "jgkjd", NULL};

        execve("./prog1.out", args, NULL);

        // If execve fails, this will run
        perror("execve failed");
        _exit(1);
    } else {
        int status;
        printf("parent process with pid %d\n", getpid());
        wait(&status);	
return 0;

}
