#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
    int val;
    struct Node *next;
}Node;
Node* insert(Node* head,int val){
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->val=val;
    newnode->next=NULL;
    
    if(head==NULL){
        head=newnode;
        return head;
    }
    Node*temp=head;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=newnode;
    return head;
}
Node* deleteNode(Node* head, int val) {

 if (head == NULL) return NULL;
    if (head->val == val) {
        Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }

    Node* prev = head;
    Node* curr = head->next;
    while (curr != NULL && curr->val != val) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        prev->next = curr->next;
        free(curr);
    }

    return head;
}
Node* deletedouble(Node** head, int val){
    if(head==NULL) return NULL;
    if((*head)->val==val){
        Node* temp=*head;
        (*head)=(*head)->next;
        free(temp);
    }
    Node* prev = (*head);
    Node* curr = (*head)->next;
    while (curr != NULL && curr->val != val) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        prev->next = curr->next;
        free(curr);
    }

    return *head;
}
void printnode(Node* head){
    while(head!=NULL){
        printf("%d ->",head->val);
        head=head->next;
    }
    printf("NULL\n");
}
int main(int argc, char **argv)
{
	Node* head=NULL;
    head=insert(head,5);
    head=insert(head,7);
//    head=deleteNode(head,5);
head=deletedouble(&head,7);
    printnode(head);
    
	return 0;
}
