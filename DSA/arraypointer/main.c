#include <stdio.h>
int add(int a,int b){
    return a+b;
}
int sub(int a,int b){
    return a-b;
}
int main(int argc, char **argv)
{
    int(*fun[])(int,int)={add,sub};
    printf("%d\n",(*fun[0])(5,6));
    
    int(*fun3)(int,int)=add;
    int(**fun1)(int,int)=&fun3;
    printf("%d\n",(*fun1)(3,5));
	int arr[20];
    for(int i=0;i<20;i++){
        arr[i]=i+1;
    }
    int(*arrptr)[10]=(int(*)[10])arr;
    printf("%d\n",(*(arrptr+1))[0]);
	return 0;
}
