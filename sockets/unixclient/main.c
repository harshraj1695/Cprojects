#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>


#define SOCKET_NAME "/tmp/demosocket"
#define BUFFER_SIZE 128
int main(int argc, char **argv)
{
	struct sockaddr_un addr;
    int i, data_socket, ret;
    char buffer[BUFFER_SIZE];
    
    data_socket=socket(AF_UNIX, SOCK_STREAM, 0);  // step 1
    if(data_socket==-1){
        perror("socket");
        exit(1);
    }
    printf("socket creation successfull\n");
    
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path,SOCKET_NAME,sizeof(addr.sun_path)-1);
    
    
    ret=connect(data_socket,(const struct sockaddr*)&addr, sizeof(struct sockaddr_un)); // step 2
    if(ret==-1){
        fprintf(stderr,"server id down \n");
        exit(1);
    }
    
    printf("connection successful\n");
    
    
    do{   // send data
        printf("enter the no and 0 to quit\n");
        scanf("%d", &i);
        ret= write(data_socket,&i, sizeof(int));
        if(ret==-1){
            perror("write");
            break;
        }
        printf("No of bye send is %d : and data sent is %d\n",ret,i);
    }while(i);
    
    
    // receive result
    
    memset(buffer,0,BUFFER_SIZE);
    ret= read(data_socket, buffer, BUFFER_SIZE);
    if(ret==-1){
        perror("read");
        exit(1);
    }
    buffer[BUFFER_SIZE-1]=0;
    printf("%s",buffer);
    close(data_socket);
    exit(0);
    
    
    
    
    
	return 0;
}
