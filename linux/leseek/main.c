#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    off_t pos;
   int fd=open("file.txt", O_RDWR| O_CREAT, 0666);
   
   char buff[100]="hellow my name is hharsh";  
   printf(",length of string written is %d\n", strlen(buff));
int ll=   write(fd,buff,strlen(buff)); 
  printf("CUREENT fil discriptor  VALIUE IS %d\n",ll);
 pos = lseek(fd,10, SEEK_CUR);
  printf("CUREENT file descriptor VALIUE IS %d\n",pos);
  
  pos =lseek(fd, -20, SEEK_END);
    printf("CUREENT fil discriptor VALIUE IS %d\n",pos);

	return 0;
}