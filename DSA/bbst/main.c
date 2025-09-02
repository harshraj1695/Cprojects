#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int val;
    struct node* left;
    struct node * right;

}node;

node * createnode(int key){
    node* newnode=(node*)malloc(sizeof(node));
    newnode->val=key;
    newnode->left=NULL;
    newnode->right=NULL;
    return newnode;
}

node* insert(node* root, int key){
    if(root==NULL)return createnode(key);
    if(key<=root->val){
        root->left= insert(root->left,key);
    }else{
        root->right=insert(root->right,key);
    }
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

void leftview(node* root,int level, int**arr, int*size){
    if(root==NULL) return;
    if(level==*size){
        *arr=realloc(*arr,(*size +1)*(sizeof(int)));
        (*arr)[*size]=root->val;
        (*size)++;
    }
    leftview(root->left,level+1, arr,size);
    leftview(root->right, level+1,arr,size);
    
}
int main(int argc, char **argv)
{
	node* root=NULL;
    while(1){
        int key;
        printf("enter key ");
        scanf("%d",&key);
        if(key==-1 ) break;
        root=insert(root,key);
    }
    inorder(root);
    printf("NULL\n");
     int* res = NULL;
    int size = 0;
    leftview(root, 0, &res, &size);
    printf("Left view: ");
    for(int i = 0; i < size; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");
    free(res);
	return 0;
}
