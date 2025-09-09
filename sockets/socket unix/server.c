#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>


#define SOCKET_NAME "/tmp/demosocket"
#define BUFFER_SIZE 128

int main(){
    struct sockaddr_un addr;
    int ret, socket_conn, data_socket, data;
    char buffer[BUFFER_SIZE];
    
    unlink(SOCKET_NAME);  // step 1
    socket_conn =socket(AF_UNIX, SOCK_STREAM, 0);    //step 2
    if(socket_conn==-1){
        perror("socket");
        exit(1);
    }
    printf("socket() success\n");

    memset(&addr, 0, sizeof( struct sockaddr_un));        // preparintg the structure addr
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME,sizeof(addr.sun_path)-1);


    ret=bind(socket_conn, (struct sockaddr *) &addr,sizeof(struct sockaddr_un)); // step 3
    if(ret==-1){
        perror("bind");
        exit(1);
    }


    printf("bind() success\n");
    ret=listen(socket_conn,5);
    if(ret==-1){
        perror("listen");
        exit(1);
    }
    printf("listen() success\n");

    while(1){ // becasuse server is 24/7 active

        printf("Waiting for accept");
        data_socket=accept(socket_conn, NULL, NULL);  // step 4
        if(data_socket==-1){
            perror("accept");
            exit(1);
        }
        printf("accept successs\n");

        int result=0;
        while(1){ /// data process condion 
           memset(buffer, 0,BUFFER_SIZE);
           printf("waiting for data ro client \n");
           ret= read(data_socket, buffer,BUFFER_SIZE);  //step 5
           if(ret==-1){
            perror("read");
            exit(1);
           }

           memcpy(&data, buffer, sizeof(int));   // receving dataaa
           if(data==0)break;  // after dtaa ==0 we cannot get data condion
           result+=data;
        }

        close(data_socket);
    }
    close(socket_conn);
    printf("connection close ...\n");
    unlink(SOCKET_NAME);
    exit(0);

}