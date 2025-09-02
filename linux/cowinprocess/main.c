#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    // when COW copy on write come into ffect with memory space 
//    int a=10;
//    printf("address of a->%d  before fork() %p\n",a,&a);
//    
//	pid_t id=fork();
//    if(id<0){
//        printf("process creation falied");
//    }else if(id==0){
//        printf("child process nad address of a-> %din chaid is %p \n",a, (void*)&a); 
//    }else{
//        printf("parent process with address of a -> %d, is %p\n",a,(void*)&a);
//    }



    
    // Without same memory space 
     int a=10;
    printf("address of a->%d  before fork() %p\n",a,&a);
    
	pid_t id=fork();
    if(id<0){
        printf("process creation falied");
    }else if(id==0){
        a*=10;
        printf("child process nad address of a-> %din chaid is %p \n",a, (void*)&a); 
    }else{
//        wait(NULL);  // first parent and than child -> wait for the child process to complete execution
        printf("parent process with address of a -> %d, is %p\n",a,(void*)&a);
    }
	return 0;
}


