#include <stdio.h>
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
int main(){

    int i=0; // varible t terminate loop
     int a=0;// varible to take input
        int sum=0;// varible to store sum
    while(i==0){
       
        printf("Enter a number and -1 to quit "); // take user input
        scanf("%d",&a);
        if(a==-1){
            i=1; // i is using as flag to termiate the loop we can use break also
        }
        process_fun(a,&sum); // function call after every input 
    }
    return 0;
}
