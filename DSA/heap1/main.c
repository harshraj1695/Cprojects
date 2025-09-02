#include <stdio.h>
#include <stdlib.h>

typedef struct heap{
    int*arr;
    int capacity;
    int size;
}heap;

void inti(heap* head,int cap){
    head->capacity=cap;
    head->arr=(int*)malloc(cap*sizeof(int));
    head->size=0;
}
void swap(int *a,int*b){
    int temp=*a;
    *a=*b;
    *b=temp;
    
}
void insert(heap* head,int val){
    if(head->capacity==head->size){
        printf("heap overflow \n");
        return;
    }
    head->size++;
    int index=head->size;
    head->arr[index]=val;
    while(index>1){
        int parent=index/2;
        if(head->arr[index]>head->arr[parent]){
            swap(&(head->arr[index]), &(head->arr[parent]));
            index=parent;
        }else{
            break;
        }
    }
    
}
int delete(heap* head){
    int temp=head->arr[1];
    head->arr[1]=head->arr[head->size];
    head->size--;
    int index=1;
    while(index<head->size){
        int left=2*index;
        int right=2*index+1;
        int largest=index;
        if(left<=head->size && head->arr[largest]<head->arr[left]){
            largest=left;
        }
        if(right<=head->size && head->arr[largest]<head->arr[right]){
            largest=right;
        }
        if(index==largest){
            break;
        }
        else{
            swap(&(head->arr[index]), &(head->arr[largest]));
            index=largest;
        }
    }
    return temp;
}
void heapify(int *arr, int n, int index){
    int left=2*index;
    int right=2*index+1;
    int largest=index;
    if(left<=n && arr[left]>arr[largest]){
        largest=left;
    }
    if(right<=n && arr[right]>arr[largest]){
        largest=right;
    }
    if(largest!=index){
        swap(&arr[index],&arr[largest]);
        index=largest;
        heapify(arr,n,index);
    }
}
int main(int argc, char **argv)
{
	printf("hello world\n");
    heap head;
    inti(&head,10+1);
    insert(&head,10);
    insert(&head, 78);
    printf("%d\n",delete(&head));
	return 0;
}
