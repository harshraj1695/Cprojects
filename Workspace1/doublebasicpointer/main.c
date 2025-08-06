#include <stdio.h>

int main(int argc, char **argv)
{
    int a=5;
    int*ab=&a;
    int **aab=&ab;
    printf("%p  add ab\n",ab);
    printf("%d *ab\n",*ab);
    printf("%p &ab\n",&ab);
    printf("%p *aab\n",*aab);
        printf("%p aab\n",aab);

    printf("%d\n",&a==*aab);
	return 0;
}
