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

bool hasPathSum(node* root, int targetSum) {
    if (!root) return false;

    // Subtract current node value from target sum
    targetSum -= root->val;

    // If leaf node, check if remaining sum is zero
    if (!root->left && !root->right) {
        return (targetSum == 0);
    }

    // Recur on left or right
    return hasPathSum(root->left, targetSum) || hasPathSum(root->right, targetSum);
}

int main() {
    printf("Build your binary tree:\n");
    node* root = insertval();

    int target;
    printf("Enter target sum: ");
    scanf("%d", &target);

    if (hasPathSum(root, target)) {
        printf("Yes, there exists a root-to-leaf path with sum %d.\n", target);
    } else {
        printf("No, such path exists.\n");
    }

    return 0;
}
