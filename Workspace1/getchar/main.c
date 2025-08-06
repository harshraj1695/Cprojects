#include <stdio.h>

int main(int argc, char **argv)
{
	int ch=0;
    while((ch=getchar())!=EOF)
        printf("%c",ch);
	return 0;
}
