#include <stdio.h>
int sum(int n){
    if(n<=1) return n;
    return n+sum(n-1);
}
int max(int x, int y) {
    return (x > y) ? x : y;
}   
int gcd(int a, int b) {
    if(b == 0)
        return a;
    return gcd(b, a % b);
}

int main(int argc, char **argv)
{
	printf("hello world\n");
    int a=sum(5);
    printf("%d\n",a);
    printf("%d\n",gcd(10,15));
	return 0;
}
