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

void recursionleft(node* root, int level, int** res, int* size)
{
    if(root == NULL)
        return;

    // If size == level, it means this level is visited first time
    if(*size == level) {
        *res = realloc(*res, (*size + 1) * sizeof(int));
        (*res)[*size] = root->val;
        (*size)++;
    }

    recursionleft(root->left, level + 1, res, size);
    recursionleft(root->right, level + 1, res, size);
}

void recursionright(node* root, int level, int** res, int* size)
{
    if(root == NULL)
        return;

    // If size == level, it means this level is visited first time
    if(*size == level) {
        *res = realloc(*res, (*size + 1) * sizeof(int));
        (*res)[*size] = root->val;
        (*size)++;
    }

    recursionright(root->right, level + 1, res, size);
    recursionright(root->left, level + 1, res, size);
}
int main(int argc, char** argv)
{
    printf("Build your binary tree:\n");
    node* root = insertval();

    int* res = NULL;
    int size = 0;
    recursionleft(root, 0, &res, &size);
    printf("Left view: ");
    for(int i = 0; i < size; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");
    free(res);

    int* res2 = NULL;
    int size2 = 0;
    recursionleft(root, 0, &res2, &size2);
    printf("right view: ");
    for(int i = 0; i < size2; i++) {
        printf("%d ", res2[i]);
    }
    printf("\n");
    free(res);

    return 0;
}
