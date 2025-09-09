#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	FILE *fp=fopen("file.txt","w");
    while(1){
        int a;
        printf("enter -1 to come out from writing from file ");
        scanf("%d",&a);
        getchar();
        if(a==-1)break;
            char text[100];
              scanf("%[^\n]",text);
              fprintf(fp,"%s\n",text);  
//              fprintf(fp,"\n");      
    }
     fclose(fp);

    // Now reopen for reading
    fp = fopen("file.txt", "r");
    if (!fp) {
        perror("File open failed for reading");
        return 1;
    }
    char *line=NULL;
    size_t size=0;
    while(getline(&line, &size, fp)!=-1){
//        line[strcspn(line, "\n")] = '\0';
        printf("%s", line);
    }
	return 0;
}
