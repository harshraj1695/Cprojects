#include <stdio.h>
#include <stdlib.h>
 #include <alloca.h>
int main(int argc, char **argv)
{
	int *arr;
    
    printf("Doing malloc of size 10\n ");
    arr=(int*)malloc(10*sizeof(int));
    for(int i=0;i<10;i++){
       arr[i]=i+1;
    }
    printf("printfing the array o 10 malloc\n");
    for(int i=0;i<10;i++){
        printf("arr[%d] -> %d\n",i,arr[i]);
    }
    int *arr1;
    
    
    
    printf("Doing calloc of size 10\n ");
    arr1=(int*)calloc(10, sizeof(int));
    for(int i=0;i<10;i++){
       arr1[i]=i+10;
    }
    printf("printfing the array o 10 calloc\n");
    for(int i=0;i<10;i++){
        printf("arr1[%d] -> %d\n",i,arr1[i]);
    }
    
        
        
        printf("Doing realloc of size 10\n ");
    arr=(int*)realloc(arr, 10*sizeof(int));
    for(int i=10;i<20;i++){
       arr[i]=arr[i-10]+10;
    }
    printf("printfing the array of 20 size after realloc\n");
    for(int i=0;i<20;i++){
        printf("arr[%d] -> %d\n",i,arr[i]);
    }
    
    
    int *arr2;
    printf("Doing alloca of size 10\n ");
    arr2=(int*)alloca(10*sizeof(int));
    for(int i=0;i<10;i++){
       arr2[i]=i+1;
    }
    printf("printfing the array o 10 malloc\n");
    for(int i=0;i<10;i++){
        printf("arr[%d] -> %d\n",i,arr2[i]);
    }
	return 0;
}
