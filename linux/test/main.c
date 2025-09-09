#include <stdio.h>
#include <limits.h>
struct st{
    char ch;
    int a;
    int arr[3];
};
static int add(int a, int b) {
    return a + b;
}

//int (*fun())(int,int) {
//    return add;   // call add
//}
int fun(int,int){
    return add;
}
int main(int argc, char **argv)
{
//	int id=fork();
//    
//    if(id==0){
//        sleep(5);
//    }
//    
//    printf("pid of current process %d\n", getppid());
int a=INT_MAX;
char*ptr=(char*)&a;
printf("%d\n", *(int*)ptr);

double dd=9;
int *ptr1=(int*)&dd;
printf("%lf", *(double*)ptr1);


    
    
    
     //wrong canot access it
//    struct st s={'a',3,{1,2,3}};
//    char *chr=(char*)&s;
//    printf("%c %d",*((char*)chr->ch), *((char*)chr->a));
//    
    
	return 0;
}
