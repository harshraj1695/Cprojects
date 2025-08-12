#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

int subtreesum(node* root) {
    if (!root) return 0;
    return root->val + subtreesum(root->left) + subtreesum(root->right);
}

// Function to check if tree is sum-balanced
int sumbalanced(node* root) {
    if (!root) return 1; 

    int leftSum = subtreesum(root->left);
    int rightSum = subtreesum(root->right);

    if (leftSum != rightSum)
        return 0;

    return sumbalanced(root->left) && sumbalanced(root->right);
}
int main(int argc, char **argv)
{
	printf("Build your binary tree:\n");
    node* root = insertval();
    if(sumbalanced(root)==1){
        printf("Tree is sum balanced\n");
    }else{
        printf("Tree is not sum Balanced");
    }
	return 0;
}
