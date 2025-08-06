#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

int main() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    char buffer[1024];
    struct sockaddr_in src;
    socklen_t src_len = sizeof(src);

    printf("Listening for ICMP packets...\n");

    while (1) {
        ssize_t bytes = recvfrom(sock, buffer, sizeof(buffer), 0,
                                 (struct sockaddr*)&src, &src_len);

        if (bytes > 0) {
            struct iphdr *ip = (struct iphdr *)buffer;
            struct icmphdr *icmp = (struct icmphdr *)(buffer + ip->ihl * 4);

            printf("Received ICMP packet from %s\n", inet_ntoa(src.sin_addr));
            printf("ICMP Type: %d, Code: %d\n", icmp->type, icmp->code);
        }
    }

    close(sock);
    return 0;
}
