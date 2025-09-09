#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node* next;
} node;

node* insert(node* head, int key)
{
    if(head == NULL) {
        node* newnode = (node*)malloc(sizeof(node));
        newnode->val = key;
        newnode->next = NULL;
        head = newnode;
    } else {
        node* temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        node* newnode = (node*)malloc(sizeof(node));
        newnode->val = key;
        newnode->next = NULL;
        temp->next = newnode;
    }
    return head;

}

int main(int argc, char** argv)
{
    node* head = NULL;
    while(1) {
        int key;
        printf("enter value -1 to ext ");
        scanf("%d", &key);
        if(key == -1)
            break;
        head = insert(head, key);
    }
    while(head) {
        printf("%d-> ", head->val);
        head = head->next;
    }
    return 0;
}
