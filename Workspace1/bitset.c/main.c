#include <math.h>
#include <stdio.h>

int count(int n)
{
    int count = 0;
    while(n) {
        n = n & (n - 1); // remove the lowest set bit
        count++;
    }
    return count;
}
// brut force approach for checking each bit if it is set  not
int brut(int n)
{
    int size = log2(n) + 1; // used to fid the the no ofbits required to represnt n in binary no
    //    printf("%d\n",size);
    int cnt = 0;
    for(int i = 0; i < size; i++) {
        if((n >> i) & 1) {
            cnt++;
        }
    }
    return cnt;
}
int main(int argc, char** argv)
{
    printf("hello world\n");
    int a = count(0x77);
    int aa = brut(0x77);
    printf("%d %d\n", a, aa);
    return 0;
}
