#include <stdio.h>
void foo(int **ptr){
 int a=39;
 *ptr=&a; // its like derefrencibg double pointer to single pointer
}
/* by using single pointer
 void foo(int *ptr){
  int a=39;
   ptr=&a;
    // *ptr=a   // her it will change a to 39 because we are modifing the value directly
    }
     */
     
int main(int argc, char **argv)
{
	printf("hello world\n");
    int a=30;
    int *ptr=&a;
    foo(&ptr); // using double pointer
    // foo(ptr); //using single pointer
    printf("%d\n",*ptr);
	return 0;
}
