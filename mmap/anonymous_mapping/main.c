#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>   // for sysconf
#include <errno.h>

int main(int argc, char **argv)
{
    int size=4096;
	void * ptr=mmap(NULL, size, PROT_READ| PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,0);
    if(ptr==MAP_FAILED){
        perror("mmap");
        return 1;
    }
    
    sprintf(ptr,"hellow from harsh\n");
    printf("%s",(char*)ptr);
    
    munmap(ptr, size);
	return 0;
}
