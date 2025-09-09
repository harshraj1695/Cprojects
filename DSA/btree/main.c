#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int val;
    struct node *left;
struct node *right; 
}node;

node* createnode(int key){
   node* newnode=(node*)malloc(sizeof(node));
   newnode->val=key;
   newnode->left=NULL;
   newnode->right=NULL;
    
    return newnode;
}
node* insert(){
    int key;
    printf("enter root value -1 to stop ");
    scanf("%d",&key);
    if(key==-1) return NULL;
    node* root=createnode(key);
    
    printf("enter the left of %d\n",root->val);
    root->left=insert();
    printf("enter right of %d\n", root->val);
    root->right=insert();
    return root;
}

void inorder(node* root)
{
    if(root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ->", root->val);
    inorder(root->right);
}
int main(int argc, char **argv)
{
	node *root=insert();
    inorder(root);
        printf("NULL\n");

	return 0;
}
