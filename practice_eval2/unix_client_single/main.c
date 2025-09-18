#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET "/tmp/unix"
#define PORT 8080

int main(int argc, char** argv)
{
    int master_soc, conn;
    char buffer[100];
    master_soc = socket(AF_UNIX, SOCK_STREAM, 0);
    if(master_soc < 0) {
        perror("socket");
    }
    struct sockaddr_un name;
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET, sizeof(name.sun_path) - 1);
    conn = connect(master_soc, (struct sockaddr*)&name, sizeof(name));
    if(conn<0){
        perror("connect");
    }
    while(1) {
        strcpy(buffer, "hellow from client");
        write(master_soc, buffer, strlen(buffer));

       memset(buffer, 0, sizeof(buffer));
        int n = read(master_soc, buffer, sizeof(buffer));
        if (n <= 0) {
            printf("server disconnected\n");
            break;
        }

        printf("server reply: %s\n", buffer);
        sleep(1); // prevent flooding
    }

    close(master_soc);
    return 0;
}
