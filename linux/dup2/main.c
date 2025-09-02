#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd=open("new.txt", O_RDWR|O_CREAT, 0666);
    printf("file is created and a file descriptor with value %d\n", fd);
    
    dup2(fd,1);
    int a=fclose(1);
    if(a==0){
        printf("dhgfd\n");
    }else{
        printf("dgfhgddfdkdkfkk\n");
    }
    printf("writing in file not on termnial with printf function wit file fd %d",fd);
    
	return 0;
    
}
