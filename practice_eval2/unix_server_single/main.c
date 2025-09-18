#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/un.h>


#define SOCKET "/tmp/unix"
#define PORT 8080

int main(int argc, char** argv)
{
    int master_soc, acept_sock;
    char buffer[200];

    unlink(SOCKET);
    master_soc = socket(AF_UNIX, SOCK_STREAM, 0);

    if(master_soc < 0) {
        perror("socket");
        exit(1);
    }
    printf("master socket created \n");
    struct sockaddr_un name;
    memset(buffer, 0, sizeof(buffer));
    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET, sizeof(name.sun_path) - 1);

    if(bind(master_soc,(struct sockaddr*)&name,sizeof(name))<0){
        perror("bind");
        exit(1);
    }
    printf("bind operation sucess\n");
    
    if(listen(master_soc,5)<0){
        perror("listen");
        exit(1);
    }
    printf("listen call success\n");
    
    
    while(1){
        
        printf("Wiating sor acceept\n");
        acept_sock = accept(master_soc, NULL, NULL);
        if(acept_sock < 0) {
            perror("accept");
            exit(1);
        }
        printf("Connection successful\n");
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            int n=read(acept_sock, buffer, sizeof(buffer));
            if(n<=0){
                printf("client disconnected\n");
                close(acept_sock);
                break;
            }
            printf("server reads : %s\n", buffer);

            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "server got you info\n");
            write(acept_sock, buffer, strlen(buffer));
        }
        close(acept_sock);
    }
    
    

    return 0;
}
