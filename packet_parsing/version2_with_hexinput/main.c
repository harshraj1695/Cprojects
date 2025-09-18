#include <stdio.h>
#include <stdint.h>

int main() {
     
   unsigned char pkt[] = {
    0x00, 0x16, 0x3e, 0x4a, 0x1b, 0x2c,  
    0x08, 0x00, 0x27, 0x89, 0xab, 0xcd,  
    0x08, 0x00,                           
    0x45, 0x00, 0x00, 0x34,              
    0xa6, 0xf4, 0x00, 0x00,               
    0x40, 0x06, 0xb1, 0xe6,               
    0xc0, 0xa8, 0x01, 0x01,             
    0xc0, 0xa8, 0x01, 0x02,              
    0x00, 0x50, 0x00, 0x50,              
    0x00, 0x00, 0x00, 0x00,              
    0x00, 0x00, 0x00, 0x00,              
    0x50, 0x02, 0x20, 0x00,               
    0x91, 0x7c, 0x00, 0x00                
};

    // L2 Header processing
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        pkt[0], pkt[1], pkt[2], pkt[3], pkt[4], pkt[5]);
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        pkt[6], pkt[7], pkt[8], pkt[9], pkt[10], pkt[11]);

    uint16_t ethertype = (pkt[12] << 8) | pkt[13];
    printf("EtherType: 0x%04x\n", ethertype);

    // L3 with IPV4 Processing
    if(ethertype == 0x0800) {
        printf("This is an IPv4 packet\n");

        int ihl = pkt[14] & 0x0F;
        int l3size = ihl * 4;
        printf("IPv4 header size: %d bytes\n", l3size);

        uint8_t protocol = pkt[23];
        printf("Protocol: %d\n", protocol);

        printf("Source IP: %d.%d.%d.%d\n", pkt[26], pkt[27], pkt[28], pkt[29]);
        printf("Destination IP: %d.%d.%d.%d\n", pkt[30], pkt[31], pkt[32], pkt[33]);
 
    // L4 for TCP or UDP
        if(protocol == 6) { // TCP
            int tcp_start = 14 + l3size;
            uint16_t src_port = (pkt[tcp_start] << 8) | pkt[tcp_start+1];
            uint16_t dst_port = (pkt[tcp_start+2] << 8) | pkt[tcp_start+3];
            printf("TCP Source Port: %u\n", src_port);
            printf("TCP Destination Port: %u\n", dst_port);
        }
        else if(protocol == 17) { // UDP
            int udp_start = 14 + l3size;
            uint16_t src_port = (pkt[udp_start] << 8) | pkt[udp_start+1];
            uint16_t dst_port = (pkt[udp_start+2] << 8) | pkt[udp_start+3];
            printf("UDP Source Port: %u\n", src_port);
            printf("UDP Destination Port: %u\n", dst_port);
        }
    }

    // L3 with IPv6 Processing
    else if(ethertype == 0x86DD) {
        printf("This is an IPv6 packet\n");

        uint8_t next_header = pkt[20];
        printf("Next Header (protocol): %d\n", next_header);

        printf("Source IPv6: ");
        for(int i = 22; i < 38; i += 2) {
            printf("%02x%02x", pkt[i], pkt[i+1]);
            if(i < 36) printf(":");
        }
        printf("\n");

        printf("Destination IPv6: ");
        for(int i = 38; i < 54; i += 2) {
            printf("%02x%02x", pkt[i], pkt[i+1]);
            if(i < 52) printf(":");
        }
        printf("\n");

        if(next_header == 6) { // TCP
            int tcp_start = 14 + 40; // Ethernet + IPv6 fixed header
            uint16_t src_port = (pkt[tcp_start] << 8) | pkt[tcp_start+1];
            uint16_t dst_port = (pkt[tcp_start+2] << 8) | pkt[tcp_start+3];
            printf("TCP Source Port: %u\n", src_port);
            printf("TCP Destination Port: %u\n", dst_port);
        }
        else if(next_header == 17) { // UDP
            int udp_start = 14 + 40; // Ethernet + IPv6 fixed header
            uint16_t src_port = (pkt[udp_start] << 8) | pkt[udp_start+1];
            uint16_t dst_port = (pkt[udp_start+2] << 8) | pkt[udp_start+3];
            printf("UDP Source Port: %u\n", src_port);
            printf("UDP Destination Port: %u\n", dst_port);
        }
    }

    else {
        printf("Unknown EtherType!\n");
    }

    return 0;
}
