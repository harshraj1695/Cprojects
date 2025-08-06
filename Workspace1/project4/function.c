#include "function.h"
void process_fun(int a ,int *sum){
    static int count=0; // count the number of valid inputs
    if(a==-1){
        printf("Total sum is: %d and valid inputs are: %d ",*sum,count);
    }else{
        if(a>=0){
           *sum+=a;
            count++;
        }
    }
}