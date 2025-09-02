#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct node {
    int val;
    struct node* left;
    struct node* right;
} node;


//int diameter=0;
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

// Function to calculate height of a subtree
int diameter; // global or passed by pointer

int calculateheight(node* root) {
    if (root == NULL) return 0;

    int leftheight = calculateheight(root->left);
    int rightheight = calculateheight(root->right);

    // +1 to include current node in count
    if (leftheight + rightheight + 1 > diameter) {
        diameter = leftheight + rightheight + 1;
    }

    return 1 + (leftheight > rightheight ? leftheight : rightheight);
}

int maxpathsum(node* root) {
//    diameter = 0; 
    calculateheight(root);
    return diameter; // returns nodes
}

int main(int argc, char** argv)
{
    printf("Build your binary tree:\n");
    node* root = insertval();
    maxpathsum(root);
    printf("maximaimum path length or tree is %d\n", diameter);
}