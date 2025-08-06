#include <stdio.h>
#include <math.h>

int convert(char *binary, int n){
int i=n-1;
int sum=0;
int j=0;
while(i>=0){
    if(binary[i]=='1'){
        sum+=pow(2,j);
    }
    j++;
    i--;
}
return sum;
}
int main(int argc, char **argv)
{
    int n=0;
    printf("enter the length og bainary no ");
    scanf("%d", &n);
    char binary[n];
    printf("enter the binary string ");
    scanf("%s",binary);
    int res=convert(binary,n);
    printf("%d ",res);
	return 0;
}
