#include <stdio.h>
int sum(int a,int b){
    return a+b;
}
int sub(int a,int b){
    return a-b;
}
/*
 void printr(int a,int b,int (*fun)(int,int)){
    printf("sum of a and b is %d\n",fun(a,b));
}
 * */
 
int main(int argc, char **argv)
{
//    printr(5,6,sum);
    int (*fun[])(int,int)={sum,sub};
    printf("sum is %d\n",fun[0](5,6));
	return 0;
}
