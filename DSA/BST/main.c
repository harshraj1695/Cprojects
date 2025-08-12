#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
} node;

node* createnode(int key) {
    node* newnode = (node*)malloc(sizeof(node));
    if (!newnode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newnode->val = key;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

void insert(node **root, int key) {
    if (key == -1) { // skip null marker
        return;
    }
    if (*root == NULL) {
        *root = createnode(key);
        return;
    }
    if (key < (*root)->val) {
        insert(&((*root)->left), key);
    } else {
        insert(&((*root)->right), key);
    }
}
node* insertval(node* root, int val) {
    if (root == NULL) {
        return createnode(val);
    }
    if (val < root->val) {
        root->left = insertval(root->left, val);
    } else {
        root->right = insertval(root->right, val);
    }
    return root;
}
node* buildbst() {
    node* root = NULL;
    int val;
    printf("Enter values for BST (-1 to stop):\n");
    while (1) {
        scanf("%d", &val);
        if (val == -1) break;
        root = insertval(root, val);
    }
    return root;
}

void inorder(node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d -> ", root->val);
    inorder(root->right);
}

int main() {
     printf("Build your binary tree:\n");
    node* root = buildbst();
    inorder(root);
    printf("NULL\n");

    return 0;
}
