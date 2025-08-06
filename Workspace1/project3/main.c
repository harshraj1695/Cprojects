#include <stdio.h>
void process_fun(int a , int *b,int *sum){
    if(a==-1){
        printf("Total sum is: %d and valid inputs are: %d ",*sum,*b);
    }else{
        *sum+=a;
        if(a>=0){
            (*b)++;
        }
    }
}
int main(){

    int i=0;
     int a=0;
        int t=0;
        int sum=0;
    while(i==0){
       
        printf("Enter a number and -1 to quit "); // take user input
        scanf("%d",&a);
        if(a==-1){
            i=1; // i is using as flag to termiate the loop
        }
        process_fun(a,&t,&sum); // function call after every input 
    }
    return 0;
}
