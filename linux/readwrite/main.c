#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
//	int fd=open("file.txt", O_RDWR || O_CREAT, 0666 );
    
    
    char str[20];
    scanf("%[^\n]",str);
    printf("input string is -> %s\n",str);
    
	return 0;
}
