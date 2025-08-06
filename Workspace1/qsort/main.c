#include <stdio.h>
#include <stdlib.h>

int cmpa(const void *p1,const void*p2){
    return (*(int *)p1-*(int *)p2);
}
int cmpdesc(const void *p1,const void*p2){
    return (*(int *)p2-*(int *)p1);
}
int main(int argc, char **argv)
{
	int arr[]={1,56,43,23,5};
    qsort(arr,5,sizeof(int),cmpa);
    for(int i=0;i<5;i++){
        printf("%d " ,arr[i]);
    }
    printf("\n");
        qsort(arr,5,sizeof(int),cmpdesc);
  for(int i=0;i<5;i++){
        printf("%d " ,arr[i]);
    }
	return 0;
}
