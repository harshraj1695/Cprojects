#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int val;
    struct node* next;
    struct node* prev;
}node;
node* create(node* head, int key){
    if(head==NULL){
        node* newnode=(node*)malloc(sizeof(node));
        newnode->val=key;
        newnode->next=NULL;
        newnode->prev=NULL;
        head=newnode;
    }else{
        node* temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        node* newnode=(node*) malloc(sizeof(node));
        newnode->val=key;
        temp->next=newnode;
        newnode->prev=temp;
        newnode->next=NULL;
    }
    return head;
}
int main(int argc, char **argv)
{
	node* head = NULL;
    while(1) {
        int key;
        printf("enter value -1 to ext ");
        scanf("%d", &key);
        if(key == -1)
            break;
        head = create(head, key);
    }
    while(head) {
        printf("%d-> ", head->val);
        head = head->next;
    }
	return 0;
}
