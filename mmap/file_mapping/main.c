#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>   // for sysconf
#include <errno.h>
 #include <fcntl.h>
 #include <string.h>
 

size_t size = 4096;

int main(int argc, char **argv)
{
	int fd=open("file.txt", O_RDWR | O_CREAT, 0666);
     // 2. Resize file so it's at least "size" bytes
    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }
    printf("%d",fd);
     
    char *ptr =mmap(NULL, size, PROT_READ | PROT_WRITE , MAP_SHARED, fd,0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // 4. Write into the mapping
    const char *msg = "hello from harsh\n";
    snprintf(ptr, size, "%s", msg);

//     5. Flush changes to disk
//    if (msync(ptr, size, MS_SYNC) == -1) {
//        perror("msync");
//    }

    // 6. Clean up
    if (munmap(ptr, size) == -1) {
        perror("munmap");
    }
    close(fd);

    printf("Message written via mmap: %s", msg);
    return 0;
}