#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "btree.h"

void init(queuetree *q, int cap) {
    q->capacity = cap;
    q->size=0;
    q->arr = (btree**)calloc(q->capacity, sizeof(btree*));
    q->front = -1;
    q->rear = -1;
}
int returnsize(queuetree* q){
    return q->size;
}

int isfull(queuetree *q) {
    return q->size == q->capacity;

}

int isempty(queuetree *q) {
    return (q->front == -1 || q->front > q->rear);
}

void push(queuetree *q, btree *node) {
    if (isfull(q)) {
        printf("Overflow\n");
        return;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear++;
    q->size++;
    q->arr[q->rear] = node;
}

btree* pop(queuetree *q) {
    if (isempty(q)) {
        printf("Underflow\n");
        return NULL;
    }
    btree* temp = q->arr[q->front];
    q->front++;
    q->size--;
    return temp;
}


btree* createnode(int num){
   btree *newnode=(btree*)malloc(sizeof(btree));
newnode->val=num;
newnode->left=NULL;
newnode->right=NULL;    
return newnode;
}

btree* insertval(){
    int key;
    printf("Enter the node value -1 to exit\n");
    scanf("%d",&key);
    if(key==-1)return NULL;
    btree* node=createnode(key);
    printf("Enter left of %d\n ",key);
    node->left=insertval();
    printf("Enter the right of %d\n",key);
    node->right=insertval();
    return node;
}

void inorder(btree *root){
    if(root==NULL) return ;
    
    inorder(root->left);
    printf("%d-> ",root->val);
    inorder(root->right);
}

void postorder(btree *root){
    if(root==NULL) return ;
    
    postorder(root->left);
    postorder(root->right);
    
    printf("%d-> ",root->val);
}

void preorder(btree *root){
    if(root==NULL) return;
    
    printf("%d-> ",root->val);
    preorder(root->left);
    preorder(root->right);
}


void levelorder(btree *root){
    queuetree q;
    init(&q,10);
    
    push(&q,root);
    
    while(!isempty(&q)){
        int size=returnsize(&q);
        for(int i=0;i<size;i++){
            btree *node=pop(&q);
            printf("%d-> ",node->val);
            if(node->left!=NULL){
                push(&q,node->left);
            }
            if(node->right!=NULL){
                push(&q,node->right);
            }
        }
    }
}