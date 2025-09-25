#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SHM "/shm1"

#define SHM_SIZE 2048
int main(int argc, char **argv)
{
	int fd=shm_open(SHM,O_CREAT | O_RDWR,0666);
    
    ftruncate(fd,SHM_SIZE);
    char *fp = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  sprintf(fp, "hii harsh raj singh");
  
  char* ptr=NULL;
  ssize_t size;
  int n=3;
//  while(n){
//    getline(&ptr,&size, stdin);
//            strncat(fp, ptr, SHM_SIZE - strlen(fp) - 1);
//    n--;
//  }
//  n=4;
int fd2=open("file.txt",O_RDWR,0666);
char buffer[SHM_SIZE];
while((n=read(fd2,buffer,SHM_SIZE-1))>0){
    buffer[n]='\0';
    strncat(fp,buffer,SHM_SIZE-1);
}
int fd1 = open("fie.txt", O_CREAT | O_WRONLY, 0666);
      write(fd1,fp,strlen(fp));
  munmap(fp,SHM_SIZE);
    shm_unlink(SHM);

	return 0;
}
