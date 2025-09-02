#include <stdio.h>


#pragma pack(1)
struct person {
    char a;
    int b;
    int arr[3];
} p1;
#pragma pack()
int main(int argc, char** argv)
{
//    p.a = 'a';
//    p.b = 5;
//    p.arr[0] = 1;
//    p.arr[1] = 2;
//    p.arr[2] = 3;
//    char *ch;
//    ch=(char*)&p;
//    printf("%c ",*(ch));
//    ch+=4;
//    printf("%d ",*((int*)(ch)));
//    ch+=4;
//    int l=3;
//    while(l--){
//        printf("%d ",*((int*)(ch)));
//        ch+=4;
//    }
      
      printf("%d\n", (int)sizeof(p1));
      
      p1.a='a';
      p1.b=5;
      p1.arr[0]=1;
      p1.arr[1]=2;
      p1.arr[2]=3;
      int *ch=(int*)&p1;
      printf("%d\n",*(ch));
      ch++;
      printf("%d\n",*((int*)(ch)));
      ch++;
      printf("%d\n",*((int*)(ch)));
      ch++;
      printf("%d\n",*((int*)(ch)));
      
      printf("%d\n", 'a');
    return 0;
}
