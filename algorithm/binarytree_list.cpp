//
// Created by Administrator on 2018/12/13.
//

#include<assert.h>

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"list_queue.h"

typedef struct _treenode
{
    int data;
    struct _treenode *lchild;
    struct _treenode *rchild;
}Tnode, Tree; //Tree means root node

void binary_tree_create(Tree **Root) {
    int a = 0;
    printf("Enter node value(Enter 100 finish):\r\n");
    scanf("%d", &a);

    if(a == 100){
        *Root = nullptr;
    }else{
        *Root = static_cast<Tree *>(malloc(sizeof(Tnode)));
        if(*Root == nullptr){
            return;
        }
        (*Root)->data = a;
        printf("create %d's left child:\r\n", a);
        binary_tree_create(&((*Root)->lchild));
        printf("create %d's right child\r\n", a);
        binary_tree_create(&((*Root)->rchild));
    }
}

void binary_tree_destroy(Tree *root){
    if (root == nullptr) {
        return;
    }

    binary_tree_destroy(root->lchild);
    binary_tree_destroy(root->rchild);
    free(root);
}

//pre-order traversal
void binary_tree_pre_order(Tree *root){
    if (root == nullptr) {
        return;
    }
    printf(" %d ", root->data);
    binary_tree_pre_order(root->lchild);
    binary_tree_pre_order(root->rchild);
}

//in-order traversal
void binary_tree_in_order(Tree *root){
    if (root == nullptr) {
        return;
    }
    binary_tree_in_order(root->lchild);
    printf(" %d ", root->data);
    binary_tree_in_order(root->rchild);
}

//post-order traversal
void binary_tree_post_order(Tree *root){
    if (root == nullptr) {
        return;
    }
    binary_tree_post_order(root->lchild);
    binary_tree_post_order(root->rchild);
    printf(" %d ", root->data);
}

//level-order traversal: use queue
void binary_tree_level_order(Tree *root){
    list_queue *queue = nullptr;
    Tnode *node = nullptr;
    if (root == nullptr) {
        return;
    }
    queue = list_queue_create();

    list_queue_enqueue(queue, (void *)root); //root node enqueue

    while (!list_queue_is_empty(queue)) {
        list_queue_dequeue(queue, (void **)&node);
        printf(" %d ", node->data);
        if (node->lchild != nullptr) {
            list_queue_enqueue(queue, (void *)node->lchild);
        }
        if (node->rchild != nullptr) {
            list_queue_enqueue(queue, (void *) node->rchild);
        }
    }

    free(queue);
}

void binary_tree_level_reverse_order(Tree *root){
    if (root == nullptr) {
        return;
    }

    binary_tree_level_reverse_order(root->lchild);
    binary_tree_level_reverse_order(root->rchild);

    if (root->lchild != nullptr) {
        printf(" %d ", root->lchild->data);
    }
    if (root->rchild != nullptr) {
        printf(" %d ", root->rchild->data);
    }
}


// print all leaf node
void binary_tree_printf_leaf(Tree *root){
    if (root == nullptr) {
        return;
    }

    if ((root->lchild == nullptr) && (root->rchild == nullptr)) {
        printf(" %d ", root->data);
    }else{
        binary_tree_printf_leaf(root->lchild);
        binary_tree_printf_leaf(root->rchild);
    }
}

// calculate leaf number
int binary_tree_get_leaf_num(Tree *root){
    if (root == nullptr) {
        return 0;
    }
    if ((root->lchild == nullptr) && (root->rchild == nullptr)) {
        return 1;
    }
    return binary_tree_get_leaf_num(root->lchild) + binary_tree_get_leaf_num(root->rchild);
}

//calculate tree height
int binary_tree_get_height(Tree *root){
    if (root == nullptr) {
        return 0;
    }

    int lheight = 0;
    int rheight = 0;

    lheight = binary_tree_get_height(root->lchild);
    rheight = binary_tree_get_height(root->rchild);

    return ((lheight > rheight) ? (lheight + 1):(rheight + 1));
}

int main(){
    Tree *root = nullptr;

    printf("Create binary tree:\r\n");
    binary_tree_create(&root);
    printf("\r\nPre-order traversal:\r\n");
    binary_tree_pre_order(root);
    printf("\r\nIn-order traversal:\r\n");
    binary_tree_in_order(root);
    printf("\r\nPost-order traversal:\r\n");
    binary_tree_post_order(root);
    printf("\r\nLevel-order traversal:\r\n");
    binary_tree_level_order(root);
    printf("\r\nLevel-order reverse traversal:\r\n");
    binary_tree_level_reverse_order(root);

    printf("\r\nPrint binary tree leaf:\r\n");
    binary_tree_printf_leaf(root);
    printf("\r\nPrint binary tree leaf num: %d\r\n", binary_tree_get_leaf_num(root));
    printf("\r\nPrint binary tree height: %d\r\n", binary_tree_get_height(root));

    binary_tree_destroy(root);

    return 0;
}

