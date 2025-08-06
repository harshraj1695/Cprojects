#include <stdio.h>

union Data {
    int num;  

    struct {
        unsigned char c1 : 8; 
        unsigned char c2 : 8;
        unsigned char c3 : 8;
        unsigned char c4 : 8;  
    } bytes;
};

int main() {
    union Data d;

    d.num = 0x41424344;  

    printf("Integer: 0x%X\n", d.num);
    printf("Bytes as chars: %c %c %c %c\n", d.bytes.c1, d.bytes.c2, d.bytes.c3, d.bytes.c4);
    printf("Bytes as hex: 0x%X 0x%X 0x%X 0x%X\n", d.bytes.c1, d.bytes.c2, d.bytes.c3, d.bytes.c4);

    return 0;
}
