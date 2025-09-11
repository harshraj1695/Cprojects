#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>   // for struct sockaddr_in

#define PORT 8080

extern struct sockaddr_in serv_addr;

void load_config();

#endif
