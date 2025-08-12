#include <stdio.h>
#include <stdlib.h>
typedef struct node {
    int val;
    struct node* left;
    struct node* right;
} node;

node* createnode(int key)
{
    node* newnode = (node*)malloc(sizeof(node));
    if(!newnode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newnode->val = key;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}
node* insertval()
{
    int val;
    printf("Enter node value (-1 for NULL): ");
    scanf("%d", &val);

    if(val == -1) {
        return NULL;
    }

    node* root = createnode(val);

    printf("Enter left child of %d:\n", val);
    root->left = insertval();

    printf("Enter right child of %d:\n", val);
    root->right = insertval();

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
void postorder(node* root)
{
    if(root == NULL) {
        return;
    }
    inorder(root->left);
    inorder(root->right);
    printf("%d ->", root->val);
}
void preorder(node* root)
{
    if(root == NULL) {
        return;
    }
    printf("%d ->", root->val);
    inorder(root->left);
    inorder(root->right);
}

int main(int argc, char** argv)
{
    printf("Build your binary tree:\n");
    node* root = insertval();
    
    printf("\nInorder traversal\n");
    inorder(root);
    printf("NULL\n");

    printf("\n pre order traversal\n");
    preorder(root);
    printf("NULL\n");

    printf("\n post order traversal\n");
    postorder(root);
    printf("NULL\n");

    return 0;
}
