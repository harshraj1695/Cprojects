#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void signal_handler(int signum){
    printf("\nCTRL + C is pressed program exiting \n");
    exit(1);
}
int main(int argc, char **argv)
{
    printf("Process id is %d/n",getpid());
	printf("hello world\n");
    signal(SIGINT,signal_handler);
    while(1){
        printf("press ctrl +c to break the program\n");
        sleep(1);
    }
	return 0;
}
