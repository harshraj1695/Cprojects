#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
	int fd= open("file.txt", O_RDWR |O_CREAT, 0666);
    printf("FD is cretaed %d\n",fd);
    
    // closing the write fd defult that is 1
    close(1);
    dup(fd);
    printf("writting some darta");
    write(fd,"hiiii",6);
    
	return 0;
}
