#include <stdio.h>
void res(int *restrict ptr1, int *restrict ptr2){
    for(int i=0;i<4;i++){
        *ptr1+=*ptr2;
    *ptr2+=*ptr1;
    ptr1++;
    ptr2++;
    }
    
    
}
int main(int argc, char **argv)
{
	printf("hello world\n");
    int arr[]={1,2,3,4};
    int arr2[]={5,6,7,8};
    res(arr,arr2);
    for(int i=0;i<4;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    for(int i=0;i<4;i++){
        printf("%d ",arr2[i]);
    }
	return 0;
}
