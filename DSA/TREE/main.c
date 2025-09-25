#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "btree.h"

void leftview(btree *root, int *size,int level){
    if(root==NULL) return;
    
    if((*size)==level){
        printf("%d-> ",root->val);
        (*size)++;
    }
    leftview(root->left,size,level+1);
    leftview(root->right,size,level+1);
}

void rightview(btree *root, int *size,int level){
    if(root==NULL) return;
    
    if((*size)==level){
        printf("%d-> ",root->val);
        (*size)++;
    }
    rightview(root->right,size,level+1);
    rightview(root->left,size,level+1);
}

int hasPathSum(btree *root,int sum){
     if (root == NULL) return 0;

    if (root->left == NULL && root->right == NULL) {
        return (sum == root->val);
    }

    int remaining = sum - root->val;

    return hasPathSum(root->left, remaining) || hasPathSum(root->right, remaining);
}
int max(int a,int b){
    if(a>b)return a;
    return b;
}
int height(btree* root,int *maxi){
    if(root==NULL){
        return 0;
    }
    int lh=height(root->left,maxi);
    int rh=height(root->right,maxi);
    *maxi=max(*maxi,lh+rh+1);
    if(lh>rh){
        return lh+1;
    }
    return rh+1;
}
#define SIZE 1000
#define OFFSET 500
void toporder(btree* root){
    if (!root) return;

    int arr[SIZE] = {0};       // stores node values
    int filled[SIZE] = {0};    // marks if index is already filled

    queuetree q;
    init(&q, 100);   // queue capacity = 100
    push(&q, root);
    root->h = 0;

    int minHd = 500, maxHd = -500;

    while (!isempty(&q)) {
        btree *node = pop(&q);
        int hor = node->h;

        // store only first time we see this horizontal distance
        if (!filled[hor + OFFSET]) {
            arr[hor + OFFSET] = node->val;
            filled[hor + OFFSET] = 1;
        }

        if (node->left) {
            node->left->h = hor - 1;
            push(&q, node->left);
        }
        if (node->right) {
            node->right->h = hor + 1;
            push(&q, node->right);
        }

        if (hor < minHd) minHd = hor;
        if (hor > maxHd) maxHd = hor;
    }

    // print result
    for (int i = minHd; i <= maxHd; i++) {
        if (filled[i + OFFSET]) {
            printf("%d ", arr[i + OFFSET]);
        }
    }
    printf("\n");
}


void bottomview(btree *root){
    if (!root) return;

    int arr[SIZE] = {0};       // stores node values
    int filled[SIZE] = {0};    // marks if index is already filled

    queuetree q;
    init(&q, 100);   // queue capacity = 100
    push(&q, root);
    root->h = 0;

    int minHd = 500, maxHd = -500;
    while(!isempty(&q)){
        btree * node=pop(&q);
        int hor=node->h;
        filled[hor+OFFSET]=1;
        arr[hor+OFFSET]=node->val;
        if(node->left){
            node->left->h=hor-1;
            push(&q,node->left);
        }
        if(node->right){
            node->right->h=hor+1;
            push(&q,node->right);
        }
        if(hor>maxHd) maxHd=hor;
        if(hor<minHd) minHd=hor;
        
        for (int i = minHd; i <= maxHd; i++) {
        if (filled[i + OFFSET]) {
            printf("%d ", arr[i + OFFSET]);
        }
    }
    printf("\n");
        
    }
}


void morris_inorder(btree *root) {
    btree *curr = root;
    while (curr != NULL) {
        if (curr->left == NULL) {
            // If no left child, print this node and move right
            printf("%d-> ", curr->val);
            curr = curr->right;
        } else {
            // Find inorder predecessor (rightmost node in left subtree)
            btree *pred = curr->left;
            while (pred->right != NULL && pred->right != curr) {
                pred = pred->right;
            }

            if (pred->right == NULL) {
                // Make a temporary link (thread) to current node
                pred->right = curr;
                curr = curr->left;
            } else {
                // Thread already exists: remove it, visit node, go right
                pred->right = NULL;
                printf("%d-> ", curr->val);
                curr = curr->right;
            }
        }
    }
}

void morris_preorder(btree *root) {
    btree *curr = root;
    while (curr != NULL) {
                    

        if (curr->left == NULL) {
            // If no left child, print this node and move right
            printf("%d-> ", curr->val);
            curr = curr->right;
        } else {
            // Find inorder predecessor (rightmost node in left subtree)
            btree *pred = curr->left;
            while (pred->right != NULL && pred->right != curr) {
                pred = pred->right;
            }

            if (pred->right == NULL) {
                // Make a temporary link (thread) to current node
                printf("%d-> ", curr->val);
                pred->right = curr;
                curr = curr->left;
            } else {
                // Thread already exists: remove it, visit node, go right
                pred->right = NULL;
//                printf("%d-> ", curr->val);
                curr = curr->right;
            }
        }
    }
}
void leftboundary(btree *root){
    if(root==NULL)return;
    if(root->left==NULL && root->right==NULL){
        return;
    }
    printf("%d-> ", root->val);
    if(root->left!=NULL){
        leftboundary(root->left);
    }else{
        leftboundary(root->right);
    }
}
void leafbounday(btree *root){
    if(root==NULL)return;
    if(root->left==NULL && root->right==NULL){
        printf("%d-> ",root->val);
    }
    leafbounday(root->left);
    leafbounday(root->right);
    
    
}

void rightboundary(btree *root){
    if(root==NULL)return;
    if(root->left==NULL && root->right==NULL){
        return;
    }
    if(root->right!=NULL){
        rightboundary(root->right);
    }else{
        rightboundary(root->left);
    }
    printf("%d-> ", root->val);
}
void boundarytraversal(btree *root){
    if(root==NULL) printf("NULL\n");
    leftboundary(root);
    leafbounday(root->left);
    leafbounday(root->right);
    rightboundary(root->right);
}

int treesum(btree*root, int sum){
    if(root==NULL) return 0;
    sum-=root->val;
    if(root->left==NULL && root->right==NULL){
        if(sum==0){
            return 1;
        }else{
            return 0;
        }
    }
    
    return treesum(root->left, sum) || treesum(root->right,sum);
     
}
int main() {
//    queue q;
//    init(&q, 5);
//
//    push(&q, 10);
//    push(&q, 20);
//    push(&q, 30);
//    display(&q);
//
//    printf("Popped: %d\n", pop(&q));
//    display(&q);
    btree *root;
    root=insertval();
    inorder(root);
    printf("\n");
    levelorder(root);
    printf("\n");
    int size=0;
    leftview(root,&size,0);
    printf("\n");
    size=0;
    rightview(root,&size,0);
    
    printf("\n");
    int maxi=0;
    printf("height of tree : %d\n",height(root,&maxi));
    printf("diameter of tree : %d\n", maxi);
    
    printf("\n");
    toporder(root);
    
    
    bottomview(root);
    printf("\n moris traversal :");
    morris_inorder(root);
    
    printf("\n moris preeorder : ");
    morris_preorder(root);
    printf("\n");
    boundarytraversal(root);
    printf("\n");
    int sum=10;
    int a=treesum(root,sum);
    printf("%d \n",a);
    return 0;
}
