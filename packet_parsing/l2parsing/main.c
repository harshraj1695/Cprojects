#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Hex to byte conversion ---
int hextobyte(char c1, char c2)
{
    int hi, lo;

    if(c1 >= '0' && c1 <= '9')
        hi = c1 - '0';
    else if(c1 >= 'A' && c1 <= 'F')
        hi = c1 - 'A' + 10;
    else
        hi = c1 - 'a' + 10;

    if(c2 >= '0' && c2 <= '9')
        lo = c2 - '0';
    else if(c2 >= 'A' && c2 <= 'F')
        lo = c2 - 'A' + 10;
    else
        lo = c2 - 'a' + 10;

    return hi * 16 + lo;
}

// --- Print MAC ---
void printMAC(char* input, int start)
{
    for(int i = 0; i < 6; i++) {
        int b = hextobyte(input[start + i * 2], input[start + i * 2 + 1]);
        printf("%02x", b);
        if(i < 5)
            printf(":");
    }
    printf("\n");
}

// --- Print IPv6 ---
// --- Print IPv6 ---
void printIPv6(char* input, int start)
{
    for(int i = 0; i < 16; i++) {
        int b1 = hextobyte(input[start + i * 2], input[start + i * 2 + 1]);
        if(i % 2 == 0 && i != 0)
            printf(":"); // put colon every 2 bytes
        printf("%02x", b1);
    }
    printf("\n");
}

int main()
{
    char head[1000];
    int ipv4 = 0, ipv6 = 0, tcp = 0, udp = 0;
    int l3size = 0;

    printf("Enter L2+L3+L4 header only hex string and no data:\n");
    scanf("%s", head);
    int ipv6_start = 28; // Ethernet header is 14 bytes, which is 28 hex characters.

    // --- L2 (Ethernet) ---
    printf("Destination MAC address: ");
    printMAC(head, 0);
    printf("Source MAC address: ");
    printMAC(head, 12);

    char type[5];
    strncpy(type, head + 24, 4);
    type[4] = '\0';

    if(strncasecmp(type, "0800", 4) == 0) {
        printf("This is an IPv4 packet\n");
        ipv4 = 1;
    } else if(strncasecmp(type, "86DD", 4) == 0) {
        printf("This is an IPv6 packet\n");
        ipv6 = 1;
    }

    // --- L3 (IPv4) ---
    if(ipv4) {
        int ipv4_start_hex = 28; // Ethernet header is 14 bytes, which is 28 hex characters.

        int firstByte = hextobyte(head[ipv4_start_hex], head[ipv4_start_hex + 1]);
        int ihl = firstByte & 0x0F;
        l3size = ihl * 4;
        printf("IPv4 header size: %d bytes\n", l3size);

        int protocol = hextobyte(head[ipv4_start_hex + 18], head[ipv4_start_hex + 19]);
        printf("Protocol: %d\n", protocol);
        if(protocol == 6) {
            printf("This is a TCP connection..\n");
            tcp = 1;
        }

        else if(protocol == 17) {
            printf("This is a UDP connection..\n");
            udp = 1;
        }

        printf("Source IP: ");
        for(int i = 0; i < 4; i++) {
            int b = hextobyte(head[ipv4_start_hex + 24 + i * 2], head[ipv4_start_hex + 24 + i * 2 + 1]);
            printf("%d", b);
            if(i < 3)
                printf(".");
        }
        printf("\n");

        printf("Destination IP: ");
        for(int i = 0; i < 4; i++) {
            int b = hextobyte(head[ipv4_start_hex + 32 + i * 2], head[ipv4_start_hex + 32 + i * 2 + 1]);
            printf("%d", b);
            if(i < 3)
                printf(".");
        }
        printf("\n");
    }

    // --- L3 (IPv6) ---
    if(ipv6) {
        l3size = 40;

        int next_header = hextobyte(head[ipv6_start + 12], head[ipv6_start + 13]);
        printf("Next Header (L4 Protocol): %d\n", next_header);
        if(next_header == 6)
            tcp = 1;
        else if(next_header == 17)
            udp = 1;

        char src_ipv6[33];                     // 32 hex chars + '\0'
        strncpy(src_ipv6, head + 28 + 16, 32); // 28 = Ethernet, +16 = first 8 hex chars (version etc)
        src_ipv6[32] = '\0';

        char dst_ipv6[33];
        strncpy(dst_ipv6, head + 28 + 48, 32); // +48 = 24 bytes offset in hex
        dst_ipv6[32] = '\0';

        // Print IPv6 in standard : format
        printf("Source IPv6: ");
        for(int i = 0; i < 32; i += 4) {
            printf("%c%c%c%c", src_ipv6[i], src_ipv6[i + 1], src_ipv6[i + 2], src_ipv6[i + 3]);
            if(i < 28)
                printf(":");
        }
        printf("\n");

        printf("Destination IPv6: ");
        for(int i = 0; i < 32; i += 4) {
            printf("%c%c%c%c", dst_ipv6[i], dst_ipv6[i + 1], dst_ipv6[i + 2], dst_ipv6[i + 3]);
            if(i < 28)
                printf(":");
        }
        printf("\n");
    }

    // --- L4 (TCP/UDP) ---
    if(tcp) {
        int tcp_start_hex;
        if(ipv4)
            tcp_start_hex = 28 + l3size * 2;
        else
            tcp_start_hex = 28 + 40 * 2;

        int src_port = (hextobyte(head[tcp_start_hex], head[tcp_start_hex + 1]) << 8) |
                       hextobyte(head[tcp_start_hex + 2], head[tcp_start_hex + 3]);
        int dst_port = (hextobyte(head[tcp_start_hex + 4], head[tcp_start_hex + 5]) << 8) |
                       hextobyte(head[tcp_start_hex + 6], head[tcp_start_hex + 7]);

        printf("TCP Source Port: %d\n", src_port);
        printf("TCP Destination Port: %d\n", dst_port);
    }

    if(udp) {
    int udp_start_hex;
    if(ipv4)
        udp_start_hex = 28 + l3size * 2;   // after IPv4 header
    else
        udp_start_hex = 28 + 40 * 2;       // after IPv6 header

    // --- Source Port ---
    int src_hi = hextobyte(head[udp_start_hex], head[udp_start_hex + 1]);   // first byte of source port
    int src_lo = hextobyte(head[udp_start_hex + 2], head[udp_start_hex + 3]); // second byte
    int src_port = src_hi * 256 + src_lo;    // combine high and low bytes

    // --- Destination Port ---
    int dst_hi = hextobyte(head[udp_start_hex + 4], head[udp_start_hex + 5]); // first byte of dest port
    int dst_lo = hextobyte(head[udp_start_hex + 6], head[udp_start_hex + 7]); // second byte
    int dst_port = dst_hi * 256 + dst_lo;    // combine high and low bytes

    printf("UDP Source Port: %d\n", src_port);
    printf("UDP Destination Port: %d\n", dst_port);
}

    return 0;
}