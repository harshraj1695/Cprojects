#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd =open("name.txt", O_RDWR | O_CREAT, 0666);
    printf("FD before duplicating %d\n",fd);
    int ne;
    ne=dup(fd);
   printf("FD after duplicating prenset fd %d\n",ne); 
	return 0;
}
