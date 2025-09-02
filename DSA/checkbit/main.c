#include <stdio.h>


void inttobin(int n) {
    if (n == 0) {
        printf("0");
        return;
    }

    int bits[32];  // enough for 32-bit int
    int i = 0;

    while (n > 0) {
        bits[i++] = n % 2;
        n /= 2;
    }

    // print in reverse (MSB → LSB)
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", bits[j]);
    }
}
int main(int argc, char **argv)
{
	int pos=5;
    int num=17;
    inttobin(num);
    int mask=1<<7 | 1<<1;
    printf("\n%d\n",num & mask);
//    inttobin(num^mask);
    
//    int mask2=1<<0;
////    inttobin(num&~mask2);
//printf("%d",num & 1<<3);
	return 0;
}
