#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    srand(5);
    int row=5;
    int col=9;
	int **arr=(int**)malloc(sizeof(int*)*row);
    for(int i=0;i<row;i++){
        arr[i]=(int*)malloc(sizeof(int)*col);
    }
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            arr[i][j]=2+rand()%(12-2+1);
        }
    }
    
    for(int i=0;i<row;i++){
        
        for(int j=0;j<col;j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
	return 0;
}
