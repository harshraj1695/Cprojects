#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char **argv){
    
	int fd = open("text.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if(fd==-1){
        printf("eroooor");
        return 1;
    }
    
    char* str=NULL;
    size_t size=0;
    getline(&str, &size, stdin);
    
    
    write(fd, str, strlen(str));
    
    lseek(fd, 0, SEEK_SET);
    
    char buf[20];
    
    int bytes_read=read(fd, buf, 5);
    
    printf("bytes read : %d\n", bytes_read);
    
    buf[bytes_read]='\0';
    
    printf("%s\n", buf);
    
    int bytes_read2=read(fd, buf, 2);
       buf[bytes_read2]='\0';
    
    printf("bytes read2 : %d\n", bytes_read2);
    
    printf("%s\n", buf);
    
	return 0;
}
