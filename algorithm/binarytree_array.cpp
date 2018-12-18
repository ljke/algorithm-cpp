//
// Created by Administrator on 2018/12/12.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_TREE_NODES (1 << 8)

struct node{
    int data;
};

struct binary_tree{
    union {
        unsigned long nodes;
        struct node *n[MAX_TREE_NODES];
    };
};

void init_binary_tree(struct binary_tree *tree){
    for (auto &i : tree->n) {
        i = nullptr;
    }
}

struct node* create_node(int data){
    struct node *n = nullptr;
    n = static_cast<node *>(malloc(sizeof(struct node)));
    if(n != nullptr) {
        n->data = data;
    }
    return n;
}

void fake_a_tree(struct binary_tree *tree){
    int data[10] = {7, 4, 9, 2, 6, 8, 10, 1, 3, 5};

    /* data is in ordered */
    init_binary_tree(tree);

    /* root start at 1 */
    for (int i = 0; i < 10; ++i) {
        tree->n[i + 1] = create_node(data[i]);
    }

    tree->nodes = 10;
}

//In-order traversal
void _in_order(struct binary_tree *tree, int index){
    if (!tree->n[index]) {
        return;
    }

    //left child at (index * 2)
    _in_order(tree, index << 1);

    printf("[%2d]: %4d\n", index, tree->n[index]->data);

    //right child at (index * 2 + 1)
    _in_order(tree, (index << 1) + 1);

}

void in_order(struct binary_tree *tree){
    _in_order(tree, 1);
}

int main(){
    struct binary_tree tree{};

    fake_a_tree(&tree);
    in_order(&tree);

    return 0;
}

