#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int val;
    struct node* left;
    struct node* right;
} node;

node* createnode(int key) {
    node* newnode = (node*)malloc(sizeof(node));
    newnode->val = key;
    newnode->left = newnode->right = NULL;
    return newnode;
}

node* insertval() {
    int val;
    printf("Enter node value (-1 for NULL): ");
    scanf("%d", &val);
    if (val == -1) return NULL;
    node* root = createnode(val);
    printf("Enter left child of %d:\n", val);
    root->left = insertval();
    printf("Enter right child of %d:\n", val);
    root->right = insertval();
    return root;
}

// Function to flip (mirror) the binary tree
void flipTree(node* root) {
    if (root == NULL) return;

    // Swap left and right
    struct node* temp = root->left;
    root->left = root->right;
    root->right = temp;

    // Recurse for children
    flipTree(root->left);
    flipTree(root->right);
}

// Inorder traversal to check result
void inorder(struct node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->val);
    inorder(root->right);
}

int main() {
     printf("Build your binary tree:\n");
    node* root = insertval();

    printf("Inorder before flip: ");
    inorder(root);
    printf("\n");

    flipTree(root);

    printf("Inorder after flip: ");
    inorder(root);
    printf("\n");

    return 0;
}
