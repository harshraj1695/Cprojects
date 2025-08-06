#include <stdio.h>

#define PRT(a,b)\
((a)*(b)); \
 printf("value 1: %d\n",a); \
 printf("value 2: %d\n",b);
/*
#define PRT(a, b)\
printf("value 1: %d\n", a)\
printf("value 2: %d\n", b);
*/
#define AREA(a, b) ((a) * (b));
int main(int argc, char** argv)
{
    printf("hello world\n");
    //   int d=PRT(5,6);
    int d=PRT(5, 6);

    int c = AREA(5, 6);
    printf("%d %d\n", c,d);
    return 0;
}
