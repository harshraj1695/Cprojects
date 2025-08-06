#include<stdio.h>

#include<stdlib.h>

#include<string.h>

int main(){

        FILE *fptr;

     //   fptr=fopen("exp.txt","w");

   //     fprintf(fptr,"this is my file");
   fptr=fopen("exp.txt","r");
   char *str=NULL;
   size_t len=0;
   while(getline(&str,&len,fptr)!=-1){
		   printf("%s",str);
		   }
    free(str);
    fclose(fptr);

        return 0;

} 
