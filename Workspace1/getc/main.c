#include <stdio.h>

int main(int argc, char** argv)
{
    FILE* file = fopen("ab.txt", "r");
    int ch = 0;
    if(file == NULL) {
        printf("file not find");
        return 1;
    }
    //    while((ch = getc(file)) != EOF)
    //        putchar(ch);
    //    fclose(file);
    ch = fgetc(stdin);
    printf("%c", ch);

    return 0;
}
